/* *
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <assert.h>

#include <windows.h>

//=== constant

#define APP_TITLE TEXT("ACCF-v0.01")
#define WNDCLASS_CUSTOM TEXT("CUSTOM_CLASS")

enum ACTION_ID{
  ACTION_DUMMY,
  ACTION_ADD,
  ACTION_QUIT
};

//=== public

static UINT cspbModel = 0;
static UINT cspbCallbackCounter = 0;
static TCHAR cspbWorkingDirectory[MAX_PATH+1];
static TCHAR cspbTextBuf[512];
static HWND cspbLabel=NULL;

//=== utility

void ccTagINT(const char* pxTag, int pxVal){
  printf(pxTag);
  printf(":%d\n",pxVal);
}//+++

int fsPover(){
  PostQuitMessage(0);
  return 0;
}//+++

void ccReadType(const HWND pxHandle){
  TCHAR lpNameBUF[128];
  for(int i=0;i<128;i++){lpNameBUF[i]=0;}
  int lpRes=GetClassName(pxHandle,lpNameBUF,127);
  if(lpRes<=0){
    ccTagINT("--falied to get name",0);
    return;
  }else{
    wprintf(TEXT("--ReadType():"));
    wprintf(lpNameBUF);
    wprintf(TEXT("\n"));
    return;
  }//..?
}//+++

boolean ccInstanceOf(const HWND pxHandle, LPCTSTR pxTypeName){
  TCHAR lpNameBUF[128];
  for(int i=0;i<128;i++){lpNameBUF[i]=0;}
  int lpRes=GetClassName(pxHandle,lpNameBUF,127);
  if(lpRes==0){return FALSE;}//..?
  lpRes=lstrcmp(lpNameBUF,pxTypeName);
  if(lpRes==0){
    return TRUE;
  }else{
    return FALSE;
  }//..?
}//+++

void ccMsgbox(LPCTSTR pxLine){
  MessageBox(NULL,pxLine,APP_TITLE,MB_OK);
}//+++

void ccMsgboxRaw(const char* pxRawLine){
  int lpSupposedSize=MultiByteToWideChar
    (CP_ACP,0,pxRawLine,-1,NULL,0);
  wchar_t* lpWideLine=malloc(lpSupposedSize*sizeof(wchar_t));
  int lpActualSize=MultiByteToWideChar
    (CP_ACP,0,pxRawLine,-1,lpWideLine,lpSupposedSize);
  assert(lpSupposedSize==lpActualSize);
  ccMsgbox(lpWideLine);
  free(lpWideLine);
}//+++

void ccComboBoxAddItem(HWND pxTarget, LPCTSTR pxItem){
  assert(pxTarget!=NULL);
  assert(pxItem!=NULL);
  if(ccInstanceOf(pxTarget,TEXT("ComboBox"))){
    SendMessage(pxTarget, CB_ADDSTRING, 0, (LPARAM)pxItem);
  }else{
    printf("--ccComboBoxAddItem():failed to validate given handle");
    assert(FALSE);
  }//..?
}//+++

int ccComboBoxGetSelectedIndex(const HWND const pxTarget){
  assert(pxTarget!=NULL);
  if(ccInstanceOf(pxTarget,TEXT("ComboBox"))){
     return SendMessage(pxTarget , CB_GETCURSEL , 0 , 0);
  }else{
    printf("--ccComboBoxGetSelectedIndex():failed to validate given handle");
    assert(FALSE);
    return -1;
  }//..?
}//+++

void ccSetAlwaysOnTopWithClassName(
  const HWND const pxWindow, LPCTSTR pxTypeName
){
  assert(pxWindow!=NULL);
  if(ccInstanceOf(pxWindow,pxTypeName)){
    SetWindowPos(
      pxWindow , HWND_TOPMOST ,
      0 , 0 , 0 , 0 , SWP_NOMOVE | SWP_NOSIZE
    );
  }else{
    printf("--ccSetAlwaysOnTop():failed to validate given handle");
    assert(FALSE);
  }//..?
}//+++

HBRUSH ccSetStaticBackColor(
  const HDC pxContext, COLORREF pxColor
){
  assert(pxContext!=NULL);
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
  assert(pxHost!=NULL);
  return (pxHost->cmX)+(pxHost->cmW);
}//+++

int ccRectGetEndY(const EcRect* const pxHost){
  assert(pxHost!=NULL);
  return (pxHost->cmY)+(pxHost->cmH);
}//+++

//=== factory

HWND ccMyLabel(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText
){
  assert(pxOwner != NULL);
  assert(pxBound != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("STATIC"),pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|SS_CENTER,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,NULL,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

HWND ccMyButton(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("BUTTON"),pxText,
    WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

HWND ccMyMasterRadioButton(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("BUTTON"),pxText,
    WS_CHILD|WS_VISIBLE|WS_GROUP|BS_AUTORADIOBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}

HWND ccMySlaveRadioButton(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("BUTTON"),pxText,
    WS_CHILD|WS_VISIBLE|BS_AUTORADIOBUTTON,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}

HWND ccMyTextBox(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != NULL);
  assert(pxBound != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("EDIT"),pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

HWND ccMyContentBox(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText
){
  assert(pxOwner != NULL);
  assert(pxBound != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("EDIT"),pxText,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_RIGHT|WS_DISABLED,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,NULL,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

HWND ccMyTextArea(
  const HWND pxOwner,const EcRect* pxBound, LPCTSTR pxText, int pxID
){
  assert(pxOwner != NULL);
  assert(pxBound != NULL);
  assert(pxText != NULL);
  return CreateWindow(
    TEXT("EDIT"),pxText,
    WS_CHILD | WS_VISIBLE | WS_BORDER
     | WS_HSCROLL | WS_VSCROLL |ES_AUTOHSCROLL | ES_AUTOVSCROLL 
     | ES_LEFT | ES_MULTILINE,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

HWND ccMyCombobox(
  const HWND pxOwner,const EcRect* pxBound, int pxID
){
  assert(pxOwner != NULL);
  assert(pxBound != NULL);
  return CreateWindow(
    TEXT("COMBOBOX"),NULL,
    WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

//=== sub

void ssOnCreate(HWND pxCaller){
  
  EcRect pbBoundaryBUF;

  ccRectInitWith(&pbBoundaryBUF,10,10,320-10*3,24);
  cspbLabel=ccMyLabel(pxCaller,&pbBoundaryBUF,TEXT("-test-"));

  ccRectInitWith(&pbBoundaryBUF,10,50,48,32);
  ccMyButton(pxCaller,&pbBoundaryBUF,TEXT("add"),ACTION_ADD);

  ccRectShiftLocation(&pbBoundaryBUF,3,0);
  ccMyButton(pxCaller,&pbBoundaryBUF,TEXT("quit"),ACTION_QUIT);

}//+++

LRESULT ssOnControlColorStatic(WPARAM pxWord, LPARAM pxLong){
  if(cspbLabel==(HWND)pxLong){
    return (LRESULT)ccSetStaticBackColor(
      (HDC)pxWord,
      cspbModel>7?
        RGB(0xEE,0x33,0x33):
        RGB(0xEE,0xEE,0xEE)
    );
  }//..?
}//+++

void ssOnCommand(WPARAM pxWord, LPARAM pxLong){
      
  switch(HIWORD(pxWord)){

    case CBN_SELCHANGE:
    break;

    default:break;

  }//..?

  switch(LOWORD(pxWord)){

    case ACTION_ADD:
      cspbModel++;cspbModel&=0x0F;
      if(cspbModel>7){
        wsprintf(cspbTextBuf,L"Over:%d <\n",cspbModel);
      }else{
        wsprintf(cspbTextBuf,L"Below:%d <\n",cspbModel);
      }
      SetWindowText(cspbLabel,cspbTextBuf);
    break;

    case ACTION_QUIT:
      fsPover();
    break;

    default:break;

  }//..?

}//+++

//=== callback

LRESULT CALLBACK WndProc(
  HWND pxCaller, UINT pxMessage,
  WPARAM pxWord, LPARAM pxLong
){
  
  //-- draw-resource
  HDC lpContext;
  PAINTSTRUCT lpPainter;
  
  //-- action performed
  cspbCallbackCounter++;
  switch(pxMessage){
    
    case WM_CREATE:{
      ssOnCreate(pxCaller);
    }return 0;
    
    case WM_CTLCOLORSTATIC:{
      return ssOnControlColorStatic(pxWord,pxLong);
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
      ssOnCommand(pxWord,pxLong);
    }return 0;
    
    case WM_PAINT:{
      lpContext=BeginPaint(pxCaller, &lpPainter);
      if(lpContext!=NULL)
      { 
        wsprintf(cspbTextBuf,TEXT("--with mingw32 gcc/c99 using w32api"));
        TextOut(lpContext,2,200-24,cspbTextBuf,lstrlen(cspbTextBuf)&511);
      }
      EndPaint(pxCaller, &lpPainter);
    }return 0;
    
  }//..?
  return DefWindowProc(pxCaller, pxMessage, pxWord, pxLong);
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
  lpWindowClass.style = CS_HREDRAW | CS_VREDRAW;
  lpWindowClass.lpfnWndProc = WndProc;
  lpWindowClass.cbClsExtra = lpWindowClass.cbWndExtra = 0;
  lpWindowClass.hInstance = hInstance;
  lpWindowClass.hIcon = LoadIcon(NULL, IDI_WARNING);
  lpWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  lpWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  lpWindowClass.lpszMenuName = NULL;
  lpWindowClass.lpszClassName = WNDCLASS_CUSTOM;
  if(!RegisterClass(&lpWindowClass)){
    ccTagINT("--failed to register class",255);
    return -1;
  }//..?
  
  //-- create window 
  lpMainFrame = CreateWindow(
    WNDCLASS_CUSTOM,APP_TITLE,
    WS_DLGFRAME | WS_VISIBLE, 
    CW_USEDEFAULT,CW_USEDEFAULT,
    320,240,
    NULL,NULL,
    hInstance,NULL
  );
  if(lpMainFrame==NULL){
    ccTagINT("--failed to create window",255);
    return -1;
  }//..?

  //-- loop
  while(GetMessage(&lpMessage, NULL,0,0)){
    DispatchMessage(&lpMessage);
  }//..~
  
  //-- exit
  ccTagINT("--for callback be called ",cspbCallbackCounter);
  ccTagINT("--exit with",0);
  return lpMessage.wParam;
  
}//***EOF
