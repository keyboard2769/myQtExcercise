/* Fig.9-7 */

#define C_FIG_NAME TEXT("POPPAD1")

#define ID_EDIT 1

static HWND hwndEdit;

//===

int ssOnCreate(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  hwndEdit = CreateWindow(
    TEXT("edit"), NULL,
    WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL
     | WS_BORDER | ES_LEFT | ES_MULTILINE
     | ES_AUTOHSCROLL | ES_AUTOVSCROLL,
    0, 0, 0, 0,
    hwnd, (HMENU)ID_EDIT,
    ((LPCREATESTRUCT)lParam)->hInstance, NULL
  );
  return 0;
}//+++

int ssOnSetFocus(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  SetFocus(hwndEdit);
  return 0;
}//+++

int ssOnSize(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  MoveWindow(hwndEdit, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
  return 0;
}//+++

int ssOnCommand(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
  int lpChildWindowID = LOWORD(wParam);
  int lpNotificationCode = HIWORD(wParam);
  if(lpChildWindowID == ID_EDIT){
    if(LB_OR(
      lpNotificationCode == EN_ERRSPACE,
      lpNotificationCode == EN_MAXTEXT
    )){
      MessageBox(
        hwnd, TEXT("Edit control out of space."),
        C_FIG_NAME, MB_OK | MB_ICONSTOP
      );
    }//..?
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
    case WM_SETFOCUS:return ssOnSetFocus(hwnd,message,wParam,lParam);
    case WM_SIZE:return ssOnSize(hwnd,message,wParam,lParam);
    case WM_COMMAND:return ssOnCommand(hwnd,message,wParam,lParam);
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
