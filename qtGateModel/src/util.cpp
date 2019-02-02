/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <iostream>

template <class T> void println(std::string pxTag,T pxVal) {
  std::cout << pxTag << " : " << pxVal << std::endl;
}//+++

void println(std::string pxLine){
  std::cout<< pxLine <<std::endl;
}//+++

//***eof
