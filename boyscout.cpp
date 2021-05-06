// boyscout.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "boyscout.h"
#include "boyscoutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CboyscoutApp

BEGIN_MESSAGE_MAP(CboyscoutApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CboyscoutApp construction

CboyscoutApp::CboyscoutApp()
{
	EnableHtmlHelp();

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CboyscoutApp object

CboyscoutApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x919C4FF, 0xF79A, 0x4AE9, { 0x98, 0x8A, 0xA3, 0x5C, 0x7C, 0x56, 0x7, 0xA } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CboyscoutApp initialization

BOOL CboyscoutApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("rmortega77.AT.yahoo.es"));
	// Parse command line for automation or reg/unreg switches.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register class factories via CoRegisterClassObject().
		COleTemplateServer::RegisterAll();
	}
	// App was launched with /Unregserver or /Unregister switch.  Remove
	// entries from the registry.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
		return FALSE;
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
		if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppRegister)
			return FALSE;
	}

	CboyscoutDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
