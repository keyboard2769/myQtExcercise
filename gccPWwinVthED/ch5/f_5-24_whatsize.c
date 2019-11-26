/* Fig.5-24 : WHATSIZE */

static HDC hdc;
static PAINTSTRUCT ps;
static TEXTMETRIC tm ;
static TCHAR szHeading []
  = TEXT ("[mapping_mode -left -right -top -bottom]") ;
static TCHAR szUndLine []
  = TEXT ("-------------------------------------------") ;
static int cxChar, cyChar ;

void ccShow(
  HWND pxWindow, HDC pxContext, int pxTextX, int pxTextY,
  int pxMapModeL,TCHAR * pxMapMode
){
  TCHAR lpBUF [60] ;
  RECT lpRECT ;
  SaveDC (pxContext) ;
  SetMapMode (pxContext, pxMapModeL) ;
  GetClientRect (pxWindow, &lpRECT) ;
  DPtoLP (pxContext, (PPOINT) &lpRECT, 2) ;
  RestoreDC (pxContext,-1) ;
  TextOut (
    hdc, pxTextX, pxTextY, lpBUF,
    wsprintf(lpBUF, TEXT ("%s %7d %7d %7d %7d"), pxMapMode,
      lpRECT.left, lpRECT.right, lpRECT.top, lpRECT.bottom)
  );
}//+++

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = GetDC (hwnd) ;
  SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
  GetTextMetrics (hdc, &tm) ;
  cxChar = tm.tmAveCharWidth ;
  cyChar = tm.tmHeight + tm.tmExternalLeading ;
  ReleaseDC (hwnd, hdc) ;
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
    SetMapMode (hdc, MM_ANISOTROPIC) ;
    SetWindowExtEx (hdc, 1, 1, NULL) ;
    SetViewportExtEx (hdc, cxChar, cyChar, NULL) ;
    TextOut (hdc, 1, 1, szHeading, lstrlen (szHeading)) ;
    TextOut (hdc, 1, 2, szUndLine, lstrlen (szUndLine)) ;
    ccShow (hwnd, hdc, 1, 3, MM_TEXT,      TEXT ("TEXT (      pixels)")) ;
    ccShow (hwnd, hdc, 1, 4, MM_LOMETRIC,  TEXT ("LOMETRIC (   .1 mm)")) ;
    ccShow (hwnd, hdc, 1, 5, MM_HIMETRIC,  TEXT ("HIMETRIC (  .01 mm)")) ;
    ccShow (hwnd, hdc, 1, 6, MM_LOENGLISH, TEXT ("LOENGLISH ( .01 in)")) ;
    ccShow (hwnd, hdc, 1, 7, MM_HIENGLISH, TEXT ("HIENGLISH (.001 in)")) ;
    ccShow (hwnd, hdc, 1, 8, MM_TWIPS,     TEXT ("TWIPS (  1/1440 in)")) ;
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
    szAppName,TEXT("WHATSIZE"),WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,
    640,240,
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
