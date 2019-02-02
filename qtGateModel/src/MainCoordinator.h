/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainCoordinator.h
 * Author: keypad
 *
 * Created on 2019/02/01, 19:44
 */

#ifndef MAINCOORDINATOR_H
#define MAINCOORDINATOR_H

#include <QObject>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
QT_END_NAMESPACE

class MainCoordinator {

public:
  static MainCoordinator* ccGetReference();
  static void ccSetActivated(QLabel*, bool);
  
  //--
  QLabel
    *cmRollerBox,
    *cmPositionBOX,
    *cmOPMV,*cmCLMV,
    *cmOPAS,*cmMMAS,*cmCLAS
  ;//...
  
  QSlider *cmGateBar;
  
private:
  static MainCoordinator* self;
  MainCoordinator();
  QLabel* ssCreateTextLamp(QString);
  
};

#endif /* MAINCOORDINATOR_H */

