/* Fig.5-16 : BEZIER*/

static HDC hdc;
static PAINTSTRUCT ps;
static POINT apt[4] ;
static int cxClient, cyClient ;

void ccDrawBezier(HDC pxHDC, POINT pxDesPoint[]){
  
  PolyBezier (pxHDC, pxDesPoint, 4) ;
  
  MoveToEx (pxHDC, pxDesPoint[0].x, pxDesPoint[0].y, NULL) ;
  LineTo (pxHDC, pxDesPoint[1].x, pxDesPoint[1].y) ;
  
  MoveToEx (pxHDC, pxDesPoint[2].x, pxDesPoint[2].y, NULL) ;
  LineTo (pxHDC, pxDesPoint[3].x, pxDesPoint[3].y) ;
  
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxClient = LOWORD(lParam);
  cyClient = HIWORD(lParam);
  apt[0].x = cxClient / 4 ;
  apt[0].y = cyClient / 2 ;
  apt[1].x = cxClient / 2 ;
  apt[1].y = cyClient / 4 ;
  apt[2].x = cxClient / 2 ;
  apt[2].y = 3 * cyClient / 4 ;
  apt[3].x = 3 * cxClient / 4 ;
  apt[3].y = cyClient / 2 ;
  return 0;
}//+++

int ssOnMouseMotion(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if(wParam & MK_LBUTTON || wParam & MK_RBUTTON){
    hdc = GetDC (hwnd) ;
    SelectObject (hdc, GetStockObject (WHITE_PEN)) ;
    ccDrawBezier (hdc, apt) ;
    if (wParam & MK_LBUTTON){
      apt[1].x = LOWORD (lParam) ;
      apt[1].y = HIWORD (lParam) ;
    }//..?
    if (wParam & MK_RBUTTON){
      apt[2].x = LOWORD (lParam) ;
      apt[2].y = HIWORD (lParam) ;
    }//..?
    SelectObject (hdc, GetStockObject (BLACK_PEN)) ;
    ccDrawBezier (hdc, apt) ;
    ReleaseDC (hwnd, hdc) ;
  }//..?
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  InvalidateRect (hwnd, NULL, TRUE) ;
  hdc = BeginPaint(hwnd,&ps);
  {
    ccDrawBezier (hdc, apt) ;
  }
  EndPaint (hwnd, &ps) ;
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_MOUSEMOVE:return ssOnMouseMotion(hwnd,message,wParam,lParam);
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
    szAppName,TEXT("BEZIER"),WS_OVERLAPPEDWINDOW,
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
