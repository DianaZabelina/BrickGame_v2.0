#include "controller.h"

using namespace s21;

Controller::Controller() : model_(s21::Game::getGame()) {}

void Controller::handleUserInput(int input) {
  switch (input) {
    case 'w':
      userInput(Up, 0);
      break;
    case 's':
      userInput(Down, 0);
      break;
    case 'a':
      userInput(Left, 0);
      break;
    case 'd':
      userInput(Right, 0);
      break;
    case 'e':
      userInput(Action, 0);
      break;
    case SPACE:
      userInput(Pause, 0);
      break;
    case ESCAPE:
      userInput(Terminate, 0);
      break;
    default:
      break;
  }
}