// demoDoc.cpp : implementation of the CDemoDoc class
//

#include "stdafx.h"
#include "demo.h"
#include "demoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc

IMPLEMENT_DYNCREATE(CDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDemoDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction

CDemoDoc::CDemoDoc()
{
	m_hDIB = NULL;
	m_palDIB = NULL;
	m_sizeDoc = CSize(1,1);
	avi = NULL;
	isAvi = false;
}

CDemoDoc::~CDemoDoc()
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
	}
	if (avi != NULL)
		delete avi;
}

BOOL CDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemoDoc serialization

void CDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc diagnostics

#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands

void CDemoDoc::InitDIBData()
{
	if (m_palDIB != NULL)
	{
		delete m_palDIB;
		m_palDIB = NULL;
	}
	if (m_hDIB == NULL)
	{
		
		return;
	}
	// Set up document size
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) m_hDIB);
	if (::DIBWidth(lpDIB) > INT_MAX ||::DIBHeight(lpDIB) > INT_MAX)
	{
		::GlobalUnlock((HGLOBAL) m_hDIB);
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		CString strMsg="this dib is too big";
		MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
		return;
	}
	m_sizeDoc = CSize((int) ::DIBWidth(lpDIB), (int) ::DIBHeight(lpDIB));
	::GlobalUnlock((HGLOBAL) m_hDIB);
	// Create copy of palette
	m_palDIB = new CPalette;
	if (m_palDIB == NULL)
	{
		// we must be really low on memory
		::GlobalFree((HGLOBAL) m_hDIB);
		m_hDIB = NULL;
		return;
	}
	if (::CreateDIBPalette(m_hDIB, m_palDIB) == NULL)
	{
		// DIB may not have a palette
		delete m_palDIB;
		m_palDIB = NULL;
		return;
	}
}

BOOL CDemoDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{

	CFile file;
	CFileException fe;
	//m_dib.Write(lpszPathName);
	if (!file.Open(lpszPathName, CFile::modeCreate |
	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
	{
		ReportSaveLoadException(lpszPathName, &fe,
			TRUE, AFX_IDP_INVALID_FILENAME);
		return FALSE;
	}

	// replace calls to Serialize with SaveDIB function
	BOOL bSuccess = FALSE;
	TRY
	{
		BeginWaitCursor();
		bSuccess = ::SaveDIB(m_hDIB, file);
		file.Close();
	}
	CATCH (CException, eSave)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eSave,
			TRUE, AFX_IDP_FAILED_TO_SAVE_DOC);
		return FALSE;
	}
	END_CATCH

	EndWaitCursor();
	SetModifiedFlag(FALSE);     // back to unmodified

	if (!bSuccess)
	{
		// may be other-style DIB (load supported but not save)
		//  or other problem in SaveDIB
		if (m_dib.Write(lpszPathName) != TRUE) {
			CString strMsg="cantnot save this dib";
			MessageBox(NULL, strMsg, NULL, MB_ICONINFORMATION | MB_OK);
			return FALSE;
		}
	}
	
	OnOpenDocument(lpszPathName);
	SetModifiedFlag(FALSE);
	return TRUE;
}

BOOL CDemoDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CFile file;
	CFileException fe;
	m_dib.Read(lpszPathName);
	if (!file.Open(lpszPathName, CFile::modeRead | CFile::shareDenyWrite, &fe))
	{
		if (!OnOpenDocument1(lpszPathName)) {
            ReportSaveLoadException(lpszPathName, &fe,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		    return FALSE;
		}
		else {
			return TRUE;
		}
	}

	DeleteContents();
	BeginWaitCursor();

	// replace calls to Serialize with ReadDIBFile function
	TRY
	{
		m_hDIB = ::ReadDIBFile(file);
	    //m_dib.Read(lpszPathName);
	}
	CATCH (CFileException, eLoad)
	{
		file.Abort(); // will not throw an exception
		EndWaitCursor();
		ReportSaveLoadException(lpszPathName, eLoad,
			FALSE, AFX_IDP_FAILED_TO_OPEN_DOC);
		m_hDIB = NULL;
		return FALSE;
	}
	END_CATCH

	InitDIBData();
	EndWaitCursor();

	if (m_hDIB == NULL)
	{
		// may not be DIB format
		CString strMsg;
		strMsg = "读取图像时出错！可能是不支持该类型的图像文件！";
		
		// 提示出错
        ::MessageBox(NULL, strMsg, "系统提示", MB_ICONINFORMATION | MB_OK);
		
		// 返回FALSE
		return FALSE;
	}
	SetPathName(lpszPathName);
	SetModifiedFlag(FALSE);     // start off with unmodified
	return TRUE;
	
}

void CDemoDoc::ReplaceHDIB(HDIB hDIB)
{
	if (m_hDIB != NULL)
	{
		::GlobalFree((HGLOBAL) m_hDIB);
	}
	m_hDIB = hDIB;
}
//////---------------------------以下为新建项
BOOL CDemoDoc::OnSaveDocument1(LPCTSTR lpszPathName)
{
	if (m_dib.Write(lpszPathName) == TRUE) {
		SetModifiedFlag(FALSE);     // have saved
		return TRUE;
	}
	else
		return 0;
}

BOOL CDemoDoc::OnOpenDocument1(LPCTSTR lpszPathName)
{

	if (m_dib.Read(lpszPathName) == TRUE) {
		SetModifiedFlag(FALSE);     // start off with unmodified
		return TRUE;
	}
	else
		return 0;
}
