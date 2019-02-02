/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainLoop.h
 * Author: keypad
 *
 * Created on 2019/02/01, 19:37
 */

#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <QObject>
#include "MainCoordinator.h"
#include "inner.cpp"

//QT_BEGIN_NAMESPACE
//QT_END_NAMESPACE

class MainLoop : public QObject{
  Q_OBJECT
  
public:
  static MainLoop* ccGetReference();
  MainCoordinator* cmCoordinator;
  //--
  bool dcCLMV,dcOPMV;
  bool dcCLAS,dcMMAS,dcOPAS;
  ZcRangedValueModel *dcGate;
  //--
  int ccGetRoller();
  
public slots:
  void ccLoop();
  
private:
  static MainLoop* self;
  MainLoop();
  
  //-- primitive
  int cmRoller;

};

#endif /* MAINLOOP_H */

