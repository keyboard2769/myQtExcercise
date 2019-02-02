/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

class ZcRangedModel{
  
private:
  int cmMin,cmMax;
  
public:
  ZcRangedModel(int pxMin, int pxRange){
    cmMin=pxMin;
    if(pxRange<=0)
      {cmMax=cmMin+1;}
    else
      {cmMax=pxMin+pxRange;}
  }//++!
  
  int ccLimit(int pxSource){
    if(pxSource<cmMin){return cmMin;}
    if(pxSource>cmMax){return cmMax;}
    return pxSource;
  }//+++
  
};//***

class ZcRangedValueModel:public ZcRangedModel{
  
private:
  int cmValue;
  
public:
  ZcRangedValueModel(int pxMin, int pxRange):ZcRangedModel(pxMin, pxRange){
    cmValue=pxMin;
  }//++!
  
  void ccSet(int pxValue){
    cmValue=ccLimit(pxValue);
  }//+++
  
  void ccShift(int pxOffset){
    cmValue+=pxOffset;
    cmValue=ccLimit(cmValue);
  }//+++
  
  int ccGet(){
    return cmValue;
  }//+++
  
  bool ccEquals(int pxTarget){
    return cmValue==pxTarget;
  }//+++

};//***

//***eof
