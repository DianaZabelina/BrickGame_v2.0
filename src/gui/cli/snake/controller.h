#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../../../brick_game/snake/snake.h"

#define ESCAPE 27
#define SPACE 32

namespace s21 {

class Controller {
 public:
  Controller();
  void handleUserInput(int input);
  Game& getModel() { return model_; }

 private:
  Game& model_;
};

}  // namespace s21
#endif  // CONTROLLER_H