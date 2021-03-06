/* Fig.7-6 */

#define C_FIG_NAME TEXT("CHECKER2")

#define DIVISIONS 5

static BOOL fState[DIVISIONS][DIVISIONS];
static int  cxBlock, cyBlock;
static HDC hdc;
static int x, y;
static PAINTSTRUCT ps;
static POINT point;
static RECT rect;

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxBlock = LOWORD(lParam) / DIVISIONS;
  cyBlock = HIWORD(lParam) / DIVISIONS;
  return 0;
}//+++

int ssOnKeyDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  GetCursorPos(&point);
  ScreenToClient(hwnd, &point);
  x = max(0, min(DIVISIONS - 1, point.x / cxBlock));
  y = max(0, min(DIVISIONS - 1, point.y / cyBlock));
  switch (wParam){
    case 'Q':case VK_ESCAPE:
      PostQuitMessage(0);
    break;
    //--
    case 'W':case VK_UP:  y--;break;
    case 'S':case VK_DOWN:y++;break;
    case 'A':case VK_LEFT: x--;break;
    case 'D':case VK_RIGHT:x++;break;
    //--
    case VK_HOME:
      x = 0;
      y = 0;
    break;
    case VK_END:
      x = DIVISIONS - 1;
      y = DIVISIONS - 1;
    break;
    case 'J':case VK_RETURN:case VK_SPACE:
      SendMessage(hwnd,
        WM_LBUTTONDOWN, MK_LBUTTON,
        MAKELONG(x * cxBlock, y * cyBlock)
      );
    break;
  }//..?
  
  //--
  x = (x + DIVISIONS) % DIVISIONS;
  y = (y + DIVISIONS) % DIVISIONS;
  point.x = x * cxBlock + cxBlock / 2;
  point.y = y * cyBlock + cyBlock / 2;
  
  //--
  ClientToScreen(hwnd, &point);
  SetCursorPos(point.x, point.y);
  return 0;
  
}//+++

int ssOnLeftButtonDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  x = LOWORD(lParam) / cxBlock;
  y = HIWORD(lParam) / cyBlock;
  if (x < DIVISIONS && y < DIVISIONS){
    fState[x][y] ^= 1;
    rect.left = x * cxBlock;
    rect.top = y * cyBlock;
    rect.right = (x + 1) * cxBlock;
    rect.bottom = (y + 1) * cyBlock;
    InvalidateRect(hwnd, &rect, FALSE);
  }else{
    MessageBeep(0);
  }//..?
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
		for (x = 0; x < DIVISIONS; x++){for (y = 0; y < DIVISIONS; y++){
      Rectangle(hdc,
         x      * cxBlock, y      * cyBlock,
        (x + 1) * cxBlock,(y + 1) * cyBlock
      );
      if (fState[x][y]){
        MoveToEx( hdc,  x      * cxBlock , y      * cyBlock, NULL);
        LineTo(   hdc, (x + 1) * cxBlock, (y + 1) * cyBlock);
        MoveToEx( hdc,  x      * cxBlock, (y + 1) * cyBlock, NULL);
        LineTo(   hdc, (x + 1) * cxBlock,  y      * cyBlock);
      }
    }}//..~
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
    case WM_SETFOCUS:ShowCursor(TRUE);return 0;
    case WM_KILLFOCUS:ShowCursor(FALSE);return 0;
    //--
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_KEYDOWN:return ssOnKeyDown(hwnd,message,wParam,lParam);
    case WM_LBUTTONDOWN:return ssOnLeftButtonDown(hwnd,message,wParam,lParam);
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
  wndclass.hCursor       = LoadCursor(NULL, IDC_CROSS);
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
