

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0622 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for demo.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0622 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */


#ifndef __demo_h_h__
#define __demo_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __Idemo_FWD_DEFINED__
#define __Idemo_FWD_DEFINED__
typedef interface Idemo Idemo;

#endif 	/* __Idemo_FWD_DEFINED__ */


#ifndef __CdemoDoc_FWD_DEFINED__
#define __CdemoDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CdemoDoc CdemoDoc;
#else
typedef struct CdemoDoc CdemoDoc;
#endif /* __cplusplus */

#endif 	/* __CdemoDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __demo_LIBRARY_DEFINED__
#define __demo_LIBRARY_DEFINED__

/* library demo */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_demo;

#ifndef __Idemo_DISPINTERFACE_DEFINED__
#define __Idemo_DISPINTERFACE_DEFINED__

/* dispinterface Idemo */
/* [uuid] */ 


EXTERN_C const IID DIID_Idemo;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("61a5d8ea-e160-4fcd-b3a0-5ef4ee2be41f")
    Idemo : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IdemoVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            Idemo * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            Idemo * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            Idemo * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            Idemo * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            Idemo * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            Idemo * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            Idemo * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IdemoVtbl;

    interface Idemo
    {
        CONST_VTBL struct IdemoVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define Idemo_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define Idemo_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define Idemo_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define Idemo_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define Idemo_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define Idemo_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define Idemo_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __Idemo_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CdemoDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("ad584c78-4c2a-4237-8229-eb1c30f31f80")
CdemoDoc;
#endif
#endif /* __demo_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


