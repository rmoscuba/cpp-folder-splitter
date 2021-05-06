// boyscoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "boyscout.h"
#include "boyscoutDlg.h"
#include "DlgProxy.h"

#include <shlwapi.h>

#include <strsafe.h>

#include "shlobj.h"
#include ".\boyscoutdlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//	ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CboyscoutDlg dialog




IMPLEMENT_DYNAMIC(CboyscoutDlg, CDialog);
CboyscoutDlg::CboyscoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CboyscoutDlg::IDD, pParent)
	, piecesSize(700)
	, checkedButton(0)
	, OldPercent(-1)
	, countStart(FALSE)
	, copyStart(FALSE)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = NULL;

}
CboyscoutDlg::~CboyscoutDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to NULL, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != NULL)
		m_pAutoProxy->m_pDialog = NULL;

}

//void AFXAPI DDX_Dir(CDataExchange* pDX, int nIDC, CString& strDir)
//{
//	HWND hWndCtrl = pDX->PrepareEditCtrl(nIDC);
//	if (pDX->m_bSaveAndValidate)
//	{
//		TCHAR szT[261];
//		::GetWindowText(hWndCtrl, szT, 261);
//		if ((!PathFileExistsA(szT)) && (  strcmp(szT, "") == 0 ))
//		{
//			AfxMessageBox(IDS_DIR_DO_NOT_EXIST);
//			pDX->Fail();
//		}
//		else
//			strDir = _T(szT);
//	}
//	else
//		::SetWindowText(hWndCtrl, strDir);
//}

void CboyscoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ORIGIN, folderOrigin);
	DDX_Text(pDX, IDC_EDIT_DESTINY, folderDestiny);
	DDX_Text(pDX, IDC_EDIT_SIZE, piecesSize);
	DDX_Control(pDX, IDC_PROGRESS, progressBar);
	DDX_Radio(pDX, IDC_RADIO_CONTINUE, checkedButton);
	DDX_Control(pDX, IDOK, startButton);
	DDX_Control(pDX, IDC_EDIT_PROGRESS, progressText);
	DDV_MinMaxInt(pDX, piecesSize, 1, INT_MAX);
}

BEGIN_MESSAGE_MAP(CboyscoutDlg, CDialog)
	ON_MESSAGE(WM_USER_PROGRESS_SET, OnProgressSet)
	ON_MESSAGE(WM_USER_PROGRESS_STEP, OnProgressStep)
	ON_MESSAGE(WM_USER_PROGRESS_ENDCOPY, OnProgressEndCopy)
	ON_MESSAGE(WM_USER_PROGRESS_ENDCOUNT, OnProgressEndCount)
	//ON_MESSAGE(WM_USER_THREAD_CANCEL, OnThreadCancel)
	ON_MESSAGE(WM_USER_NOTIFYICON, OnTrayIconMessage)
	ON_WM_SYSCOMMAND()
//	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_ACTIVATE, OnBnClickedActivate)
	ON_BN_CLICKED(ID_ACTIVATE_EXIT, OnBnClickedExit)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDABOUT, OnBnClickedAbout)
	ON_BN_CLICKED(IDC_BUTTON_SETORIGEN, OnBnClickedButtonSetorigen)
	ON_BN_CLICKED(IDC_BUTTON_SETDESTINY, OnBnClickedButtonSetdestiny)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//	ON_WM_LBUTTONDOWN()
	//ON_EN_CHANGE(IDC_EDIT_ORIGIN, OnEnChangeEditOrigin)
	//ON_EN_CHANGE(IDC_EDIT_DESTINY, OnEnChangeEditDestiny)
//	ON_EN_KILLFOCUS(IDC_EDIT_ORIGIN, OnEnKillfocusEditOrigin)
//	ON_EN_KILLFOCUS(IDC_EDIT_DESTINY, OnEnKillfocusEditDestiny)
	ON_EN_KILLFOCUS(IDC_EDIT_SIZE, OnEnKillfocusEditSize)
	ON_EN_CHANGE(IDC_EDIT_ORIGIN, OnEnChangeEditOrigin)
	ON_EN_CHANGE(IDC_EDIT_DESTINY, OnEnChangeEditDestiny)
//	ON_EN_UPDATE(IDC_EDIT_ORIGIN, OnEnUpdateEditOrigin)
ON_WM_ACTIVATE()
ON_WM_CREATE()
ON_WM_DESTROY()
END_MESSAGE_MAP()


// CboyscoutDlg message handlers

BOOL CboyscoutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	KillThread = CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset
	ThreadKilled = CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset


	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CboyscoutDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CboyscoutDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CboyscoutDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

//void CboyscoutDlg::OnClose() 
//{	if (CanExit())
//		CDialog::OnClose();
//}

void CboyscoutDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CboyscoutDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CboyscoutDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != NULL)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

void CboyscoutDlg::OnBnClickedActivate()
{
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);
	AfxGetApp()->m_pMainWnd->ActivateTopParent();
}

void CboyscoutDlg::OnBnClickedExit()
{
	AfxGetApp()->m_pMainWnd->PostMessage(WM_CLOSE);
}

void CboyscoutDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//SendMessage(WM_USER_THREAD_CANCEL, 0, 0);
	if ( countStart ) {
		fdc->Stop( );
		countStart = FALSE;
		delete ( fdc );
	}
	if ( copyStart ) {
		fds->Stop( );
		copyStart = FALSE;
		delete ( fds );
	}

	CloseHandle(KillThread);
	CloseHandle(ThreadKilled);

	Shell_NotifyIcon( NIM_DELETE, &nIconData);

	OnCancel();
}

void CboyscoutDlg::OnBnClickedAbout()
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

// Set selected path of dialog to lParam
int CALLBACK initilizeBrowseDialog( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	if (uMsg == BFFM_INITIALIZED) {
		SendMessage( hwnd, BFFM_SETSELECTIONW, TRUE, lpData);
	}
	return 0;
}

void CboyscoutDlg::OnBnClickedButtonSetorigen()
{
	// TODO: Add your control notification handler code here
	
	// Init conversion Macros
	USES_CONVERSION;

	// Convet CStrinT to LPWSTR
	LPWSTR temp = T2W(folderOrigin);

	// Init LPBROWSEINFO structure
	BROWSEINFO brInfo = {0};
	
	//
	brInfo.hwndOwner = m_hWnd;
    brInfo.pidlRoot = NULL;
    brInfo.lpszTitle = "Choose the origin folder...";
    brInfo.ulFlags = BIF_RETURNONLYFSDIRS;
    brInfo.lpfn = &initilizeBrowseDialog;
    brInfo.lParam = (LPARAM)temp;

	// Init COM Call
	CoInitialize(NULL);
	LPITEMIDLIST fdrList = SHBrowseForFolder(&brInfo);
	if (fdrList)
	{
		// Get path
		//SHGetPathFromIDListW(fdrList,temp);
		HRESULT hr = SHGetTargetFolderPath(static_cast <LPCITEMIDLIST>(fdrList),temp,0);
		// Free fdrList ID List
		CoTaskMemFree(fdrList);
		if (SUCCEEDED(hr))
		{
			// Set the Destiny property
			folderOrigin = _T(temp);
			// Update Controls in the form
			UpdateData(FALSE);

			startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
		}
	}
}

void CboyscoutDlg::OnBnClickedButtonSetdestiny()
{
	// TODO: Add your control notification handler code here
	
	// Init conversion Macros
	USES_CONVERSION;

	// Convert CStrinT to LPWSTR
	LPWSTR temp = T2W(folderDestiny);

	// Init LPBROWSEINFO structure
	BROWSEINFO brInfo = {0};
	//
	brInfo.hwndOwner = m_hWnd;
    brInfo.pidlRoot = NULL;
    brInfo.lpszTitle = "Choose the destination folder...";
    brInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    brInfo.lpfn = &initilizeBrowseDialog;
    brInfo.lParam = (LPARAM)temp;

	// Init COM Call
	OleInitialize(NULL);
	//
	LPITEMIDLIST fdrList = SHBrowseForFolder(&brInfo);
	if (fdrList)
	{
		// Get path
		//SHGetPathFromIDListW(fdrList,temp);
		HRESULT hr = SHGetTargetFolderPath(static_cast <LPCITEMIDLIST>(fdrList),temp,0);
		// Free fdrList ID List
		CoTaskMemFree(fdrList);
		if (SUCCEEDED(hr))
		{
			// Set the Destiny property
			folderDestiny = _T(temp);
			// Update Controls in the form
			UpdateData(FALSE);

			startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
		}
	}
}

void CboyscoutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	progressBar.SetStep(1);

	fdc = new folderCount( folderOrigin, this->m_hWnd);

	countStart = TRUE;
	fdc->Start( );

	fds = new folderSplitter( folderOrigin, folderDestiny, piecesSize, this->m_hWnd, KillThread, ThreadKilled);

	copyStart = TRUE;
	fds->Start( );

	startButton.EnableWindow( FALSE );

	return;
	//OnOK();
}

//void CAboutDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//	OnOK();
//	//CDialog::OnLButtonDblClk(nFlags, point);
//}

//void CboyscoutDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: Add your message handler code here and/or call default
//	OnOK();
//	//CDialog::OnLButtonDown(nFlags, point);
//}

void CAboutDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	OnOK();
	//CDialog::OnLButtonDown(nFlags, point);
}

void CboyscoutDlg::OnEnChangeEditOrigin()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	//
	startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
}

void CboyscoutDlg::OnEnChangeEditDestiny()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData(TRUE);
	startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
}

//void CboyscoutDlg::OnEnKillfocusEditOrigin()
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//	startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
//}

//void CboyscoutDlg::OnEnKillfocusEditDestiny()
//{
//	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//	startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
//}

void CboyscoutDlg::OnEnKillfocusEditSize()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
}

//void CboyscoutDlg::OnEnUpdateEditOrigin()
//{
//	EN_UPDATE 
//	// TODO:  If this is a RICHEDIT control, the control will not
//	// send this notification unless you override the CDialog::OnInitDialog()
//	// function to send the EM_SETEVENTMASK message to the control
//	// with the ENM_UPDATE flag ORed into the lParam mask.
//
//	// TODO:  Add your control notification handler code here
//	startButton.EnableWindow(PathFileExistsA(folderDestiny) && PathFileExistsA(folderOrigin));
//}

LRESULT CboyscoutDlg::OnProgressSet(WPARAM wParam, LPARAM lParam)
{
	progressBar.SetRange32( 0, (int)wParam );
	//
	return 0;
}

VOID CboyscoutDlg::SetNotifyIcon(int perCent)
{
	// 
	char porCiento[] = "FS";

	if (perCent < 100)
		_itoa(perCent, porCiento, 10);

	HICON hIcon3;      // icon handle 

	CBitmap bmp;

	bmp.LoadBitmap(TRAY_BITMAP);

	CClientDC dc(this); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 

	CBitmap* pOldBitmap = dcMem.SelectObject( &bmp );

	// draw a rectangle in the background (window) color 
	CRect rect(0, 0, 16, 16); 

	dcMem.SetTextColor(COLOR_WINDOW);
	dcMem.DrawText(porCiento, rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	int p16 = (int)floor(perCent / (double)100 * 16);

	CRect irect(0, 16-p16, 16, 16);
	dcMem.InvertRect(irect);
	dcMem.MoveTo(0, 16-p16);
	dcMem.LineTo(16, 16-p16);

	dcMem.SelectObject(pOldBitmap); 
	dcMem.DeleteDC();

	BYTE p[32];

	DWORD dw = bmp.GetBitmapBits(32, &p);

	//
	BYTE XORmaskIcon[] = {0x00, 0x00, // line 1 
					0x00, 0x00, // line 2 
					0x00, 0x00, // line 3 
					0x00, 0x00, // line 4 
	
					0x00, 0x00, // line 5 
					0x00, 0x00, // line 6 
					0x00, 0x00, // line 7 
					0x00, 0x00, // line 8 
	
					0x00, 0x00, // line 9 
					0x00, 0x00, // line 10 
					0x00, 0x00, // line 11 
					0x00, 0x00, // line 12 
	
					0x00, 0x00, // line 13 
					0x00, 0x00, // line 14 
					0x00, 0x00, // line 15 
					0x00, 0x00 // line 16 
	
					};
	
	hIcon3 = CreateIcon(AfxGetApp()->m_hInstance,    // application instance  
			16,              // icon width 
			16,              // icon height 
			1,               // number of XOR planes 
			1,               // number of bits per pixel 
			p,     // AND bitmask  
			XORmaskIcon);    // XOR bitmask 

	//--- . ---

	nIconData.cbSize = sizeof(NOTIFYICONDATA);

	nIconData.uID = TRAY_ICON;
	
	//HICON mhicon = AfxGetApp()->LoadIcon(TRAY_ICON);

	nIconData.hIcon = hIcon3;

	nIconData.hWnd = m_hWnd;

	LPSTR tmp = "Folder Splitter"; //porCiento.GetString();

	StringCbCopyN(nIconData.szTip, sizeof(nIconData.szTip), tmp, 
						sizeof(nIconData.szTip));
	
	nIconData.uFlags = NIF_MESSAGE |  NIF_ICON | NIF_TIP;

	nIconData.uCallbackMessage = WM_USER_NOTIFYICON;

	Shell_NotifyIcon( NIM_MODIFY, &nIconData);

	if (hIcon3) 
	DestroyIcon(hIcon3);
}

LRESULT CboyscoutDlg::OnProgressStep(WPARAM pString, LPARAM)
{
	int val, up;
	progressBar.GetRange( val, up);	
	val = progressBar.GetPos();
	//
	if ( val+1 > up )
		progressBar.SetRange( 0, val+1);
	//
	progressBar.StepIt( );

	progressText.SetWindowText( (LPCTSTR)pString );

	int perCent = (int)floor(val / (double)up * 100);

	if (perCent != OldPercent) {
		
		OldPercent = perCent;	

		SetNotifyIcon(perCent);
	}

	// --- . ---
	return 0;
}

LRESULT CboyscoutDlg::OnProgressEndCount(WPARAM, LPARAM)
{
	delete ( fdc );
	countStart = FALSE;
	//
	return 0;
}

//LRESULT CboyscoutDlg::OnThreadCancel(WPARAM, LPARAM)
//{
//	fds->Stop( );
//	//
//	return 0;
//}

LRESULT CboyscoutDlg::OnProgressEndCopy(WPARAM, LPARAM)
{
	delete ( fds );
	copyStart = FALSE;
	//
	//CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset

	SetNotifyIcon(100);
	//
	startButton.EnableWindow();
	//
	return 0;
}

VOID APIENTRY DisplayContextMenu(HWND hwnd, POINT pt) 
{ 
    HMENU hmenu;            // top-level menu 
    HMENU hmenuTrackPopup;  // shortcut menu 
 
    // Load the menu resource. 

	CMenu menu;
 
    if (menu.LoadMenu(TRAY_MENU) == FALSE)
        return;

	hmenu = menu.m_hMenu;
 
    // TrackPopupMenu cannot display the menu bar so get 
    // a handle to the first shortcut menu. 
 
    hmenuTrackPopup = GetSubMenu(hmenu, 0); 
 
    // Display the shortcut menu. Track the right mouse 
    // button. 
	
	// needed to the menu automatically disappear on outclicling
	::SetForegroundWindow (hwnd);
 
    TrackPopupMenu(hmenuTrackPopup, 
            TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
            pt.x, pt.y, 0, hwnd, NULL); 
 
    // Destroy the menu. 
 
    DestroyMenu(hmenu); 
} 

BOOL WINAPI ShowContextMenu(HWND hwnd, int x, int y) 
{ 
    //RECT rc;                    // client area of window 
    POINT pt = { x, y };        // location of mouse click 
 
    //// Get the bounding rectangle of the client area. 
 
    //GetClientRect(hwnd, &rc); 
 
    //// Convert the mouse position to client coordinates. 
 
    //ScreenToClient(hwnd, &pt); 
 
    //// If the position is in the client area, display a  
    //// shortcut menu. 
 
    //if (PtInRect(&rc, pt)) 
    //{ 
       // ClientToScreen(hwnd, &pt); 
        DisplayContextMenu(hwnd, pt); 
        return TRUE; 
    //} 
 
    //// Return FALSE if no menu is displayed. 
 
    //return FALSE; 
}

LRESULT CboyscoutDlg::OnTrayIconMessage(WPARAM wParam, LPARAM lParam) 
{ 
	//TODO: Add Tray Icon message notification here

	UINT uID; 
    UINT uMouseMsg;

	POINT p;
	GetCursorPos(&p);
 
    uID = (UINT) wParam; 
    uMouseMsg = (UINT) lParam; 

	if (uMouseMsg == WM_LBUTTONDOWN) 
    { 
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOWNORMAL);

		AfxGetApp()->m_pMainWnd->ActivateTopParent();
	}
	if (uMouseMsg == WM_RBUTTONDOWN)
	{
		// Show menu
		ShowContextMenu(AfxGetApp()->m_pMainWnd->m_hWnd, p.x,
				p.y);
	}

	//char buff[10] = "Prueba";
	//itoa((UINT) lParam, buff, 2);
	//progressText.SetWindowText(buff);
	// hide the application's windows before closing all the documents

	// AfxGetApp()->m_pMainWnd->ShowWindow(SW_RESTORE);
	// AfxGetApp()->m_pMainWnd->ShowOwnedPopups(FALSE);

	// // put the window at the bottom of z-order, so it isn't activated
	// AfxGetApp()->m_pMainWnd->SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0,
	//    SWP_NOMOVE|SWP_NOSIZE);


	// AnimateWindow(200, AW_HOR_POSITIVE | AW_VER_POSITIVE);

    return 0; 
}

void CboyscoutDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here

	if ( bMinimized ) {

		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);

	}

	return;

}

int CboyscoutDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

		//--- . ---

	HICON hIcon3;      // icon handle 

	CBitmap bmp;

	bmp.LoadBitmap(TRAY_BITMAP);

	CClientDC dc(this); 
	CDC dcMem; 
	dcMem.CreateCompatibleDC(&dc); 

	CBitmap* pOldBitmap = dcMem.SelectObject( &bmp );

	// draw a rectangle in the background (window) color 
	CRect rect(0, 0, 16, 16); 

	dcMem.SetTextColor(COLOR_WINDOW);
	dcMem.DrawText("FS", rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	dcMem.SelectObject(pOldBitmap); 
	dcMem.DeleteDC();

	BYTE p[16*16];

	bmp.GetBitmapBits(16*16, &p);

	////
	//BYTE XORmaskIcon[] = 
	//				 {0x00, 0x00, // line 1 
 //                     0x00, 0x00, // line 2 
 //                     0x00, 0x00, // line 3 
 //                     0x00, 0x00, // line 4 
 //
 //                     0x00, 0x00, // line 5 
 //                     0x00, 0x00, // line 6 
 //                     0x00, 0x00, // line 7 
 //                     0x00, 0x00, // line 8 
 //
 //                     0x00, 0x00, // line 9 
 //                     0x00, 0x00, // line 10 
 //                     0x00, 0x00, // line 11 
 //                     0x00, 0x00, // line 12 
 //
 //                     0x00, 0x00, // line 13 
 //                     0x00, 0x00, // line 14 
 //                     0x00, 0x00, // line 15 
 //                     0x00, 0x00  // line 16 

 //
 //                     };

	hIcon3 = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(TRAY_ICON)); 
 
	//hIcon3 = CreateIcon(AfxGetApp()->m_hInstance,    // application instance  
 //            16,              // icon width 
 //            16,              // icon height 
 //            1,               // number of XOR planes 
 //            1,               // number of bits per pixel 
 //            p,     // AND bitmask  
 //            XORmaskIcon);    // XOR bitmask 


	//--- . ---

	nIconData.cbSize = sizeof(NOTIFYICONDATA);

	nIconData.uID = TRAY_ICON;
	
	//HICON mhicon = AfxGetApp()->LoadIcon(TRAY_ICON);

	nIconData.hIcon = hIcon3;

	nIconData.hWnd = m_hWnd;

	LPSTR tmp = "Folder Splitter";

	StringCbCopyN(nIconData.szTip, sizeof(nIconData.szTip), tmp, 
                           sizeof(nIconData.szTip));
	
	nIconData.uFlags = NIF_MESSAGE |  NIF_ICON | NIF_TIP;

	nIconData.uCallbackMessage = WM_USER_NOTIFYICON;

	Shell_NotifyIcon( NIM_ADD, &nIconData);

	if (hIcon3) 
       DestroyIcon(hIcon3);

	return 0;
}

