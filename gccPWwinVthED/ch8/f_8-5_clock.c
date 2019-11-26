/* Fig.8-5 */

#include <math.h>

#define C_FIG_NAME TEXT("CLOCK")

#define ID_TIMER 1
#define TWOPI (2 * 3.1415926)

static POINT pt[3][5] = {
  {{0, -150}, {100, 0}, {0, 600}, {-100, 0}, {0, -150}},
  {{0, -200}, { 50, 0}, {0, 800}, { -50, 0}, {0, -200}},
  {{0,    0}, {  0, 0}, {0,   0}, {   0, 0}, {0,  800}} 
};

static int cxClient, cyClient;
static SYSTEMTIME stPrevious;
static BOOL fChange;
static HDC hdc;
static PAINTSTRUCT ps;
static SYSTEMTIME st;

//===

void ccSetIsotropic(HDC hdc, int cxClient, int cyClient){
  SetMapMode(hdc, MM_ISOTROPIC);
  SetWindowExtEx(hdc, 1000, 1000, NULL);
  SetViewportExtEx(hdc, cxClient / 2, -cyClient / 2, NULL);
  SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
}//+++

void ccRotatePoint(POINT pt[], int iNum, int iAngle){
  int   i;
  POINT ptTemp;
  for (i = 0; i < iNum; i++){
    ptTemp.x = (int)(
      pt[i].x * cos(TWOPI * iAngle / 360)
     +pt[i].y * sin(TWOPI * iAngle / 360)
    );
    ptTemp.y = (int)(
      pt[i].y * cos(TWOPI * iAngle / 360)
     -pt[i].x * sin(TWOPI * iAngle / 360)
    );
    pt[i] = ptTemp;
  }//..~
}//+++

void ccDrawClock(HDC hdc){
	int   iAngle;
	POINT pt[3];
	for (iAngle = 0; iAngle < 360; iAngle += 6){
		pt[0].x = 0;pt[0].y = 900;
		ccRotatePoint(pt, 1, iAngle);
		pt[2].x = pt[2].y = iAngle % 5 ? 33 : 100;
		pt[0].x -= pt[2].x / 2;pt[0].y -= pt[2].y / 2;
		pt[1].x = pt[0].x + pt[2].x;pt[1].y = pt[0].y + pt[2].y;
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Ellipse(hdc, pt[0].x, pt[0].y, pt[1].x, pt[1].y);
	}//..?
}//+++

void ccDrawHands(HDC hdc, SYSTEMTIME * pst, BOOL fChange){
  int i, iAngle[3];
  POINT ptTemp[3][5];
  iAngle[0] = (pst->wHour * 30) % 360 + pst->wMinute / 2;
    iAngle[1] = pst->wMinute * 6;
    iAngle[2] = pst->wSecond * 6;
  memcpy(ptTemp, pt, sizeof(pt));
  for (i = fChange ? 0 : 2; i < 3; i++){
    ccRotatePoint(ptTemp[i], 5, iAngle[i]);
    Polyline(hdc, ptTemp[i], 5);
  }//..~
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  SetTimer(hwnd, ID_TIMER, 1000, NULL);
  GetLocalTime(&st);
  stPrevious = st;
  return 0;
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxClient = LOWORD(lParam);
  cyClient = HIWORD(lParam);
  return 0;
}//+++

int ssOnTimer(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  GetLocalTime(&st);
  fChange = 
    (st.wHour != stPrevious.wHour)
  ||(st.wMinute != stPrevious.wMinute);
  hdc = GetDC(hwnd);
  {
    ccSetIsotropic(hdc, cxClient, cyClient);
    SelectObject(hdc, GetStockObject(WHITE_PEN));
      ccDrawHands(hdc, &stPrevious, fChange);
    SelectObject(hdc, GetStockObject(BLACK_PEN));
      ccDrawHands(hdc, &st, TRUE);
  }
  ReleaseDC(hwnd, hdc);
  stPrevious = st;
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    ccSetIsotropic(hdc, cxClient, cyClient);
    ccDrawClock(hdc);
    ccDrawHands(hdc, &stPrevious, TRUE);
  }
  EndPaint(hwnd,&ps);
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  KillTimer(hwnd, ID_TIMER);
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
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
