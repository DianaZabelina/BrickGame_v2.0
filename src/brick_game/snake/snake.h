#ifndef SNAKE_H
#define SNAKE_H

#include <chrono>
#include <deque>
#include <fstream>
#include <stdexcept>
#include <vector>

#include "../library_specification.h"

#define FIELD_WIDTH 10
#define FIELD_HEIGHT 20

#define FRAME_DELAY_NANO 1000000000

#define START_X FIELD_WIDTH / 2
#define START_Y FIELD_HEIGHT / 2

namespace s21 {

class Snake {
 public:
  enum Direction { Left, Right, Up, Down };

  Snake();

  /**
   * Получение ссылки на тело змейки, представленное в виде двусторонней очереди
   * пар координат
   *
   * @return ссылка на тело змейки
   */
  std::deque<std::pair<int, int>>& getBody() { return body; }

  /**
   * Получение координат головы змейки
   *
   * @return координаты головы змейки
   */
  std::pair<int, int> getHead() const { return body.front(); }

  /**
   * Получение длины змейки
   *
   * @return длина змейки
   */
  size_t getLength() const { return body.size(); }

  /**
   * Получение текущего направления движения змейки
   *
   * @return направление движения
   */
  Direction getDirection() const { return direction; }

  /**
   * Получение последнего направления движения змейки
   *
   * @return последнее направление движения
   */
  Direction getLastDirection() const { return lastDirection; }

  bool isOpposite(Direction newDirection) const;
  void setDirection(Direction newDirection);

  /**
   * Установка последнего направления движения змейки
   *
   * @param newLastDirection новое последнее направление движения
   */
  void setLastDirection(Direction newLastDirection) {
    lastDirection = newLastDirection;
  }

 private:
  std::deque<std::pair<int, int>> body;
  Direction direction;
  Direction lastDirection;
};

class Field {
 public:
  Field();
  ~Field();

  /**
   * Получение ширины игрового поля
   */
  int getWidth() const { return width; }

  /**
   * Получение длины игрового поля
   */
  int getHeight() const { return height; }

  bool isInside(int x, int y) const;
  char getBlock(int x, int y) const;
  void setBlock(int x, int y, char value);
  void resetField();

 private:
  static const int width = FIELD_WIDTH;
  static const int height = FIELD_HEIGHT;
  char* blocks;
};

enum GAME_STATE { GAMEOVER, PLAYING, WIN };

class Game {
 public:
  Game(const Game&) = delete;
  Game(Game&&) = delete;
  Game& operator=(const Game&) = delete;
  Game& operator=(Game&&) = delete;
  static Game& getGame();

  void freeGameInfo();

  /**
   * Получение константной ссылки на объект игрового поля
   *
   * @return константная ссылка на игровое поле
   */
  const Field& getField() const { return field; }

  /**
   * Получение ссылки на объект змейки
   *
   * @return ссылка на змейку
   */
  Snake& getSnake() { return snake; }

  /**
   * Получение координат яблока
   *
   * @return координаты яблока
   */
  std::pair<int, int> getApple() const { return apple; }

  /**
   * Получение состояния яблока
   *
   * @return true - яблоко съедено
   * @return false - яблоко не съедено
   */
  bool getAppleEaten() const { return appleEaten; }

  /**
   * Получение текущего состояния игры (проиграна, идет, выиграна)
   *
   * @return текущее состояние игры
   */
  char getPlaying() const { return playing; }

  /**
   * Получение состояние ускорения
   *
   * @return true - ускорение активно
   * @return false - ускорение неактивно
   */
  bool getBoost() const { return isBoosted; }

  /**
   * Получение множителя ускорения
   *
   * @return множитель ускорения
   */
  double getBoostFactor() const { return boostFactor; }

  /**
   * Получение структуры с информацией о текущем состоянии игры
   *
   * @return структура с информацией об игре
   */
  GameInfo_t getGameInfo() const { return info; }

  /**
   * Получение текущей скорости игры
   *
   * @return скорость игры
   */
  int getInfoSpeed() const { return info.speed; }

  /**
   * Получение времени последнего действия
   *
   * @return время последнего действия
   */
  std::chrono::steady_clock::time_point getLastActionTime() const {
    return lastActionTime;
  }

  /**
   * Установка состояния игры
   *
   * @param newGameState новое состояние игры
   */
  void setPlaying(bool newGameState) { playing = newGameState; }

  /**
   * Переключение состояния ускорения
   */
  void setBoost() { isBoosted = !isBoosted; }

  /**
   * Установка состояния паузы
   *
   * @param newState новое состояние игры
   */
  void setPause(int newState) { info.pause = newState; }

  /**
   * Установка времени последнего действия
   *
   * @param newTime новое время последнего действия
   */
  void setLastActionTime(std::chrono::steady_clock::time_point newTime) {
    lastActionTime = newTime;
  }

  void resetSnake();
  void resetInfoField();
  void resetInfo();
  void resetGame();

  void move(int dx, int dy);
  void moveLeft();
  void moveRight();
  void moveUp();
  void moveDown();

  void addSnake();
  void addApple();
  bool snakeCollision();
  bool appleCollision();
  void updateSnake();

  void initHighScore();
  void writeHighScore(const std::string& filename, int high_score);
  void compareHighScores();
  void calculateTurn();

 private:
  Field field;
  Snake snake;
  std::pair<int, int> apple;
  bool appleEaten;
  char playing;
  bool isBoosted;
  double boostFactor;
  GameInfo_t info;
  std::chrono::time_point<std::chrono::steady_clock> lastActionTime;

  Game();
  ~Game();
};

long* getFrameDelayLeft();

}  // namespace s21
#endif  // SNAKE_H