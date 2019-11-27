/* Fig.9-3 */

#define C_FIG_NAME TEXT("OWNDRAW")

#define ID_SMALLER 1
#define ID_LARGER  2

static HINSTANCE hInst;
static HWND hwndSmaller, hwndLarger;
static int cxClient, cyClient, cxChar, cyChar;
static int cx, cy;
static LPDRAWITEMSTRUCT pdis;
static POINT pt[3];
static RECT rc;

//===

int ccButtonWidth(){return 8*cxChar;}

int ccButtonHeight(){return 4*cyChar;}

void ccTriangle(HDC pxContext, POINT pxLesPoint[]){
  SelectObject(pxContext, GetStockObject(BLACK_BRUSH));
  Polygon(pxContext, pxLesPoint, 3);
  SelectObject(pxContext, GetStockObject(WHITE_BRUSH));
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  cxChar = LOWORD(GetDialogBaseUnits());
  cyChar = HIWORD(GetDialogBaseUnits());

  //--
  hwndSmaller = CreateWindow(
    TEXT("button"), TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0, 0, ccButtonWidth(), ccButtonHeight(),
    hwnd, (HMENU)ID_SMALLER, hInst, NULL
  );

  //--
  hwndLarger = CreateWindow(
    TEXT("button"), TEXT(""),
    WS_CHILD | WS_VISIBLE | BS_OWNERDRAW,
    0, 0, ccButtonWidth(), ccButtonHeight(),
    hwnd, (HMENU)ID_LARGER, hInst, NULL
  );
  
  return 0;
  
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  cxClient = LOWORD(lParam);
  cyClient = HIWORD(lParam);

  //-- 
  MoveWindow(hwndSmaller,
    cxClient / 2 - 3 * ccButtonWidth() / 2,
    cyClient / 2 - ccButtonHeight() / 2,
    ccButtonWidth(), ccButtonHeight(), TRUE
  );

  //--
  MoveWindow(hwndLarger,
    cxClient / 2 + ccButtonWidth() / 2,
    cyClient / 2 - ccButtonHeight() / 2,
    ccButtonWidth(), ccButtonHeight(), TRUE
  );
  
  return 0;
  
}//+++

int ssOnCommand(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  GetWindowRect(hwnd, &rc);

  //--
  switch (wParam){
    case ID_SMALLER:
      rc.left += cxClient / 20;
      rc.right -= cxClient / 20;
      rc.top += cyClient / 20;
      rc.bottom -= cyClient / 20;
    break;

    case ID_LARGER:
      rc.left -= cxClient / 20;
      rc.right += cxClient / 20;
      rc.top -= cyClient / 20;
      rc.bottom += cyClient / 20;
    break;
  }//..?
  
  //--
  MoveWindow(hwnd,
    rc.left, rc.top,
    rc.right - rc.left,
    rc.bottom - rc.top,
    TRUE
  );
  
  return 0;
  
}//+++

int ssOnDrawItem(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //-- 
  pdis = (LPDRAWITEMSTRUCT)lParam;

  //--
  FillRect(pdis->hDC, &pdis->rcItem, GetStockObject(WHITE_BRUSH));
  FrameRect(pdis->hDC, &pdis->rcItem, GetStockObject(BLACK_BRUSH));

  //--
  cx = pdis->rcItem.right - pdis->rcItem.left;
  cy = pdis->rcItem.bottom - pdis->rcItem.top;

  switch (pdis->CtlID){
    case ID_SMALLER:
      
      pt[0].x = 3 * cx / 8;  pt[0].y = 1 * cy / 8;
      pt[1].x = 5 * cx / 8;  pt[1].y = 1 * cy / 8;
      pt[2].x = 4 * cx / 8;  pt[2].y = 3 * cy / 8;
      ccTriangle(pdis->hDC, pt);

      pt[0].x = 7 * cx / 8;  pt[0].y = 3 * cy / 8;
      pt[1].x = 7 * cx / 8;  pt[1].y = 5 * cy / 8;
      pt[2].x = 5 * cx / 8;  pt[2].y = 4 * cy / 8;
      ccTriangle(pdis->hDC, pt);

      pt[0].x = 5 * cx / 8;  pt[0].y = 7 * cy / 8;
      pt[1].x = 3 * cx / 8;  pt[1].y = 7 * cy / 8;
      pt[2].x = 4 * cx / 8;  pt[2].y = 5 * cy / 8;
      ccTriangle(pdis->hDC, pt);

      pt[0].x = 1 * cx / 8;  pt[0].y = 5 * cy / 8;
      pt[1].x = 1 * cx / 8;  pt[1].y = 3 * cy / 8;
      pt[2].x = 3 * cx / 8;  pt[2].y = 4 * cy / 8;
      ccTriangle(pdis->hDC, pt);
      
    break;

    case ID_LARGER:

      pt[0].x = 5 * cx / 8;  pt[0].y = 3 * cy / 8;
      pt[1].x = 3 * cx / 8;  pt[1].y = 3 * cy / 8;
      pt[2].x = 4 * cx / 8;  pt[2].y = 1 * cy / 8;
      ccTriangle(pdis->hDC, pt);

      pt[0].x = 5 * cx / 8;  pt[0].y = 5 * cy / 8;
      pt[1].x = 5 * cx / 8;  pt[1].y = 3 * cy / 8;
      pt[2].x = 7 * cx / 8;  pt[2].y = 4 * cy / 8;
      ccTriangle(pdis->hDC, pt);

      pt[0].x = 3 * cx / 8;  pt[0].y = 5 * cy / 8;
      pt[1].x = 5 * cx / 8;  pt[1].y = 5 * cy / 8;
      pt[2].x = 4 * cx / 8;  pt[2].y = 7 * cy / 8;
      ccTriangle(pdis->hDC, pt);

      pt[0].x = 3 * cx / 8;  pt[0].y = 3 * cy / 8;
      pt[1].x = 3 * cx / 8;  pt[1].y = 5 * cy / 8;
      pt[2].x = 1 * cx / 8;  pt[2].y = 4 * cy / 8;
      ccTriangle(pdis->hDC, pt);
    break;
    
  }//..?

  //--
  if (pdis->itemState & ODS_SELECTED){
    InvertRect(pdis->hDC, &pdis->rcItem);
  }//..?
  
  //--
  if (pdis->itemState & ODS_FOCUS){
    pdis->rcItem.left += cx / 16;
    pdis->rcItem.top += cy / 16;
    pdis->rcItem.right -= cx / 16;
    pdis->rcItem.bottom -= cy / 16;
    DrawFocusRect(pdis->hDC, &pdis->rcItem);
  }//..?
  
  return 0;
  
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_COMMAND:return ssOnCommand(hwnd,message,wParam,lParam);
    case WM_DRAWITEM:return ssOnDrawItem(hwnd,message,wParam,lParam);
    case WM_DESTROY:return ssOnDestroy(hwnd,message,wParam,lParam);
    default:return DefWindowProc(hwnd,message,wParam,lParam);
  }//..?
}//+++

int WinSub(
  HINSTANCE hInstance,HINSTANCE hPrevInstance,
  PSTR szCmdLine,int iCmdShow
){
  
  //--
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;
  
  hInst = hInstance;
  
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
  wndclass.lpszClassName = TEXT("MAINWINDOW");
  if(!RegisterClass(&wndclass)){
    MessageBox(NULL,
      TEXT("not_windows_nt"),
      C_FIG_NAME, MB_ICONERROR
    );
    return -1;
  }//..?
  
  //--
  hwnd = CreateWindow(
    wndclass.lpszClassName,C_FIG_NAME,WS_OVERLAPPEDWINDOW,
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
