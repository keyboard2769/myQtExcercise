/* Fig.7-7 */

#define C_FIG_NAME TEXT("CHECKER3")
#define DIVISIONS 5

static int cxBlock, cyBlock, x, y;
static TCHAR szChildClassName[] = TEXT("c3cc");
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
        GWLP_USERDATA,
        0 //..on-off flag
      );       
    return 0;

    //--
    case WM_LBUTTONDOWN:
      SetWindowLongPtr(hwnd, 
        GWLP_USERDATA,
        1 ^ GetWindowLongPtr(hwnd, GWLP_USERDATA) //.. flip?
      );
      InvalidateRect(hwnd, NULL, FALSE);
    return 0;

    //--
    case WM_PAINT:
      lpChildContext = BeginPaint(hwnd, &lpChilePainter);
      {
        GetClientRect(hwnd, &lpChildRect);
        Rectangle(lpChildContext, 0, 0, lpChildRect.right, lpChildRect.bottom);
        if (GetWindowLongPtr(hwnd, GWLP_USERDATA)){
          MoveToEx(lpChildContext, 0, 0, NULL);
          LineTo(lpChildContext, lpChildRect.right, lpChildRect.bottom);
          MoveToEx(lpChildContext, 0, lpChildRect.bottom, NULL);
          LineTo(lpChildContext, lpChildRect.right, 0);
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
