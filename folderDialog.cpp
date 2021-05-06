
#include "foldrDialog.h"

// Retrieves the UIObject interface for the specified full PIDL
STDAPI SHGetUIObjectFromFullPIDL(LPCITEMIDLIST pidl, HWND hwnd, REFIID riid, void **ppv)
{
    LPCITEMIDLIST pidlChild;
    IShellFolder* psf;

    *ppv = NULL;

    HRESULT hr = SHBindToParent(pidl, IID_PPV_ARG(IShellFolder, &psf), &pidlChild);
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
    DWORD cbTotal = 0;

    if (pidl) 
    {
        cbTotal += sizeof(pidl->mkid.cb);
        while (pidl->mkid.cb) 
        {
            cbTotal += pidl->mkid.cb;
            pidl = ILNext(pidl);
        }
    }
    
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
    
    HRESULT hr = SHGetUIObjectFromFullPIDL(pidlFolder, NULL, IID_PPV_ARG(IShellLink, &psl));
    
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