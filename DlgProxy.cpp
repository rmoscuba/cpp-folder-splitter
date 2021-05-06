// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "boyscout.h"
#include "DlgProxy.h"
#include "boyscoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CboyscoutDlgAutoProxy

IMPLEMENT_DYNCREATE(CboyscoutDlgAutoProxy, CCmdTarget)

CboyscoutDlgAutoProxy::CboyscoutDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CboyscoutDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CboyscoutDlg)))
		{
			m_pDialog = reinterpret_cast<CboyscoutDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CboyscoutDlgAutoProxy::~CboyscoutDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CboyscoutDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CboyscoutDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CboyscoutDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_Iboyscout to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {C7A71A4F-2333-40AA-B4FA-90FA9ED8427A}
static const IID IID_Iboyscout =
{ 0xC7A71A4F, 0x2333, 0x40AA, { 0xB4, 0xFA, 0x90, 0xFA, 0x9E, 0xD8, 0x42, 0x7A } };

BEGIN_INTERFACE_MAP(CboyscoutDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CboyscoutDlgAutoProxy, IID_Iboyscout, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {36808C2E-C2C1-451D-A28B-A62CFCB5A245}
IMPLEMENT_OLECREATE2(CboyscoutDlgAutoProxy, "boyscout.Application", 0x36808c2e, 0xc2c1, 0x451d, 0xa2, 0x8b, 0xa6, 0x2c, 0xfc, 0xb5, 0xa2, 0x45)


// CboyscoutDlgAutoProxy message handlers
