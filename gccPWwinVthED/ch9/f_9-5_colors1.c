/* Fig.9-5 */

#include <commctrl.h>
#include <windowsx.h>

#define C_FIG_NAME TEXT("COLORS1")

#define N (3)  // Number of colors.

static int idFocus;
static WNDPROC OldScroll[3] ;

static COLORREF crPrim[N] = {
  RGB(255, 0, 0),
  RGB(0, 255, 0),
  RGB(0, 0, 255)
};
static HBRUSH hBrush[N], hBrushStatic;
static HWND hwndScroll[N], hwndLabel[N], hwndValue[N], hwndRect;
static int color[N], cyChar;
static RECT rcColor;
static TCHAR* szColorLabel[] = {
  TEXT("Red"),
  TEXT("Green"),
  TEXT("Blue")
};
static HINSTANCE pbInstance;

//===

LRESULT CALLBACK ScrollProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  int id = GetWindowLong(hwnd, GWLP_ID);
  switch (message){
    case WM_KEYDOWN:
      if (wParam == VK_TAB){
        SetFocus(
          GetDlgItem(GetParent(hwnd),
          (id + (GetKeyState(VK_SHIFT) < 0) ? 2 : 1)
           % N)
        );
      }//..?
    break;
    case WM_SETFOCUS:
      idFocus = id;
    break;
  }//..?
  return CallWindowProc(OldScroll[id],hwnd,message, wParam, lParam);
}//+++

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  pbInstance = GetWindowInstance(hwnd);

  //--
  hwndRect = CreateWindow(
    TEXT("static"), NULL,
    WS_CHILD | WS_VISIBLE | SS_WHITERECT,
    0, 0, 0, 0,
    hwnd, (HMENU)9, pbInstance, NULL
  );
  for (int i = 0; i < N; i++){
    
    //--
    hwndScroll[i] = CreateWindow(
      TEXT("scrollbar"), NULL,
      WS_CHILD | WS_VISIBLE |
      WS_TABSTOP | SBS_VERT,
      0, 0, 0, 0,
      hwnd, (HMENU)i, pbInstance, NULL
    );
    SetScrollRange(hwndScroll[i], SB_CTL, 0, 255, FALSE);
    SetScrollPos(hwndScroll[i], SB_CTL, 0, FALSE);

    //--
    hwndLabel[i] = CreateWindow(
      TEXT("static"), szColorLabel[i],
      WS_CHILD | WS_VISIBLE | SS_CENTER,
      0, 0, 0, 0,
      hwnd, (HMENU)(i + N),
      pbInstance, NULL
    );
 
    //--
    hwndValue[i] = CreateWindow(
      TEXT("static"), TEXT("0"),
      WS_CHILD | WS_VISIBLE | SS_CENTER,
      0, 0, 0, 0,
      hwnd, (HMENU)(i + 2 * N),
      pbInstance, NULL
    );
    
    //--
    OldScroll[i] = (WNDPROC)SetWindowLong(
      hwndScroll[i],GWL_WNDPROC,(LONG)ScrollProc
    );
    hBrush[i] = CreateSolidBrush(crPrim[i]);
    
  }//..~

  //--
  hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
  cyChar = HIWORD(GetDialogBaseUnits());
  
  return 0;
  
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  int cxClient = GET_X_LPARAM(lParam);
  int cyClient = GET_Y_LPARAM(lParam);
  SetRect(&rcColor, cxClient / 2, 0, cxClient, cyClient);
  MoveWindow(hwndRect, 0, 0, cxClient / 2, cyClient, TRUE);
  for (int i = 0; i < N; i++){
    MoveWindow(hwndScroll[i],
      (2 * i + 1) * cxClient / 14, 2 * cyChar,
      cxClient / 14, cyClient - 4 * cyChar, TRUE
    );
    MoveWindow(hwndLabel[i],
      (4 * i + 1) * cxClient / 28, cyChar / 2,
      cxClient / 7, cyChar, TRUE
    );
    MoveWindow(hwndValue[i],
      (4 * i + 1) * cxClient / 28,
      cyClient - 3 * cyChar / 2,
      cxClient / 7, cyChar, TRUE
    );
  }//..~
  SetFocus(hwnd);
  return 0;
}//+++

int ssOnSetFocus(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  SetFocus(hwndScroll[idFocus]);
  return 0;
}//+++

int ssOnVerticalScroll(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  int i = GetWindowLong((HWND)lParam, GWLP_ID);
  switch (LOWORD(wParam)){
    case SB_PAGEDOWN:color[i] += 15;//..fall through
    case SB_LINEDOWN:
      color[i] = min(255, color[i] + 1);
    break;
    case SB_PAGEUP:color[i] -= 15;//..fall through
    case SB_LINEUP:
      color[i] = max(0, color[i] - 1);
    break;
    case SB_TOP: color[i] = 0; break;
    case SB_BOTTOM: color[i] = 255; break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK:  color[i] = HIWORD(wParam); break;
    default:break;
  }//..?
  
  //--
  TCHAR szBuffer[256];
  SetScrollPos(hwndScroll[i], SB_CTL, color[i], TRUE);
  wsprintf(szBuffer, TEXT("%i"), color[i]);
  SetWindowText(hwndValue[i], szBuffer);
  
  //--
  DeleteObject (
    (HBRUSH) SetClassLong (
      hwnd,
      GCL_HBRBACKGROUND,
      (LONG)CreateSolidBrush(RGB(color[0], color[1], color[2]))
    )
  );
  InvalidateRect(hwnd, &rcColor, TRUE);
  
  return 0;
  
}//+++

int ssOnControlColorSCrollBar(
  HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam
){
  int i = GetWindowLong((HWND)lParam, GWLP_ID);
  return (LRESULT)hBrush[i];
}//+++

int ssOnControlColorStatic(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  int i = GetWindowLong((HWND)lParam, GWLP_ID);
  if (i >= N && i <= 8){
    SetTextColor((HDC)wParam, crPrim[i % N]);
    SetBkColor((HDC)wParam, GetSysColor(COLOR_BTNHIGHLIGHT));
    return (LRESULT)hBrushStatic;
  }//..?
  return DefWindowProc(hwnd,message,wParam,lParam);
}//+++

int ssOnSystemColorChange(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  DeleteObject(hBrushStatic);
  hBrushStatic = CreateSolidBrush(GetSysColor(COLOR_BTNHIGHLIGHT));
  return 0;
}//+++

int ssOnDestroy(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  DeleteObject (
    (HBRUSH)SetClassLong(
      hwnd,
      GCL_HBRBACKGROUND,
      (LONG)GetStockObject(WHITE_BRUSH)
    )
  );
  for (int i = 0; i < N; i++){
    DeleteBrush(hBrush[i]);
  }//..~
  DeleteObject(hBrushStatic);
  PostQuitMessage(0);
  return 0;
}//+++

LRESULT CALLBACK
WndProc (HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(message){
    case WM_CREATE:
      return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SIZE:
      return ssOnSize(hwnd,message,wParam,lParam);
    case WM_SETFOCUS:
      return ssOnSetFocus(hwnd,message,wParam,lParam);
    case WM_VSCROLL:
      return ssOnVerticalScroll(hwnd,message,wParam,lParam);
    case WM_CTLCOLORSCROLLBAR:
      return ssOnControlColorSCrollBar(hwnd,message,wParam,lParam);
    case WM_CTLCOLORSTATIC:
      return ssOnControlColorStatic(hwnd,message,wParam,lParam);
    case WM_SYSCOLORCHANGE:
      return ssOnSystemColorChange(hwnd,message,wParam,lParam);
    case WM_DESTROY:
      return ssOnDestroy(hwnd,message,wParam,lParam);
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
