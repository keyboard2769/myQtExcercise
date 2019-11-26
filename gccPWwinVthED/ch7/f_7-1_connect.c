/* Fig.7-1 : CONNECT */

#define MAXPOINTS 256
#define SLOWRATE 8

static int iCount, iSlowDown;
static POINT pt[MAXPOINTS] ;
static HDC hdc ;
static PAINTSTRUCT ps ;

int ssOnLeftButtonDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  iCount = 0 ;
  iSlowDown = 0;
  InvalidateRect (hwnd, NULL, TRUE) ;
  return 0;
}//+++

int ssOnLeftButtonUp(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  InvalidateRect (hwnd, NULL, FALSE) ;
  return 0 ;
}//+++

int ssOnMouseMove(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if (wParam & MK_LBUTTON && iCount < MAXPOINTS){
    pt[iCount].x  = LOWORD (lParam) ;
    pt[iCount].y = HIWORD (lParam) ;
    iSlowDown++;if(iSlowDown%SLOWRATE==1){iCount++;}
    hdc = GetDC (hwnd) ;
    {
      SetPixel (hdc, LOWORD (lParam), HIWORD (lParam), 0) ;
    }
    ReleaseDC (hwnd, hdc) ;
  }//..?
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
    ShowCursor (TRUE) ;
    for(int i = 0;i<iCount-1;i++){for(int j=i+1;j<iCount;j++){
      MoveToEx (hdc, pt[i].x, pt[i].y, NULL) ;
      LineTo (hdc, pt[j].x, pt[j].y) ;
    }}//..~
    ShowCursor (FALSE) ;
    SetCursor (LoadCursor (NULL, IDC_ARROW));
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
    case WM_LBUTTONDOWN:return ssOnLeftButtonDown(hwnd,message,wParam,lParam);
    case WM_LBUTTONUP:return ssOnLeftButtonUp(hwnd,message,wParam,lParam);
    case WM_MOUSEMOVE:return ssOnMouseMove(hwnd,message,wParam,lParam);
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
    szAppName,TEXT("CONNECT"),WS_OVERLAPPEDWINDOW,
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
