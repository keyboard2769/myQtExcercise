/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   main.cpp
 * Author: keypad
 *
 * Created on 2019/02/01, 19:15
 */

#include <QApplication>
#include "MainWindow.h"

void *_Unwind_Resume = 0;
void *__gxx_personality_v0 = 0;

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow::ccGetReference()->show();
  return app.exec();
}//***eof
