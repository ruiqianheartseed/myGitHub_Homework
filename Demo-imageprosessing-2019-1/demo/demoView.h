// demoView.h : interface of the CDemoView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEMOVIEW_H__DE734DBE_1CB6_4B2F_862C_8C090198DB38__INCLUDED_)
#define AFX_DEMOVIEW_H__DE734DBE_1CB6_4B2F_862C_8C090198DB38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDemoView : public CScrollView
{
protected: // create from serialization only
	CDemoView();
	DECLARE_DYNCREATE(CDemoView)

// Attributes
public:
	CDemoDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual void OnDraw1(CDC* pDC);//added
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnInitialUpdate1();//added
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDemoView)
	afx_msg void OnSobel();
	afx_msg void OnLaplace();
	afx_msg void OnRoberts();
	afx_msg void OnFft();
	afx_msg void OnFftGrid();
	afx_msg void OnNearest2();
	afx_msg void OnNearest3();
	afx_msg void OnNearest4();
	afx_msg void OnBilinear2();
	afx_msg void OnBilinear3();
	afx_msg void OnBilinear4();
	afx_msg void OnBicubic2();
	afx_msg void OnBicubic3();
	afx_msg void OnBicubic4();
	afx_msg void OnAver3();
	afx_msg void OnAver5();
	afx_msg void OnAver7();
	afx_msg void OnMedian5();
	afx_msg void OnMedian7();
	afx_msg void OnMedian3();
	afx_msg void OnHisteq();
	afx_msg void OnStrech();
	afx_msg void OnOtsu();
	afx_msg void OnCorrect();
	afx_msg void OnGray();
    afx_msg void OnIfft();
	afx_msg void OnColortogray();
	afx_msg void OnKrich();
	afx_msg void OnTophatvally();
	afx_msg void OnBinary1();
	afx_msg void OnHough();
	afx_msg void OnHoughline();
	afx_msg void OnRepairline();
	afx_msg void OnTakeline();
	afx_msg void OnRepairline2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	
	
	
};

#ifndef _DEBUG  // debug version in demoView.cpp
inline CDemoDoc* CDemoView::GetDocument()
   { return (CDemoDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOVIEW_H__DE734DBE_1CB6_4B2F_862C_8C090198DB38__INCLUDED_)
