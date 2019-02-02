/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainWindow.h
 * Author: keypad
 *
 * Created on 2019/02/01, 19:15
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "MainLoop.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QWidget;
QT_END_NAMESPACE

class MainWindow : public QWidget {
  Q_OBJECT

public:
  static MainWindow* ccGetReference();
  MainLoop* cmLoop;
  
private slots:
  void ccGateClose();
  void ccGateStop();
  void ccGateOpen();
  void ccExit();
  
private:
  static MainWindow* self;
  MainWindow();

};

#endif /* MAINWINDOW_H */

