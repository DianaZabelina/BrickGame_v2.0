#include <ctime>

#include "view.h"

#define COLOR_DARK_GREEN 11

#define GAME_BACKGROUND_COLOR 20
#define TEXT_COLOR COLOR_WHITE
#define MENU_BACKGROUND_COLOR COLOR_BLACK

#define ENTER 10
#define ESCAPE 27

void configureInterface() {
  cbreak();  // переключаем режим для немедленного ввода символов, ctrl+c можно
             // использовать для прерывания
  noecho();  // выключаем отображение вводимых символов, нужно для getch()
  keypad(stdscr, TRUE);  // включаем режим чтения функциональных клавиш
  curs_set(0);  // устанавливаем невидимость курсора
  start_color();  // включаем поддержку цветов
}

void initInterface() {
  initscr();  // инициализируем библиотеку ncurses
  configureInterface();  // настраиваем конфигурацию интерфейса
}

void initGameInterface() {
  init_color(GAME_BACKGROUND_COLOR, 150, 150, 150);
  init_pair(1, TEXT_COLOR, GAME_BACKGROUND_COLOR);
  init_pair(2, TEXT_COLOR, COLOR_BLACK);
  init_pair(4, 0, COLOR_WHITE);
  init_pair(10, 0, COLOR_GREEN);
  init_color(COLOR_DARK_GREEN, 0, 500, 0);
  init_pair(11, 0, COLOR_DARK_GREEN);
  init_pair(12, 0, COLOR_RED);
}

void startGame() {
  initGameInterface();
  refresh();
  std::srand(std::time(0));
  s21::Controller controller;
  s21::View view(controller);
  halfdelay(1);
  while (view.getController().getModel().getPlaying() == s21::PLAYING) {
    int i = getch();
    view.getController().handleUserInput(i);
    updateCurrentState();
    view.drawGame();
    view.drawInterfaceExtras();
    view.drawBorders();
    refresh();
  }
  view.getController().getModel().resetGame();
  cbreak();
}

void createMenuInterface() {
  init_pair(3, TEXT_COLOR, MENU_BACKGROUND_COLOR);

  WINDOW* menu = newwin(4, 25, 2, 10);
  WINDOW* control = newwin(9, 25, 6, 10);
  box(menu, 0, 0);
  box(control, 0, 0);
  mvwprintw(menu, 0, 9, "Snake");
  mvwprintw(control, 0, 9, "Control");
  attron(COLOR_PAIR(3));
  mvwprintw(menu, 1, 2, "Press ENTER to start");
  mvwprintw(menu, 2, 2, "Press ESCAPE to exit");
  mvwprintw(control, 1, 2, "W      move up");
  mvwprintw(control, 2, 2, "A      move left");
  mvwprintw(control, 3, 2, "S      move down");
  mvwprintw(control, 4, 2, "D      move right");
  mvwprintw(control, 5, 2, "E      boost speed");
  mvwprintw(control, 6, 2, "SPACE  to pause");
  mvwprintw(control, 7, 2, "ESCAPE to finish");
  attroff(COLOR_PAIR(3));
  refresh();
  wrefresh(menu);
  wrefresh(control);
  refresh();
  delwin(menu);
  delwin(control);
}

int main() {
  initInterface();
  refresh();
  createMenuInterface();
  bool exit = 0;
  while (!exit) {
    switch (getch()) {
      case ENTER:
        clear();
        startGame();
        clear();
        createMenuInterface();
        break;
      case ESCAPE:
        exit = 1;
        break;
      default:
        break;
    }
  }
  endwin();
  return 0;
}