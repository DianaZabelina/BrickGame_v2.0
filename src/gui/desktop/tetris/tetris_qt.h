#ifndef TETRIS_QT_H
#define TETRIS_QT_H

#include <QKeyEvent>
#include <QLabel>
#include <QPainter>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

#ifdef __cplusplus
extern "C" {
#endif
#include "../../../brick_game/tetris/tetris.h"
#ifdef __cplusplus
}
#endif

#define TETRIS_X 10
#define TETRIS_Y 5

#define GAME_INFO_X TETRIS_X + 15
#define GAME_INFO_Y TETRIS_Y

namespace s21 {

class Menu : public QWidget {
  Q_OBJECT

 public:
  explicit Menu(QWidget *parent = nullptr);

 private:
  QLabel *titleLabel;
  QLabel *menuLabel;
  QLabel *controlLabel;
};

class Tetris : public QWidget {
  Q_OBJECT

 public:
  Tetris(QWidget *parent = nullptr);
  ~Tetris();

 protected:
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *e) override;

 private:
  Menu *menu;
  bool gameIsRunning;
  GameInfo_t gameInfo;
  int cellSize = 20;

  void startGame();
  void endGame();

  void drawBorders(QPainter &painter);
  void setGameInfo(GameInfo_t gi);
  QColor getColorForBlock(int block);
  void drawGame(QPainter &painter);
  void drawNextFigure(QPainter &painter);
  void drawInterfaceExtras(QPainter &painter);

  friend class Controller;
};

class Controller : public QObject {
  Q_OBJECT

 public:
  // Удаленные функции гарантируют, что нельзя создать второй экземпляр
  // контроллера
  Controller() = delete;
  Controller(QObject *parent = nullptr) = delete;
  Controller(const Controller &) = delete;
  void operator=(const Controller &) = delete;
  // Статический метод предоставляет глобальный доступ к единственному
  // экземпляру контроллера
  static Controller *getController(Tetris *t);
  void processUserInput(int key, bool hold);
  void resetTetris();

 public slots:
  void updateGame();

 private:
  Controller(Tetris *t);
  ~Controller();

  Tetris *tetris;
  QTimer *timer;
};
}  // namespace s21
#endif  // TETRIS_QT_H
