/* Fig.5-21 : ALTWIND */

static HDC hdc;
static PAINTSTRUCT ps;
static int cxClient, cyClient ;
static POINT apt[10] ;
static POINT aptFigure [10] = {
  10,70, 50,70, 50,10, 90,10, 90,50,
  30,50, 30,90, 70,90, 70,30, 10,30
};//***

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxClient = LOWORD (lParam) ;
  cyClient = HIWORD (lParam) ;
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    SelectObject (hdc, GetStockObject (GRAY_BRUSH)) ;
    for (int i = 0 ; i < 10 ; i++){
      apt[i].x = cxClient * aptFigure[i].x / 200 ;
      apt[i].y = cyClient * aptFigure[i].y / 100 ;
    }//..~
    SetPolyFillMode (hdc, ALTERNATE) ;
    Polygon (hdc, apt, 10);
    for (int i = 0 ; i < 10 ; i++){
      apt[i].x += cxClient / 2 ;
    }//..~
    SetPolyFillMode (hdc, WINDING) ;
    Polygon (hdc, apt, 10) ;
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
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
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
    szAppName,TEXT("ALTWIND"),WS_OVERLAPPEDWINDOW,
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
