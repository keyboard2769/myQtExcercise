/* *
 * 
 * latest chapter: A95-location-and-order
 * 
 */

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

//=== constant

#define null NULL
#define APP_TITLE "ACPG-v0.01"

enum ACTION_ID{
  ACTION_DUMMY,
  ACTION_ADD,
  ACTION_QUIT
};

//=== utility

void ccTagINT(const char* pxTag, int pxVal){
  printf(pxTag);
  printf(":%d\n",pxVal);
}//+++

int fsPover(){
  PostQuitMessage(0);
  return 0;
}//+++

void ccMsgbox(LPSTR pxLine){
  MessageBox(null,pxLine,APP_TITLE,MB_OK);
}//+++

void ccComboBoxAddItem(HWND pxTarget, LPSTR pxItem){
  assert(pxTarget!=null);
  assert(pxItem!=null);
  //[TODO]::type check
  /* 
  TCHAR name[256];
  GetClassName(pxTarget,name,255);
  printf(name);
   */
  SendMessage(pxTarget, CB_ADDSTRING, 0, (LPARAM)pxItem);
}//+++

int ccComboBoxGetSelectedIndex(const HWND const pxHost){
  //[NOTYET]::
  /**
   if(ccIsTypeOf("ComboBox")){
     return SendMessage(pxHost , CB_GETCURSEL , 0 , 0);
   }
   assert(false);
   */
  return -1;
}//+++

//=== inner ** EcRect

typedef struct{
  int cmX,cmY,cmW,cmH;
}EcRect;

void ccRectInit(EcRect* const self){
  self->cmX=8;
  self->cmY=8;
  self->cmW=8;
  self->cmH=8;
}//+++

void ccRectInitWith(EcRect* const self, int pxX, int pxY, int pxW, int pxH){
  self->cmX=pxX;
  self->cmY=pxY;
  self->cmW=pxW;
  self->cmH=pxH;
}//+++

void ccRectClone(EcRect* const pxHost, const EcRect* const pxGuest){
  pxHost->cmX=pxGuest->cmX;
  pxHost->cmY=pxGuest->cmY;
  pxHost->cmW=pxGuest->cmW;
  pxHost->cmH=pxGuest->cmH;
}//+++

void ccRectSetLocation(EcRect* const self, int pxX, int pxY){
  self->cmX=pxX;
  self->cmY=pxY;
}//+++

void ccRectSetSize(EcRect* const self, int pxW, int pxH){
  self->cmW=pxW;
  self->cmH=pxH;
}//+++

void ccRectShiftLocation(EcRect* const self, int pxGapX, int pxGapY){
  if(pxGapX!=0){self->cmX+=(self->cmW+pxGapX);}
  if(pxGapY!=0){self->cmY+=(self->cmH+pxGapY);}
}//+++

void ccRectOffsetLocation(EcRect* const self, int pxOffsetX, int pxOffsetY){
  self->cmX+=pxOffsetX;
  self->cmY+=pxOffsetY;
}//+++

int ccRectGetEndX(const EcRect* const pxHost){
  assert(pxHost!=null);
  return (pxHost->cmX)+(pxHost->cmW);
}//+++

int ccRectGetEndY(const EcRect* const pxHost){
  assert(pxHost!=null);
  return (pxHost->cmY)+(pxHost->cmH);
}//+++

//=== factory

HWND ccMyLabel(
  const HWND pxOwner,const EcRect* pxBound, const LPSTR pxText
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    "STATIC",pxText,
    WS_CHILD|WS_VISIBLE|SS_CENTER,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,null,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

void ccMyButton(
  HWND pxOwner,const EcRect* pxBound, LPSTR pxText, int pxID
){
  if(pxOwner == null){return;}
  if(pxText == null){return;}
  CreateWindow(
    "BUTTON",pxText,
    WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}

void ccMyMasterRadioButton(
  HWND pxOwner,const EcRect* pxBound, LPSTR pxText, int pxID
){
  if(pxOwner == null){return;}
  if(pxText == null){return;}
  CreateWindow(
    "BUTTON",pxText,
    WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}

void ccMySlaveRadioButton(
  HWND pxOwner,const EcRect* pxBound, LPSTR pxText, int pxID
){
  if(pxOwner == null){return;}
  if(pxText == null){return;}
  CreateWindow(
    "BUTTON",pxText,
    WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}

HWND ccMyTextBox(
  const HWND pxOwner,const EcRect* pxBound, const LPSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    "EDIT",pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyContentBox(
  const HWND pxOwner,const EcRect* pxBound, const LPSTR pxText
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    "EDIT",pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_RIGHT|WS_DISABLED,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,null,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyTextArea(
  const HWND pxOwner,const EcRect* pxBound, const LPSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    "EDIT",pxText,
    WS_CHILD | WS_VISIBLE | WS_BORDER
     | WS_HSCROLL | WS_VSCROLL |ES_AUTOHSCROLL | ES_AUTOVSCROLL 
     | ES_LEFT | ES_MULTILINE,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyCombobox(
  const HWND pxOwner,const EcRect* pxBound, int pxID
){
  assert(pxOwner != null);
  assert(pxBound != null);
  return CreateWindow(
    "COMBOBOX",null,
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

//=== callback

LRESULT CALLBACK ccProcessWindow(
  HWND pxHandle,
  UINT pxMessage,
  WPARAM pxWordP,
  LPARAM pxLongP
){
  
  //-- draw-resource
  HDC lpContext;
  PAINTSTRUCT lpPainter;
  
  //-- public
  static HWND pbLabel;
  static EcRect pbBoundaryBUF;
  static unsigned int pbModel =0;
  static TCHAR pbTextBuf[512];
  
  //-- initiating
  
  //-- action performed
  switch(pxMessage){
    
    case WM_CREATE:
      
      ccRectInitWith(&pbBoundaryBUF,2,2,300,24);
      pbLabel=ccMyLabel(pxHandle,&pbBoundaryBUF,TEXT("-test-"));
      
      ccRectInitWith(&pbBoundaryBUF,10,50,48,32);
      ccMyButton(pxHandle,&pbBoundaryBUF,"add",ACTION_ADD);
      
      ccRectShiftLocation(&pbBoundaryBUF,3,0);
      ccMyButton(pxHandle,&pbBoundaryBUF,"quit",ACTION_QUIT);
      
      ccRectShiftLocation(&pbBoundaryBUF,0,3);
      ccRectSetSize(&pbBoundaryBUF,160,22);
      ccMyContentBox(pxHandle,&pbBoundaryBUF,"--shower! yeah!");
      
    return 0;
    
    case WM_DESTROY:
    return fsPover();
    
    case WM_KEYDOWN:
    return fsPover();
    
    case WM_LBUTTONDOWN:
    return 0;
    
    case WM_TIMER:
    return 0;
    
    case WM_COMMAND:
      
      switch(HIWORD(pxWordP)){
        
        case CBN_SELCHANGE:
        break;
        
        default:break;
        
      }//..?
      
      switch(LOWORD(pxWordP)){
        
        case ACTION_ADD:
          pbModel++;pbModel&=0x0F;
          wsprintf(pbTextBuf,"roller:%d <\n",pbModel);
          SetWindowText(pbLabel,pbTextBuf);
        break;
        
        case ACTION_QUIT:
          return fsPover();
        break;
        
        default:break;
      
      }//..?
      
    return 0;
    
    case WM_PAINT:
      lpContext=BeginPaint(pxHandle, &lpPainter);
      if(lpContext!=null)
      { 
        wsprintf(pbTextBuf,"--by mingw32 gcc/c99 using w32api");
        TextOut(lpContext,2,200-24,pbTextBuf,lstrlen(pbTextBuf)&511);
      }
      EndPaint(pxHandle, &lpPainter);
    return 0;
    
  }//..?
  return DefWindowProc(pxHandle, pxMessage, pxWordP, pxLongP);
}//***

//=== entry

int WINAPI WinMain(
  HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPSTR lpCmdLine,
  int nCmdSHow
){
  
  //-- get current working directory
  TCHAR lpCWDPath[PATH_MAX];
  TCHAR lpInitInfo[PATH_MAX+8];
  if(getcwd(lpCWDPath,sizeof(lpCWDPath))!=null){
    wsprintf(lpInitInfo,"with:\n%s\n",lpCWDPath);
    ccMsgbox(lpInitInfo);
  }else{
    ccTagINT("--failed to get current working directory",255);
  }
  
  //-- local
  HWND lpMainFrame;
  MSG lpMessage;
  WNDCLASS lpWindowClass;
  
  //-- setup class
  lpWindowClass.style = CS_DISABLE;
  lpWindowClass.lpfnWndProc = ccProcessWindow;
  lpWindowClass.cbClsExtra = lpWindowClass.cbWndExtra = 0;
  lpWindowClass.hInstance = hInstance;
  lpWindowClass.hIcon = LoadIcon(null, IDI_APPLICATION);
  lpWindowClass.hCursor = LoadCursor(null, IDC_ARROW);
  lpWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  lpWindowClass.lpszMenuName = null;
  lpWindowClass.lpszClassName = "MYCLASS";
  if(!RegisterClass(&lpWindowClass)){
    ccTagINT("--failed to reggist class",255);
    return -1;
  }//..?
  
  //-- create window 
  lpMainFrame = CreateWindow(
    "MYCLASS",APP_TITLE,
    WS_DLGFRAME | WS_VISIBLE, 
    CW_USEDEFAULT,CW_USEDEFAULT,
    320,240,
    null,null,
    hInstance,null
  );
  if(lpMainFrame==null){
    ccTagINT("--failed to create window",255);
    return -1;
  }//..?

  //-- loop
  while(GetMessage(&lpMessage, null,0,0)){
    DispatchMessage(&lpMessage);
  }//..~
  
  //-- exit
  ccTagINT("--exit with",0);
  return lpMessage.wParam;
  
}//***EOF
