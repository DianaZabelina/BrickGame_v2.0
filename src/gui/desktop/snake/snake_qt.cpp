#include "snake_qt.h"

using namespace s21;

View::View(Controller& controller, QWidget* parent)
    : QWidget(parent), controller_(controller), isGameRunning_(false) {
  setWindowTitle("Snake");
  setFixedSize((SNAKE_X + FIELD_WIDTH + GAME_INFO_X) * cellSize,
               (SNAKE_Y + FIELD_HEIGHT + GAME_INFO_Y) * cellSize);
  menu = new Menu(this);
  menu->show();
  setFocusPolicy(Qt::StrongFocus);
  setFocus();
  timer_ = new QTimer(this);
  connect(timer_, &QTimer::timeout, this, &View::updateGame);
  std::srand(std::time(0));
}

void View::startGame() {
  menu->hide();
  isGameRunning_ = true;
  setFocus();
  controller_.getModel().resetGame();
  timer_->start(100);
}

void View::updateGame() {
  if (controller_.getModel().getPlaying() == s21::PLAYING) {
    updateCurrentState();
    update();
  } else {
    timer_->stop();
    isGameRunning_ = false;
    controller_.getModel().resetGame();
    menu->show();
  }
}

void View::keyPressEvent(QKeyEvent* event) {
  if (isGameRunning_) {
    controller_.handleUserInput(event->key(), event->isAutoRepeat());
  } else {
    if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
      startGame();
    } else if (event->key() == Qt::Key_Escape) {
      close();
    }
  }
}

void View::paintEvent(QPaintEvent* event) {
  Q_UNUSED(event);
  QPainter painter(this);
  if (isGameRunning_) {
    drawGame(painter);
    drawInterfaceExtras(painter);
    drawBorders(painter);
  }
}

QColor View::getColorForBlock(int block) {
  QColor color;
  switch (block) {
    case 1:
      color = Qt::green;
      break;
    case 2:
      color = Qt::darkGreen;
      break;
    case 3:
      color = Qt::red;
      break;
    default:
      color = Qt::black;
      break;
  }
  return color;
}

void View::drawGame(QPainter& painter) {
  for (int i = 0; i < FIELD_HEIGHT; i++) {
    for (int j = 0; j < FIELD_WIDTH; j++) {
      int block = controller_.getModel().getGameInfo().field[i][j];
      QColor color = getColorForBlock(block);
      painter.fillRect((SNAKE_X + j) * cellSize, (SNAKE_Y + i - 1) * cellSize,
                       cellSize, cellSize, color);
      painter.setPen(Qt::black);
      painter.drawRect((SNAKE_X + j) * cellSize, (SNAKE_Y + i - 1) * cellSize,
                       cellSize, cellSize);
    }
  }
}

void View::drawInterfaceExtras(QPainter& painter) {
  painter.setPen(Qt::white);
  painter.drawText(
      (GAME_INFO_X)*cellSize, GAME_INFO_Y * cellSize,
      QString("Score: %1").arg(controller_.getModel().getGameInfo().score));
  painter.drawText((GAME_INFO_X)*cellSize, (GAME_INFO_Y + 1) * cellSize,
                   QString("High Score: %1")
                       .arg(controller_.getModel().getGameInfo().high_score));
  painter.drawText(
      (GAME_INFO_X)*cellSize, (GAME_INFO_Y + 2) * cellSize,
      QString("Level: %1").arg(controller_.getModel().getGameInfo().level));
  painter.drawText(
      (GAME_INFO_X)*cellSize, (GAME_INFO_Y + 3) * cellSize,
      QString("Speed: %1 %2")
          .arg(controller_.getModel().getGameInfo().speed)
          .arg(controller_.getModel().getBoost() ? "Boosted" : ""));
  painter.drawText(
      (GAME_INFO_X)*cellSize, (GAME_INFO_Y + 4) * cellSize,
      QString("Length: %1").arg(controller_.getModel().getSnake().getLength()));

  if (controller_.getModel().getGameInfo().pause) {
    painter.drawText(20, 20, "PAUSE");
  }
}

void View::drawBorders(QPainter& painter) {
  painter.setPen(Qt::black);
  // Отрисовка границ игрового поля
  painter.drawRect((SNAKE_X)*cellSize, (SNAKE_Y - 1) * cellSize,
                   FIELD_WIDTH * cellSize, FIELD_HEIGHT * cellSize);
  // Отрисовка границ информации об игре
  painter.drawRect((GAME_INFO_X - 1) * cellSize, (GAME_INFO_Y - 1) * cellSize,
                   10 * cellSize, 5 * cellSize + 10);
}

Menu::Menu(QWidget* parent) : QWidget(parent) {
  QVBoxLayout* layout = new QVBoxLayout(this);

  layout->setAlignment(Qt::AlignCenter);
  int widthMargin = (SNAKE_X + FIELD_WIDTH + GAME_INFO_X - 10) * 20 / 2;
  int heightMargin = (SNAKE_Y + FIELD_HEIGHT + GAME_INFO_Y) * 20 / 6;
  layout->setContentsMargins(widthMargin, heightMargin, widthMargin,
                             heightMargin);

  QFont titleFont;
  titleFont.setPointSize(20);
  QFont font;
  titleFont.setPointSize(14);

  titleLabel = new QLabel("Snake", this);
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
      "W               move up\n"
      "A                move left\n"
      "S                move down\n"
      "D                move right\n"
      "E                boost speed\n"
      "SPACE       to pause\n"
      "ESCAPE     to finish",
      this);
  controlLabel->setAlignment(Qt::AlignLeft);
  layout->addWidget(controlLabel);

  setLayout(layout);
}
