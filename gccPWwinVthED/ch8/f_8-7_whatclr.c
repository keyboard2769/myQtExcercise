
/* Fig.8-7 */

#define C_FIG_NAME TEXT("WHATCLR")

#define ID_TIMER 1

static COLORREF cr, crLast;
static HDC hdcScreen;
static HDC hdc;
static PAINTSTRUCT ps;
static POINT pt;
static RECT rc;

//===

void ccFindWindowSize(int* pcxWindow, int* pcyWindow){
  HDC hdcScreen;
  TEXTMETRIC tm;
  hdcScreen = CreateIC(TEXT("DISPLAY"), NULL, NULL, NULL);
  GetTextMetrics(hdcScreen, &tm);
  DeleteDC(hdcScreen);
  *pcxWindow = 
      2 * GetSystemMetrics(SM_CXBORDER)
    +12 * tm.tmAveCharWidth;
  *pcyWindow =
    2 * GetSystemMetrics(SM_CYBORDER)
   +GetSystemMetrics(SM_CYCAPTION)
   +2 * tm.tmHeight;
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
  SetTimer(hwnd, ID_TIMER, 100, NULL);
  return 0;
}//+++

int ssOnDisplayChange(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  DeleteDC(hdcScreen);
  hdcScreen = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
  return 0;
}//+++

int ssOnTimer(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  GetCursorPos(&pt);
  cr = GetPixel(hdcScreen, pt.x, pt.y);
  if (cr != crLast){
    crLast = cr;
    InvalidateRect(hwnd, NULL, FALSE);
  }//..?
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    TCHAR szBuffer[256];
    GetClientRect(hwnd, &rc);
    wsprintf(
      szBuffer,
      TEXT("  %02X %02X %02X  "),
      GetRValue(cr), GetGValue(cr), GetBValue(cr)
    );
    DrawText(
      hdc,
      szBuffer, -1,
      &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER
    );
  }
  EndPaint(hwnd,&ps);
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  DeleteDC(hdcScreen);
  KillTimer(hwnd, ID_TIMER);
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_DISPLAYCHANGE:return ssOnDisplayChange(hwnd,message,wParam,lParam);
    case WM_TIMER:return ssOnTimer(hwnd,message,wParam,lParam);
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
  static TCHAR szClassName[] = TEXT("CUSTOMCLS");
  HWND hwnd;
  int cxWindow, cyWindow;
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
  wndclass.lpszClassName = szClassName;
  if(!RegisterClass(&wndclass)){
    MessageBox(NULL,
      TEXT("not_windows_nt"),
      szClassName, MB_ICONERROR
    );
    return -1;
  }//..?
  
  //--
  ccFindWindowSize(&cxWindow, &cyWindow);
  
  //--
  hwnd = CreateWindow(
    szClassName,C_FIG_NAME,WS_OVERLAPPEDWINDOW,
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
