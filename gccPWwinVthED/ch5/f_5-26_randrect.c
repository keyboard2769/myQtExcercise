/* Fig.5-26 : RANDRECT */

static int cxClient, cyClient ;

void ccDrawRectangle (HWND pxWindow){
  HBRUSH lpBrush ;
  HDC lpContext ;
  RECT lpRect ;
  if (cxClient == 0 || cyClient == 0){return ;}
  SetRect(
    &lpRect,
    rand()%cxClient, rand()%cyClient,
    rand()%cxClient, rand()%cyClient
  );
  lpBrush=CreateSolidBrush(RGB(
    rand()%256, rand()%256, rand()%256
  ));
  lpContext = GetDC (pxWindow) ;
  {
    FillRect(lpContext, &lpRect, lpBrush);
  }
  ReleaseDC(pxWindow, lpContext) ;
  DeleteObject(lpBrush) ;
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxClient = LOWORD (lParam) ;
  cyClient = HIWORD (lParam) ;
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_CREATE:return ssOnSize(hwnd,message,wParam,lParam);
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
    szAppName,TEXT("RANDRECT"),WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,CW_USEDEFAULT,
    320,240,
    NULL,NULL,hInstance,NULL
  );
  ShowWindow(hwnd,iCmdShow);
  UpdateWindow(hwnd);
  while(TRUE){
    if(PeekMessage (&msg, NULL, 0, 0, PM_REMOVE)){
      if(msg.message == WM_QUIT){break;}
      TranslateMessage (&msg) ;
      DispatchMessage (&msg) ;
    }else{
      ccDrawRectangle(hwnd);
    }//..?
  }//..~
  
  //--
  return msg.wParam;
  
}//!!!
