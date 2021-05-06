#include "StdAfx.h"
#include <conio.h>
#include <objidl.h>

#include <shlwapi.h>

#include <stdexcept>

//#include <exception>

//#include <sys/utime.h>

#include <shobjidl.h>
#include ".\foldersplitter.h"

#define ILSkip(pidl, cb)       ((LPITEMIDLIST)(((BYTE*)(pidl))+cb))
#define ILNext(pidl)           ILSkip(pidl, (pidl)->mkid.cb)
 
UINT GetSize(LPCITEMIDLIST pidl)
{
    UINT cbTotal = 0;

	LPCITEMIDLIST tmp = pidl;

    if (tmp) 
    {
        cbTotal += sizeof(tmp->mkid.cb);
        while (tmp->mkid.cb) 
        {
            cbTotal += tmp->mkid.cb;
            tmp = ILNext(tmp);
        }
    }

    return cbTotal;
}

LPITEMIDLIST Append(LPITEMIDLIST pidlBase, LPITEMIDLIST pidlAdd)
{
	LPMALLOC g_pMalloc;
	SHGetMalloc(&g_pMalloc);

    if(pidlBase == NULL)
        return NULL;
    if(pidlAdd == NULL)
        return pidlBase;
    
    LPITEMIDLIST pidlNew;

    UINT cb1 =  GetSize(pidlBase) - sizeof(pidlBase->mkid.cb);
    UINT cb2 = GetSize(pidlAdd);

    pidlNew = (LPITEMIDLIST)g_pMalloc->Alloc(cb1 + cb2);
    if (pidlNew)
    {
        CopyMemory(pidlNew, pidlBase, cb1);
        CopyMemory(((LPSTR)pidlNew) + cb1, pidlAdd, cb2);
    }
    return pidlNew;
}

// Retrieves the UIObject interface for the specified full PIDL
STDAPI SHGetUIObjectFromFullPIDL(LPCITEMIDLIST pidl, HWND hwnd, REFIID riid, void **ppv)
{
    LPCITEMIDLIST pidlChild;
    IShellFolder* psf;

    *ppv = NULL;

    HRESULT hr = SHBindToParent(pidl,
		IID_IShellFolder, 
        (void**) &psf,
		&pidlChild);

    if (SUCCEEDED(hr))
    {
        hr = psf->GetUIObjectOf(hwnd, 1, &pidlChild, riid, NULL, ppv);
        psf->Release();
    }
    return hr;
}
 
#define ILSkip(pidl, cb)       ((LPITEMIDLIST)(((BYTE*)(pidl))+cb))
#define ILNext(pidl)           ILSkip(pidl, (pidl)->mkid.cb)
 
HRESULT SHILClone(LPCITEMIDLIST pidl, LPITEMIDLIST *ppidl)
{
	DWORD cbTotal = GetSize(pidl);
    
    *ppidl = (LPITEMIDLIST)CoTaskMemAlloc(cbTotal);
    
    if (*ppidl)
        CopyMemory(*ppidl, pidl, cbTotal);
 
    return  *ppidl ? S_OK: E_OUTOFMEMORY;
}
 
// Get the target PIDL for a folder PIDL. This deals with cases where a folder
// is an alias to a real folder, folder shortcuts, etc.
STDAPI SHGetTargetFolderIDList(LPCITEMIDLIST pidlFolder, LPITEMIDLIST *ppidl)
{
    IShellLink *psl;
	
    *ppidl = NULL;
    
    HRESULT hr = SHGetUIObjectFromFullPIDL(
		pidlFolder, 
		NULL, 
		IID_IShellLink, 
		(void**) &psl);
    
    if (SUCCEEDED(hr))
    {
        hr = psl->GetIDList(ppidl);
        psl->Release();
    }
    
    // It's not a folder shortcut so get the PIDL normally.
    if (FAILED(hr))
        hr = SHILClone(pidlFolder, ppidl);
    return hr;
}

// Get the target folder for a folder PIDL. This deals with cases where a folder
// is an alias to a real folder, folder shortcuts, the My Documents folder, etc.
STDAPI SHGetTargetFolderPath(LPCITEMIDLIST pidlFolder, LPWSTR pszPath, UINT cchPath)
{
    LPITEMIDLIST pidlTarget;
	
    *pszPath = 0;

    HRESULT hr = SHGetTargetFolderIDList(pidlFolder, &pidlTarget);
    
    if (SUCCEEDED(hr))
    {
        SHGetPathFromIDListW(pidlTarget, pszPath);   // Make sure it is a path
        CoTaskMemFree(pidlTarget);
    }
    
    return *pszPath ? S_OK : E_FAIL;
}

folderCount::folderCount(CString Orig, HWND windowHandle)
{
	// Constructor
	countInfo.dir = Orig;
	countInfo.wh = windowHandle;
}

folderCount::~folderCount(VOID)
{
	// Destructor
}

BOOL copyFile(CString inputFullPath, CString outputFullPath, HANDLE KillThread, HANDLE ThreadKilled)
{
	// FileStream input
	HANDLE input;

	// FileStream output
	HANDLE output;

	// Stream Buffer
	char buffer[bufferSize];
			
	if ((
		input = CreateFile(
			inputFullPath,          // open input file 
			GENERIC_READ,           // open for reading 
			FILE_SHARE_READ,        // share for reading 
			NULL,                   // no security 
			OPEN_EXISTING,          // existing file only 
			FILE_ATTRIBUTE_NORMAL,	// normal file 
			NULL) 
	) != INVALID_HANDLE_VALUE) {

		//
		BY_HANDLE_FILE_INFORMATION bhfi;
		//
		GetFileInformationByHandle(input, &bhfi);
		//
		if ((
			output = CreateFile(
					outputFullPath,			// create output file
					GENERIC_WRITE,          // open for writing 
					0,                      // do not share 
					NULL,                   // no security 
					CREATE_ALWAYS,          // overwrite existing 
					bhfi.dwFileAttributes,	// normal file
					NULL) 
		) != INVALID_HANDLE_VALUE) {
			
			//
			DWORD count = 0;
			//
			DWORD wCount = 0;
			
			try {
				// Attempt a synchronous read operation.

				DWORD waitResult = WaitForSingleObject(KillThread, 0);

				while (
					waitResult != WAIT_OBJECT_0
					&& (ReadFile(	input, 
									&buffer, 
									bufferSize, 
									&count, 
									NULL))
					&& count > 0 
				)
				{
					WriteFile(
							output,
							&buffer,
							count,
							&wCount,
							NULL
						);

					waitResult = WaitForSingleObject(KillThread, 0);
				}

				//while (true)
				//{

				//waitResult = WaitForSingleObject(KillThread, 0) ;

				//switch ( waitResult )
				//{
				//	case WAIT_OBJECT_0:
				//		AfxMessageBox("WAIT_OBJECT_0");
				//		break;
				//	case WAIT_ABANDONED:
				//		AfxMessageBox("WAIT_ABANDONED");
				//		break;
				//	case WAIT_TIMEOUT:
				//	//	AfxMessageBox("WAIT_TIMEOUT");
				//		break;
				//}

				//}

				if (waitResult == WAIT_OBJECT_0) {
					//
					CloseHandle ( output );
					CloseHandle ( input );
					//
					return FALSE;
				}

			}
			catch ( std::exception &e )
			{
				MessageBox(NULL, e.what( ), "Exception... ", MB_OK);
			}
			//
			SetFileTime(output,&bhfi.ftCreationTime, &bhfi.ftLastAccessTime, &bhfi.ftLastWriteTime);
			//
			CloseHandle( output );	

		} 

		CloseHandle( input );

	}
	return TRUE;
}

// Stop running threads
VOID folderCount::Stop(VOID)
{
	return VOID();
}

// Supend running threads
VOID folderCount::Suspend(VOID)
{
	return VOID();
}

// Resume supended threads
VOID folderCount::Resume(VOID)
{
	return VOID();
}

VOID createDir( CString dir )
{
	//
	;
	//
	if ( ( _mkdir( (LPCTSTR)(dir) ) != 0 ) && ( errno == ENOENT ) ) {
		//
		int curPos= 0;
		//
		CString newDir = _T("");
		//
		CString resToken= dir.Tokenize(_T("\\"),curPos);
		newDir += resToken;
		//
		while (resToken != "") {
			_mkdir((LPCTSTR)(newDir));
			resToken = dir.Tokenize(_T("\\"),curPos);
			newDir += _T("\\") + resToken;
		};
	}
	//
	return;
}

typedef BOOL (CALLBACK* LPFNDLLFUNC)
	(LPCTSTR,
	PULARGE_INTEGER,
	PULARGE_INTEGER,
	PULARGE_INTEGER);

ULARGE_INTEGER GetPathFreeSpace(LPCTSTR pszDrive)
{
	LPFNDLLFUNC pGetDiskFreeSpaceEx = (LPFNDLLFUNC)
		GetProcAddress( GetModuleHandle("kernel32.dll"),
							"GetDiskFreeSpaceExA");

	ULARGE_INTEGER 
		i64FreeBytesToCaller,
		i64TotalBytes,
		i64FreeBytes;

	BOOL fResult;

	DWORD 
		dwSectPerClust,
		dwBytesPerSect,
		dwFreeClusters,
		dwTotalClusters;

	ULARGE_INTEGER lInt;

	if (pGetDiskFreeSpaceEx)
	{
		fResult = pGetDiskFreeSpaceEx (pszDrive,
						&i64FreeBytesToCaller,
						&i64TotalBytes,
						&i64FreeBytes);

		// Process GetDiskFreeSpaceEx results.
		lInt = i64FreeBytesToCaller;
		return lInt;
	}
	else 
	{
		fResult = GetDiskFreeSpace (pszDrive, 
						&dwSectPerClust, 
						&dwBytesPerSect,
						&dwFreeClusters, 
						&dwTotalClusters);

		// Process GetDiskFreeSpace results.
        lInt.QuadPart = Int32x32To64(dwSectPerClust*dwBytesPerSect,
							 dwFreeClusters);
		return lInt;

	}
}

struct PATH            // Declare PATH structure
{
	CString path;       // Define members x and y
	CString name;
	_fsize_t size;
	bool isfile;
};

// Start counting thread
// Recursion using a stack
UINT CopyMoveDir(CString Orig, CString Dest, LONG bLength, HWND windowHandle, BOOL deleteOriginFolder, BOOL createEmptyDirs, HANDLE KillThread, HANDLE ThreadKilled)
{ 
	//
	PATH path;
	//
	std::stack <PATH> stk;
	std::stack <PATH> tmp;
	//
	UINT fileCount = 0;
	
	//--- . ---

	int piecesNumber = 1;
	long copiedSize = 0;
	long baseSize = 0;
	//
	CString baseDestiny = "";
	CString pieceDestiny = "";
	//
	baseSize = bLength * oneMega; // in Mb

	//---

	// Destiny folder
	CString folderDestiny = Dest.TrimRight(_T("\\ ")).TrimLeft(_T(" "));
	// base subfolders name
	baseDestiny = folderDestiny + "\\CD";
	// folder destiny subfolder name
	pieceDestiny = baseDestiny + "_001";
	//
	Orig  = Orig.TrimRight(_T("\\ ")).TrimLeft(_T(" "));
	//
	int lastSlash = Orig.ReverseFind('\\');
	if (lastSlash == -1)
		lastSlash = Orig.ReverseFind(':') + 1;
	// Origin folder
	CString folderOrigin = Orig.Right( Orig.GetLength( ) - lastSlash - 1 );
	// Origin base folder
	CString baseOrigin = Orig.Left( lastSlash );

	//---

	//Create destination folder
	_mkdir((LPCTSTR)pieceDestiny);

	//--- . ---

	path.path = folderOrigin;
	path.isfile = false;
	//
	stk.push( path );
	//
	CString dir;
	//
	intptr_t hdl = -1;
	//
	_finddata_t fdt;
	//
	DWORD waitResult = WaitForSingleObject(KillThread, 0);
	while (!stk.empty( ) && (waitResult != WAIT_OBJECT_0))
	{ 
		//
		path = stk.top( );
		//
		dir = path.path;
		//
		stk.pop( );
		//
		hdl = -1;

		//
		//BOOL isDirEmpty = TRUE;
		//

		BOOL firstFile = TRUE;

		//
		if (path.isfile) { // is a file

			//AfxMessageBox(dir + "\\" + path.name);
			
			copiedSize += path.size;
			//
			if (copiedSize > baseSize) {
				//
				//isDirEmpty = FALSE;
				//
				piecesNumber ++;
				copiedSize = path.size;
				//
				char num[5];
				itoa(1000 + piecesNumber, num, 10);
				//
				CString nums = _T(num);
				nums = nums.Right(3);
				//
				pieceDestiny = baseDestiny + _T("_") + nums;
				// Create destination folder
				createDir(pieceDestiny + _T("\\") + dir);
			}
			//
			//if (isDirEmpty) {
			//	//
			//	isDirEmpty = FALSE;
			//	//Create destination folder
			//	createDir(pieceDestiny + _T("\\") + dir );
			//}
			//
			USES_CONVERSION;
			waitResult = WaitForSingleObject(KillThread, 0);
			if (waitResult == WAIT_OBJECT_0)
				break;
			SendMessage( windowHandle, WM_USER_PROGRESS_STEP, (WPARAM)T2CA( pieceDestiny + _T("\\") + dir + _T("\\") + path.name ), 0 );
			//
			if (!copyFile(baseOrigin + _T("\\") + dir + _T("\\") + path.name, pieceDestiny + _T("\\") + dir + _T("\\") + path.name, KillThread, ThreadKilled))
			{
				if (!stk.empty( ))  
					while(!stk.empty( ))
						stk.pop( );
				return 1;
			}
			//// deleteOriginFolder
			//if ( deleteOriginFolder )
			//	_rmdir( (LPCTSTR)(baseOrigin + _T("\\") + dir) );
		}
		else 
		{

			//
			// push dirs
			//
			try {
				// Get the files and folders under folderOrigin directory enumerator handle
				hdl= _findfirst((LPCTSTR)(baseOrigin + _T("\\") + dir + _T("\\") + _T("*.*")),&fdt);
			}
			catch ( std::exception &e )
			{
				AfxMessageBox(e.what( ));
			}

			if (hdl != -1)
			{
				//
				do { 
					//if ((fdt.attrib & _A_SUBDIR) != 0) 
					//{ // is a dir 
						// skip . and ..
						if (( strcmp(fdt.name, ".") == 0)  || ( strcmp( fdt.name , ".." ) == 0 ) )
							continue;
						//	
						if ((fdt.attrib & _A_SUBDIR) != 0) {
							path.isfile = false;
							path.path = dir + _T("\\") + fdt.name;
							tmp.push( path );
						}
						//} else {/*
						//	path.isfile = true;
						//	path.name = fdt.name;
						//	path.size = fdt.size;
						//	path.path = dir;*/
						//	continue;
						//}
						
						
					//}
				}
				//
				while( _findnext(hdl, &fdt) == 0);
				
				// Create target dir
				//if (/*isDirEmpty && */createEmptyDirs) {
				//	//
				//	createDir(pieceDestiny + _T("\\") + dir );
				//}
				//// deleteOriginFolder
				//if ( deleteOriginFolder )
				//	_rmdir( (LPCTSTR)(baseOrigin + _T("\\") + dir) );
				// free enumerator resources
				_findclose(hdl);
			}

			//
			// push files
			//

			//
			try {
				// Get the files and folders under folderOrigin directory enumerator handle
				hdl= _findfirst((LPCTSTR)(baseOrigin + _T("\\") + dir + _T("\\") + _T("*.*")),&fdt);
			}
			catch ( std::exception &e )
			{
				AfxMessageBox(e.what( ));
			}

			if (hdl != -1)
			{
				//
				do { 
					//if ((fdt.attrib & _A_SUBDIR) != 0) 
					//{ // is a dir 
						// skip . and ..
						if (( strcmp(fdt.name, ".") == 0)  || ( strcmp( fdt.name , ".." ) == 0 ) )
							continue;
						//	
						if ((fdt.attrib & _A_SUBDIR) == 0) {
							path.isfile = true;
							path.name = fdt.name;
							path.size = fdt.size;
							path.path = dir;

							tmp.push( path );

						if (firstFile) {
								//
								createDir(pieceDestiny + _T("\\") + dir );
								//
								firstFile = FALSE;
							}
						}
						
					//}
				}
				//
				while( _findnext(hdl, &fdt) == 0);
				
				// Create target dir
				//if (/*isDirEmpty && */createEmptyDirs) {
				//	//
				//	createDir(pieceDestiny + _T("\\") + dir );
				//}
				//// deleteOriginFolder
				//if ( deleteOriginFolder )
				//	_rmdir( (LPCTSTR)(baseOrigin + _T("\\") + dir) );
				// free enumerator resources
				_findclose(hdl);
			}

			//
			// Reorder dirs stack
			//

			while (!tmp.empty( )) {
				//
				path = tmp.top( );
				//
				stk.push(path);
				//
				tmp.pop( );
			}
		
		}

	}

	if (waitResult == WAIT_OBJECT_0) {
		if (!stk.empty( ))  
			while(!stk.empty( ))
				stk.pop( );
		return 1;
	}
	//
	return 0;
}

// Start counting thread
// Recursion using a stack
UINT countFolderFiles(CString folder,  HWND whandle)
{
	std::stack <CString> stk;
	//
	folder = folder.TrimRight('\\');
	//
	UINT fileCount = 0;
	//
	stk.push(_T("\\"));
	//
	CString dir;
	//
	intptr_t hdl = -1;
	//
	_finddata_t fdt;
	//
	while (!stk.empty())
	{ 
		//
		dir = stk.top( );
		//
		stk.pop( );
		//
		hdl = -1;
		//
		fdt;
		//
		try {
			// Get the files and folders under folderOrigin directory enumerator handle
			hdl= _findfirst((LPCTSTR)(folder + dir + _T("*.*")),&fdt);
		}
		catch ( std::exception &e )
		{
			AfxMessageBox(e.what( ));
		}
		//
		if (hdl != -1)
		{
			//
			do {
				if ((fdt.attrib & _A_SUBDIR) == 0) {
					// Is a file
					fileCount++;
					SendMessage( whandle, WM_USER_PROGRESS_SET, fileCount, 0);
				}
				else {
					// is a dir 
					// skip . and ..
					if (( strcmp(fdt.name, ".") == 0)  || ( strcmp( fdt.name , ".." ) == 0 ) )
						continue;
					//
					stk.push(dir + fdt.name + _T("\\"));
				}
			}
			//
			while( _findnext(hdl, &fdt) == 0);
			// free enumerator resources
			_findclose(hdl);
		}	
	}
	//
	return fileCount;
}

// Recursive 
//UINT folderSplitter::countFolderFiles(CString folder)
//{
//	//
//	_finddata_t fdt;
//	//
//	intptr_t hdl = -1;
//
//	try {
//		// Get the files and folders under folderOrigin directory enumerator handle
//		hdl= _findfirst((LPCTSTR)(folder + _T("\\*.*")),&fdt);
//	}
//	catch ( std::exception &e )
//	{
//		AfxMessageBox(e.what( ));
//	}
//	
//	UINT fileCount = 0;
//
//	if (hdl != -1)
//	{
//		//char buffer[65];
//		do {
//			if ((fdt.attrib & _A_SUBDIR) == 0) {
//				// Is a file
//				fileCount++;
//			}
//			else {
//				// is a dir 
//				// skip . and ..
//				if (( strcmp(fdt.name, ".") == 0)  || ( strcmp( fdt.name , ".." ) == 0 ) )
//					continue;
//				//
//				fileCount += countFolderFiles( folder + _T("\\") + fdt.name); //, fdt);
//			}
//		}
//		while( _findnext(hdl, &fdt) == 0);
//		_findclose(hdl);
//	}
//
//	return fileCount;
//}

// Working copy thread function
UINT Copy(LPVOID pParam)
{
	//
	SCopyINFO* ci = (SCopyINFO*)pParam;
	//
	UINT res = CopyMoveDir(ci->Orig, ci->Dest, ci->bLength, 
		ci->windowHandle, ci->deleteOriginFolder, ci->createEmptyDirs, 
		ci->KillThread, ci->ThreadKilled);

	//
	if ( res > 0 ) {
		SetEvent(ci->ThreadKilled);
	}
	else {
		SendMessage(ci->windowHandle, WM_USER_PROGRESS_ENDCOPY, 0, 0);
	}
	//
	AfxEndThread(0, TRUE);

	return 0;
}

// Working count thread
UINT Count(LPVOID pParam)
{
	SCountINFO* ci = (SCountINFO*)pParam;
	// Thread count procedure
	UINT fileCount = countFolderFiles( ci->dir, ci->wh );

	//CHAR str[64];
	//AfxMessageBox( itoa(fileCount, str, 10) );
	//
	PostMessage( ci->wh, WM_USER_PROGRESS_ENDCOUNT, 0, 0);
	//
	AfxEndThread(0, TRUE);
	return 0;
}

// Start the Count Thread
VOID folderCount::Start(VOID)
{
	countThread = AfxBeginThread( Count, &countInfo );
	return VOID();
}

folderSplitter::folderSplitter(CString Orig, CString Dest, LONG bLength, HWND windowHandle, HANDLE KThread, HANDLE ThreadK)
{
	// Constructor
	copyInfo.Orig = Orig;
	copyInfo.Dest = Dest;
	copyInfo.bLength = bLength;
	copyInfo.windowHandle = windowHandle;
	//
	copyInfo.createEmptyDirs = FALSE;
	copyInfo.deleteOriginFolder = FALSE;
	//
	copyInfo.KillThread = KThread;
	copyInfo.ThreadKilled = ThreadK;
};

folderSplitter::~folderSplitter(VOID)
{
	// Destructor
}

// Start copy and count threads
VOID folderSplitter::Start( VOID )
{
	//
	ResetEvent(copyInfo.KillThread);
	ResetEvent(copyInfo.ThreadKilled);
	//
	copyThread = AfxBeginThread( Copy, &copyInfo );
	//
	return VOID();
}

// Supend running threads
VOID folderSplitter::Suspend(VOID)
{
	return VOID();
}

// Resume supended threads
VOID folderSplitter::Resume(VOID)
{
	return VOID();
}

// Stop running threads
VOID folderSplitter::Stop(VOID)
{
	if(copyThread != NULL)
	{
		//
		SetEvent(copyInfo.KillThread);
		//
		while(WaitForSingleObject(copyInfo.ThreadKilled, 0) != WAIT_OBJECT_0);
		//
		copyThread = NULL;
		//
	}
	return VOID();
}
