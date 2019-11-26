/* Fig.6-2 : SYSMETS4 */

#define NUMLINES 99

static int 
  cxChar,cxCaps,cyChar,cxClient,cyClient,iMaxWidth,
  iVertPos,iHorzPos
  ;
static HDC hdc;
static PAINTSTRUCT ps;
static SCROLLINFO si;
static TEXTMETRIC tm;

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = GetDC(hwnd);
  GetTextMetrics(hdc,&tm);
  cxChar = tm.tmAveCharWidth;
  cxCaps = (tm.tmPitchAndFamily&0x1? 3:2) * cxChar / 2;
  cyChar = tm.tmHeight + tm.tmExternalLeading;
  ReleaseDC(hwnd,hdc);
  iMaxWidth = 40*cxChar + 22*cxCaps;
  return 0;
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //-- getting
  cxClient = LOWORD(lParam);
  cyClient = HIWORD(lParam);
  
  //-- vert
  si.cbSize = sizeof(si);
  si.fMask  = SIF_RANGE | SIF_PAGE;
  si.nMin   = 0;
  si.nMax   = NUMLINES - 1;
  si.nPage  = cyClient / cyChar;
  SetScrollInfo(hwnd,SB_VERT,&si,TRUE);
  
  //-- horizon
  si.cbSize = sizeof(si);
  si.fMask  = SIF_RANGE | SIF_PAGE;
  si.nMin   = 0;
  si.nMax   = 2 + iMaxWidth / cxChar;
  si.nPage  = cxClient / cxChar;
  SetScrollInfo(hwnd,SB_HORZ,&si,TRUE);
  
  //-- post
  return 0;
  
}//+++

int ssOnVerticleScroll(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  si.cbSize = sizeof(si);
  si.fMask  = SIF_ALL;
  GetScrollInfo(hwnd,SB_VERT,&si);
  iVertPos = si.nPos;
  switch(LOWORD(wParam)){
    case SB_TOP : si.nPos = si.nMin;break;
    case SB_BOTTOM : si.nPos = si.nMax;break;
    case SB_LINEUP : si.nPos--;break;
    case SB_LINEDOWN : si.nPos++;break;
    case SB_PAGEUP : si.nPos+=si.nPage;break;
    case SB_PAGEDOWN : si.nPos-=si.nPage;break;
    case SB_THUMBTRACK : si.nPos = si.nTrackPos;break;
    default:break;
  }//..?
  si.fMask = SIF_POS;
  SetScrollInfo(hwnd,SB_VERT,&si,TRUE);
  
  //--
  GetScrollInfo(hwnd,SB_VERT,&si);
  if(si.nPos!=iVertPos){
    ScrollWindow(hwnd,0,cyChar*(iVertPos - si.nPos),NULL,NULL);
    UpdateWindow(hwnd);
  }//..?
  
  //--
  return 0;
  
}//+++


int ssOnHorizontalScroll(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  
  //--
  si.cbSize = sizeof(si);
  si.fMask  = SIF_ALL;
  GetScrollInfo(hwnd,SB_HORZ,&si);
  iHorzPos = si.nPos;
  switch(LOWORD(wParam)){
    case SB_LINELEFT:si.nPos--;break;
    case SB_LINERIGHT:si.nPos++;break;
    case SB_PAGELEFT:si.nPos+=si.nPage;break;
    case SB_PAGERIGHT:si.nPos-=si.nPage;break;
    case SB_THUMBPOSITION:si.nPos=si.nTrackPos;break;
    default:break;
  }//..?
  si.fMask = SIF_POS;
  SetScrollInfo(hwnd,SB_HORZ,&si,TRUE);
  
  //--
  GetScrollInfo(hwnd,SB_HORZ,&si);
  if(si.nPos!=iHorzPos){
    ScrollWindow(hwnd,cxChar*(iHorzPos - si.nPos),0,NULL,NULL);
  }//..?
  return 0;
  
}//+++

int ssOnKeyDown(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  switch(wParam){
    //--
    case VK_HOME :SendMessage(hwnd,WM_VSCROLL, SB_TOP, 0);break;
    case VK_END  :SendMessage(hwnd,WM_VSCROLL, SB_BOTTOM, 0);break;
    case VK_PRIOR:SendMessage(hwnd,WM_VSCROLL, SB_PAGEDOWN, 0);break;
    case VK_NEXT :SendMessage(hwnd,WM_VSCROLL, SB_PAGEUP, 0);break;
    //--
    case VK_UP   :SendMessage(hwnd,WM_VSCROLL, SB_LINEUP, 0);break;
    case VK_DOWN :SendMessage(hwnd,WM_VSCROLL, SB_LINEDOWN, 0);break;
    case VK_LEFT :SendMessage(hwnd,WM_HSCROLL, SB_PAGEDOWN, 0);break;
    case VK_RIGHT:SendMessage(hwnd,WM_HSCROLL, SB_PAGEUP, 0);break;
  }//..~
  return 0;
}//+++

int ssOnPaint(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hdc = BeginPaint(hwnd,&ps);
  {
    
    //-- get vert pos
    si.cbSize = sizeof(si);
    si.fMask  = SIF_POS;
    GetScrollInfo(hwnd, SB_VERT, &si);
    iVertPos = si.nPos;
    
    //-- get limit
    TCHAR lpBuf[255];
    int lpLen;
    int iPaintBeg,iPaintEnd,x,y;
    iPaintBeg = max(0,iVertPos+ps.rcPaint.top / cyChar);
    iPaintEnd = min(NUMLINES-1, iVertPos+ps.rcPaint.bottom/cyChar);
    for(int i = iPaintBeg;i<=iPaintEnd;i++){
      x = cxChar * (1-iHorzPos);
      y = cyChar * (i-iVertPos);
      
      lpLen=wsprintf(lpBuf,TEXT("2x->%d"),2*i);
      TextOut(hdc,x,y,lpBuf,lpLen);
      
      lpLen=wsprintf(lpBuf,TEXT("3x->%d"),3*i);
      TextOut(hdc,x+22*cxCaps,y,lpBuf,lpLen);
      
      SetTextAlign(hdc,TA_RIGHT | TA_TOP);
      lpLen=wsprintf(lpBuf,TEXT("4x->%d"),4*i);
      TextOut(hdc,x+22*cxCaps+40*cxChar,y,lpBuf,lpLen);
      SetTextAlign(hdc,TA_LEFT | TA_TOP);
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
    case WM_CREATE:return ssOnCreate(hwnd,message,wParam,lParam);
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_VSCROLL:return ssOnVerticleScroll(hwnd,message,wParam,lParam);
    case WM_HSCROLL:return ssOnHorizontalScroll(hwnd,message,wParam,lParam);
    case WM_KEYDOWN:return ssOnKeyDown(hwnd,message,wParam,lParam);
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
  static TCHAR szAppName[] = TEXT("Hello");
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
    szAppName,TEXT("SYSMETS4"),WS_OVERLAPPEDWINDOW,
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
