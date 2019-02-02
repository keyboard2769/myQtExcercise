/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainCoordinator.cpp
 * Author: keypad
 * 
 * Created on 2019/02/01, 19:44
 */

#include <QtGui>
#include "MainCoordinator.h"

MainCoordinator* MainCoordinator::self = NULL;

MainCoordinator* MainCoordinator::ccGetReference(){
  if(self==NULL){self = new MainCoordinator;}
  return self;
}//++!

MainCoordinator::MainCoordinator() {
  
  cmRollerBox=ssCreateTextLamp("--");
  
  cmPositionBOX=ssCreateTextLamp("RR");
  
  cmCLAS=ssCreateTextLamp("CLAS");
  cmMMAS=ssCreateTextLamp("MMAS");
  cmOPAS=ssCreateTextLamp("OPAS");
  
  cmCLMV=ssCreateTextLamp("CLMV");
  cmOPMV=ssCreateTextLamp("OPMV");
  
  cmGateBar=new QSlider;
  cmGateBar->setRange(0,20);
  cmGateBar->setOrientation(Qt::Horizontal);
  cmGateBar->setDisabled(true);
  
}//++!

QLabel* MainCoordinator::ssCreateTextLamp(QString pxText) {
  QLabel* lpRes=new QLabel;
  lpRes->setFrameStyle(QFrame::Sunken|QFrame::Panel);
  lpRes->setText(pxText);
  lpRes->setAutoFillBackground(true);
  lpRes->setMinimumSize(20,20);
  return lpRes;
}//+++

void MainCoordinator::ccSetActivated(QLabel *pxTarget,bool pxStatus) {
  pxTarget->setPalette(QPalette(pxStatus?(Qt::green):(Qt::gray)));
}//+++

//***eof

