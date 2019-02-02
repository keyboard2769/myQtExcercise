/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MainLoop.cpp
 * Author: keypad
 * 
 * Created on 2019/02/01, 19:37
 */

#include <QtGui>
#include "MainLoop.h"

MainLoop* MainLoop::self=NULL;

MainLoop* MainLoop::ccGetReference(){
  if(self==NULL){self=new MainLoop;}
  return self;
}//++!

MainLoop::MainLoop() {
  
  cmCoordinator=MainCoordinator::ccGetReference();
  
  cmRoller=0;
  dcCLMV=false;
  dcOPMV=false;
  
  dcCLAS=false;
  dcMMAS=false;
  dcOPAS=false;
  
  dcGate = new ZcRangedValueModel(0,20);
  
}//++!

void MainLoop::ccLoop(){

  cmRoller++;cmRoller&=0x0F;
  
  //-- simulate
  if(dcOPMV){dcGate->ccShift(1);}
  if(dcCLMV){dcGate->ccShift(-1);}
  dcCLAS=dcGate->ccEquals(1);
  dcMMAS=dcGate->ccEquals(9);
  dcOPAS=dcGate->ccEquals(19);
  
  //-- wire
  cmCoordinator->cmRollerBox->setNum(cmRoller);
  cmCoordinator->cmPositionBOX->setNum(dcGate->ccGet());
  //--
  MainCoordinator::ccSetActivated(cmCoordinator->cmCLMV,dcCLMV);
  MainCoordinator::ccSetActivated(cmCoordinator->cmOPMV,dcOPMV);
  //--
  MainCoordinator::ccSetActivated(cmCoordinator->cmCLAS,dcCLAS);
  MainCoordinator::ccSetActivated(cmCoordinator->cmMMAS,dcMMAS);
  MainCoordinator::ccSetActivated(cmCoordinator->cmOPAS,dcOPAS);
  //--
  cmCoordinator->cmGateBar->setValue(dcGate->ccGet());
  
}//+++

//===

int MainLoop::ccGetRoller() {
  return cmRoller;
}//+++

//***eof
