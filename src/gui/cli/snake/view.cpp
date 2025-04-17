#include "view.h"

using namespace s21;

View::View(Controller& controller) : controller_(controller) {}

void View::drawGame() {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) {
      int color = 1;
      if (controller_.getModel().getGameInfo().field[i][j])
        color = controller_.getModel().getGameInfo().field[i][j] + 9;
      attron(COLOR_PAIR(color));
      mvaddch(i + SNAKE_Y, j * 2 + SNAKE_X, ' ');
      mvaddch(i + SNAKE_Y, j * 2 + SNAKE_X + 1, ' ');
      attroff(COLOR_PAIR(color));
    }
}

void View::drawInterfaceExtras() {
  attron(COLOR_PAIR(2));
  mvprintw(GAME_INFO_Y + 1, GAME_INFO_X, "Score: %d",
           controller_.getModel().getGameInfo().score);
  mvprintw(GAME_INFO_Y + 2, GAME_INFO_X, "High Score: %d",
           controller_.getModel().getGameInfo().high_score);
  mvprintw(GAME_INFO_Y + 3, GAME_INFO_X, "Level: %d",
           controller_.getModel().getGameInfo().level);
  mvprintw(GAME_INFO_Y + 4, GAME_INFO_X, "Speed: %d %s",
           controller_.getModel().getGameInfo().speed,
           controller_.getModel().getBoost() ? "Boosted" : "       ");
  mvprintw(GAME_INFO_Y + 5, GAME_INFO_X, "Length: %d",
           controller_.getModel().getSnake().getLength());
  mvprintw(1, 1, "%s",
           controller_.getModel().getGameInfo().pause ? "PAUSE" : "     ");
  attroff(COLOR_PAIR(2));
}

void View::drawBorder(int x, int y, int width, int height) {
  attron(COLOR_PAIR(4));
  for (int i = x; i < width + 2 + x; i++)
    mvaddch(y, i, ' ');  // верхняя граница
  for (int i = x; i < width + 2 + x; i++)
    mvaddch(y + height + 1, i, ' ');  // нижняя граница
  for (int i = y; i < height + 2 + y; i++) mvaddch(i, x, ' ');  // левая граница
  for (int i = y; i < height + 2 + y; i++)
    mvaddch(i, x + width + 1, ' ');  // правая граница
  attroff(COLOR_PAIR(4));
}

void View::drawBorders() {
  drawBorder(SNAKE_X - 1, SNAKE_Y - 1, FIELD_WIDTH * 2, FIELD_HEIGHT);  // поле
  drawBorder(25 + SNAKE_X - 1, GAME_INFO_Y, 20, 5);  // игровая информация
}