/* *
 * a counter count from zero to fifteen
 */

/* * [NOTE]::to compile resource file:
 * $ gcc -c -Wall -g %sourceFile%
 * $ windres -i %resourceFile% -o %resourceObject%
 * $ gcc -g -o %executableFile% %..ObjectFiles% -mwindows -D Unicode
 * */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>
#include <windows.h>

//=== constant

#define null NULL
#define APP_TITLE TEXT("ACPG-v0.01")

enum ACTION_ID{
  ACTION_DUMMY,
  ACTION_ADD,
  ACTION_QUIT
};

//=== public

static TCHAR cspbWorkingDirectory[MAX_PATH+1];
static int cspbCallbackCounter=0;

//=== utility

void ccTagINT(const char* pxTag, int pxVal){
  printf(pxTag);
  printf(":%d\n",pxVal);
}//+++

int fsPover(){
  PostQuitMessage(0);
  return 0;
}//+++

void ccMsgbox(LPCTSTR pxLine){
  MessageBox(null,pxLine,APP_TITLE,MB_OK);
}//+++

void ccMsgboxRaw(const char* pxRawLine){
  int lpSupposedSize=MultiByteToWideChar
    (CP_ACP,0,pxRawLine,-1,null,0);
  wchar_t* lpWideLine=malloc(lpSupposedSize*sizeof(wchar_t));
  int lpActualSize=MultiByteToWideChar
    (CP_ACP,0,pxRawLine,-1,lpWideLine,lpSupposedSize);
  assert(lpSupposedSize==lpActualSize);
  ccMsgbox(lpWideLine);
  free(lpWideLine);
}//+++

void ccComboBoxAddItem(HWND pxTarget, LPCTSTR pxItem){
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

void ccSetAlwaysOnTop(const HWND const pxWindow){
  //[NOTYET]::
  /**
   if(!ccIsTypeOf("Window")){
     assert(false);
     return SendMessage(pxHost , CB_GETCURSEL , 0 , 0);
   }
   */
  SetWindowPos(
    pxWindow , HWND_TOPMOST ,
    0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE
  );
}//+++

HBRUSH ccSetStaticBackColor(
  const HDC pxContext, COLORREF pxColor
){
  assert(pxContext!=null);
  SetBkColor(pxContext,pxColor);
  return CreateSolidBrush(pxColor);
}//+++

//=== inner ** EcRect

typedef struct{
  int cmX,cmY,cmW,cmH;
}EcRect;

void ccRectInit(EcRect* const pxHost){
  pxHost->cmX=8;
  pxHost->cmY=8;
  pxHost->cmW=8;
  pxHost->cmH=8;
}//+++

void ccRectInitWith(EcRect* const pxHost, int pxX, int pxY, int pxW, int pxH){
  pxHost->cmX=pxX;
  pxHost->cmY=pxY;
  pxHost->cmW=pxW;
  pxHost->cmH=pxH;
}//+++

void ccRectClone(EcRect* const pxHost, const EcRect* const pxGuest){
  pxHost->cmX=pxGuest->cmX;
  pxHost->cmY=pxGuest->cmY;
  pxHost->cmW=pxGuest->cmW;
  pxHost->cmH=pxGuest->cmH;
}//+++

void ccRectSetLocation(EcRect* const pxHost, int pxX, int pxY){
  pxHost->cmX=pxX;
  pxHost->cmY=pxY;
}//+++

void ccRectSetSize(EcRect* const pxHost, int pxW, int pxH){
  pxHost->cmW=pxW;
  pxHost->cmH=pxH;
}//+++

void ccRectShiftLocation(EcRect* const pxHost, int pxGapX, int pxGapY){
  if(pxGapX!=0){pxHost->cmX+=(pxHost->cmW+pxGapX);}
  if(pxGapY!=0){pxHost->cmY+=(pxHost->cmH+pxGapY);}
}//+++

void ccRectOffsetLocation(EcRect* const pxHost, int pxOffsetX, int pxOffsetY){
  pxHost->cmX+=pxOffsetX;
  pxHost->cmY+=pxOffsetY;
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
  const HWND pxOwner,const EcRect* pxBound, const LPCTSTR pxText
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("STATIC"),pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|SS_CENTER,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,null,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyButton(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("BUTTON"),pxText,
    WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyMasterRadioButton(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("BUTTON"),pxText,
    WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}

HWND ccMySlaveRadioButton(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("BUTTON"),pxText,
    WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}

HWND ccMyTextBox(
  const HWND pxOwner,const EcRect* pxBound, const LPCTSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("EDIT"),pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyContentBox(
  const HWND pxOwner,const EcRect* pxBound, const LPCTSTR pxText
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("EDIT"),pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_RIGHT|WS_DISABLED,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,null,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

HWND ccMyTextArea(
  const HWND pxOwner,const EcRect* pxBound, const LPCTSTR pxText, int pxID
){
  assert(pxOwner != null);
  assert(pxBound != null);
  assert(pxText != null);
  return CreateWindow(
    TEXT("EDIT"),pxText,
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
    TEXT("COMBOBOX"),null,
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    null
  );
}//+++

//=== callback

LRESULT CALLBACK WndProc(
  HWND pxHandle, UINT pxMessage,
  WPARAM pxWord, LPARAM pxLong
){
  
  //-- draw-resource
  HDC lpContext;
  PAINTSTRUCT lpPainter;
  
  //-- public
  static HWND pbLabel;
  static EcRect pbBoundaryBUF;
  static unsigned int pbModel =0;
  static TCHAR pbTextBuf[512];
  
  //-- action performed
  cspbCallbackCounter++;
  switch(pxMessage){
    
    case WM_CREATE:{
      
      ccRectInitWith(&pbBoundaryBUF,10,10,320-10*3,24);
      pbLabel=ccMyLabel(pxHandle,&pbBoundaryBUF,TEXT("-test-"));
      
      ccRectInitWith(&pbBoundaryBUF,10,50,48,32);
      ccMyButton(pxHandle,&pbBoundaryBUF,TEXT("add"),ACTION_ADD);
      
      ccRectShiftLocation(&pbBoundaryBUF,3,0);
      ccMyButton(pxHandle,&pbBoundaryBUF,TEXT("quit"),ACTION_QUIT);
      
    }return 0;
    
    case WM_CTLCOLORSTATIC:{
      
      if(pbLabel==(HWND)pxLong){
        return (LRESULT)ccSetStaticBackColor(
          (HDC)pxWord,
          pbModel>7?
            RGB(0xEE,0x33,0x33):
            RGB(0xEE,0xEE,0xEE)
        );
      }//..?
      
    }return 0;
    
    case WM_CTLCOLOREDIT:{
      ccTagINT("control color edit",(int)pxLong);
    }return 0;
    
    case WM_DESTROY:{
    }return fsPover();
    
    case WM_KEYDOWN:{
    }return fsPover();
    
    case WM_LBUTTONDOWN:{
    }return 0;
    
    case WM_TIMER:{
    }return 0;
    
    case WM_COMMAND:{
      
      switch(HIWORD(pxWord)){
        
        case CBN_SELCHANGE:
        break;
        
        default:break;
        
      }//..?
      
      switch(LOWORD(pxWord)){
        
        case ACTION_ADD:
          pbModel++;pbModel&=0x0F;
          if(pbModel>7){
            wsprintf(pbTextBuf,L"Over:%d <\n",pbModel);
          }else{
            wsprintf(pbTextBuf,L"Below:%d <\n",pbModel);
          }
          SetWindowText(pbLabel,pbTextBuf);
        break;
        
        case ACTION_QUIT:
          return fsPover();
        break;
        
        default:break;
      
      }//..?
      
    }return 0;
    
    case WM_PAINT:{
      lpContext=BeginPaint(pxHandle, &lpPainter);
      if(lpContext!=null)
      { 
        wsprintf(pbTextBuf,TEXT("--with mingw32 gcc/c99 using w32api"));
        TextOut(lpContext,2,200-24,pbTextBuf,lstrlen(pbTextBuf)&511);
      }
      EndPaint(pxHandle, &lpPainter);
    }return 0;
    
  }//..?
  return DefWindowProc(pxHandle, pxMessage, pxWord, pxLong);
}//***

//=== entry

int WINAPI WinMain(
  HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdSHow
){
  
  //-- get current working directory
  if(GetCurrentDirectory(MAX_PATH+1,cspbWorkingDirectory)!=0){
    TCHAR lpInitInfo[MAX_PATH+1+256];
    wsprintf(lpInitInfo,L"with:\n%ls\n",cspbWorkingDirectory);
    ccMsgbox(lpInitInfo);
  }else{
    ccTagINT("--failed to get current working directory",255);
  }//..?
  
  //-- local
  HWND lpMainFrame;
  MSG lpMessage;
  WNDCLASS lpWindowClass;
  
  //-- setup class
  lpWindowClass.style = CS_DISABLE;
  lpWindowClass.lpfnWndProc = WndProc;
  lpWindowClass.cbClsExtra = lpWindowClass.cbWndExtra = 0;
  lpWindowClass.hInstance = hInstance;
  lpWindowClass.hIcon = LoadIcon(null, IDI_WINLOGO);
  lpWindowClass.hCursor = LoadCursor(null, IDC_ARROW);
  lpWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  lpWindowClass.lpszMenuName = null;
  lpWindowClass.lpszClassName = TEXT("MYCLASS");
  if(!RegisterClass(&lpWindowClass)){
    ccTagINT("--failed to register class",255);
    return -1;
  }//..?
  
  //-- create window 
  lpMainFrame = CreateWindow(
    TEXT("MYCLASS"),APP_TITLE,
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
  ccTagINT("--for callback be called ",cspbCallbackCounter);
  ccTagINT("--exit with",0);
  return lpMessage.wParam;
  
}//***EOF
