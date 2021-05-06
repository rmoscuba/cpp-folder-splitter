

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Wed May 29 23:38:06 2013
 */
/* Compiler settings for boyscout.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __boyscout_h_h__
#define __boyscout_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Iboyscout_FWD_DEFINED__
#define __Iboyscout_FWD_DEFINED__
typedef interface Iboyscout Iboyscout;
#endif 	/* __Iboyscout_FWD_DEFINED__ */


#ifndef __boyscout_FWD_DEFINED__
#define __boyscout_FWD_DEFINED__

#ifdef __cplusplus
typedef class boyscout boyscout;
#else
typedef struct boyscout boyscout;
#endif /* __cplusplus */

#endif 	/* __boyscout_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __boyscout_LIBRARY_DEFINED__
#define __boyscout_LIBRARY_DEFINED__

/* library boyscout */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_boyscout;

#ifndef __Iboyscout_DISPINTERFACE_DEFINED__
#define __Iboyscout_DISPINTERFACE_DEFINED__

/* dispinterface Iboyscout */
/* [uuid] */ 


EXTERN_C const IID DIID_Iboyscout;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("C7A71A4F-2333-40AA-B4FA-90FA9ED8427A")
    Iboyscout : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IboyscoutVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Iboyscout * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Iboyscout * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Iboyscout * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Iboyscout * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Iboyscout * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Iboyscout * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Iboyscout * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IboyscoutVtbl;

    interface Iboyscout
    {
        CONST_VTBL struct IboyscoutVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Iboyscout_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Iboyscout_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Iboyscout_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Iboyscout_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Iboyscout_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Iboyscout_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Iboyscout_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __Iboyscout_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_boyscout;

#ifdef __cplusplus

class DECLSPEC_UUID("36808C2E-C2C1-451D-A28B-A62CFCB5A245")
boyscout;
#endif
#endif /* __boyscout_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


