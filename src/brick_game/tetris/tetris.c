#include "tetris.h"

/**
 * Шаблоны фигур
 *
 * @param i номер фигуры
 *
 * @return массив с формой фигуры
 */
char* getFigureFromTemplate(int i) {
  static char figureTemplates[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
                                   1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                                   0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 2, 2,
                                   2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                                   0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 3, 3,
                                   3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                                   0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 4, 4,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                                   0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 5, 5,
                                   0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                                   0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 6,
                                   6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

                                   0, 0, 0, 0, 0, 0, 7, 7, 0, 0, 0, 0, 7,
                                   7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  return (figureTemplates) + (i * FIGURE_AREA);
}

/**
 * Создание игрового поля
 *
 * @return новая структура Field
 */
Field* createField() {
  Field* field = (Field*)malloc(sizeof(Field));
  field->width = FIELD_WIDTH;
  field->height = FIELD_HEIGHT;
  field->blocks = (char*)calloc(FIELD_WIDTH * FIELD_HEIGHT, sizeof(char));
  return field;
}

/**
 * Создание фигуры
 *
 * @return новая структура Figure
 */
Figure* createFigure() {
  Figure* figure = (Figure*)calloc(1, sizeof(Figure));
  figure->size = FIGURE_SIZE;
  figure->blocks = (char*)calloc(1, FIGURE_SIZE * FIGURE_SIZE * sizeof(char));
  return figure;
}

/**
 * Создание следующей фигуры
 */
Figure* createNextFigure() {
  Figure* figure = createFigure();
  figure->x = FIELD_WIDTH / 2 - figure->size / 2;
  figure->y = 0;
  int figureNumber = rand() % FIGURES_COUNT;
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      figure->blocks[i * FIGURE_SIZE + j] =
          getFigureFromTemplate(figureNumber)[i * FIGURE_SIZE + j];
  return figure;
}

/**
 * Очищение фигуры
 *
 * @return освобожденная структура Figure
 */
Figure* freeFigure(Figure* figure) {
  if (figure) {
    free(figure->blocks);
    figure->blocks = 0;
    free(figure);
    figure = 0;
  }
  return figure;
}

/**
 * Получение следующей фигуры
 *
 * @param create создание фигуры
 */
Figure* nextFigure(bool create) {
  static Figure* figure = 0;
  if (create) {
    freeFigure(figure);
    figure = createNextFigure();
  }
  return figure;
}

/**
 * Добавление фигуры
 *
 * @param figure структура фигуры
 */
Figure* dropFigure(Figure* figure) {
  Figure* newFigure = createFigure();
  newFigure->x = figure->x;
  newFigure->y = figure->y;
  for (int i = 0; i < FIGURE_SIZE * FIGURE_SIZE; i++)
    newFigure->blocks[i] = figure->blocks[i];
  return newFigure;
}

/**
 * Создание игры
 *
 * @return новая структура Game
 */
Game* createGame() {
  Game* game = (Game*)calloc(1, sizeof(Game));
  game->field = createField();
  game->playing = PLAYING;
  game->speed = 1;
  game->figure = dropFigure(nextFigure(1));
  return game;
};

/**
 * Получение игры
 */
Game* getGame() {
  static Game* game = 0;
  if (game == 0) {
    game = createGame();
    FILE* file = fopen("tetris_high_score.bin", "rb");
    if (file) {
      size_t readed = fread(&game->high_score, sizeof(int), 1, file);
      if (readed == 0) game->high_score = 0;
      fclose(file);
    } else {
      file = fopen("tetris_high_score.bin", "wb");
      fwrite(&game->high_score, sizeof(int), 1, file);
      fclose(file);
    }
  }
  return game;
}

/**
 * Получение игровой информации
 */
GameInfo_t* getGameInfo() {
  static GameInfo_t* gameInfo = 0;
  if (gameInfo == 0) {
    gameInfo = calloc(1, sizeof(GameInfo_t));
    int* field = calloc(1, sizeof(int) * FIELD_WIDTH * FIELD_HEIGHT);
    gameInfo->field = calloc(FIELD_HEIGHT, sizeof(int*));
    for (int row = 0; row < FIELD_HEIGHT; row++)
      gameInfo->field[row] = field + FIELD_WIDTH * row;
    gameInfo->next = calloc(1, sizeof(int*) * FIGURE_SIZE);
    Figure* figure = nextFigure(1);
    for (int i = 0; i < FIGURE_SIZE; i++) {
      gameInfo->next[i] = calloc(1, sizeof(int) * FIGURE_SIZE);
      for (int j = 0; j < FIGURE_SIZE; j++)
        gameInfo->next[i][j] = figure->blocks[i * FIGURE_SIZE + j];
    }
    gameInfo->level = 1;
    gameInfo->speed = 1;
  }
  return gameInfo;
}

/**
 * Обновление игровой информации о следующей фигуре
 */
void updateNextFigureInfo() {
  GameInfo_t* info = getGameInfo();
  Figure* figure = nextFigure(1);
  for (int i = 0; i < FIGURE_SIZE; i++)
    for (int j = 0; j < FIGURE_SIZE; j++)
      info->next[i][j] = figure->blocks[i * FIGURE_SIZE + j];
}

/**
 * Обнуление Singletones для Game и GameInfo_t
 */
void resetSingletones() {
  Game* game = getGame();
  GameInfo_t* info = getGameInfo();
  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) {
      game->field->blocks[i * FIELD_WIDTH + j] = 0;
      (*info->field)[i * FIELD_WIDTH + j] = 0;
    }
  freeFigure(game->figure);
  game->figure = dropFigure(nextFigure(0));
  updateNextFigureInfo();
  game->score = 0;
  game->playing = PLAYING;
  game->speed = 1;
  info->level = 1;
  info->speed = 1;
  info->score = 0;
}

/**
 * Очищение Singletones для Game и GameInfo_t
 */
void freeSingletones() {
  // Очищение Game
  free(getGame()->figure->blocks);
  free(getGame()->figure);
  free(getGame()->field->blocks);
  free(getGame()->field);
  free(getGame());
  // Очищение GameInfo
  free(*getGameInfo()->field);
  free(getGameInfo()->field);
  for (int i = 0; i < FIGURE_SIZE; i++) {
    free(getGameInfo()->next[i]);
  }
  free(getGameInfo()->next);
  free(getGameInfo());
  // Очищение NextFigure
  freeFigure(nextFigure(0));
}

/**
 * Падение фигуры
 */
void moveFigureDown() {
  Game* game = getGame();
  game->figure->y++;
}

/**
 * Движение фигуры вверх
 */
void moveFigureUp() {
  Game* game = getGame();
  game->figure->y--;
}

/**
 * Движение фигуры вправо
 */
void moveFigureRight() {
  Game* game = getGame();
  game->figure->x++;
}

/**
 * Движение фигуры влево
 */
void moveFigureLeft() {
  Game* game = getGame();
  game->figure->x--;
}

/**
 * Столкновения фигуры с границами поля и непустыми блоками
 *
 * @return 0 - OK
 * @return 1 - произошло столкновение
 */
char figureCollision() {
  Game* game = getGame();
  Figure* figure = game->figure;
  Field* field = game->field;
  bool collided = 0;
  for (int i = 0; i < figure->size && !collided; i++)
    for (int j = 0; j < figure->size && !collided; j++)
      if (figure->blocks[i * figure->size + j]) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        if (fx < 0 || fx >= field->width || fy >= field->height ||
            field->blocks[fx + field->width * fy]) {
          collided = 1;
        }
      }
  return collided;
}

/**
 * Определение положения фигуры после падения
 */
void plantFigure() {
  Game* game = getGame();
  Figure* figure = game->figure;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      if (figure->blocks[i * figure->size + j]) {
        int fx = figure->x + j;
        int fy = figure->y + i;
        game->field->blocks[fy * game->field->width + fx] =
            figure->blocks[i * figure->size + j];
      }
}

/**
 * Проверка заполнения строки
 *
 * @param i номер клетки
 * @param field структура поля
 *
 * @return 0 - неполная
 * @return 1 - заполненная
 */
char lineIsFull(int i, Field* field) {
  bool isFull = 1;
  for (int j = 0; j < field->width && isFull; j++)
    if (!field->blocks[i * field->width + j]) isFull = 0;
  return isFull;
}

/**
 * Сдвиг строки
 */
void shiftLine(int i, Field* field) {
  for (int k = i; k >= 1; k--)
    for (int j = 0; j < field->width; j++) {
      int currentBlock = k * field->width + j;
      field->blocks[currentBlock] = field->blocks[currentBlock - field->width];
    }
  for (int i = 0; i < field->width; i++) field->blocks[i] = 0;
}

/**
 * Удаление заполненных строк и начисление очков
 *
 * @return 100 очков за 1 линию
 * @return 300 очков за 2 линии
 * @return 700 очков за 3 линии
 * @return 1500 очков за 4 линии
 */
int eraseLines() {
  Game* game = getGame();
  Field* field = game->field;
  int count = 0;
  for (int i = field->height - 1; i >= 0; i--)
    while (lineIsFull(i, field)) {
      shiftLine(i, field);
      count++;
    }
  switch (count) {
    case 1:
      count = 100;
      break;
    case 2:
      count = 300;
      break;
    case 3:
      count = 700;
      break;
    case 4:
      count = 1500;
      break;
    default:
      break;
  }
  return count;
}

/**
 * Вращение фигуры
 */
Figure* rotateFigure() {
  Game* game = getGame();
  Figure* figure = createFigure();
  Figure* oldFigure = game->figure;
  figure->x = oldFigure->x;
  figure->y = oldFigure->y;
  for (int i = 0; i < figure->size; i++)
    for (int j = 0; j < figure->size; j++)
      figure->blocks[(FIGURE_SIZE - j - 1) * FIGURE_SIZE + i] =
          oldFigure->blocks[i * FIGURE_SIZE + j];
  return figure;
}

/**
 * Сравнение рекордов
 */
void compareHighScores() {
  Game* game = getGame();
  if (game->high_score < game->score) game->high_score = game->score;
  int prevHighScore = 0;
  FILE* file = fopen("tetris_high_score.bin", "rb");
  if (file) {
    size_t readed = fread(&prevHighScore, sizeof(int), 1, file);
    fclose(file);
    if (readed == 1 && prevHighScore < game->high_score) {
      file = fopen("tetris_high_score.bin", "wb");
      fwrite(&game->high_score, sizeof(int), 1, file);
      fclose(file);
    }
  }
}

/**
 * Просчёт такта игрового цикла
 */
void calculateTurn() {
  Game* game = getGame();
  plantFigure();
  game->score += eraseLines();
  compareHighScores();
  // Когда игрок набирает 600 очков, уровень увеличивается на 1
  game->speed = game->speed > 10 ? 10 : game->score / 600 + 1;
  getGameInfo()->level = game->speed;
  getGameInfo()->speed = game->speed;

  freeFigure(game->figure);
  game->figure = dropFigure(nextFigure(0));
  updateNextFigureInfo();

  if (figureCollision()) game->playing = GAMEOVER;
}

/**
 * Обработка действий пользователя
 *
 * @param action действие пользователя
 * @param hold зажатие клавиши
 */
void userInput(UserAction_t action, bool hold) {
  if (action != Terminate && getGameInfo()->pause) {
    if (action == Pause) getGameInfo()->pause = 0;
    return;
  }
  Game* game = getGame();
  Figure* rotated = NULL;
  switch (action) {
    case Right:
      moveFigureRight();
      if (figureCollision()) moveFigureLeft();
      break;
    case Left:
      moveFigureLeft();
      if (figureCollision()) moveFigureRight();
      break;
    case Down:
      while (!figureCollision()) moveFigureDown();
      moveFigureUp();
      calculateTurn();
      break;
    case Action:
      rotated = rotateFigure();
      Figure* old = game->figure;
      game->figure = rotated;
      if (figureCollision()) {
        freeFigure(rotated);
        game->figure = old;
      } else {
        freeFigure(old);
      }
      break;
    case Pause:
      getGameInfo()->pause = 1;
      break;
    case Terminate:
      game->playing = GAMEOVER;
      getGameInfo()->pause = 0;
      compareHighScores();
      break;
    default:
      break;
  }
  hold = !hold;
}

/**
 * Получение задержки кадра
 */
long* getFrameDelayLeft() {
  static long frameDelayLeft = FRAME_DELAY_NANO;
  return (long*)&frameDelayLeft;
}

/**
 * Разница времени старта и окончания кадра
 *
 * @param start структура времени старта
 * @param end структура времени окончания
 *
 * @return разница времени в наносекундах
 */
long timeDifference(const struct timespec* start, const struct timespec* end) {
  long difference = end->tv_nsec - start->tv_nsec;
  if (end->tv_sec - start->tv_sec) difference += 1000000000;
  return difference;
}

/**
 * Обновление состояния игры
 */
GameInfo_t updateCurrentState() {
  if (getGameInfo()->pause) return *getGameInfo();
  static struct timespec sp_start, sp_end;
  clock_gettime(CLOCK_MONOTONIC, &sp_start);

  Game* game = getGame();
  GameInfo_t* info = getGameInfo();
  for (int i = 0; i < FIELD_HEIGHT * FIELD_WIDTH; i++) (*info->field)[i] = 0;
  if (*getFrameDelayLeft() < 0) {
    moveFigureDown();
    if (figureCollision()) {
      moveFigureUp();
      calculateTurn();
    }
  }

  Field* tf = game->field;
  Figure* t = game->figure;

  for (int i = 0; i < FIELD_HEIGHT; i++)
    for (int j = 0; j < FIELD_WIDTH; j++) {
      if (tf->blocks[i * FIELD_WIDTH + j]) {
        (*info->field)[i * tf->width + j] = tf->blocks[i * FIELD_WIDTH + j];
      } else {
        int x = j - t->x;
        int y = i - t->y;
        if (x >= 0 && x < t->size && y >= 0 && y < t->size &&
            t->blocks[y * t->size + x] != 0) {
          (*info->field)[i * FIELD_WIDTH + j] = t->blocks[y * t->size + x];
        }
      }
    }

  clock_gettime(CLOCK_MONOTONIC, &sp_end);
  if (*getFrameDelayLeft() < 0)
    *getFrameDelayLeft() =
        FRAME_DELAY_NANO / (game->speed * 0.2) + *getFrameDelayLeft();
  *getFrameDelayLeft() -= timeDifference(&sp_start, &sp_end);

  return *info;
}
