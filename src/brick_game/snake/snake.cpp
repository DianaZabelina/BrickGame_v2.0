#include "snake.h"

using namespace s21;

// ----------Snake----------

/**
 * Конструктор змейки инициализирует тело змейки 4 точками, напралением движения
 * и последним направлением
 */
Snake::Snake()
    : body({{START_X, START_Y},
            {START_X, START_Y + 1},
            {START_X, START_Y + 2},
            {START_X, START_Y + 3}}),
      direction(Direction::Up),
      lastDirection(Direction::Up) {}

/**
 * Проверка на противоположность нового направления движения последнему
 * направлению
 *
 * @param newDirection новое направление
 *
 * @return true - новое направление противоположно
 * @return false - новое направление не противоположно
 */
bool Snake::isOpposite(Direction newDirection) const {
  Direction cur = getLastDirection();
  if ((cur == Direction::Left && newDirection == Direction::Right) ||
      (cur == Direction::Right && newDirection == Direction::Left) ||
      (cur == Direction::Up && newDirection == Direction::Down) ||
      (cur == Direction::Down && newDirection == Direction::Up))
    return true;
  else
    return false;
}

/**
 * Задание нового направления движения змейки
 *
 * @param newDirection новое направление
 */
void Snake::setDirection(Direction newDirection) {
  if (!isOpposite(newDirection)) direction = newDirection;
}

// ----------Field----------

/**
 * Конструктор поля, инициализирует поле заданной ширины и высоты, выделяет
 * память для массива блоков и заполняет его нулями
 */
Field::Field() { blocks = new char[width * height](); }

/**
 * Деструктор поля, освобождает память, выделенную для массива блоков
 */
Field::~Field() {
  if (blocks) {
    delete[] blocks;
    blocks = nullptr;
  }
}

/**
 * Проверка нахождения координат в пределах поля
 *
 * @param x координата по горизонтали
 * @param y координата по вертикали
 *
 * @return true - координаты находятся в пределах поля
 * @return false - координаты находятся за пределами поля
 */
bool Field::isInside(int x, int y) const {
  return x >= 0 && x < width && y >= 0 && y < height;
}

/**
 * Определение значения блока на поле по координатам
 *
 * @param x координата по горизонтали
 * @param y координата по вертикали
 *
 * @return значение блока на поле по координатам (x, y)
 */
char Field::getBlock(int x, int y) const {
  if (!isInside(x, y)) throw std::out_of_range("Block outside the field");
  return blocks[y * width + x];
}

/**
 * Установка значения блока на поле по координатам
 *
 * @param x координата по горизонтали
 * @param y координата по вертикали
 * @param value значение для установка *
 * @return значение блока на поле по координатам
 */
void Field::setBlock(int x, int y, char value) {
  if (!isInside(x, y)) throw std::out_of_range("Block outside the field");
  blocks[y * width + x] = value;
}

/**
 * Сброс всех блоков на поле в нулевое значение
 */
void Field::resetField() {
  for (int i = 0; i < height; i++)
    for (int j = 0; j < width; j++) setBlock(j, i, 0);
}

// ----------Game----------

/**
 * Конструктор игры, создает поле, змейку и яблоко, устанавливает начальные
 * значения для игровых параметров
 */
Game::Game()
    : field(),
      snake(),
      appleEaten(0),
      playing(PLAYING),
      isBoosted(0),
      boostFactor(1.5) {
  lastActionTime = std::chrono::steady_clock::now();
  info.field = new int*[FIELD_HEIGHT];
  for (int i = 0; i < FIELD_HEIGHT; i++) info.field[i] = new int[FIELD_WIDTH];
  field.resetField();
  addSnake();
  addApple();
  info.next = nullptr;
  info.score = 0;
  initHighScore();
  info.level = 1;
  info.speed = 1;
  info.pause = 0;
}

/**
 * Деструктор игры, освобождает память, выделенную для игрового поля и других
 * ресурсов
 */
Game::~Game() { freeGameInfo(); }

/**
 * Создание единственного экземпляра класса Game
 *
 * @return статический экземпляр класса Game
 */
Game& Game::getGame() {
  static Game game;
  return game;
}

/**
 * Освобождение памяти, выделенной для двумерного массива info.field
 */
void Game::freeGameInfo() {
  if (info.field)
    for (int i = 0; i < FIELD_HEIGHT; i++) delete[] info.field[i];
  delete[] info.field;
}

/**
 * Обнуление значений сегментов змейки
 */
void Game::resetSnake() {
  std::deque<std::pair<int, int>>& body = snake.getBody();
  for (size_t i = 0; i < body.size(); i++) {
    int x = body[i].first;
    int y = body[i].second;
    field.setBlock(x, y, 0);
  }
}

/**
 * Обнуление info.field
 */
void Game::resetInfoField() {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) info.field[i][j] = 0;
}

/**
 * Обнуление всей информации о состоянии игры
 */
void Game::resetInfo() {
  resetInfoField();
  info.next = nullptr;
  info.score = 0;
  initHighScore();
  info.level = 1;
  info.speed = 1;
  info.pause = 0;
}

/**
 * Сброс игры в начальное состояние
 */
void Game::resetGame() {
  field.resetField();
  snake = Snake();
  addApple();
  appleEaten = 0;
  playing = PLAYING;
  isBoosted = 0;
  boostFactor = 1.5;
  resetInfo();
  lastActionTime = std::chrono::steady_clock::now();
}

/**
 * Перемещение змейки на заданное количество клеток
 *
 * @param dx смещение по горизонтали
 * @param dy смещение по вертикали
 */
void Game::move(int dx, int dy) {
  std::deque<std::pair<int, int>>& body = snake.getBody();
  std::pair<int, int> newHead = snake.getHead();
  newHead.first += dx;
  newHead.second += dy;
  body.push_front(newHead);
  if (!appleCollision()) body.pop_back();
  snake.setLastDirection(snake.getDirection());
}

/**
 * Движение змейки влево
 */
void Game::moveLeft() { move(-1, 0); }

/**
 * Движение змейки вправо
 */
void Game::moveRight() { move(1, 0); }

/**
 * Движение змейки вверх
 */
void Game::moveUp() { move(0, -1); }

/**
 * Движение змейки вниз
 */
void Game::moveDown() { move(0, 1); }

/**
 * Добавление змейки на поле
 */
void Game::addSnake() {
  std::deque<std::pair<int, int>>& body = snake.getBody();
  for (size_t i = 0; i < body.size(); i++) {
    int x = body[i].first;
    int y = body[i].second;
    if (field.isInside(x, y)) {
      if (i == 0)
        field.setBlock(x, y, 2);
      else
        field.setBlock(x, y, 1);
    }
  }
}

/**
 * Добавление яблока на поле в случайное место
 */
void Game::addApple() {
  appleEaten = 0;
  std::vector<std::pair<int, int>> emptyBlocks;
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++)
      if (field.getBlock(j, i) == 0) emptyBlocks.push_back({j, i});
  int randomIndex = std::rand() % emptyBlocks.size();
  auto& block = emptyBlocks[randomIndex];
  apple = block;
  field.setBlock(block.first, block.second, 3);
}

/**
 * Столкновения змейки с границами поля и самой собой
 *
 * @return true - произошло столкновение
 * @return false - не произошло столкновение
 */
bool Game::snakeCollision() {
  std::deque<std::pair<int, int>>& body = snake.getBody();
  std::pair<int, int> head = snake.getHead();
  bool collided = 0;
  if (!field.isInside(head.first, head.second)) collided = 1;
  for (auto i = body.begin() + 1; i != body.end(); i++)
    if (head.first == i->first && head.second == i->second) collided = 1;
  return collided;
}

/**
 * Столкновения змейки с яблоком
 *
 * @return true - яблоко съедено
 * @return false - яблоко не съедено
 */
bool Game::appleCollision() {
  std::pair<int, int> head = snake.getHead();
  bool collided = 0;
  if (head.first == apple.first && head.second == apple.second) {
    collided = 1;
    appleEaten = 1;
  }
  return collided;
}

/**
 * Обновление положения змейки в зависимости от текущего направления
 */
void Game::updateSnake() {
  switch (snake.getDirection()) {
    case Snake::Direction::Left:
      moveLeft();
      break;
    case Snake::Direction::Right:
      moveRight();
      break;
    case Snake::Direction::Up:
      moveUp();
      break;
    case Snake::Direction::Down:
      moveDown();
      break;
  }
  addSnake();
}

/**
 * Инициализация рекордного счета из файла
 */
void Game::initHighScore() {
  std::ifstream file("snake_high_score.bin", std::ios::binary);
  if (file) {
    file.read(reinterpret_cast<char*>(&info.high_score), sizeof(int));
    if (!file) info.high_score = 0;
    file.close();
  } else {
    writeHighScore("snake_high_score.bin", info.high_score);
  }
}

/**
 * Запись рекорда в файл
 *
 * @param filename имя файла
 * @param high_score рекорд
 */
void Game::writeHighScore(const std::string& filename, int high_score) {
  std::ofstream file(filename, std::ios::binary);
  if (file) {
    file.write(reinterpret_cast<const char*>(&high_score), sizeof(int));
    file.close();
  }
}

/**
 * Сравнение рекордов
 */
void Game::compareHighScores() {
  if (info.high_score < info.score) info.high_score = info.score;

  int prevHighScore = 0;
  std::ifstream file("snake_high_score.bin", std::ios::binary);
  if (file) {
    file.read(reinterpret_cast<char*>(&prevHighScore), sizeof(int));
    file.close();
  }
  if (prevHighScore < info.high_score)
    writeHighScore("snake_high_score.bin", info.high_score);
}

/**
 * Просчёт такта игрового цикла
 */
void Game::calculateTurn() {
  appleEaten = 0;
  info.score += 1;
  compareHighScores();
  // Когда игрок набирает 5 очков, уровень увеличивается на 1
  info.speed = info.speed > 10 ? 10 : info.score / 5 + 1;
  info.level = info.speed;
  if (snake.getLength() == 200) playing = WIN;
  addApple();
}

// ----------Other----------

/**
 * Обработка действий пользователя
 *
 * @param action действие пользователя
 * @param hold зажатие клавиши
 */
void userInput(UserAction_t action, bool hold) {
  Game& game = Game::getGame();
  GameInfo_t gameInfo = game.getGameInfo();
  if (action != Terminate && gameInfo.pause) {
    if (action == Pause) game.setPause(0);
    return;
  }
  switch (action) {
    case Left:
      game.getSnake().setDirection(Snake::Direction::Left);
      break;
    case Right:
      game.getSnake().setDirection(Snake::Direction::Right);
      break;
    case Up:
      game.getSnake().setDirection(Snake::Direction::Up);
      break;
    case Down:
      game.getSnake().setDirection(Snake::Direction::Down);
      break;
    case Action:
      game.setBoost();
      break;
    case Pause:
      game.setPause(1);
      break;
    case Terminate:
      game.setPlaying(GAMEOVER);
      game.setPause(0);
      game.compareHighScores();
      break;
    default:
      break;
  }
  hold = !hold;
}

/**
 * Получение задержки кадра
 */
long* s21::getFrameDelayLeft() {
  static long frameDelayLeft = FRAME_DELAY_NANO;
  return (long*)&frameDelayLeft;
}

/**
 * Обновление состояния игры
 */
GameInfo_t updateCurrentState() {
  Game& game = Game::getGame();
  GameInfo_t gameInfo = game.getGameInfo();

  if (gameInfo.pause) return gameInfo;

  auto curTime = std::chrono::steady_clock::now();
  auto timeDifference = curTime - game.getLastActionTime();

  double speed = game.getInfoSpeed();
  if (game.getBoost()) speed *= game.getBoostFactor();

  auto frameDelayLeft = std::chrono::nanoseconds(*getFrameDelayLeft()) / speed;

  if (std::chrono::duration_cast<std::chrono::nanoseconds>(timeDifference) >=
      frameDelayLeft) {
    game.resetSnake();
    game.updateSnake();
    if (game.getAppleEaten()) game.calculateTurn();
    if (game.snakeCollision()) game.setPlaying(GAMEOVER);
    game.setLastActionTime(curTime);
  }

  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++)
      gameInfo.field[i][j] = game.getField().getBlock(j, i);

  return gameInfo;
}