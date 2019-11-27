/* Fig.9-1 */

#define C_FIG_NAME TEXT("BTNLOOK")

struct{
  int iStyle;
  TCHAR* szText;
}button[] ={
  {BS_PUSHBUTTON,      TEXT("0:PUSHBUTTON")},
  {BS_DEFPUSHBUTTON,   TEXT("1:DEFPUSHBUTTON")},
  {BS_CHECKBOX,        TEXT("2:CHECKBOX")},
  {BS_AUTOCHECKBOX,    TEXT("3:AUTOCHECKBOX")},
  {BS_RADIOBUTTON,     TEXT("4:RADIOBUTTON")},
  {BS_3STATE,          TEXT("5;3STATE")},
  {BS_AUTO3STATE,      TEXT("6:AUTO3STATE")},
  {BS_GROUPBOX,        TEXT("7:GROUPBOX")},
  {BS_AUTORADIOBUTTON, TEXT("8:AUTORADIO")},
  {BS_OWNERDRAW,       TEXT("9:OWNERDRAW"})
};

#define NUM (sizeof button / sizeof button[0])

static HWND  hwndButton[NUM];
static RECT  rect;
static TCHAR szTop[] = TEXT("message            wParam       lParam");
static TCHAR szUnd[] = TEXT("_______            ______       ______");
static TCHAR szFormat[] = TEXT(" %s %04x - %04x %04x  -%04x ");
static TCHAR szBuffer[50];
static int cxChar, cyChar;
static HDC hdc;
static PAINTSTRUCT ps;

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxChar = LOWORD(GetDialogBaseUnits());
  cyChar = HIWORD(GetDialogBaseUnits());
  for (unsigned i = 0; i < NUM; i++){
    hwndButton[i] = CreateWindowEx(
      0, TEXT("button"),button[i].szText,
      WS_CHILD | WS_VISIBLE | button[i].iStyle,
      cxChar, cyChar * (1 + 2 * i),
      20 * cxChar, 7 * cyChar / 4,
      hwnd, (HMENU)i,
      ((LPCREATESTRUCT)lParam)->hInstance, NULL
    );
  }//...~
  return 0;
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  rect.left = 24 * cxChar;
  rect.top = 2 * cyChar;
  rect.right =  LOWORD(lParam);
  rect.bottom = HIWORD(lParam);
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  InvalidateRect(hwnd, &rect, TRUE);
  hdc = BeginPaint(hwnd,&ps);
  {
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 24 * cxChar, cyChar, szTop, lstrlen(szTop));
    TextOut(hdc, 24 * cxChar, cyChar, szUnd, lstrlen(szUnd));
  }
  EndPaint(hwnd,&ps);
  return 0;
}//+++

int ssOnCommand(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  ScrollWindow(hwnd, 0, -cyChar, &rect, &rect);
  TCHAR szBuffer[256];
  hdc = GetDC(hwnd);
  {
    SelectObject(hdc, GetStockObject(SYSTEM_FIXED_FONT));
    TextOut(
      hdc,
      24 * cxChar, cyChar * (rect.bottom / cyChar - 1),
      szBuffer,
      wsprintf(
        szBuffer, szFormat,
        message == WM_DRAWITEM
          ?TEXT("WM_DRAWITEM")
          :TEXT("WM_COMMAND"),
        HIWORD(wParam), LOWORD(wParam),
        HIWORD(lParam), LOWORD(lParam)
      )
    );
  }
  ReleaseDC(hwnd, hdc);
  ValidateRect(hwnd, &rect);
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
    case WM_PAINT:return ssOnPaint(hwnd,message,wParam,lParam);
    case WM_DRAWITEM:
    case WM_COMMAND:return ssOnCommand(hwnd,message,wParam,lParam);
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
    800,600,
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
