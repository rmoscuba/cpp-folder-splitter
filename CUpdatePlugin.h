// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CUpdatePlugin wrapper class

class CUpdatePlugin : public COleDispatchDriver
{
public:
	CUpdatePlugin(){} // Calls COleDispatchDriver default constructor
	CUpdatePlugin(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CUpdatePlugin(const CUpdatePlugin& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// IUpdatePlugin methods
public:
	void StartUpdaterTask(LPCTSTR StartBy, LPCTSTR strTaskType, LPCTSTR strTaskID, LPCTSTR strSource, LPCTSTR strDest)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, StartBy, strTaskType, strTaskID, strSource, strDest);
	}
	void StopUpdaterTask(LPCTSTR strTaskType)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strTaskType);
	}
	void GetUpdaterTaskStatus(LPCTSTR strTaskType, long * pStatus)
	{
		static BYTE parms[] = VTS_BSTR VTS_PI4 ;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strTaskType, pStatus);
	}
	void SetUpdaterTaskOption(LPCTSTR strSectionName, LPCTSTR strValueName, LPCTSTR strValue)
	{
		static BYTE parms[] = VTS_BSTR VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_EMPTY, NULL, parms, strSectionName, strValueName, strValue);
	}

	// IUpdatePlugin properties
public:

};
