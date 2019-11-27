/* Fig.9-8 */

#include <malloc.h>

#define C_FIG_NAME TEXT("ENVIRON")

#define ID_LIST 1
#define ID_TEXT 2

static HWND hwndList, hwndText;

//===

void ccFillListBox(HWND hwndList){
  
  //--
  /* (from the errata)
   * This function has a logic error.
   * The problem is that pVarBlock is modified in the meantime.
   * If you were to check the return value for FreeEnvironmentStrings,
     you will see it is 0, indicating an error.
   * Calling GetLastError will show the error code 87 (ERROR_INVALID_PARAMETER).
   * To fix this, he could merely have to store the original return value
     of GetEnvironmentStrings in a variable that will not be modified,
     and pass this unchanged value to FreeEnvironmentStrings.
   */
  int iLength;
  TCHAR* pVarBlock;
  TCHAR* pVarBeg;
  TCHAR* pVarEnd;
  TCHAR* pVarName;
  pVarBlock = GetEnvironmentStrings();
  
  //--
  while (QB_DEREF(pVarBlock)){
    if (QB_DEREF(pVarBlock) != '=') {
      
      //--
      pVarBeg = pVarBlock;
      while(*pVarBlock++ != '='){;}
      pVarEnd = pVarBlock - 1;
      iLength = pVarEnd - pVarBeg;

      //--
      pVarName = calloc(iLength + 1, sizeof(TCHAR));
      CopyMemory(pVarName, pVarBeg, iLength * sizeof(TCHAR));
      pVarName[iLength] = '\0';

      //--
      SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)pVarName);
      free(pVarName);
      
    }//..?
    while(*pVarBlock++ != '\0'){;}
  }//..~
  
  //--
  FreeEnvironmentStrings(pVarBlock);
  
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //-- 
  int cxChar = LOWORD(GetDialogBaseUnits());
  int cyChar = HIWORD(GetDialogBaseUnits());
  
  //--
  RECT lpClient;
  GetClientRect(hwnd, QB_TOREF(lpClient));
  int lpWidth = lpClient.right;
  int lpHeight = lpClient.bottom;
  wprintf(TEXT("%d %d %d %d"),
    lpClient.left,lpClient.top,lpClient.right,lpClient.bottom);

  //--
  hwndList = CreateWindow(
    TEXT("listbox"), NULL,
    WS_CHILD | WS_VISIBLE | LBS_STANDARD,
    cxChar, cyChar * 3,
    //cxChar * 16 + GetSystemMetrics(SM_CXVSCROLL), cyChar * 5,
    lpWidth-(cxChar*2),lpHeight-(cyChar*6),
    hwnd, (HMENU)ID_LIST,
    (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
    NULL
  );
  
  hwndText = CreateWindow(
    TEXT("static"), NULL,
    WS_CHILD | WS_VISIBLE | SS_LEFT,
    cxChar, cyChar,
    //GetSystemMetrics(SM_CXSCREEN), cyChar,
    lpWidth-(cxChar*2), cyChar,
    hwnd, (HMENU)ID_TEXT,
    (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
    NULL
  );

  //--
  ccFillListBox(hwndList);
  return 0;
  
}//+++

int ssOnSetFocus(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  SetFocus(hwndList);
  return 0;
}//+++

int ssOnCommand(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  int lpChildWindowID = LOWORD(wParam);
  int lpNotificationCode = HIWORD(wParam);
  if (LB_AND(
    lpChildWindowID == ID_LIST,
    lpNotificationCode == LBN_SELCHANGE
  )){
    
    //--
    int iIndex = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
    int iLength = SendMessage(hwndList, LB_GETTEXTLEN, iIndex, 0) + 1;
    TCHAR* pVarName = calloc(iLength, sizeof(TCHAR));
    SendMessage(hwndList, LB_GETTEXT, iIndex, (LPARAM)pVarName);

    //--
    iLength = GetEnvironmentVariable(pVarName, NULL, 0);
    TCHAR* pVarValue = calloc(iLength, sizeof(TCHAR));
    GetEnvironmentVariable(pVarName, pVarValue, iLength);

    //--
    SetWindowText(hwndText, pVarValue);
    free(pVarName);
    free(pVarValue);
    
  }//..?
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SETFOCUS:return ssOnSetFocus(hwnd,message,wParam,lParam);
    case WM_COMMAND:return ssOnCommand(hwnd,message,wParam,lParam);
    case WM_KEYDOWN:
    case WM_DESTROY:return ssOnDestroy(hwnd,message,wParam,lParam);
    default:return DefWindowProc(hwnd,message,wParam,lParam);
  }//..?
}//+++

int WinSub(
  HINSTANCE hInstance,HINSTANCE hPrevInstance,
  PSTR szCmdLine,int iCmdShow
){
  
  //--
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;
  
  //--
  wndclass.style         = CS_HREDRAW|CS_VREDRAW;
  wndclass.lpfnWndProc   = WndProc;
  wndclass.cbClsExtra    = 0;
  wndclass.cbWndExtra    = 0;
  wndclass.hInstance     = hInstance;
  wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName  = NULL;
  wndclass.lpszClassName = TEXT("MAINWINDOW");
  if(!RegisterClass(&wndclass)){
    MessageBox(NULL,
      TEXT("not_windows_nt"),
      C_FIG_NAME, MB_ICONERROR
    );
    return -1;
  }//..?
  
  //--
  hwnd = CreateWindow(
    wndclass.lpszClassName,C_FIG_NAME,WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,
    320,240,
    NULL,NULL,hInstance,NULL
  );
  ShowWindow(hwnd,iCmdShow);
  UpdateWindow(hwnd);
  while(GetMessage(&msg, NULL, 0, 0)){
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }//..~
  
  //--
  return msg.wParam;
  
}//!!!
