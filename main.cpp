#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
  qApp->setStyle(QStyleFactory::create("Fusion"));

  MainWindow w;
  w.show();
  return a.exec();
}
