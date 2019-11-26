/* Fig.5-27 : CLOVER*/

#include <math.h>
#define TWO_PI (2.0 * 3.14159)

static int cxClient, cyClient ;
static double fAngle, fRadius ;
static HDC hdc;
static PAINTSTRUCT ps;
static HCURSOR hCursor ;
static HRGN hRgnTemp[6];
static HRGN hRgnClip;

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxClient = LOWORD (lParam) ;
  cyClient = HIWORD (lParam) ;
  hCursor = SetCursor (LoadCursor (NULL, IDC_WAIT)) ;
  ShowCursor (TRUE) ;
  if (hRgnClip){DeleteObject (hRgnClip);}
  hRgnTemp[0] = CreateEllipticRgn (
    0, cyClient / 3,
    cxClient / 2, 2 * cyClient / 3
  ) ;
  hRgnTemp[1] = CreateEllipticRgn (
    cxClient / 2, cyClient / 3,
    cxClient, 2 * cyClient / 3
  ) ;
  hRgnTemp[2] = CreateEllipticRgn (
    cxClient / 3, 0,
    2 * cxClient / 3, cyClient / 2
  ) ;
  hRgnTemp[3] = CreateEllipticRgn (
    cxClient / 3, cyClient / 2,
    2 * cxClient / 3, cyClient
  ) ;
  hRgnTemp[4] = CreateRectRgn (
    0, 0,
    1, 1
  ) ;
  hRgnTemp[5] = CreateRectRgn (
    0, 0,
    1, 1
  ) ;
  hRgnClip = CreateRectRgn (0, 0, 1, 1) ;
  CombineRgn (hRgnTemp[4], hRgnTemp[0], hRgnTemp[1], RGN_OR) ;
  CombineRgn (hRgnTemp[5], hRgnTemp[2], hRgnTemp[3], RGN_OR) ;
  CombineRgn (hRgnClip, hRgnTemp[4], hRgnTemp[5], RGN_XOR) ;
  for (int i = 0 ; i < 6 ; i++){
    DeleteObject (hRgnTemp[i]) ;
  }//..~
  SetCursor (hCursor) ;
  ShowCursor (FALSE) ;
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    SetViewportOrgEx (hdc, cxClient / 2, cyClient / 2, NULL) ;
    SelectClipRgn (hdc, hRgnClip) ;
    fRadius = hypot (cxClient / 2.0, cyClient / 2.0) ;
    for (fAngle = 0.0 ; fAngle < TWO_PI ; fAngle += TWO_PI / 360){
      MoveToEx (hdc, 0, 0, NULL) ;
      LineTo (hdc,
        (int)( 1* fRadius * cos (fAngle) + 0.5),
        (int)(-1* fRadius * sin (fAngle) + 0.5)
      );
    }//..~
  }
  EndPaint(hwnd,&ps);
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  DeleteObject (hRgnClip) ;
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
    szAppName,TEXT("CLOVER"),WS_OVERLAPPEDWINDOW,
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
