/* Fig.8-2 */

#define C_FIG_NAME TEXT("BEEPER2")
#define ID_TIMER    1

static BOOL fFlipFlop = FALSE;
static HBRUSH hBrush;
static HDC hdc;
static PAINTSTRUCT ps;
static RECT rc;

VOID CALLBACK TimerProc(HWND hwnd,UINT message,UINT iTimerID,DWORD dwTimer){
  MessageBeep(MB_ICONQUESTION);
  fFlipFlop = !fFlipFlop;
  GetClientRect(hwnd, &rc);
  hdc = GetDC(hwnd);
  hBrush = CreateSolidBrush(fFlipFlop
    ?RGB(0x33, 0x33, 0x33)
    :RGB(0xCC, 0xCC, 0xCC)
  );
  FillRect(hdc, &rc, hBrush);
  ReleaseDC(hwnd,hdc);
  DeleteObject(hBrush);
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    
    case WM_CREATE:
      SetTimer(hwnd, ID_TIMER, 500, TimerProc);
    return 0;
    
    case WM_DESTROY:
      PostQuitMessage(0);
    return 0;
  
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
