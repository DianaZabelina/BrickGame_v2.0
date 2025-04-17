#ifndef VIEW_H
#define VIEW_H

#include <ncurses.h>

#include "controller.h"

#define SNAKE_X 10
#define SNAKE_Y 5

#define GAME_INFO_X SNAKE_X + 25
#define GAME_INFO_Y SNAKE_Y - 1

namespace s21 {

class View {
 public:
  View(Controller& controller);
  Controller& getController() { return controller_; }
  void drawGame();
  void drawInterfaceExtras();
  void drawBorder(int x, int y, int width, int height);
  void drawBorders();

 private:
  Controller& controller_;
};

}  // namespace s21
#endif  // VIEW_H