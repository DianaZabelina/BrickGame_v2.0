#include "tetris_qt.h"

using namespace s21;

Tetris::Tetris(QWidget *parent) : QWidget(parent) {
  setWindowTitle("Tetris");
  setFixedSize((TETRIS_X + FIELD_WIDTH + GAME_INFO_X) * cellSize,
               (TETRIS_Y + FIELD_HEIGHT + GAME_INFO_Y) * cellSize);
  menu = new Menu(this);
  menu->show();
  gameIsRunning = false;
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
}

Tetris::~Tetris() { freeSingletones(); }

void Tetris::startGame() {
  menu->hide();
  gameIsRunning = true;
  setFocus();
  Controller::getController(this)->updateGame();
}

void Tetris::endGame() {
  gameIsRunning = false;
  menu->show();
}

void Tetris::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  if (gameIsRunning) {
    QPainter painter(this);
    drawBorders(painter);
    drawGame(painter);
    drawNextFigure(painter);
    drawInterfaceExtras(painter);
  }
}

void Tetris::keyPressEvent(QKeyEvent *event) {
  Controller::getController(this)->processUserInput(event->key(),
                                                    event->isAutoRepeat());
}

void Tetris::drawBorders(QPainter &painter) {
  painter.setPen(Qt::black);
  // Отрисовка границ игрового поля
  painter.drawRect((TETRIS_X)*cellSize, (TETRIS_Y)*cellSize,
                   FIELD_WIDTH * cellSize, FIELD_HEIGHT * cellSize);
  // Отрисовка границ информации об игре
  painter.drawRect((GAME_INFO_X - 1) * cellSize, (GAME_INFO_Y)*cellSize,
                   10 * cellSize, 5 * cellSize - 8);
  // Отрисовка границ следующей фигуры
  painter.drawRect((TETRIS_X + 17) * cellSize, (GAME_INFO_Y + 10) * cellSize,
                   5 * cellSize, 5 * cellSize);
}

void Tetris::setGameInfo(GameInfo_t gi) {
  gameInfo.field = gi.field;
  gameInfo.next = gi.next;
  gameInfo.score = gi.score;
  gameInfo.high_score = gi.high_score;
  gameInfo.level = gi.level;
  gameInfo.speed = gi.speed;
  gameInfo.pause = gi.pause;
}

QColor Tetris::getColorForBlock(int block) {
  switch (block) {
    case 1:
      return Qt::red;
    case 2:
      return QColor(255, 165, 0);  // оранжевый
    case 3:
      return Qt::yellow;
    case 4:
      return QColor(255, 192, 203);  // розовый
    case 5:
      return Qt::green;
    case 6:
      return Qt::cyan;
    case 7:
      return Qt::magenta;
    default:
      return Qt::black;
  }
}

void Tetris::drawGame(QPainter &painter) {
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = 0; x < FIELD_WIDTH; ++x) {
      int block = gameInfo.field[y][x];
      painter.fillRect((TETRIS_X + x) * cellSize, (TETRIS_Y + y) * cellSize,
                       cellSize, cellSize, getColorForBlock(block));
      painter.setPen(Qt::black);
      painter.drawRect((TETRIS_X + x) * cellSize, (TETRIS_Y + y) * cellSize,
                       cellSize, cellSize);
    }
  }
}

void Tetris::drawNextFigure(QPainter &painter) {
  for (int y = 0; y < FIGURE_SIZE; ++y) {
    for (int x = 0; x < FIGURE_SIZE; ++x) {
      int block = gameInfo.next[y][x];
      if (block) {
        painter.fillRect((18 + TETRIS_X + x) * cellSize,
                         (GAME_INFO_Y + 11 + y) * cellSize, cellSize, cellSize,
                         getColorForBlock(block));
        painter.drawRect((18 + TETRIS_X + x) * cellSize,
                         (GAME_INFO_Y + 11 + y) * cellSize, cellSize, cellSize);
      }
    }
  }
}

void Tetris::drawInterfaceExtras(QPainter &painter) {
  QFont font = painter.font();
  font.setPointSize(14);
  painter.setFont(font);
  painter.setPen(Qt::white);
  painter.drawText((GAME_INFO_X)*cellSize, (GAME_INFO_Y + 1) * cellSize,
                   QString("Score: %1").arg(getGame()->score));
  painter.drawText((GAME_INFO_X)*cellSize, (GAME_INFO_Y + 2) * cellSize,
                   QString("High Score: %1").arg(getGame()->high_score));
  painter.drawText((GAME_INFO_X)*cellSize, (GAME_INFO_Y + 3) * cellSize,
                   QString("Level: %1").arg(gameInfo.level));
  painter.drawText((GAME_INFO_X)*cellSize, (GAME_INFO_Y + 4) * cellSize,
                   QString("Speed: %1").arg(gameInfo.speed));

  if (getGameInfo()->pause) {
    painter.drawText(20, 20, "PAUSE");
  } else {
    painter.drawText(20, 20, "     ");
  }
}

Controller *Controller::getController(Tetris *t) {
  static Controller controller(t);
  return &controller;
}

void Controller::processUserInput(int key, bool hold) {
  if (!tetris->gameIsRunning) {
    switch (key) {
      case Qt::Key_Enter:
      case Qt::Key_Return:
        if (!tetris->gameIsRunning) {
          timer->start(1);
          tetris->startGame();
        }
        break;
      case Qt::Key_Escape:
        tetris->close();
        break;
    }
  } else {
    switch (key) {
      case Qt::Key_D:
        userInput(Right, hold);
        break;
      case Qt::Key_A:
        userInput(Left, hold);
        break;
      case Qt::Key_S:
        userInput(Down, hold);
        break;
      case Qt::Key_W:
        userInput(Action, hold);
        break;
      case Qt::Key_Space:
        userInput(Pause, hold);
        break;
      case Qt::Key_Escape:
        userInput(Terminate, hold);
        if (timer->isActive()) {
          updateGame();
          timer->stop();
        }
        break;
    }
  }
}

void Controller::resetTetris() { resetSingletones(); }

void Controller::updateGame() {
  GameInfo_t gameInfo = updateCurrentState();
  tetris->setGameInfo(gameInfo);
  tetris->update();
  if (!getGame()->playing) {
    timer->stop();
    tetris->endGame();
    resetTetris();
  }
}

Controller::Controller(Tetris *t) : tetris(t) {
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &Controller::updateGame);
}

Controller::~Controller() { delete timer; }

Menu::Menu(QWidget *parent) : QWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  layout->setAlignment(Qt::AlignCenter);
  int widthMargin = (TETRIS_X + FIELD_WIDTH + GAME_INFO_X - 10) * 20 / 2;
  int heightMargin = (TETRIS_Y + FIELD_HEIGHT + GAME_INFO_Y) * 20 / 6;
  layout->setContentsMargins(widthMargin, heightMargin, widthMargin,
                             heightMargin);

  QFont titleFont;
  titleFont.setPointSize(20);
  QFont font;
  titleFont.setPointSize(14);

  titleLabel = new QLabel("Tetris", this);
  titleLabel->setAlignment(Qt::AlignCenter);
  titleLabel->setFont(titleFont);
  layout->addWidget(titleLabel);

  menuLabel = new QLabel(
      "Press ENTER to start\n"
      "Press ESCAPE to exit\n\n\n\n\n",
      this);
  menuLabel->setAlignment(Qt::AlignCenter);
  menuLabel->setFont(font);
  layout->addWidget(menuLabel);

  controlLabel = new QLabel("Control", this);
  controlLabel->setAlignment(Qt::AlignCenter);
  controlLabel->setFont(titleFont);
  layout->addWidget(controlLabel);

  controlLabel = new QLabel(
      "W               rotate\n"
      "A                move left\n"
      "S                drop\n"
      "D                move right\n"
      "SPACE       to pause\n"
      "ESCAPE     to finish",
      this);
  controlLabel->setAlignment(Qt::AlignLeft);
  layout->addWidget(controlLabel);

  setLayout(layout);
}
