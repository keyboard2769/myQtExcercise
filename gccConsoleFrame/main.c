/* *
 * Template Console Frame
 */

#include <stdio.h>
#include <assert.h>
#include <windows.h>
#include <commdlg.h>
#include <commctrl.h>

/*== constant ==*/

#define APP_TITLE TEXT("ACCF-v0.01")
#define WNDCLASS_CUSTOM TEXT("CUSTOM_CLASS")
#define CONSOLE_DEFAULT TEXT("::\r\n")

#define APP_WIDTH 320
#define APP_HEIGHT 240

#define AREA_ALL_MAX_CHAR 4096
#define AREA_LINE_MAX_CHAR 256

enum ACTION_ID{
  ACTION_FILE_BROWSE  = 0xC911,
  ACTION_FILE_DUMP    = 0xC912,
  ACTION_FILE_EXIT    = 0xC913,
  ACTION_EDIT_CLEAR   = 0xC921,
  ACTION_HELP_INFO    = 0xC931
};

enum ELEMENT_ID{
  ELEMENT_STATUS_BAR = 0xE901,
  ELEMENT_INPUT_BOX  = 0xE902,
};

/* == public == */

static TCHAR cspbWorkingDirectory[MAX_PATH+1];
static TCHAR cspbAreaBUF[AREA_ALL_MAX_CHAR];

static HWND cspbMainFrame=NULL;
static HWND cspbStatusBar=NULL;
static HWND cspbConsoleArea=NULL;
static HWND cspbInputBox=NULL;
static WNDPROC cspbInputBoxDefaultProcedure;

LRESULT CALLBACK InputProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/* == utility == */

void ccTagINT(const char* pxTag, int pxVal){
  printf(pxTag);
  printf(":%d\n",pxVal);
}//+++

/*== inner ** EcRect ==*/

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

void ccRectSetWidth(EcRect* const pxHost, int pxW){
  pxHost->cmW=pxW;
}//+++

void ccRectSetHeight(EcRect* const pxHost, int pxH){
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

/*== factory ** glues ==*/

int fsPover(){
  PostQuitMessage(0);
  return 0;
}//+++

BOOL ccSaveTextFile(LPCTSTR pxFilePath, LPCTSTR pxText){
  assert(pxFilePath!=NULL);
  assert(pxText!=NULL);
  BOOL cslpRes=FALSE;
  HANDLE cslpFile=CreateFile(
    pxFilePath,
    GENERIC_WRITE,0,NULL,
    CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL
  );
  if(cslpFile!=INVALID_HANDLE_VALUE){
    DWORD cslpLength=lstrlen(pxText)*(sizeof(TCHAR));
    DWORD cslpWritten=0;
    cslpRes=WriteFile(cslpFile,pxText,cslpLength,&cslpWritten,NULL);
    CloseHandle(cslpFile);
  }return cslpRes;
}//+++

void ccReadType(const HWND pxHandle){
  TCHAR cslpNameBUF[128];
  for(int i=0;i<128;i++){cslpNameBUF[i]=0;}
  int cslpRes=GetClassName(pxHandle,cslpNameBUF,127);
  if(cslpRes<=0){
    ccTagINT("--falied to get name",0);
    return;
  }else{
    wprintf(TEXT("--ReadType():"));
    wprintf(cslpNameBUF);
    wprintf(TEXT("\n"));
    return;
  }//..?
}//+++

boolean ccInstanceOf(const HWND pxHandle, LPCTSTR pxTypeName){
  TCHAR cslpNameBUF[128];
  for(int i=0;i<128;i++){cslpNameBUF[i]=0;}
  int cslpRes=GetClassName(pxHandle,cslpNameBUF,127);
  if(cslpRes==0){return FALSE;}//..?
  cslpRes=lstrcmp(cslpNameBUF,pxTypeName);
  if(cslpRes==0){
    return TRUE;
  }else{
    return FALSE;
  }//..?
}//+++

void ccMsgbox(LPCTSTR pxLine){
  MessageBox(NULL,pxLine,APP_TITLE,MB_OK);
}//+++

BOOL ccFileChooser(HWND pxCaller, LPTSTR pxBuffer){
  assert(pxCaller!=NULL);
  assert(pxBuffer!=NULL);
  OPENFILENAME cslpOpener;
  ZeroMemory(&cslpOpener,sizeof(cslpOpener));
  cslpOpener.lStructSize = sizeof(cslpOpener);
  cslpOpener.hwndOwner=pxCaller;
  cslpOpener.lpstrFilter=TEXT(
    "data files (*.dat)\0*.dat\0All Files (*.*)\0*.*\0"
  );
  cslpOpener.lpstrFile=pxBuffer;
  cslpOpener.nMaxFile=MAX_PATH;
  cslpOpener.Flags=OFN_EXPLORER|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY;
  cslpOpener.lpstrDefExt=TEXT("dat");
  return GetOpenFileName(&cslpOpener);
}//+++

void ccGenerateTimeStamp(LPTSTR pxBuff){
  assert(pxBuff!=NULL);
  SYSTEMTIME cslpTime;
  ZeroMemory(&cslpTime,sizeof(cslpTime));
  GetLocalTime(&cslpTime);
  wsprintf(
    pxBuff,TEXT("_%02d%02d%02d%02d%02d"),
    cslpTime.wYear%2000,cslpTime.wMonth,cslpTime.wDay,
    cslpTime.wHour,cslpTime.wMinute
  );
}//+++

/*== factory ** controls ==*/

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

HWND ccMyTextBox(
  const HWND pxOwner,const EcRect* pxBound, int pxID
){
  assert(pxOwner != NULL);
  assert(pxBound != NULL);
  return CreateWindow(
    TEXT("EDIT"),NULL,
    WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_WANTRETURN,
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
     WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | WS_VSCROLL //| WS_DISABLED
    |ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_LEFT | ES_MULTILINE | ES_READONLY,
    pxBound->cmX,pxBound->cmY,pxBound->cmW,pxBound->cmH,
    pxOwner,(HMENU)pxID,
    (HINSTANCE)GetWindowLong(pxOwner,GWL_HINSTANCE),
    NULL
  );
}//+++

//===

void ssSetStatusBar(LPCTSTR pxLine){
  assert(pxLine!=NULL);
  SendMessage(cspbStatusBar, WM_SETTEXT, 0, (LPARAM) pxLine);
}//+++

void ssStackLine(LPCTSTR pxLine){
  assert(cspbConsoleArea!=NULL);
  assert(pxLine!=NULL);
  int cslpInputLength=lstrlen(pxLine);
  if(cslpInputLength>AREA_LINE_MAX_CHAR){return;}
  int cslpCurrentLength=lstrlen(cspbAreaBUF);
  if((cslpCurrentLength+cslpInputLength)>AREA_ALL_MAX_CHAR){
    wsprintf(cspbAreaBUF,CONSOLE_DEFAULT);
    SendMessage(cspbConsoleArea, EM_LINESCROLL,0,-1023);
  }
  lstrcat(cspbAreaBUF,pxLine);
  lstrcat(cspbAreaBUF,TEXT("\r\n"));
  SendMessage(cspbConsoleArea, WM_SETTEXT, 0, (LPARAM) cspbAreaBUF);
  SendMessage(cspbConsoleArea, EM_LINESCROLL,0,1023);
  SendMessage(cspbConsoleArea, EM_SETSEL,cslpCurrentLength,cslpCurrentLength);
}//+++

void ssClear(){
  ZeroMemory(cspbAreaBUF,sizeof(cspbAreaBUF));
  wsprintf(cspbAreaBUF,CONSOLE_DEFAULT);
  SendMessage(cspbConsoleArea, WM_SETTEXT, 0, (LPARAM) cspbAreaBUF);
}//+++

/*=== subroutine ===*/

void ssOnInput(HWND pxCaller, UINT pxMessage, WPARAM pxWord, LPARAM pxLong){
  if(pxWord==VK_RETURN){
    DWORD cslpLength=GetWindowTextLength(cspbInputBox);
    if(cslpLength>=AREA_LINE_MAX_CHAR || cslpLength==0){
      ssStackLine(TEXT("[ERR]::invalid input."));
    }else{
      TCHAR cslpBuff[AREA_LINE_MAX_CHAR]=TEXT("");
      GetWindowText(cspbInputBox,cslpBuff,cslpLength+1);
      ssStackLine(TEXT("[ECHO]::"));
      ssStackLine(cslpBuff);
    }SendMessage(cspbInputBox, WM_SETTEXT, 0, (LPARAM) TEXT(""));
  }//..?
}//+++

void ssOnCreate(HWND pxCaller){
  
  //-- basic menu bar
  HMENU cslpMenuBar=CreateMenu();
  HMENU cslpFileMenu=CreatePopupMenu();
  HMENU cslpEditMenu=CreatePopupMenu();
  HMENU cslpHelpMenu=CreatePopupMenu();
    
  //-- basic menu bar ** file 
  AppendMenu(cslpFileMenu,MF_STRING,ACTION_FILE_BROWSE,TEXT("Browse"));
  AppendMenu(cslpFileMenu,MF_STRING,ACTION_FILE_DUMP,TEXT("Dump"));
  AppendMenu(cslpFileMenu,MF_STRING,ACTION_FILE_EXIT,TEXT("Exit"));
  AppendMenu(cslpMenuBar,MF_STRING|MF_POPUP,(UINT)cslpFileMenu,TEXT("File"));
  
  //-- basic menu bar ** edit
  AppendMenu(cslpEditMenu,MF_STRING,ACTION_EDIT_CLEAR,TEXT("Clear"));
  AppendMenu(cslpMenuBar,MF_STRING|MF_POPUP,(UINT)cslpEditMenu,TEXT("Edit"));
  
  //-- basic menu bar ** help
  AppendMenu(cslpHelpMenu,MF_STRING,ACTION_HELP_INFO,TEXT("Info"));
  AppendMenu(cslpMenuBar,MF_STRING|MF_POPUP,(UINT)cslpHelpMenu,TEXT("Help"));

  //-- basic menu bar ** pack
  SetMenu(pxCaller,cslpMenuBar);  
  
  //-- status bar
  cspbStatusBar=CreateStatusWindow(
    WS_CHILD|WS_VISIBLE,
    TEXT("--4096::256"),
    pxCaller,
    ELEMENT_STATUS_BAR
  );
  
  //-- console area
  int cslpInputBoxOffset=120;
  EcRect lpBoundBuf;
  ccRectInitWith(&lpBoundBuf,
    5,10,
    APP_WIDTH-20,APP_HEIGHT-cslpInputBoxOffset
  );
  cspbConsoleArea=ccMyTextArea(pxCaller,&lpBoundBuf,TEXT("<nn>"),0);
  wsprintf(cspbAreaBUF,CONSOLE_DEFAULT);
  SendMessage(cspbConsoleArea, WM_SETTEXT, 0, (LPARAM) cspbAreaBUF);
  
  //-- input box 
  ccRectOffsetLocation(&lpBoundBuf, 0,APP_HEIGHT-cslpInputBoxOffset+5);
  ccRectSetHeight(&lpBoundBuf,22);
  cspbInputBox=ccMyTextBox(pxCaller,&lpBoundBuf,ELEMENT_INPUT_BOX);
  SetFocus(cspbInputBox);
  cspbInputBoxDefaultProcedure=(WNDPROC)GetWindowLong(cspbInputBox,GWL_WNDPROC);
  SetWindowLong(cspbInputBox,GWL_WNDPROC,(LONG)InputProc);
  
  //-- initiate content
  ssStackLine(TEXT("[SYS]started with::"));
  if(GetCurrentDirectory(MAX_PATH+1,cspbWorkingDirectory)!=0){
    ssStackLine(cspbWorkingDirectory);
  }else{
    ccMsgbox(TEXT("FAILED TO GET WORKING DIRECTORY"));
  }//..?
  
}//+++

void ssOnCommand(WPARAM pxWord, LPARAM pxLong){
  
  switch(HIWORD(pxWord)){

    case CBN_SELCHANGE:
    break;

    default:break;

  }//..?

  switch(LOWORD(pxWord)){
    
    case ACTION_FILE_BROWSE:
    {
      TCHAR cslpPath[MAX_PATH]=TEXT("");
      BOOL cslpRes=ccFileChooser(cspbMainFrame,cslpPath);
      if(cslpRes){
        ssStackLine(TEXT("[SYS]selected path::"));
        ssStackLine(cslpPath);
      }else{
        ccMsgbox(TEXT("FAILED TO SELECT PATH!!"));
      }//..?
    }  
    break;
    
    case ACTION_FILE_DUMP:
    {
      TCHAR cslpTimeStamp[64]=TEXT("");
      ccGenerateTimeStamp(cslpTimeStamp);
      TCHAR cslpDumpFilePath[MAX_PATH]=TEXT("");
      wsprintf(
        cslpDumpFilePath,TEXT("%s\\DumpedLog%s.txt"),
        cspbWorkingDirectory,cslpTimeStamp
      );
      BOOL cslpRes=ccSaveTextFile(cslpDumpFilePath,cspbAreaBUF);
      ssStackLine(TEXT("[SYS]tried to dump log to::"));
      ssStackLine(cslpDumpFilePath);
      if(cslpRes){
        ssStackLine(TEXT("[SYS]dump succeed."));
      }else{
        ssStackLine(TEXT("[SYS]dump failed."));
      }//..?
    }  
    break;
    
    case ACTION_FILE_EXIT:
    {
      fsPover();
    }
    break;
    
    case ACTION_EDIT_CLEAR:
    {
      ssClear();
    }
    break;
    
    case ACTION_HELP_INFO:
    {
      ccMsgbox(TEXT("help info unavailable."));
    }  
    break;
    
    default:break;

  }//..?

}//+++

//=== callback

LRESULT CALLBACK 
InputProc(HWND pxCaller, UINT pxMessage,WPARAM pxWord, LPARAM pxLong){
  switch(pxMessage){
    case WM_KEYDOWN:ssOnInput(pxCaller,pxMessage,pxWord,pxLong);break;
  }return CallWindowProc(
    cspbInputBoxDefaultProcedure,
    pxCaller,pxMessage,pxWord,pxLong
  );
}//+++

LRESULT CALLBACK 
WndProc(HWND pxCaller, UINT pxMessage,WPARAM pxWord, LPARAM pxLong){
  switch(pxMessage){
    case WM_CREATE:ssOnCreate(pxCaller);return 0;
    case WM_DESTROY:return fsPover();
    case WM_COMMAND:ssOnCommand(pxWord,pxLong);return 0;
    case WM_KEYDOWN:SetFocus(cspbInputBox);break;
  }return DefWindowProc(pxCaller, pxMessage, pxWord, pxLong);
}//***

//=== entry

int WINAPI WinMain(
  HINSTANCE hInstance, HINSTANCE hPrevInstance,
  LPSTR lpCmdLine, int nCmdSHow
){

  //-- local
  MSG cslpMessage;
  WNDCLASS cslpWindowClass;

  //-- setup class
  cslpWindowClass.style = CS_HREDRAW | CS_VREDRAW;
  cslpWindowClass.lpfnWndProc = WndProc;
  cslpWindowClass.cbClsExtra = cslpWindowClass.cbWndExtra = 0;
  cslpWindowClass.hInstance = hInstance;
  cslpWindowClass.hIcon = LoadIcon(NULL, IDI_WARNING);
  cslpWindowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  cslpWindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  cslpWindowClass.lpszMenuName = NULL;
  cslpWindowClass.lpszClassName = WNDCLASS_CUSTOM;
  if(!RegisterClass(&cslpWindowClass)){
    ccTagINT("--failed to register class",255);
    return -1;
  }//..?
  
  //-- create window 
  cspbMainFrame = CreateWindow(
    WNDCLASS_CUSTOM,APP_TITLE,
    WS_DLGFRAME | WS_VISIBLE, 
    CW_USEDEFAULT,CW_USEDEFAULT,
    APP_WIDTH,APP_HEIGHT,
    NULL,NULL,
    hInstance,NULL
  );
  if(cspbMainFrame==NULL){
    ccTagINT("--failed to create window",255);
    return -1;
  }//..?
  
  //-- loop
  InitCommonControls();
  while(GetMessage(&cslpMessage, NULL,0,0)){
    TranslateMessage(&cslpMessage);
    DispatchMessage(&cslpMessage);
  }//..~
  
  //-- exit
  ccTagINT("--exit with",0);
  return cslpMessage.wParam;
  
}//***EOF
