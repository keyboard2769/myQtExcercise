/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainWindow.cpp
 * Author: keypad
 * 
 * Created on 2019/02/01, 19:15
 */

#include <QtGui>
#include <QDebug>
#include "MainWindow.h"
#include "util.cpp"

MainWindow* MainWindow::self=NULL;

MainWindow* MainWindow::ccGetReference(){
  if(self==NULL){self=new MainWindow;}
  return self;
}//++!

MainWindow::MainWindow():QWidget(){
  setWindowTitle(tr("Gate Model"));
  setGeometry(666,666,320,240);
  
  //-- loop
  cmLoop=MainLoop::ccGetReference();
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), cmLoop, SLOT(ccLoop()));
  timer->start(66);
  
  //-- model group
  QGridLayout *lpModelLayout = new QGridLayout;
  lpModelLayout->setColumnStretch(1, 10);
  lpModelLayout->setColumnStretch(2, 20);
  //--
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmCLAS,0,1);
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmMMAS,0,2);
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmOPAS,0,3);
  //--
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmCLMV,1,0);
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmPositionBOX,1,1,1,3);
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmOPMV,1,4,1,1);
  //--
  lpModelLayout->addWidget(cmLoop->cmCoordinator->cmGateBar,2,1,1,3);
  //--
  QGroupBox *lpModelGroup = new QGroupBox(tr("MODEL"));
  lpModelGroup->setLayout(lpModelLayout);
  
  //-- control group
  QHBoxLayout *lpControlLayout = new QHBoxLayout;
  QPushButton *lpCloseButton = new QPushButton(tr("CLOSE(&a)"));
  connect(lpCloseButton,SIGNAL(clicked()),this,SLOT(ccGateClose()));
  QPushButton *lpStopButton = new QPushButton(tr("STOP(&s)"));
  connect(lpStopButton,SIGNAL(clicked()),this,SLOT(ccGateStop()));
  QPushButton *lpOpenButton = new QPushButton(tr("OPEN(&d)"));
  connect(lpOpenButton,SIGNAL(clicked()),this,SLOT(ccGateOpen()));
  lpControlLayout->addWidget(lpCloseButton);
  lpControlLayout->addWidget(lpStopButton);
  lpControlLayout->addWidget(lpOpenButton);
  QGroupBox *lpControlGroup = new QGroupBox(tr("CONTROL"));
  lpControlGroup->setLayout(lpControlLayout);
  
  //-- pack
  QPushButton *lpQuitButton = new QPushButton(tr("&QUIT"));
  connect(lpQuitButton,SIGNAL(clicked()),this,SLOT(ccExit()));
  
  QLayout *lpWindowLayout = new QVBoxLayout;
  lpWindowLayout->addWidget(lpModelGroup);
  lpWindowLayout->addWidget(lpControlGroup);
  lpWindowLayout->addWidget(cmLoop->cmCoordinator->cmRollerBox);
  lpWindowLayout->addWidget(lpQuitButton);
  setLayout(lpWindowLayout);
  
}//++!

//=== slot

void MainWindow::ccGateClose() {
  cmLoop->dcCLMV=true;
  cmLoop->dcOPMV=false;
}//+++

void MainWindow::ccGateStop() {
  cmLoop->dcCLMV=false;
  cmLoop->dcOPMV=false;
}//+++

void MainWindow::ccGateOpen() {
  cmLoop->dcCLMV=false;
  cmLoop->dcOPMV=true;
}//+++

void MainWindow::ccExit(){
  println("--exit by quit button",0);
  qApp->quit();
}//+++

//***eof
