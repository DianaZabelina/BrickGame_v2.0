#include <QApplication>

#include "controller.h"
#include "snake_qt.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  s21::Controller controller;
  s21::View view(controller);
  view.show();

  return a.exec();
}
