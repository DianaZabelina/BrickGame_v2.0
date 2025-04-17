#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QKeyEvent>

#include "../../../brick_game/library_specification.h"
#include "../../../brick_game/snake/snake.h"

namespace s21 {

class Controller {
 public:
  Controller();
  void handleUserInput(int input, bool hold);
  Game& getModel() { return model_; }

 private:
  Game& model_;
};

}  // namespace s21

#endif  // CONTROLLER_H
