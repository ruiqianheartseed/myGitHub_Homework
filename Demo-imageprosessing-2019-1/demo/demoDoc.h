// demoDoc.h : interface of the CDemoDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMODOC_H__7A136F06_7B90_4D59_8347_4F11D5428BF0__INCLUDED_)
#define AFX_DEMODOC_H__7A136F06_7B90_4D59_8347_4F11D5428BF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "dibapi.h"
#include "AVIHandler.h"
#include "ImageCenterDib.h"

class CDemoDoc : public CDocument
{
protected: // create from serialization only
	CDemoDoc();
	DECLARE_DYNCREATE(CDemoDoc)

	// Attributes
public:
	HDIB GetHDIB() const
	{
		
		return m_hDIB;
	}
	CPalette* GetDocPalette() const
	{
		if (m_palDIB == NULL) {
			
		}
		return m_palDIB;
	}
	CSize GetDocSize() const
	{
		return m_sizeDoc;
	}


	// Operations
public:
	void ReplaceHDIB(HDIB hDIB);
	void InitDIBData();
	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CDemoDoc)
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoDoc();
	
public:
	AVIHandler *avi;
	bool isAvi;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	HDIB m_hDIB;
	CPalette* m_palDIB;
	CSize m_sizeDoc;
private:

	// Generated message map functions
protected:
	//{{AFX_MSG(CDemoDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
//---------------------------------------以下为新建项
public:
	virtual BOOL OnSaveDocument1(LPCTSTR lpszPathName);
	virtual BOOL OnOpenDocument1(LPCTSTR lpszPathName);
	ImgCenterDib *GetPDib()
	{
		return &m_dib;
	}
private :
	//ImgCenterDib类的对象，从打开文件中读入的数据放在该对象中
	ImgCenterDib m_dib;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMODOC_H__7A136F06_7B90_4D59_8347_4F11D5428BF0__INCLUDED_)
