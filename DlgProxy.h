// DlgProxy.h: header file
//

#pragma once

class CboyscoutDlg;


// CboyscoutDlgAutoProxy command target

class CboyscoutDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CboyscoutDlgAutoProxy)

	CboyscoutDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CboyscoutDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CboyscoutDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CboyscoutDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

