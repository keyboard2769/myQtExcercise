/* Fig.7-11 */

#define C_FIG_NAME TEXT("BLOKOUT2")

static BOOL fBlocking, fValidBox;
static POINT ptBeg, ptEnd, ptBoxBeg, ptBoxEnd;
static HDC hdc;
static PAINTSTRUCT ps;

//===

void ccDrawBoxOutline(HWND hwnd, POINT ptBeg, POINT ptEnd){
  HDC lpContext;
  lpContext = GetDC(hwnd);
  SetROP2(lpContext, R2_NOT);
  SelectObject(lpContext, GetStockObject(NULL_BRUSH));
  Rectangle(lpContext, ptBeg.x, ptBeg.y, ptEnd.x, ptEnd.y);
  ReleaseDC(hwnd, lpContext);
}//+++

//===

int ssOnLeftButtonDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  ptBeg.x = ptEnd.x = LOWORD(lParam);
  ptBeg.y = ptEnd.y = HIWORD(lParam);
  ccDrawBoxOutline(hwnd, ptBeg, ptEnd);
  SetCapture(hwnd);
  SetCursor(LoadCursor(NULL, IDC_CROSS));
  fBlocking = TRUE;
  return 0;
}//+++

int ssOnLeftButtonUp(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if (fBlocking){
    ccDrawBoxOutline(hwnd, ptBeg, ptEnd);
    ptBoxBeg = ptBeg;
    ptBoxEnd.x = LOWORD(lParam);
    ptBoxEnd.y = HIWORD(lParam);
    ReleaseCapture();
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    fBlocking = FALSE;
    fValidBox = TRUE;
    InvalidateRect(hwnd, NULL, TRUE);
  }//..?
  return 0;
}//+++

int ssOnMouseMove(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if (fBlocking){
    SetCursor(LoadCursor(NULL, IDC_CROSS));
    ccDrawBoxOutline(hwnd, ptBeg, ptEnd);
    ptEnd.x = LOWORD(lParam);
    ptEnd.y = HIWORD(lParam);
    ccDrawBoxOutline(hwnd, ptBeg, ptEnd);
  }//..?
  return 0;
}//+++

int ssOnChar(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if((wParam == 'Q')||(wParam == 'q')){
    TCHAR lpBUF[256];
    wsprintf(lpBUF,TEXT("[%d:%d|%d:%d][%d:%d|%d:%d]\n"),
      ptBeg.x,ptBeg.y,ptEnd.x,ptEnd.y,
      ptBoxBeg.x,ptBoxBeg.y,ptBoxEnd.x,ptBoxEnd.y
    );
    wprintf(lpBUF);
    PostQuitMessage(0);
  }//..?
  if (fBlocking && (wParam == '\x1B')){// i.e., Escape
    ccDrawBoxOutline(hwnd, ptBeg, ptEnd);
    ReleaseCapture();
    SetCursor(LoadCursor(NULL, IDC_ARROW));
    fBlocking = FALSE;
  }//..?
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
		if (fValidBox){
			SelectObject(hdc, GetStockObject(BLACK_BRUSH));
			Rectangle(hdc,
        ptBoxBeg.x, ptBoxBeg.y,
				ptBoxEnd.x, ptBoxEnd.y
      );
		}//..?
		if (fBlocking){
			SetROP2(hdc, R2_NOT);
			SelectObject(hdc, GetStockObject(NULL_BRUSH));
			Rectangle(hdc,
        ptBeg.x, ptBeg.y,
        ptEnd.x, ptEnd.y
        );
		}//..?
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
    case WM_CHAR:return ssOnChar(hwnd,message,wParam,lParam);
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
