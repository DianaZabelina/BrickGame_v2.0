#include <QApplication>

#include "tetris_qt.h"

using namespace s21;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  Tetris tetris;
  tetris.show();
  return app.exec();
}
