// boyscoutDlg.h : header file
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"

#include "folderSplitter.h"

class CboyscoutDlgAutoProxy;


// CboyscoutDlg dialog
class CboyscoutDlg : public CDialog
{
	DECLARE_DYNAMIC(CboyscoutDlg);
	friend class CboyscoutDlgAutoProxy;

// Construction
public:
	CboyscoutDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CboyscoutDlg();

// Dialog Data
	enum { IDD = IDD_BOYSCOUT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CboyscoutDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	NOTIFYICONDATA nIconData;

	int OldPercent;
	BOOL countStart;
	BOOL copyStart;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
//	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	LRESULT OnProgressSet(WPARAM wParam, LPARAM lParam);
	LRESULT OnProgressStep(WPARAM pString, LPARAM);
	LRESULT OnProgressEndCopy(WPARAM, LPARAM);
	LRESULT OnProgressEndCount(WPARAM, LPARAM);
	//LRESULT OnThreadCancel(WPARAM, LPARAM);
	LRESULT OnTrayIconMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedActivate();
	afx_msg void OnBnClickedExit();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAbout();
	afx_msg void OnBnClickedButtonSetorigen();
	afx_msg void OnBnClickedButtonSetdestiny();
	afx_msg void OnBnClickedOk();
private:
	// Edit Origen access property variable
	CString folderOrigin;
	// Edit Destiny access property variable
	CString folderDestiny;
	// Size in Mb of splitted folders
	int piecesSize;
	// Progress bar Control Variable
	CProgressCtrl progressBar;
	// Continue Radio Button Group Variable
	int checkedButton;

	//
	folderCount* fdc;
	//
	folderSplitter* fds;
	//
	HANDLE ThreadKilled;
	HANDLE KillThread;

	VOID CboyscoutDlg::SetNotifyIcon(int perCent);

public:
// Start button control
	CButton startButton;
	// Progress text control
	CEdit progressText;
//	afx_msg void OnEnChangeEditOrigin();
//	afx_msg void OnEnChangeEditDestiny();
//	afx_msg void OnEnKillfocusEditOrigin();
//	afx_msg void OnEnKillfocusEditDestiny();
	afx_msg void OnEnKillfocusEditSize();
	afx_msg void OnEnChangeEditOrigin();
	afx_msg void OnEnChangeEditDestiny();
//	afx_msg void OnEnUpdateEditOrigin();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
