#ifndef SNAKE_QT_H
#define SNAKE_QT_H

#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#include "../../../brick_game/snake/snake.h"
#include "controller.h"

#define SNAKE_X 10
#define SNAKE_Y 5

#define GAME_INFO_X SNAKE_X + 15
#define GAME_INFO_Y SNAKE_Y

namespace s21 {

class Menu : public QWidget {
  Q_OBJECT

 public:
  explicit Menu(QWidget* parent = nullptr);

 private:
  QLabel* titleLabel;
  QLabel* menuLabel;
  QLabel* controlLabel;
};

class View : public QWidget {
  Q_OBJECT

 public:
  View(Controller& controller, QWidget* parent = nullptr);
  void startGame();
  QColor getColorForBlock(int block);
  void updateGame();
  void drawGame(QPainter& painter);
  void drawInterfaceExtras(QPainter& painter);
  void drawBorders(QPainter& painter);

 protected:
  void keyPressEvent(QKeyEvent* event) override;
  void paintEvent(QPaintEvent* event) override;

 private:
  Menu* menu;
  Controller& controller_;
  QTimer* timer_;
  bool isGameRunning_;
  int cellSize = 20;
};

}  // namespace s21
#endif  // SNAKE_QT_H
