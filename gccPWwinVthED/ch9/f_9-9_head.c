/* Fig.9-9 */

#define C_FIG_NAME TEXT("HEAD")

#define ID_LIST 1
#define ID_TEXT 2
#define MAXREAD 256

static BOOL bValidFile;
static WNDPROC fpOldList;
static RECT rect;
static HWND hwndList, hwndText;	
static HDC hdc;
static PAINTSTRUCT ps;
static TCHAR szBuffer[MAX_PATH + 1];
static TCHAR szFile[MAX_PATH + 1];
static HANDLE hFile;

//===

int ccDirectoryAttributes(){
  return
     DDL_READWRITE | DDL_READONLY | DDL_HIDDEN | DDL_SYSTEM
   | DDL_DIRECTORY | DDL_ARCHIVE  | DDL_DRIVES;
}//+++

int ccDrawTextFlags(){
  return DT_WORDBREAK | DT_EXPANDTABS | DT_NOCLIP | DT_NOPREFIX;
}//+++

//===

LRESULT CALLBACK ListProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if(LB_AND(
    message == WM_KEYDOWN,
    message == VK_RETURN
  )){
    SendMessage(
      GetParent(hwnd), WM_COMMAND,
      MAKELONG(ID_LIST, LBN_DBLCLK), (LPARAM)hwnd
    );
  }//..?
  return CallWindowProc(fpOldList, hwnd, message, wParam, lParam);
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  int cxChar = LOWORD(GetDialogBaseUnits());
  int cyChar = HIWORD(GetDialogBaseUnits());
  rect.left = 20 * cxChar;
  rect.top = 3 * cyChar;

  //--
  hwndList = CreateWindow(
    TEXT("listbox"), NULL,
    WS_CHILDWINDOW | WS_VISIBLE | LBS_STANDARD,
    cxChar, cyChar * 3,
    cxChar * 13 + GetSystemMetrics(SM_CXVSCROLL), cyChar * 10,
    hwnd, (HMENU)ID_LIST,
    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL
  );

  //--
  GetCurrentDirectory(MAX_PATH + 1, szBuffer);
  hwndText = CreateWindow(
    TEXT("static"), szBuffer,
    WS_CHILDWINDOW | WS_VISIBLE | SS_LEFT,
    cxChar, cyChar, 
    cxChar * MAX_PATH, cyChar,
    hwnd, (HMENU)ID_TEXT,
    (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),NULL
  );

  //--
  fpOldList = (WNDPROC)SetWindowLong(hwndList, GWL_WNDPROC,(LPARAM)ListProc);
  SendMessage(hwndList, LB_DIR, ccDirectoryAttributes(), (LPARAM)TEXT("*.*"));
  return 0;
  
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  rect.right  = LOWORD(lParam);
  rect.bottom = HIWORD(lParam);
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
    lpChildWindowID    == ID_LIST,
    lpNotificationCode == LBN_DBLCLK
  )){
    
    //--
    int i = SendMessage(hwndList, LB_GETCURSEL, 0, 0);
    if (LB_ERR == i){
      return DefWindowProc(hwnd,message,wParam,lParam);
    }//..?

    //--
    SendMessage(hwndList, LB_GETTEXT, i, (LPARAM)szBuffer);
    hFile = CreateFile(
      szBuffer,
      GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL
    );
    
    //--
    if(INVALID_HANDLE_VALUE != hFile) {
      CloseHandle(hFile);
      bValidFile = TRUE;
      lstrcpy(szFile, szBuffer);
      GetCurrentDirectory(MAX_PATH + 1, szBuffer);
      if (szBuffer[lstrlen(szBuffer) - 1] != '\\'){
        lstrcat(szBuffer, TEXT("\\"));
      }//..?
      SetWindowText(hwndText, lstrcat(szBuffer, szFile));
    }else{
      bValidFile = FALSE;
      szBuffer[lstrlen(szBuffer) - 1] = '\0';
      if (!SetCurrentDirectory(szBuffer + 1)){
        szBuffer[3] = ':';
        szBuffer[4] = '\0';
        SetCurrentDirectory(szBuffer + 2);
      }//..?
      GetCurrentDirectory(MAX_PATH + 1, szBuffer);
      SetWindowText(hwndText, szBuffer);
      SendMessage(hwndList,
        LB_RESETCONTENT, 0, 0);
      SendMessage(hwndList,
        LB_DIR, ccDirectoryAttributes(),(LPARAM)TEXT("*.*"));
    }//..?
    
    //--
    InvalidateRect(hwnd, NULL, TRUE);
    
  }//..?
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //-- 
  if (!bValidFile){
    return DefWindowProc(hwnd,message,wParam,lParam);
  }//..?

  //--
  hFile = CreateFile(
    szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL
  );
  if (INVALID_HANDLE_VALUE == hFile){
    bValidFile = FALSE;
    return DefWindowProc(hwnd,message,wParam,lParam);
  }//..?
  
  //--
  /* (from the book) 
   * And now, for the first time in this chapter,
     we encounter an issue involving Unicode.
   * In a perfect world, perhaps,
     text files would be recognized by the operating system, 
     so that ReadFile could convert an ASCII file into Unicode text,
     or a Unicode file into ASCII text.
   * But this is NOT the case.
   * ReadFile just reads the bytes of the file without any conversion.
   * This means that DrawTextA (compiled without the UNICODE identifier defined)
     would interpret the text as ASCII
     and DrawTextW (in the Unicode version) would assume the text is Unicode.
   * So what the program should really be doing
     is trying to figure out whether the file has ASCII text or Unicode text
     and then calling DrawTextA or DrawTextW appropriately.
   * Instead, HEAD takes a much simpler approach and uses DrawTextA regardless.
   */
  CHAR buffer[MAXREAD];
  DWORD nBytesRead;
  ReadFile(hFile, buffer, MAXREAD, &nBytesRead, NULL);
  CloseHandle(hFile);
  
  //--
  hdc = BeginPaint(hwnd, &ps);
  {
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
    SetBkColor(hdc, GetSysColor(COLOR_BTNFACE));
    DrawTextA(hdc, buffer, nBytesRead, &rect, ccDrawTextFlags());
  }
  EndPaint(hwnd, &ps);
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
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_SETFOCUS:return ssOnSetFocus(hwnd,message,wParam,lParam);
    case WM_COMMAND:return ssOnCommand(hwnd,message,wParam,lParam);
    case WM_PAINT:return ssOnPaint(hwnd,message,wParam,lParam);
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
    640,480,
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
