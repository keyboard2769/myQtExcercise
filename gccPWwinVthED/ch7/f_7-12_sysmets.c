/* Fig.7-12 */

#define C_FIG_NAME TEXT("SYSMETS")
#define NUMLINES 256

static int cxChar, cxCaps, cyChar, cxClient, cyClient, iMaxWidth;
static int iVertPos, iHorzPos, iPaintBeg, iPaintEnd;
static int iDeltaPerLine, iAccumDelta;//..for mouse wheel logic
static ULONG ulScrollLines;//..for mouse wheel logic
static HDC hdc;
static PAINTSTRUCT ps;
static SCROLLINFO  si;
static TEXTMETRIC tm;

void ccInitMetrix(HWND hwnd){
  hdc = GetDC(hwnd);
  GetTextMetrics(hdc, &tm);
  cxChar = tm.tmAveCharWidth;
  cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar / 2;
  cyChar = tm.tmHeight + tm.tmExternalLeading;
  ReleaseDC(hwnd, hdc);
  iMaxWidth = 40 * cxChar + 22 * cxCaps;//..Save the width of the three columns
}//+++

void ccInitScrollInfo(){
  SystemParametersInfo(
    SPI_GETWHEELSCROLLLINES,
    0,
    &ulScrollLines,//ulScrollLines usually equals 3 or 0 (for no scrolling)
    0// WHEEL_DELTA equals 120, so iDeltaPerLine will be 40
  );
  if (ulScrollLines | TRUE){
    iDeltaPerLine = WHEEL_DELTA / ulScrollLines;
  }else{
    iDeltaPerLine = 0;
  }//..?
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  cxClient = LOWORD(lParam);
  cyClient = HIWORD(lParam);

  // Set vertical scroll bar range and page size
  si.cbSize = sizeof(si);
  si.fMask = SIF_RANGE | SIF_PAGE;
  si.nMin = 0;
  si.nMax = NUMLINES - 1;
  si.nPage = cyClient / cyChar;
  SetScrollInfo(hwnd, SB_VERT, &si, TRUE);

  // Set horizontal scroll bar range and page size
  si.cbSize = sizeof(si);
  si.fMask = SIF_RANGE | SIF_PAGE;
  si.nMin = 0;
  si.nMax = 2 + iMaxWidth / cxChar;
  si.nPage = cxClient / cxChar;
  SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
  
  return 0;
}//+++

int ssOnVerticalScroll(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  // Get all the vertical scroll bar information
  si.cbSize = sizeof(si);
  si.fMask = SIF_ALL;

  // Save the position for comparison later on
  GetScrollInfo(hwnd, SB_VERT, &si);
  iVertPos = si.nPos;
  switch (LOWORD(wParam)){
    case SB_TOP:si.nPos = si.nMin;break;
    case SB_BOTTOM:si.nPos = si.nMax;break;
    case SB_LINEUP:si.nPos -= 1;break;
    case SB_LINEDOWN:si.nPos += 1;break;
    case SB_PAGEUP:si.nPos -= si.nPage; break;
    case SB_PAGEDOWN:si.nPos += si.nPage;break;
    case SB_THUMBTRACK:si.nPos = si.nTrackPos;break;
    default:break;
  }//..?
  
  // Set the position and then retrieve it.  Due to adjustments
  //   by Windows it may not be the same as the value set.
  si.fMask = SIF_POS;
  SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
  GetScrollInfo(hwnd, SB_VERT, &si);

  // If the position has changed, scroll the window and update it
  if (si.nPos != iVertPos){
    ScrollWindow(hwnd,
      0, cyChar * (iVertPos - si.nPos),
      NULL, NULL
    );
    UpdateWindow(hwnd);
  }//..?
  
  return 0;
}//+++

int ssOnHorizentalScroll(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  // Get all the vertical scroll bar information
  si.cbSize = sizeof(si);
  si.fMask = SIF_ALL;

  // Save the position for comparison later on
  GetScrollInfo(hwnd, SB_HORZ, &si);
  iHorzPos = si.nPos;
  switch (LOWORD(wParam)){
    case SB_LINELEFT:si.nPos -= 1;break;
    case SB_LINERIGHT:si.nPos += 1;break;
    case SB_PAGELEFT:si.nPos -= si.nPage;break;
    case SB_PAGERIGHT:si.nPos += si.nPage;break;
    case SB_THUMBPOSITION:si.nPos = si.nTrackPos;break;
    default:break;
  }//..?
  
  // Set the position and then retrieve it.  Due to adjustments
  //   by Windows it may not be the same as the value set.
  si.fMask = SIF_POS;
  SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
  GetScrollInfo(hwnd, SB_HORZ, &si);

  // If the position has changed, scroll the window
  if (si.nPos != iHorzPos){
    ScrollWindow(hwnd,
      cxChar * (iHorzPos - si.nPos), 0,
      NULL, NULL
    );
  }//..?
  
  return 0;
}//+++

int ssOnKeyDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch (wParam){
    case VK_HOME:SendMessage(hwnd, WM_VSCROLL, SB_TOP, 0);break;
    case VK_END:SendMessage(hwnd, WM_VSCROLL, SB_BOTTOM, 0);break;
    case VK_PRIOR:SendMessage(hwnd, WM_VSCROLL, SB_PAGEUP, 0);break;
    case VK_NEXT:SendMessage(hwnd, WM_VSCROLL, SB_PAGEDOWN, 0);break;
    case VK_UP:SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);break;
    case VK_DOWN:SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);break;
    case VK_LEFT:SendMessage(hwnd, WM_HSCROLL, SB_PAGEUP, 0);break;
    case VK_RIGHT:SendMessage(hwnd, WM_HSCROLL, SB_PAGEDOWN, 0);break;
  }//..?
  return 0;
}//+++

int ssOnMouseWheel(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  if (iDeltaPerLine == 0){
    return DefWindowProc(hwnd, message, wParam, lParam);
  }//..?
  iAccumDelta += (short)HIWORD(wParam);// 120 or -120
  while (iAccumDelta >= iDeltaPerLine){
    SendMessage(hwnd, WM_VSCROLL, SB_LINEUP, 0);
    iAccumDelta -= iDeltaPerLine;
  }//..?
  while (iAccumDelta <= -iDeltaPerLine){
    SendMessage(hwnd, WM_VSCROLL, SB_LINEDOWN, 0);
    iAccumDelta += iDeltaPerLine;
  }//..?
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  { 
    // Get vertical scroll bar position
    si.cbSize = sizeof(si);
    si.fMask = SIF_POS;
    GetScrollInfo(hwnd, SB_VERT, &si);
    iVertPos = si.nPos;

    // Get horizontal scroll bar position
    GetScrollInfo(hwnd, SB_HORZ, &si);
    iHorzPos = si.nPos;

    // Find painting limits
    iPaintBeg = max(0, iVertPos + ps.rcPaint.top / cyChar);
    iPaintEnd = min(NUMLINES - 1,
  iVertPos + ps.rcPaint.bottom / cyChar);

    int x,y;
    TCHAR szBuffer[256];
    int iBufLen;
    for (int i = iPaintBeg; i <= iPaintEnd; i++){
      x = cxChar * (1 - iHorzPos);
      y = cyChar * (i - iVertPos);

      iBufLen=wsprintf(szBuffer,TEXT("x3->%d"),i*3);
      TextOut(hdc, x, y,szBuffer,iBufLen);

      iBufLen=wsprintf(szBuffer,TEXT("x4->%d"),i*4);
      TextOut(hdc, x + 22 * cxCaps, y,szBuffer,iBufLen);

      SetTextAlign(hdc, TA_RIGHT | TA_TOP);
      iBufLen=wsprintf(szBuffer,TEXT("x5->%d"),i*5);
      TextOut(hdc, x + 22 * cxCaps + 40 * cxChar, y,szBuffer,iBufLen);
      SetTextAlign(hdc, TA_LEFT | TA_TOP);
      
    }//..~
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
    
    case WM_CREATE:
      ccInitMetrix(hwnd);
      ccInitScrollInfo();
    return 0;
    
    case WM_SETTINGCHANGE:
      ccInitScrollInfo();
    return 0;
     
    //--
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_VSCROLL:return ssOnVerticalScroll(hwnd,message,wParam,lParam);
    case WM_HSCROLL:return ssOnHorizentalScroll(hwnd,message,wParam,lParam);
    case WM_KEYDOWN:return ssOnKeyDown(hwnd,message,wParam,lParam);
    case WM_MOUSEWHEEL:return ssOnMouseWheel(hwnd,message,wParam,lParam);
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
