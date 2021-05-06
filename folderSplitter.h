#pragma once

#include "stdafx.h"

#include <io.h>

#include <direct.h>

#include <stack>

#include <errno.h>


// Get the target folder for a folder PIDL. This deals with cases where a folder
// is an alias to a real folder, folder shortcuts, the My Documents folder, etc.
STDAPI SHGetTargetFolderPath(LPCITEMIDLIST pidlFolder, LPWSTR pszPath, UINT cchPath);

struct SCountINFO {
	//
	CString dir;
	//
	HWND wh;
	//
	HANDLE KillThread;
	HANDLE ThreadKilled;
};

struct SCopyINFO {
	//
	CString Orig;
	CString Dest;
	LONG bLength;
	//
	HWND windowHandle;
	//
	BOOL deleteOriginFolder;
	BOOL createEmptyDirs;
	//
	HANDLE KillThread;
	HANDLE ThreadKilled;
};

const static int oneMega = 1048575;
const static size_t bufferSize = 65535;

class folderCount
{
private:

	CWinThread* countThread;

public:

	SCountINFO countInfo;

	folderCount(CString Orig, HWND windowHandle);
	~folderCount(VOID);

	// Stop running threads
	VOID Stop(VOID);
	// Supend running threads
	VOID Suspend(VOID);
	// Resume supended threads
	VOID Resume(VOID);
	//
	VOID Start(VOID);
};

class folderSplitter
{
private:

	CWinThread* copyThread;

public:
	
	SCopyINFO copyInfo;

	folderSplitter(CString Orig, CString Dest, LONG bLength, HWND windowHandle, HANDLE KThread, HANDLE ThreadK);
	~folderSplitter(VOID);

	// Stop running threads
	VOID Stop(VOID);
	// Supend running threads
	VOID Suspend(VOID);
	// Resume supended threads
	VOID Resume(VOID);
	// Start the Copy Thread
	VOID Start(VOID);

};