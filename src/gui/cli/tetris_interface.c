#include <ncurses.h>

#include "../../brick_game/tetris/tetris.h"

#define COLOR_ORANGE 11
#define COLOR_BRIGHT_YELLOW 12
#define COLOR_PINK 13
#define COLOR_DARK_BLUE 15

#define GAME_BACKGROUND_COLOR 20
#define TEXT_COLOR COLOR_WHITE
#define MENU_BACKGROUND_COLOR COLOR_BLACK

#define TETRIS_X 10
#define TETRIS_Y 5

#define GAME_INFO_X TETRIS_X + 25
#define GAME_INFO_Y TETRIS_Y - 1

#define ENTER 10
#define ESCAPE 27
#define SPACE 32

void configureInterface() {
  cbreak();  // переключаем режим для немедленного ввода символов, ctrl+c можно
             // использовать для прерывания
  noecho();  // выключаем отображение вводимых символов, нужно для getch()
  nodelay(stdscr, TRUE);  // включаем неблокирующий ввод
  keypad(stdscr, TRUE);  // включаем режим чтения функциональных клавиш
  scrollok(stdscr, TRUE);  // включаем прокручивание
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
  // Цвета фигур
  init_pair(10, 0, COLOR_RED);
  init_color(COLOR_ORANGE, 900, 450, 0);
  init_pair(11, 0, COLOR_ORANGE);
  init_color(COLOR_BRIGHT_YELLOW, 900, 900, 0);
  init_pair(12, 0, COLOR_BRIGHT_YELLOW);
  init_color(COLOR_PINK, 1000, 686, 843);
  init_pair(13, 0, COLOR_PINK);
  init_pair(14, 0, COLOR_GREEN);
  init_color(COLOR_DARK_BLUE, 0, 0, 700);
  init_pair(15, 0, COLOR_DARK_BLUE);
  init_pair(16, 0, COLOR_MAGENTA);
}

void drawBorder(int x, int y, int width, int height) {
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

void drawBorders() {
  drawBorder(TETRIS_X - 1, TETRIS_Y - 1, FIELD_WIDTH * 2,
             FIELD_HEIGHT);                           // поле
  drawBorder(25 + TETRIS_X - 1, GAME_INFO_Y, 20, 5);  // игровая информация
  drawBorder(29 + TETRIS_X, GAME_INFO_Y + 10, 10, 5);  // следующая фигура
}

void drawInterfaceExtras() {
  attron(COLOR_PAIR(2));
  mvprintw(GAME_INFO_Y + 1, GAME_INFO_X, "Score: %d", getGame()->score);
  mvprintw(GAME_INFO_Y + 2, GAME_INFO_X, "High Score: %d",
           getGame()->high_score);
  mvprintw(GAME_INFO_Y + 3, GAME_INFO_X, "Level: %d", getGameInfo()->level);
  mvprintw(GAME_INFO_Y + 4, GAME_INFO_X, "Speed: %d", getGameInfo()->speed);
  mvprintw(1, 1, "%s", getGameInfo()->pause ? "PAUSE" : "     ");
  attroff(COLOR_PAIR(2));

  GameInfo_t* info = getGameInfo();
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++) {
      int ch = info->next[i][j];
      attron(COLOR_PAIR(ch == 0 ? 1 : info->next[i][j] + 9));
      mvaddch(GAME_INFO_Y + 11 + i, 30 + j * 2 + TETRIS_X, ' ');
      mvaddch(GAME_INFO_Y + 11 + i, 30 + j * 2 + TETRIS_X + 1, ' ');
      attroff(COLOR_PAIR(ch == 0 ? 1 : info->next[i][j] + 9));
    }
}

void drawGame(GameInfo_t* info) {
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) {
      int color = 1;
      if (info->field[i][j]) color = info->field[i][j] + 9;
      attron(COLOR_PAIR(color));
      mvaddch(i + TETRIS_Y, j * 2 + TETRIS_X, ' ');
      mvaddch(i + TETRIS_Y, j * 2 + TETRIS_X + 1, ' ');
      attroff(COLOR_PAIR(color));
    }
}

void startGame() {
  initGameInterface();
  refresh();
  while (getGame()->playing) {
    switch (getch()) {
      case 'w':
        userInput(Action, 0);
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
      case SPACE:
        userInput(Pause, 0);
        break;
      case ESCAPE:
        userInput(Terminate, 0);
        break;
      default:
        break;
    }
    GameInfo_t info = updateCurrentState();
    drawGame(&info);
    drawInterfaceExtras();
    drawBorders();
    refresh();
  }
  resetSingletones();
}

void createMenuInterface() {
  init_pair(3, TEXT_COLOR, MENU_BACKGROUND_COLOR);

  WINDOW* menu = newwin(4, 25, 2, 10);
  WINDOW* control = newwin(8, 25, 6, 10);
  box(menu, 0, 0);
  box(control, 0, 0);
  mvwprintw(menu, 0, 9, "Tetris");
  mvwprintw(control, 0, 9, "Control");
  attron(COLOR_PAIR(3));
  mvwprintw(menu, 1, 2, "Press ENTER to start");
  mvwprintw(menu, 2, 2, "Press ESCAPE to exit");
  mvwprintw(control, 1, 2, "W      rotate");
  mvwprintw(control, 2, 2, "A      move left");
  mvwprintw(control, 3, 2, "S      drop");
  mvwprintw(control, 4, 2, "D      move right");
  mvwprintw(control, 5, 2, "SPACE  to pause");
  mvwprintw(control, 6, 2, "ESCAPE to finish");
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
  freeSingletones();
  return 0;
}