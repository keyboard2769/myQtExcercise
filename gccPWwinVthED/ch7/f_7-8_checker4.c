/* Fig.7-8 */

#define C_FIG_NAME TEXT("CHECKER4")
#define DIVISIONS 5

static int idFocus = 0;
static int cxBlock, cyBlock, x, y;
static TCHAR szChildClassName[] = TEXT("CUSTOM_CHILD");
static HWND hwndChild[DIVISIONS][DIVISIONS];
static HDC hdc;
static PAINTSTRUCT ps;

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  for (x = 0; x < DIVISIONS; x++){
    for (y = 0; y < DIVISIONS; y++){
      hwndChild[x][y] = CreateWindow(
        szChildClassName, NULL,
        WS_CHILDWINDOW | WS_VISIBLE,
        0, 0, 0, 0,
        hwnd, (HMENU)(y << 8 | x),
        (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
        NULL
      );
    }}//..~
  return 0;
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  cxBlock = LOWORD(lParam) / DIVISIONS;
  cyBlock = HIWORD(lParam) / DIVISIONS;
  for (x = 0; x < DIVISIONS; x++){
    for (y = 0; y < DIVISIONS; y++){
      MoveWindow(hwndChild[x][y],
        cxBlock*x, cyBlock*y,
        cxBlock,   cyBlock,
        TRUE
      );
  }}//..~
  return 0;
}//+++

int ssOnKeyDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  x = idFocus & 0xFF;
  y = idFocus >> 8;
  switch (wParam){
    case 'Q':case VK_ESCAPE:
      PostQuitMessage(0);
    break;
    //--
    case 'W':case VK_UP:    y--;break;
    case 'S':case VK_DOWN:  y++;break;
    case 'A':case VK_LEFT:  x--;break;
    case 'D':case VK_RIGHT: x++;break;
    case VK_HOME:
      x = 0;
      y = 0;
    break;
    case VK_END:
      x = DIVISIONS - 1;
      y = DIVISIONS - 1;
    break;
    default:return 0;
  }//..?
  x = (x + DIVISIONS) % DIVISIONS;
  y = (y + DIVISIONS) % DIVISIONS;
  idFocus = y << 8 | x;
  SetFocus(GetDlgItem(hwnd, idFocus));
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_LBUTTONDOWN:MessageBeep(0);return 0;
    //--
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_KEYDOWN:return ssOnKeyDown(hwnd,message,wParam,lParam);
    case WM_DESTROY:return ssOnDestroy(hwnd,message,wParam,lParam);
    default:return DefWindowProc(hwnd,message,wParam,lParam);
  }//..?
}//+++

LRESULT CALLBACK
ChildWndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
	HDC         lpChildContext;
	PAINTSTRUCT lpChilePainter;
	RECT        lpChildRect;
	switch (message){
    
    //--
    case WM_CREATE:
      SetWindowLongPtr(hwnd,
        0,
        0 //..on-off flag
      );       
    return 0;
    
    //--
    case WM_KEYDOWN:
      if(wParam!=VK_RETURN && wParam != VK_SPACE && wParam != 'J'){
        SendMessage(GetParent(hwnd),message,wParam,lParam);
        return 0;
      }//..?
    //FALL-THROUGH

    //--
    case WM_LBUTTONDOWN:
      SetWindowLongPtr(hwnd, 
        0,
        1 ^ GetWindowLongPtr(hwnd, 0) //.. flip?
      );
      SetFocus(hwnd);
      InvalidateRect(hwnd, NULL, FALSE);
    return 0;
    
    //-- 
    case WM_SETFOCUS:idFocus = GetWindowLongPtr(hwnd, GWLP_ID);
    case WM_KILLFOCUS:InvalidateRect(hwnd, NULL, TRUE);return 0;
    
    //--
    case WM_PAINT:
      lpChildContext = BeginPaint(hwnd, &lpChilePainter);
      {
        GetClientRect(hwnd, &lpChildRect);
        Rectangle(lpChildContext, 0, 0, lpChildRect.right, lpChildRect.bottom);
        if (GetWindowLongPtr(hwnd, 0)){
          MoveToEx(lpChildContext, 0, 0, NULL);
          LineTo(lpChildContext, lpChildRect.right, lpChildRect.bottom);
          MoveToEx(lpChildContext, 0, lpChildRect.bottom, NULL);
          LineTo(lpChildContext, lpChildRect.right, 0);
        }//..?
        if (hwnd == GetFocus()){
          lpChildRect.left += lpChildRect.right / 10;
          lpChildRect.right -= lpChildRect.left;
          lpChildRect.top += lpChildRect.bottom / 10;
          lpChildRect.bottom -= lpChildRect.top;

          SelectObject(lpChildContext, GetStockObject(NULL_BRUSH));
          SelectObject(lpChildContext, CreatePen(PS_DASH, 0, 0));
          Rectangle(lpChildContext,
            lpChildRect.left, lpChildRect.top,
            lpChildRect.right, lpChildRect.bottom
          );
          DeleteObject(
            SelectObject(lpChildContext, GetStockObject(BLACK_PEN))
          );
        }//..?
      }
      EndPaint(hwnd, &lpChilePainter);
    return 0;
    
	}//..?
	return DefWindowProc(hwnd, message, wParam, lParam);
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
  wndclass.lpfnWndProc   = ChildWndProc;
  wndclass.cbWndExtra    = sizeof(long);
  wndclass.hIcon         = NULL;
  wndclass.lpszClassName = szChildClassName;
  RegisterClass(&wndclass);
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
