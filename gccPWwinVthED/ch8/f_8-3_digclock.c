/* Fig.8-3 */

#define C_FIG_NAME TEXT("DIGCLOCK")
#define ID_TIMER    1
#define C_EXT_RT (4/1)
#define C_ORG_RT (3/2)

static BOOL   f24Hour, fSuppress;
static HBRUSH hBrushRed;
static int    cxClient, cyClient;
static HDC           hdc;
static PAINTSTRUCT   ps;

static BOOL fSevenSegment[10][7] = {
  {1, 1, 1, 0, 1, 1, 1}, // 0
  {0, 0, 1, 0, 0, 1, 0}, // 1
  {1, 0, 1, 1, 1, 0, 1}, // 2
  {1, 0, 1, 1, 0, 1, 1}, // 3
  {0, 1, 1, 1, 0, 1, 0}, // 4
  {1, 1, 0, 1, 0, 1, 1}, // 5
  {1, 1, 0, 1, 1, 1, 1}, // 6
  {1, 0, 1, 0, 0, 1, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};

static POINT ptSegment[7][6] = {
  {{   7,  6},  {11,  2},  {31,  2},  {35,  6},  {31, 10},  {11, 10}},
  {{   6,  7},  {10, 11},  {10, 31},   {6, 35},   {2, 31},   {2, 11}},
  {{  36,  7},  {40, 11},  {40, 31},  {36, 35},  {32, 31},  {32, 11}},
  {{   7, 36},  {11, 32},  {31, 32},  {35, 36},  {31, 40},  {11, 40}},
  {{   6, 37},  {10, 41},  {10, 61},   {6, 65},   {2, 61},   {2, 41}},
  {{  36, 37},  {40, 41},  {40, 61},  {36, 65},  {32, 61},  {32, 41}},
  {{   7, 66},  {11, 62},  {31, 62},  {35, 66},  {31, 70},  {11, 70}}
};

static POINT ptColon[2][4] = {
  { {2,  21},  {6,  17},  {10, 21},  {6, 25},},
  { {2,  51},  {6,  47},  {10, 51},  {6, 55} } 
};

//===

void ccDisplayDigit(HDC hdc, int iNumber){
  int iSeg;
  for (iSeg = 0; iSeg < 7; iSeg++){
    if (fSevenSegment[iNumber][iSeg]){
      Polygon(hdc, ptSegment[iSeg], 6);
    }//..?
  }//..~
}//+++

void ccDisplayTwoDigits(HDC hdc, int iNumber, BOOL fSuppress){
  if (!fSuppress || (iNumber / 10 != 0)){
    ccDisplayDigit(hdc, iNumber / 10);
  }//..?
  OffsetWindowOrgEx(hdc, -42, 0, NULL);
  ccDisplayDigit(hdc, iNumber % 10);
  OffsetWindowOrgEx(hdc, -42, 0, NULL);
}//+++

void ccDisplayColon(HDC hdc){
	Polygon(hdc, ptColon[0], 4);
	Polygon(hdc, ptColon[1], 4);
	OffsetWindowOrgEx(hdc, -12, 0, NULL);
}//+++

void ccDisplayTime(HDC hdc, BOOL f24Hour, BOOL fSuppress){
  SYSTEMTIME st;
  GetLocalTime(&st);
  if (f24Hour){
    ccDisplayTwoDigits(hdc, st.wHour, fSuppress);
  }else{
    ccDisplayTwoDigits(hdc, (st.wHour %= 12) ? st.wHour : 12, fSuppress);
  }//..?
  ccDisplayColon(hdc);
  ccDisplayTwoDigits(hdc, st.wMinute, FALSE);
  ccDisplayColon(hdc);
  ccDisplayTwoDigits(hdc, st.wSecond, FALSE);
}//+++

//===

void ccInitObject(HWND hwnd){
  hBrushRed = CreateSolidBrush(RGB(255, 0, 0));
  SetTimer(hwnd, ID_TIMER, 1000, NULL);
}//+++

void ccInitTime(HWND hwnd){
  TCHAR szBuffer[8];
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, 2);
    f24Hour = (szBuffer[0] == '1');
  GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITLZERO, szBuffer, 2);
    fSuppress = (szBuffer[0] == '0');
  InvalidateRect(hwnd, NULL, TRUE);
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  ccInitObject(hwnd);
  ccInitTime(hwnd);
  return 0;
}//+++

int ssOnSettingChange(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  ccInitTime(hwnd);
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    SetMapMode(hdc, MM_ISOTROPIC);
    SetWindowExtEx(hdc, 276*C_EXT_RT, 72*C_EXT_RT, NULL);
      SetViewportExtEx(hdc, cxClient, cyClient, NULL);
    SetWindowOrgEx(hdc, 138*C_ORG_RT, 36*C_ORG_RT, NULL);
      SetViewportOrgEx(hdc, cxClient / 2, cyClient / 2, NULL);
    SelectObject(hdc, GetStockObject(NULL_PEN));
    SelectObject(hdc, hBrushRed);
    
    ccDisplayTime(hdc, f24Hour, fSuppress);
    
  }
  EndPaint(hwnd,&ps);
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  KillTimer(hwnd, ID_TIMER);
  DeleteObject(hBrushRed);
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    
    case WM_TIMER:
      InvalidateRect(hwnd, NULL, TRUE);
    return 0;
    
    //--
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SETTINGCHANGE:return ssOnSettingChange(hwnd,message,wParam,lParam);
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
