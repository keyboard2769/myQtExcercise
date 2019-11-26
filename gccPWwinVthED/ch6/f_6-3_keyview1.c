/* Fig.6-3 : KEYVIEW1 */

//under achieve

//===

static TCHAR * szFormat[2] = {
  TEXT ("%s %d %s%c%u %d %s %s %s %s"),
  TEXT ("%s 0x%X%s%c %u %d %s %s %s %s")
};
static TCHAR * szYes = TEXT ("Yes") ;
static TCHAR * szNo = TEXT ("No") ;
static TCHAR * szDown = TEXT ("Down") ;
static TCHAR * szUp = TEXT ("Up") ;
static TCHAR * szMessage [] = {
  TEXT ("WM_KEYDOWN"), TEXT ("WM_KEYUP"),
  TEXT ("WM_CHAR"), TEXT ("WM_DEADCHAR"),
  TEXT ("WM_SYSKEYDOWN"), TEXT ("WM_SYSKEYUP"),
  TEXT ("WM_SYSCHAR"), TEXT ("WM_SYSDEADCHAR")
};

//===

static HDC hdc;
static PAINTSTRUCT ps;

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  Beep(440,255);
  return 0;
}//+++

int ssOnKeyEvent(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  TCHAR szBuffer[255], szKeyName [255] ;
  int iType = message == WM_CHAR ||
    message == WM_SYSCHAR ||
    message == WM_DEADCHAR ||
    message == WM_SYSDEADCHAR;
  wsprintf (szBuffer, szFormat [iType],
    szMessage [message- WM_KEYFIRST],
    wParam,
    (PTSTR) (iType ? TEXT (" ") : szKeyName),
    (TCHAR) (iType ? wParam : ' '),
    LOWORD (lParam),
    HIWORD (lParam) & 0xFF,
    0x01000000 & lParam ? szYes : szNo,
    0x20000000 & lParam ? szYes : szNo,
    0x40000000 & lParam ? szDown : szUp,
    0x80000000 & lParam ? szUp : szDown
  );
  MessageBox(
    NULL,
    szBuffer,TEXT("KeyEvent"),
    MB_OK
  );
  if(wParam==0x1B){PostQuitMessage(0);};
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    static RECT rect;
    GetClientRect(hwnd,&rect);
    rect.bottom=rect.bottom/4;
    DrawText(
      hdc,
      TEXT("Keyboard Message Viewer #1"),-1,&rect,
      DT_SINGLELINE|DT_CENTER|DT_VCENTER
    );
    rect.top+=120;
    DrawText(
      hdc,
      TEXT("[Message|Key|Char|Repeat|Scan|Ext|ALT|Prev|Tran]"),-1,&rect,
      DT_SINGLELINE|DT_CENTER|DT_VCENTER
    );
    rect.top+=120;
    DrawText(
      hdc,
      TEXT("..Press ESC to exit"),-1,&rect,
      DT_SINGLELINE|DT_CENTER|DT_VCENTER
    );
  }
  EndPaint(hwnd,&ps);
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
    //--
    case WM_KEYDOWN:case WM_KEYUP:case WM_SYSKEYDOWN:case WM_SYSKEYUP:
    case WM_CHAR:case WM_DEADCHAR:case WM_SYSCHAR:
    case WM_SYSDEADCHAR:return ssOnKeyEvent(hwnd,message,wParam,lParam);
    //--
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
  static TCHAR szAppName[] = TEXT("CUSTOM");
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
  wndclass.lpszClassName = szAppName;
  if(!RegisterClass(&wndclass)){
    MessageBox(NULL,
      TEXT("not_windows_nt"),
      szAppName, MB_ICONERROR
    );
    return -1;
  }//..?

  //--
  hwnd = CreateWindow(
    szAppName,TEXT("KEYVIEW1"),WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,
    400,300,
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
