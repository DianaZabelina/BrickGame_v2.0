#include "controller.h"

using namespace s21;

Controller::Controller() : model_(s21::Game::getGame()) {}

void Controller::handleUserInput(int input, bool hold) {
  switch (input) {
    case Qt::Key_W:
      userInput(Up, hold);
      break;
    case Qt::Key_S:
      userInput(Down, hold);
      break;
    case Qt::Key_A:
      userInput(Left, hold);
      break;
    case Qt::Key_D:
      userInput(Right, hold);
      break;
    case Qt::Key_E:
      userInput(Action, hold);
      break;
    case Qt::Key_Space:
      userInput(Pause, hold);
      break;
    case Qt::Key_Escape:
      userInput(Terminate, hold);
      break;
    default:
      break;
  }
}
