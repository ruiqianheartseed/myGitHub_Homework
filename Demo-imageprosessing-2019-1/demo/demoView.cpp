// demoView.cpp : implementation of the CDemoView class
//�������Լ��ĳ�����...

#include "stdafx.h"
#include "demo.h"
#include "demoDoc.h"
#include "demoView.h"
#include "math.h"
#include "MainFrm.h"
#include "transform.h"
#include "ImageCenterDib.h"
#include <iostream>
#include <vector>
#include <stack>
#include <fstream>
#include "GrayTrans.h"
#include "Morphology.h"
#include "histogram.h"
#include "Segment.h"

using namespace std ;
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDemoView

IMPLEMENT_DYNCREATE(CDemoView, CScrollView)

BEGIN_MESSAGE_MAP(CDemoView, CScrollView)
	//{{AFX_MSG_MAP(CDemoView)
	ON_COMMAND(ID_SOBEL, OnSobel)
	ON_COMMAND(ID_Laplace, OnLaplace)
	ON_COMMAND(ID_Roberts, OnRoberts)
	ON_COMMAND(ID_FFT, OnFft)
	ON_COMMAND(ID_FFTGrid, OnFftGrid)
	ON_COMMAND(ID_Nearest2, OnNearest2)
	ON_COMMAND(ID_Nearest3, OnNearest3)
	ON_COMMAND(ID_Nearest4, OnNearest4)
	ON_COMMAND(ID_bilinear2, OnBilinear2)
	ON_COMMAND(ID_bilinear3, OnBilinear3)
	ON_COMMAND(ID_bilinear4, OnBilinear4)
	ON_COMMAND(ID_bicubic2, OnBicubic2)
	ON_COMMAND(ID_bicubic3, OnBicubic3)
	ON_COMMAND(ID_bicubic4, OnBicubic4)
	ON_COMMAND(ID_Aver3, OnAver3)
	ON_COMMAND(ID_Aver5, OnAver5)
	ON_COMMAND(ID_Aver7, OnAver7)
	ON_COMMAND(ID_Median3, OnMedian3)
	ON_COMMAND(ID_Median5, OnMedian5)
	ON_COMMAND(ID_Median7, OnMedian7)
	ON_COMMAND(ID_histeq, OnHisteq)
	ON_COMMAND(ID_Strech, OnStrech)
	ON_COMMAND(ID_Otsu, OnOtsu)
	ON_COMMAND(ID_Correct, OnCorrect)
	ON_COMMAND(ID_Gray, OnGray)
	ON_COMMAND(ID_IFFT, OnIfft)
	ON_COMMAND(ID_Krich, OnKrich)
	ON_COMMAND(ID_Color2Gray, OnColortogray)
	ON_COMMAND(ID_TopHatVally,OnTophatvally)
    ON_COMMAND(ID_Binary, OnBinary1)
	ON_COMMAND(ID_Hough, OnHough)
	ON_COMMAND(ID_Houghline, OnHoughline)
	ON_COMMAND(ID_Repairline, OnRepairline)
	ON_COMMAND(ID_Takeline, OnTakeline)
	ON_COMMAND(ID_Repairline2,OnRepairline2)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	
	
	
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoView construction/destruction

CDemoView::CDemoView()
{

}

CDemoView::~CDemoView()
{
}

BOOL CDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDemoView drawing

void CDemoView::OnDraw(CDC* pDC)
{
	CDemoDoc* pDoc = GetDocument();
	HDIB hDIB = pDoc->GetHDIB();
	
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
		int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
		::GlobalUnlock((HGLOBAL) hDIB);
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		CRect rcDest;
		if (pDC->IsPrinting())   // printer DC
		{
			// get size of printer page (in pixels)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			// get printer pixels per inch
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);

			//
			// Best Fit case -- create a rectangle which preserves
			// the DIB's aspect ratio, and fills the page horizontally.
			//
			// The formula in the "->bottom" field below calculates the Y
			// position of the printed bitmap, based on the size of the
			// bitmap, the width of the page, and the relative size of
			// a printed pixel (cyInch / cxInch).
			//
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
					/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;

      // �����ӡͼ��λ�ã���ֱ���У�
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
		}
		else   // not printer DC
		{
			rcDest = rcDIB;
		}
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	else {
		OnDraw1(pDC);
	}
}
void CDemoView::OnDraw1(CDC* pDC) {
	//��ȡ�ĵ���ָ��
	CDemoDoc* pDoc = GetDocument();

	if (!pDoc->isAvi) {
		//����m_dib��ָ��
		ImgCenterDib *pDib = pDoc->GetPDib();

		//��ȡDIB�ĳߴ�
		CSize sizeFileDib = pDib->GetDimensions();

		//��ʾDIB
		pDib->Draw(pDC, CPoint(0, 0), sizeFileDib);
	}
	else
	{

	}
 }
void CDemoView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	ASSERT(GetDocument() != NULL);
	SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());
}
void CDemoView::OnInitialUpdate1()
{

	CScrollView::OnInitialUpdate();

	//��ȡ�ĵ���ָ��
	CDemoDoc* pDoc = GetDocument();

	//��ȡDIB��ָ��
	ImgCenterDib *pDib = pDoc->GetPDib();

	//����DIB�ߴ������Ӵ���С
	if (pDib != NULL)
		SetScrollSizes(MM_TEXT, pDib->GetDimensions());
	else {
		//���m_dibΪ�գ�������һ���̶��ĳߴ�
		CSize sizeTotal;
		sizeTotal.cx = sizeTotal.cy = 100;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDemoView printing

BOOL CDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CDemoView diagnostics

#ifdef _DEBUG
void CDemoView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDemoView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDemoDoc* CDemoView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoView message handlers
//�����Ǳ�Ե�������
void CDemoView::OnSobel() //sobel����
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount/8;
    
	if (b != 1 && b != 3) {
		::MessageBox(0, "ֻ����ҶȺͲ�ɫͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}

	int lineByte=(width * b+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	int  i, j, k, x, y;
	for(i=1;i<height-1;i++){
		    for(j=1;j<width-1;j++){          
		        for(k=0;k<b;k++){
		          x=*(lpDIBBits+(i+1)*lineByte+(j-1)*b+k)+2**(lpDIBBits+(i)*lineByte+(j-1)*b+k)+*(lpDIBBits+(i-1)*lineByte+(j-1)*b+k)
		            -*(lpDIBBits+(i+1)*lineByte+(j+1)*b+k)-2**(lpDIBBits+(i)*lineByte+(j+1)*b+k)-*(lpDIBBits+(i-1)*lineByte+(j+1)*b+k);
		          y=*(lpDIBBits+(i+1)*lineByte+(j-1)*b+k)+2**(lpDIBBits+(i+1)*lineByte+j*b+k)+*(lpDIBBits+(i+1)*lineByte+(j+1)*b+k)
		            -*(lpDIBBits+(i-1)*lineByte+(j-1)*b+k)-2**(lpDIBBits+(i-1)*lineByte+j*b+k)-*(lpDIBBits+(i-1)*lineByte+(j+1)*b+k);
		          if (sqrt(double(x * x) + double(y*y)) < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = sqrt(double(x * x) + double(y*y));
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }
	//-----------------------������ͨ�ú�����
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);//����ʾ������
	Invalidate();
}

void CDemoView::OnLaplace() //������˹����
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;

	if (b != 1 && b != 3) {
		::MessageBox(0, "ֻ����ҶȺͲ�ɫͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}

	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	int  i, j, k, x, y;

	for(i=1;i<height-1;i++){
		for(j=1;j<width-1;j++){          
			for(k=0;k<b;k++){
				x=*(lpDIBBits+(i)*lineByte+(j-1)*b+k)+*(lpDIBBits+(i-1)*lineByte+(j)*b+k)+*(lpDIBBits+(i+1)*lineByte+(j)*b+k)
					+*(lpDIBBits+(i)*lineByte+(j+1)*b+k)-4**(lpDIBBits+(i)*lineByte+(j)*b+k);
				x = abs (x);
				*(lpDIBBitsNew+i*lineByte+j*b + k) = x;

			}
		}
	}
	//---------------------------------------------------
	::GlobalUnlock(dib); 
    ::GlobalUnlock(dibNew); 
	  CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	  pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	  CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	  CDemoDoc* pDocNew=pView->GetDocument();
	  pDocNew->ReplaceHDIB((HDIB)dibNew);
	  pDocNew->InitDIBData();
	  pDocNew->UpdateAllViews(pView);
	 //pDocNew->SetModifiedFlag(TRUE);
	  Invalidate();
}

void CDemoView::OnRoberts() //Robert����
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;

	if (b != 1 && b != 3) {
		::MessageBox(0, "ֻ����ҶȺͲ�ɫͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}

	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	int  i, j, k, x, y;

	for(i=0;i<height-1;i++){
		for(j=0;j<width-1;j++){          
			for(k=0;k<b;k++){
				x=-*(lpDIBBits+(i)*lineByte+(j)*b+k)+*(lpDIBBits+(i+1)*lineByte+(j+1)*b+k);
			    y =-*(lpDIBBits+(i)*lineByte+(j+1)*b+k)+*(lpDIBBits+(i+1)*lineByte+(j)*b+k);
				if (sqrt(double(x*x) + double(y*y)) < 255)
				{
					*(lpDIBBitsNew+i*lineByte+j*b + k) = sqrt(double(x*x) + double(y*y));
				}
				else
					*(lpDIBBitsNew+i*lineByte+j*b + k) = 255;
			}
		}
	}
	//-----------------------------------------------------------------------------
	::GlobalUnlock(dib); 
    ::GlobalUnlock(dibNew); 
	  CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	  pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	  CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	  CDemoDoc* pDocNew=pView->GetDocument();
	  pDocNew->ReplaceHDIB((HDIB)dibNew);
	  pDocNew->InitDIBData();
	  pDocNew->UpdateAllViews(pView);
	  //pDocNew->SetModifiedFlag(TRUE);
	  Invalidate();
}

void CDemoView::OnKrich()//krich����
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b != 1 && b != 3) {
		::MessageBox(0, "ֻ����ҶȺͲ�ɫͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + h * lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + h * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//----------------------------------------------------------
	int width = 1;//ģ���С
	int i, j, m, n, x, hh;
	int k = 0, kk = 0;
	int *p = new int[(2 * width + 1)*(2 * width + 1)];
	int *q = new int[(2 * width + 1)*(2 * width + 1)];

	unsigned char *buf = new unsigned char[h*lineByte];
	for (i = width; i < h - width; i++)
	{
		for (j = width; j < w - width; j++)
		{
			unsigned char* pCur = lpDIBBits + i * lineByte + j;

			for (m = -width; m <= width; m++)
			{
				for (n = -width; n <= width; n++)
				{
					p[k] = *(pCur + m * lineByte + n);
					k++;
				}
			}
			k = 0;
			q[0] = p[0] * 5;
			q[1] = p[1] * 5;
			q[2] = p[2] * 5;
			q[3] = p[3] * (-3);
			q[5] = p[5] * (-3);
			q[6] = p[6] * (-3);
			q[7] = p[7] * (-3);
			q[8] = p[8] * (-3);
			hh = q[1] + q[2] + q[3] + q[0] + q[5] + q[6] + q[7] + q[8];
			if (hh > 255)
				hh = 255;
			if (hh > 128)
				*(buf + i * lineByte + j) = hh;
			else
				*(buf + i * lineByte + j) = 0;

		}
	}
	memcpy(lpDIBBitsNew, buf, h*lineByte * sizeof(unsigned char));
	delete[] buf;
	buf = NULL;
	//-----------------------------------------------------
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

//�����Ǹ���Ҷ�任
void CDemoView::OnFft() //����Ҷ�任
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b == 3) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	
	int bufLong =width;
	if(height>bufLong) bufLong = height;
	unsigned char *buf = new unsigned char[bufLong*bufLong];
	int i,j;
	for (i = 0;i<height;i++)
	{
		for(j =0;j<width;j++)
		{
			buf[i*width +j] = *(lpDIBBits +i*lineByte +j);
		}
		for(j = width;j<bufLong;j++)
			buf[i*width+j] =0;
	}
	for(i = height;i<bufLong;i++)
	{
		for(j = width;j<bufLong;j++)
		buf[i*width+j] = 0;
	}
	fft2D(buf,bufLong,bufLong);
	for (i = 0;i<height;i++)
	{
		for (j = 0;j<width;j++)
		{
			 *(lpDIBBitsNew+i*lineByte+j) = buf[i*bufLong+j];
		 
		}
	}
	delete []buf;
	//-----------------------------------------------------
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

void CDemoView::OnIfft()//����Ҷ���任
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b == 3)
	{
		::MessageBox(0, "ֻ����Ҷ�ͼ��", NULL, MB_OK);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);

	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + h * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + h* lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//-----------------------------------------------------------------
	int   bufLong = w;
	if (h > bufLong)
		bufLong = h;
	unsigned char *buf = new unsigned char[bufLong*bufLong];
	int i, j;
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
			buf[i*w + j] = *(lpDIBBits + i * lineByte + j);
		for (j = w; j < bufLong; j++)
			buf[i*w + j] = 0;
	}

	for (i = h; i < bufLong; i++)
	{
		for (j = w; j < bufLong; j++)
			buf[i*w + j] = 0;
	}

	ifft2D(buf, bufLong, bufLong);
	for (i = 0; i < h; i++)
	{
		for (j = 0; j < w; j++)
		{
			*(lpDIBBitsNew + i * lineByte + j) = buf[i*bufLong + j];
		}
	}
	delete[]buf;
	//------------------------------------------------------
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

void CDemoView::OnFftGrid() //ȥ��
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b == 3) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������

	int bufLong =width;
	if(height>bufLong) bufLong = height;
	unsigned char *buf = new unsigned char[bufLong*bufLong];
	int i,j;
	for (i = 0;i<height;i++)
	{
		for(j =0;j<width;j++)
		{
			buf[i*width +j] = *(lpDIBBits +i*lineByte +j);
		}
		for(j = width;j<bufLong;j++)
			buf[i*width+j] =0;
	}
	for(i = height;i<bufLong;i++)
	{
		for(j = width;j<bufLong;j++)
			buf[i*width+j] = 0;
	}
	fft2D_Cutnoise(buf,bufLong,bufLong);

	ifft2D(buf,width,height);
	for (i = 0;i<height;i++)
	{
		for (j = 0;j<width;j++)
		{
			*(lpDIBBitsNew+i*lineByte+j) =buf[i*width+j];

		}
	}
	delete []buf;
	//------------------------------------------------------------------------
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);

	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();

	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

//�����ǷŴ󣬲�ɫ�ڰ׶�����
void CDemoView::OnNearest2()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount/8;
	int lineByte=(w * b+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 2;
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND,sizeof(BITMAPINFOHEADER)+palSize+times*times*h*lineByte);
	LPSTR lpDIBNew=(LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize);
	LPBITMAPINFOHEADER	pNewHead=(LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth =times*w;
	pNewHead->biHeight =times*h;
	pNewHead->biSizeImage =times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int jj,ii;
			jj = j / times;
			ii = i / times;
			for (int k = 0; k < b; k++)
			{
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = *(lpDIBBits + ii*lineByte + jj*b + k);
			}

		}
	}
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();	
}

void CDemoView::OnNearest3()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 3;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int jj, ii;
			jj = j / times;
			ii = i / times;
			for (int k = 0; k < b; k++)
			{
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = *(lpDIBBits + ii*lineByte + jj*b + k);
			}

		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnNearest4()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 4;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;

	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int jj, ii;
			jj = j / times;
			ii = i / times;
			for (int k = 0; k < b; k++)
			{
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = *(lpDIBBits + ii*lineByte + jj*b + k);
			}

		}
	}

	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);

	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();

	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);

	Invalidate();
}

void CDemoView::OnBilinear2()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 2;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double row = i / times;
			double col = j / times;
			int lRow = floor(row);
			int nRow = lRow + 1;
			int lCol = floor(col);
			int rCol = lCol + 1;
			double u = row - lRow;
			double v = col - lCol;

			for (int k = 0; k < b; k++)
			{
				int f1, f2, f3;
				f1 = *(lpDIBBits + lRow*lineByte + lCol*b + k) + v*(*(lpDIBBits + lRow*lineByte + rCol*b + k) - *(lpDIBBits + lRow*lineByte + lCol*b + k));
				f2 = *(lpDIBBits + nRow*lineByte + lCol*b + k) + v*(*(lpDIBBits + nRow*lineByte + rCol*b + k) - *(lpDIBBits + nRow*lineByte + lCol*b + k));
				f3 = f1 + u*(f2 - f1);
				
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = f3;
			}

		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnBilinear3()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 3;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double row = i / times;
			double col = j / times;
			int lRow = floor(row);
			int nRow = lRow + 1;
			int lCol = floor(col);
			int rCol = lCol + 1;
			double u = row - lRow;
			double v = col - lCol;

			for (int k = 0; k < b; k++)
			{
				int f1, f2, f3;
				f1 = *(lpDIBBits + lRow*lineByte + lCol*b + k) + v*(*(lpDIBBits + lRow*lineByte + rCol*b + k) - *(lpDIBBits + lRow*lineByte + lCol*b + k));
				f2 = *(lpDIBBits + nRow*lineByte + lCol*b + k) + v*(*(lpDIBBits + nRow*lineByte + rCol*b + k) - *(lpDIBBits + nRow*lineByte + lCol*b + k));
				f3 = f1 + u*(f2 - f1);

				*(lpDIBBitsNew + i*nlineByte + j*b + k) = f3;
			}

		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnBilinear4()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 4;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			double row = i / times;
			double col = j / times;
			int lRow = floor(row);
			int nRow = lRow + 1;
			int lCol = floor(col);
			int rCol = lCol + 1;
			double u = row - lRow;
			double v = col - lCol;

			for (int k = 0; k < b; k++)
			{
				int f1, f2, f3;
				f1 = *(lpDIBBits + lRow*lineByte + lCol*b + k) + v*(*(lpDIBBits + lRow*lineByte + rCol*b + k) - *(lpDIBBits + lRow*lineByte + lCol*b + k));
				f2 = *(lpDIBBits + nRow*lineByte + lCol*b + k) + v*(*(lpDIBBits + nRow*lineByte + rCol*b + k) - *(lpDIBBits + nRow*lineByte + lCol*b + k));
				f3 = f1 + u*(f2 - f1);

				*(lpDIBBitsNew + i*nlineByte + j*b + k) = f3;
			}

		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnBicubic2()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 2;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < b; k++)
			{
				double row = i / times;
				double col = j / times;
				int lRow = floor(row);  //���Ͻǵ�
				int lCol = floor(col);	//���Ͻǵ�
				double v = row - lRow;
				double u = col - lCol;
				double A1, A2, A3, A4, C1, C2, C3, C4;
				A1 = 4 - 8 * fabs(u + 1) + 5 * fabs(u + 1) *fabs(u + 1) - fabs(u + 1)*fabs(u + 1)*fabs(u + 1);
				A2 = 1 - 2 * fabs(u) *fabs(u) + fabs(u)* fabs(u)*fabs(u);
				A3 = 1 - 2 * fabs(u - 1) *fabs(u - 1) + fabs(u - 1)* fabs(u - 1)*fabs(u - 1);
				A4 = 4 - 8 * fabs(u - 2) + 5 * fabs(u - 2) *fabs(u - 2) - fabs(u - 2)*fabs(u - 2)*fabs(u - 2);
				C1 = 4 - 8 * fabs(v + 1) + 5 * fabs(v + 1) *fabs(v + 1) - fabs(v + 1)*fabs(v + 1)*fabs(v + 1);
				C2 = 1 - 2 * fabs(v) *fabs(v) + fabs(v)* fabs(u)*fabs(v);
				C3 = 1 - 2 * fabs(v - 1) *fabs(v - 1) + fabs(v - 1)* fabs(v - 1)*fabs(v - 1);
				C4 = 4 - 8 * fabs(v - 2) + 5 * fabs(v - 2) *fabs(v - 2) - fabs(v - 2)*fabs(v - 2)*fabs(v - 2);
				int f[16];
				int count = 0;
				for (int t1 = -1; t1 < 3;t1++)
				{
					for (int t2 = -1; t2 < 3;t2++)
					{
						f[count] = *(lpDIBBits + (lRow + t1)*lineByte + (lCol + t2)*b + k);
						count++;
					}					
				}
				int AB[4],ABC;
				AB[0] = A1 * f[0] + A2*f[4] + A3 *f[8] + A4*f[12];
				AB[1] = A1*f[1] + A2*f[5] + A3 *f[9] + A4 * f[13];
				AB[2] = A1 *f[2] + A2 * f[6] + A3*f[10] + A4*f[14];
				AB[3] = A1 *f[3] + A2 * f[7] + A3*f[11] + A4*f[15];
				ABC = AB[0] * C1 + AB[1] * C2 + AB[2] * C3 + AB[3] * C4;
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = ABC;
			}
		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnBicubic3()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 3;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < b; k++)
			{
				double row = i / times;
				double col = j / times;
				int lRow = floor(row);  //���Ͻǵ�
				int lCol = floor(col);	//���Ͻǵ�
				double v = row - lRow;
				double u = col - lCol;
				double A1, A2, A3, A4, C1, C2, C3, C4;
				A1 = 4 - 8 * fabs(u + 1) + 5 * fabs(u + 1) *fabs(u + 1) - fabs(u + 1)*fabs(u + 1)*fabs(u + 1);
				A2 = 1 - 2 * fabs(u) *fabs(u) + fabs(u)* fabs(u)*fabs(u);
				A3 = 1 - 2 * fabs(u - 1) *fabs(u - 1) + fabs(u - 1)* fabs(u - 1)*fabs(u - 1);
				A4 = 4 - 8 * fabs(u - 2) + 5 * fabs(u - 2) *fabs(u - 2) - fabs(u - 2)*fabs(u - 2)*fabs(u - 2);
				C1 = 4 - 8 * fabs(v + 1) + 5 * fabs(v + 1) *fabs(v + 1) - fabs(v + 1)*fabs(v + 1)*fabs(v + 1);
				C2 = 1 - 2 * fabs(v) *fabs(v) + fabs(v)* fabs(u)*fabs(v);
				C3 = 1 - 2 * fabs(v - 1) *fabs(v - 1) + fabs(v - 1)* fabs(v - 1)*fabs(v - 1);
				C4 = 4 - 8 * fabs(v - 2) + 5 * fabs(v - 2) *fabs(v - 2) - fabs(v - 2)*fabs(v - 2)*fabs(v - 2);
				int f[16];
				int count = 0;
				for (int t1 = -1; t1 < 3; t1++)
				{
					for (int t2 = -1; t2 < 3; t2++)
					{
						f[count] = *(lpDIBBits + (lRow + t1)*lineByte + (lCol + t2)*b + k);
						count++;
					}
				}
				int AB[4], ABC;
				AB[0] = A1 * f[0] + A2*f[4] + A3 *f[8] + A4*f[12];
				AB[1] = A1*f[1] + A2*f[5] + A3 *f[9] + A4 * f[13];
				AB[2] = A1 *f[2] + A2 * f[6] + A3*f[10] + A4*f[14];
				AB[3] = A1 *f[3] + A2 * f[7] + A3*f[11] + A4*f[15];
				ABC = AB[0] * C1 + AB[1] * C2 + AB[2] * C3 + AB[3] * C4;
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = ABC;
			}
		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnBicubic4()
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount / 8;
	int lineByte = (w * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int times;
	times = 4;
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + times*times*h*lineByte);
	LPSTR lpDIBNew = (LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize);
	LPBITMAPINFOHEADER	pNewHead = (LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth = times*w;
	pNewHead->biHeight = times*h;
	pNewHead->biSizeImage = times * times *lineByte*h;
	int width, height, nlineByte;
	width = times *w;
	height = times * h;
	nlineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < b; k++)
			{
				double row = i / times;
				double col = j / times;
				int lRow = floor(row);  //���Ͻǵ�
				int lCol = floor(col);	//���Ͻǵ�
				double v = row - lRow;
				double u = col - lCol;
				double A1, A2, A3, A4, C1, C2, C3, C4;
				A1 = 4 - 8 * fabs(u + 1) + 5 * fabs(u + 1) *fabs(u + 1) - fabs(u + 1)*fabs(u + 1)*fabs(u + 1);
				A2 = 1 - 2 * fabs(u) *fabs(u) + fabs(u)* fabs(u)*fabs(u);
				A3 = 1 - 2 * fabs(u - 1) *fabs(u - 1) + fabs(u - 1)* fabs(u - 1)*fabs(u - 1);
				A4 = 4 - 8 * fabs(u - 2) + 5 * fabs(u - 2) *fabs(u - 2) - fabs(u - 2)*fabs(u - 2)*fabs(u - 2);
				C1 = 4 - 8 * fabs(v + 1) + 5 * fabs(v + 1) *fabs(v + 1) - fabs(v + 1)*fabs(v + 1)*fabs(v + 1);
				C2 = 1 - 2 * fabs(v) *fabs(v) + fabs(v)* fabs(u)*fabs(v);
				C3 = 1 - 2 * fabs(v - 1) *fabs(v - 1) + fabs(v - 1)* fabs(v - 1)*fabs(v - 1);
				C4 = 4 - 8 * fabs(v - 2) + 5 * fabs(v - 2) *fabs(v - 2) - fabs(v - 2)*fabs(v - 2)*fabs(v - 2);
				int f[16];
				int count = 0;
				for (int t1 = -1; t1 < 3; t1++)
				{
					for (int t2 = -1; t2 < 3; t2++)
					{
						f[count] = *(lpDIBBits + (lRow + t1)*lineByte + (lCol + t2)*b + k);
						count++;
					}
				}
				int AB[4], ABC;
				AB[0] = A1 * f[0] + A2*f[4] + A3 *f[8] + A4*f[12];
				AB[1] = A1*f[1] + A2*f[5] + A3 *f[9] + A4 * f[13];
				AB[2] = A1 *f[2] + A2 * f[6] + A3*f[10] + A4*f[14];
				AB[3] = A1 *f[3] + A2 * f[7] + A3*f[11] + A4*f[15];
				ABC = AB[0] * C1 + AB[1] * C2 + AB[2] * C3 + AB[3] * C4;
				*(lpDIBBitsNew + i*nlineByte + j*b + k) = ABC;
			}
		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);

	Invalidate();
}

//�������˲�,��ֵ���ܿ죬��ֵԽ��Խ�������������ھ�ֵ
void CDemoView::OnAver3()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	int width = ::DIBWidth(lpDIB);
	int height = ::DIBHeight(lpDIB);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int biBitCount = phead->biBitCount/8;
	int b = biBitCount;
	int lineByte=(width * biBitCount+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	int  i, j, k, x, y;
	for(i=1;i<height-1;i++){
		    for(j=1;j<width-1;j++){          
		        for(k=0;k<b;k++){
		          x=*(lpDIBBits+(i-1)*lineByte+(j-1)*b+k)+*(lpDIBBits+(i-1)*lineByte+(j)*b+k)+*(lpDIBBits+(i-1)*lineByte+(j+1)*b+k)
		           +*(lpDIBBits+(i)*lineByte+(j-1)*b+k)+*(lpDIBBits+(i)*lineByte+(j)*b+k)+*(lpDIBBits+(i)*lineByte+(j+1)*b+k)
				   +*(lpDIBBits+(i+1)*lineByte+(j-1)*b+k)+*(lpDIBBits+(i+1)*lineByte+(j)*b+k)+*(lpDIBBits+(i+1)*lineByte+(j+1)*b+k);
		          y=x/9;
		          if (y < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = y;
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnAver5()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	int width = ::DIBWidth(lpDIB);
	int height = ::DIBHeight(lpDIB);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int biBitCount = phead->biBitCount/8;
	int b = biBitCount;
	int lineByte=(width * biBitCount+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	int  i, j, k, x, y,p,q;
	for(i=0;i<height-1;i++){
		    for(j=0;j<width-1;j++){          
		        for(k=0;k<b;k++)
				{
				  x = 0;
				  for(p = -2;p <=2;p++)
				  {
					  for(q = -2;q<=2;q++)
					  {
						if(p+i<=0 || p+i >=height||p+j<=0 ||p+j>=width)
						{
							break;
						}
						x = x+ *(lpDIBBitsNew+(i+p)*lineByte+(j+q)*b + k);
					  }
				  }
		          y=x/25;
		          if (y < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = y;
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnAver7()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	int width = ::DIBWidth(lpDIB);
	int height = ::DIBHeight(lpDIB);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int biBitCount = phead->biBitCount/8;
	int b = biBitCount;
	int lineByte=(width * biBitCount+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	int  i, j, k, x, y,p,q;
	for(i=0;i<height-1;i++){
		    for(j=0;j<width-1;j++){          
		        for(k=0;k<b;k++)
				{
				  x = 0;
				  for(p = -3;p <=3;p++)
				  {
					  for(q = -3;q<=3;q++)
					  {
						if(p+i<=0 || p+i >=height||p+j<=0 ||p+j>=width)
						{
							break;
						}
						x = x+ *(lpDIBBitsNew+(i+p)*lineByte+(j+q)*b + k);
					  }
				  }
		          y=x/49;
		          if (y < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = y;
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnMedian3()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	int width = ::DIBWidth(lpDIB);
	int height = ::DIBHeight(lpDIB);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int biBitCount = phead->biBitCount/8;
	int b = biBitCount;
	int lineByte=(width * biBitCount+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	unsigned char *buf = new unsigned char[9];
	int  i, j, k, x, y,p,q,t;
	for(i=0;i<height-1;i++){
		    for(j=0;j<width-1;j++){          
		        for(k=0;k<b;k++)
				{
				  t = 0;
				  for(p = -1;p <=1;p++)
				  {
					  for(q = -1;q<=1;q++)
					  {
						if(p+i<=0 || p+i >=height||p+j<=0 ||p+j>=width)
						{
							buf[t] =0;
							t++;
							break;
						}
						buf[t] =  *(lpDIBBitsNew+(i+p)*lineByte+(j+q)*b + k);
						t++;
					  }
				  }
				  int temp,x1,x2;
				  for(x2=0;x2<=8;x2++) 
				  { for (x1=0;x1<9-x2;x1++) 
				  if (buf[x1]>buf[x1+1]) 
				  { temp=buf[x1]; 
				  buf[x1]=buf[x1+1]; 
				  buf[x1+1]=temp;} 
				  } 
		          y = buf[4];
		          if (y < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = y;
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnMedian5()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	int width = ::DIBWidth(lpDIB);
	int height = ::DIBHeight(lpDIB);


	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int biBitCount = phead->biBitCount/8;
	int b = biBitCount;

	int lineByte=(width * biBitCount+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);

	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);

	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	unsigned char *buf = new unsigned char[25];
	int  i, j, k, x, y,p,q,t;
	for(i=0;i<height-1;i++){
		    for(j=0;j<width-1;j++){          
		        for(k=0;k<b;k++)
				{
				  t = 0;
				  for(p = -2;p <=2;p++)
				  {
					  for(q = -2;q<=2;q++)
					  {
						if(p+i<=0 || p+i >=height||p+j<=0 ||p+j>=width)
						{
							buf[t] =0;
							t++;
							break;
						}
						buf[t] =  *(lpDIBBitsNew+(i+p)*lineByte+(j+q)*b + k);
						t++;
					  }
				  }
				  int temp,x1,x2;
				  for(x2=0;x2<=24;x2++) 
				  { for (x1=0;x1<25-x2;x1++) 
				  if (buf[x1]>buf[x1+1]) 
				  { temp=buf[x1]; 
				  buf[x1]=buf[x1+1]; 
				  buf[x1+1]=temp;} 
				  } 
		          y = buf[12];
		          if (y < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = y;
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }

	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);

	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();

	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnMedian7()
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);  
	int width = ::DIBWidth(lpDIB);
	int height = ::DIBHeight(lpDIB);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int biBitCount = phead->biBitCount/8;
	int b = biBitCount;
	int lineByte=(width * biBitCount+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+height*lineByte);
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	unsigned char *buf = new unsigned char[49];
	int  i, j, k, x, y,p,q,t;
	for(i=0;i<height-1;i++){
		    for(j=0;j<width-1;j++){          
		        for(k=0;k<b;k++)
				{
				  t = 0;
				  for(p = -3;p <=3;p++)
				  {
					  for(q = -3;q<=3;q++)
					  {
						if(p+i<=0 || p+i >=height||p+j<=0 ||p+j>=width)
						{
							buf[t] =0;
							t++;
							break;
						}
						buf[t] =  *(lpDIBBitsNew+(i+p)*lineByte+(j+q)*b + k);
						t++;
					  }
				  }
				  int temp,x1,x2;
				  for(x2=0;x2<=48;x2++) 
				  { for (x1=0;x1<49-x2;x1++) 
				  if (buf[x1]>buf[x1+1]) 
				  { temp=buf[x1]; 
				  buf[x1]=buf[x1+1]; 
				  buf[x1+1]=temp;} 
				  } 
		          y = buf[24];
		          if (y < 255)
		          {
					  *(lpDIBBitsNew+i*lineByte+j*b + k) = y;
		          }
				  else
		            *(lpDIBBitsNew+i*lineByte+j*b + k)=255;
		        }
		    }
		  }
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

//������ֱ��ͼ�任
void CDemoView::OnHisteq()//ֱ��ͼ����
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b == 3) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	int bufLong = width;
	if (height>bufLong) bufLong = height;
	int hist[256] = { 0 };
	int i, j;
	if (b != 1)
	{
		return;
	}
	for (i = 0; i < height - 1; i++) {
		for (j = 0; j < width - 1; j++) {
			hist[*(lpDIBBits + i*lineByte + j)] = hist[*(lpDIBBits + i*lineByte + j)] + 1;
			 
		}
	}
	double  probPixel[256] = { 0 };
	for (i = 0; i < 256;i++)
	{
		probPixel[i] = hist[i] / (width * height*1.0);
	}
	double cumuPixel[256] = { 0 };
	for (i = 0; i < 256;i++)
	{
		if (i == 0)
		{
			cumuPixel[i] = probPixel[i];
		}
		else
		cumuPixel[i] = cumuPixel[i-1] + probPixel[i];
	}
	for (i = 0; i < 256; i++)
	{
		cumuPixel[i] = int(cumuPixel[i] *255 + 0.5);
	}
	for (i = 0; i < height - 1; i++) {
		for (j = 0; j < width - 1; j++) {
			*(lpDIBBitsNew + i *lineByte + j) = cumuPixel[*(lpDIBBits + i *lineByte + j)];		
		}
	}
	//----------------------------------------------------------
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

void CDemoView::OnStrech()//ֱ��ͼ����
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b == 3) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	int bufLong = width;
	if (height>bufLong) bufLong = height;
	int i,j,k,min[3], max[3];
	min[0] = *(lpDIBBits + + 0);
	min[1] = *(lpDIBBits + 1);
	min[2] = *(lpDIBBits + 2);
	max[0] = min[0];
	max[1] = min[1];
	max[2] = min[2];
	for (i = 0; i < height - 1; i++) {
		for (j = 0; j < width - 1; j++) {
			for (k = 0; k < b; k++)
			{
				if (*(lpDIBBits + i*lineByte + j*b + k)<min[k])
				{
					min[k] = *(lpDIBBits + i*lineByte + j*b + k);
				}
				if (*(lpDIBBits + i*lineByte + j*b + k) >max[k])
				{
					max[k] = *(lpDIBBits + i*lineByte + j*b + k);
				}
			}
		}
	}
	for (i = 0; i < height - 1; i++) {
		for (j = 0; j < width - 1; j++) {
			for (k = 0; k < b; k++)
			{
				*(lpDIBBitsNew + i*lineByte + j*b + k) = double((*(lpDIBBits + i*lineByte + j*b + k) - min[k])) / (max[k] - min[k]) * 255;
			}
		}
	}
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

//�����ֵ
void CDemoView::OnOtsu()//�����ֵ�ָ�
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b == 3) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	float histogram[256] = { 0.0 };
	unsigned char p;
	for (int i = 0; i < height - 1;i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			p = *(lpDIBBits + i *lineByte + j);
			histogram[p] ++;
		}
	}
	float avgValue=0;
	for (int i = 0; i < 256;i++)
	{
		histogram[i] = histogram[i] / (height * width);
		avgValue = avgValue + histogram[i] * i;
	}
	int threshold = 0;
	float gmax = 0;
	float wk = 0, uk = 0;
	for (int i = 0; i < 256;i++)
	{
		wk += histogram[i];
		uk = uk + i*histogram[i];
		float ut = avgValue*wk - uk;
		float g = ut*ut / (wk*(1 - wk));
		if (g >gmax)
		{
			gmax = g;
			threshold = i;
		}
	}
	for (int i = 0; i < height - 1; i++)
	{
		for (int j = 0; j < width - 1; j++)
		{
			
			if (*(lpDIBBits + i *lineByte + j) < threshold)
			{
				*(lpDIBBitsNew + i *lineByte + j) = 0;
			}
			else
				*(lpDIBBitsNew + i *lineByte + j) = 255;	
		}
	}
	//-------------------------------------------------------
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

//ͼ��У��
void CDemoView::OnCorrect()//��ȷ�ǽ����˵����е�ֵֹ�
{
	CDemoDoc *pDoc=GetDocument();
	HDIB dib=pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) dib);
	LPBITMAPINFOHEADER phead=(LPBITMAPINFOHEADER)lpDIB;
	int w = phead->biWidth;
	int h = phead->biHeight;
	int b = phead->biBitCount/8;
	int lineByte=(w * b+3)/4*4;
	unsigned char *lpDIBBits=(unsigned char *)::FindDIBBits(lpDIB);
	int palSize=::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew=::GlobalAlloc(GHND,sizeof(BITMAPINFOHEADER)+palSize+818*940*3);
	LPSTR lpDIBNew=(LPSTR)::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER)+ palSize+h*lineByte);
	LPBITMAPINFOHEADER	pNewHead=(LPBITMAPINFOHEADER)lpDIBNew;
	pNewHead->biWidth =940;
	pNewHead->biHeight =818;
	pNewHead->biSizeImage =818*940*3;
	int lineByteN=(940 * b+3)/4*4;
	unsigned char *lpDIBBitsNew=(unsigned char *)::FindDIBBits(lpDIBNew);
	int  i, j, x, y;

	for(i=0;i<818;i++){
		for(j=0;j<940;j++){
			*(lpDIBBitsNew+i*lineByteN+j*b)=255;
			*(lpDIBBitsNew+i*lineByteN+j*b+1)=255;
			*(lpDIBBitsNew+i*lineByteN+j*b+2)=255;
		}
	}
	for(i=0;i<h;i++){
		for(j=0;j<w;j++){
			x=255.2178-0.9662*i+0.6192*j;
			y=839.0042-0.5830*i-0.4522*j;
			if((660-x)>=0&&(660-x)<940&&y>=0&&y<818){
				*(lpDIBBitsNew+y*lineByteN+(660-x)*b)=*(lpDIBBits+i*lineByte+j*b)-255;
				*(lpDIBBitsNew+y*lineByteN+(660-x)*b+1)=*(lpDIBBits+i*lineByte+j*b+1)-255;
				*(lpDIBBitsNew+y*lineByteN+(660-x)*b+2)=*(lpDIBBits+i*lineByte+j*b+2)-255;
			}

		}
	}
	::GlobalUnlock(dib); 
	::GlobalUnlock(dibNew); 
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView=(CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew=pView->GetDocument();
	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	//pDocNew->SetModifiedFlag(TRUE);
	Invalidate();
}

//playground
void CDemoView::OnGray()//���ֲ�ɫ��ʽ���ǿ���ȥ���ɫһ��������д������
{
	CDemoDoc *pDoc = GetDocument();
	HDIB dib = pDoc->GetHDIB();
	if (dib == NULL) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	LPSTR	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)dib);
	LPBITMAPINFOHEADER phead = (LPBITMAPINFOHEADER)lpDIB;
	int width = phead->biWidth;
	int height = phead->biHeight;
	int b = phead->biBitCount / 8;
	if (b != 3) {
		::MessageBox(0, "ֻ�����ɫͼ��", MB_OK, 0);
		::GlobalUnlock(dib);
		return;
	}
	int lineByte = (width * b + 3) / 4 * 4;
	unsigned char *lpDIBBits = (unsigned char *)::FindDIBBits(lpDIB);
	int palSize = ::PaletteSize((LPSTR)lpDIB);
	HANDLE dibNew = ::GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	LPSTR lpDIBNew = (LPSTR) ::GlobalLock(dibNew);
	::memcpy((unsigned char*)lpDIBNew, (unsigned char*)lpDIB, sizeof(BITMAPINFOHEADER) + palSize + height * lineByte);
	unsigned char *lpDIBBitsNew = (unsigned char *)::FindDIBBits(lpDIBNew);
	//---------------------������ͨ��Ԥ������
	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			*(lpDIBBitsNew+i*lineByte+j*b +0)=0.11**(lpDIBBits+i*lineByte+j*b+0)
				+0.59**(lpDIBBits+i*lineByte+j*b+1)
				+0.30**(lpDIBBits+i*lineByte+j*b+2)+0.5;
			*(lpDIBBitsNew+i*lineByte+j*b +1)=0.11**(lpDIBBits+i*lineByte+j*b+0)
				+0.59**(lpDIBBits+i*lineByte+j*b+1)
				+0.30**(lpDIBBits+i*lineByte+j*b+2)+0.5;
			*(lpDIBBitsNew+i*lineByte+j*b +2)=0.11**(lpDIBBits+i*lineByte+j*b+0)
				+0.59**(lpDIBBits+i*lineByte+j*b+1)
				+0.30**(lpDIBBits+i*lineByte+j*b+2)+0.5;
		}
	}
	
	::GlobalUnlock(dib);
	::GlobalUnlock(dibNew);

	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);

	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();

	pDocNew->ReplaceHDIB((HDIB)dibNew);
	pDocNew->InitDIBData();
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////
//�����ǽ���͵��ã�ע��OnDraw1��OnInitialUpdate1��OnSaveDocument1��OnOpenDocument1

void CDemoView::OnColortogray()
{
	//��ȡ�ĵ���ָ��
	CDemoDoc *pDoc = GetDocument();
	//��ȡImgCenterDib�����m_dib��ָ�룬���ʵ�ǰDIB����
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	if (b != 24) {
		::MessageBox(0, "ֻ�����ɫͼ��", MB_OK, 0);
		return;
	}
	//��pDib�е�ͼ��������Ϊ�������ݣ����ô������Ĺ��캯����
	//����GrayTrans��Ķ���graytrans
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//��ɫ��Ҷȸ�ʽ
	graytrans.ColorToGray();
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);//����һ������ͼ����ʾ�ָ���
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);//�����½��ļ�����Ϣ������һ���µ��ĵ�-��ͼ
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();//��ȡ�½���ͼָ��
	CDemoDoc* pDocNew = pView->GetDocument();//��ȡ��������µ��ĵ���ָ��
	//��ȡ���ĵ��е�ImgCenterDib�����ָ��
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	//���任������ͼ����Ϊ�½��ĵ���DIB������ʾ
	dibNew->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	//::MessageBox(0, "���ڽӿ����⣬�˻Ҷ�ͼƬ�Ѿ�ͬdemoԭ�ر���Ϊhuidu.dmp\n��Ҫ�ô˽��������һ��������ǰ����,������ַ����", MB_OK, 0);
	//���ù�������
	pView->OnInitialUpdate1();
	//ipdib shikongde
    //����ͼˢ����ʾ
	pDocNew->UpdateAllViews(pView);
	Invalidate();
}

void CDemoView::OnTophatvally()
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}

	//������̬ѧ��Ķ���morph�����õ�ǰDIBΪ���ʼ��
	Morphology morph(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

		//����ģ��
		int M[5][5] = { {0, 0, 0 ,0, 0},
						{0, 0 ,1, 0 ,0}, 
						{0, 1, 1 ,1 ,0}, 
						{0 ,0 ,1 ,0 ,0} ,
						{0 ,0, 0 ,0 ,0} };
		
		morph.InputMask(*M, 5, 5);

		//���ȼ��
		morph.GrayTopHatVally();

		//�½���ͼ����ʾ������
		CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
		pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
		CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
		CDemoDoc* pDocNew = pView->GetDocument();
		ImgCenterDib *dibNew = pDocNew->GetPDib();
		dibNew->ReplaceDib(morph.GetDimensions(), morph.m_nBitCountOut, morph.m_lpColorTableOut, morph.m_pImgDataOut);
		pView->OnInitialUpdate1();
		//pDocNew->SetModifiedFlag(TRUE);
		pDocNew->UpdateAllViews(pView);

}



void CDemoView::OnBinary1()
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}
	//__________________________________________________
	//��ֱ��ͼ�������ͳ��ֱ��ͼ
	//����ֱ��ͼ��Ķ���hist�������õ�ǰDIB���ݶ�hist��ʼ��
	Histogram hist(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//ͳ��ֱ��ͼ
	hist.computeHistGray();
	//�÷ָ���������ֱ��ͼ��1%ͳ�Ʒָ�ֵ
	ImgSegment segment;
	int thresh = segment.calculate(hist.m_histArray);

	//1%ͳ�Ʒָ�ֵ��ֵ���ָ�Ĳ�������ͼ���ֵ���ָ�	
	//����GrayTrans��Ķ���graytrans�����õ�ǰDIB���ݶ�graytrans��ʼ��
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	graytrans.Binary(thresh);

	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate1();
	//pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}



void CDemoView::OnHough()
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}

	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i < size.cy; i++) {
		for (j = 0; j < size.cx; j++) {
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if (!flag) {
		::MessageBox(0, "������ֵ�ָ�", MB_OK, 0);
		return;
	}

	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//���뾶���ֱ��ʺͽǶȼ��ֱ���
	float radiusResolution = 1;
	float angleResolution = 1;
	float r, a;

	//Hough�任
	segment.Hough(radiusResolution, angleResolution, &r, &a);

	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate1();
	//pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}


void CDemoView::OnHoughline()
{
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}

	//�ж��Ƿ��ֵͼ��
	int i, j;
	CSize size = pDib->GetDimensions();
	int lineByte = (size.cx + 3) / 4 * 4;
	int flag = 1;
	for (i = 0; i < size.cy; i++) {
		for (j = 0; j < size.cx; j++) {
			//���ͼ����������ػҶ�ֵ�Ȳ�Ϊ0Ҳ��Ϊ255,��Ƕ�ֵͼ��
			if (*(pDib->m_pImgData + i * lineByte + j) != 0
				&& *(pDib->m_pImgData + i * lineByte + j) != 255) {
				flag = 0;
				break;
			}
		}
	}
	//�Ƕ�ֵͼ��
	if (!flag) {
		::MessageBox(0, "������ֵ�ָ�", MB_OK, 0);
		return;
	}

	//����ָ������segment�����õ�ǰDIB���ݳ�ʼ��
	ImgSegment segment(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);

	//���뾶���ֱ��ʺͽǶȼ��ֱ���
	float radiusResolution = 1;
	float angleResolution = 1;

	//������
	segment.longestLineDetectByHough(radiusResolution, angleResolution);

	//�½���ͼ����ʾ������
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	dibNew->ReplaceDib(segment.GetDimensions(), segment.m_nBitCountOut, segment.m_lpColorTableOut, segment.m_pImgDataOut);
	pView->OnInitialUpdate1();
	//pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnTakeline()
{
	//��������������ֻ����ֱ�ߣ�ʣ�²��ֶ�ȥ��
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}
	//��ֱ��ͼ�������ͳ��ֱ��ͼ
	//����ֱ��ͼ��Ķ���hist�������õ�ǰDIB���ݶ�hist��ʼ��
	Histogram hist(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//ͳ��ֱ��ͼ
	hist.computeHistGray();
	//�÷ָ���������ֱ��ͼ��1%ͳ�Ʒָ�ֵ
	ImgSegment segment;
	int thresh = segment.calculate(hist.m_histArray);

	//1%ͳ�Ʒָ�ֵ��ֵ���ָ�Ĳ�������ͼ���ֵ���ָ�	
	//����GrayTrans��Ķ���graytrans�����õ�ǰDIB���ݶ�graytrans��ʼ��
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	graytrans.Binaryforline(thresh);//��ʱ��Ҫȥ���Ĳ����Ǻ�ɫ��0������Ҫȥ���Ĳ����ǰ�ɫ��255���������200���ص���ͨ��
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate1();
	//pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
}

void CDemoView::OnRepairline()
{
	//��������������ֻ����ֱ�ߣ�ʣ�²��ֶ�ȥ��
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}
	//��ֱ��ͼ�������ͳ��ֱ��ͼ
	//����ֱ��ͼ��Ķ���hist�������õ�ǰDIB���ݶ�hist��ʼ��
	Histogram hist(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//ͳ��ֱ��ͼ
	hist.computeHistGray();
	//�÷ָ���������ֱ��ͼ��1%ͳ�Ʒָ�ֵ
	ImgSegment segment;
	int thresh = segment.calculate(hist.m_histArray);

	//1%ͳ�Ʒָ�ֵ��ֵ���ָ�Ĳ�������ͼ���ֵ���ָ�	
	//����GrayTrans��Ķ���graytrans�����õ�ǰDIB���ݶ�graytrans��ʼ��
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	graytrans.Clearlineave(thresh);//��ʱ��Ҫȥ���Ĳ����Ǻ�ɫ��0������Ҫȥ���Ĳ����ǰ�ɫ��255���������200���ص���ͨ��
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate1();
	//pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);
	
}

void CDemoView::OnRepairline2()
{
	//��������������ֻ����ֱ�ߣ�ʣ�²��ֶ�ȥ��
	//��ȡ�ĵ�����m_dib��ָ�룬���ʵ�ǰDIB����
	CDemoDoc *pDoc = GetDocument();
	ImgCenterDib *pDib = pDoc->GetPDib();
	int b = pDib->m_nBitCount;
	if ((b != 24 && b != 8) || b == 0) {
		::MessageBox(0, "��Ҫ�����������ȱ��浱ǰ���", MB_OK, 0);
		return;
	}
	//ֻ����Ҷ�ͼ��
	if (pDib->m_nBitCount != 8) {
		::MessageBox(0, "ֻ����Ҷ�ͼ��", MB_OK, 0);
		return;
	}
	//��ֱ��ͼ�������ͳ��ֱ��ͼ
	//����ֱ��ͼ��Ķ���hist�������õ�ǰDIB���ݶ�hist��ʼ��
	Histogram hist(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	//ͳ��ֱ��ͼ
	hist.computeHistGray();
	//�÷ָ���������ֱ��ͼ��1%ͳ�Ʒָ�ֵ
	ImgSegment segment;
	int thresh = segment.calculate(hist.m_histArray);

	//1%ͳ�Ʒָ�ֵ��ֵ���ָ�Ĳ�������ͼ���ֵ���ָ�	
	//����GrayTrans��Ķ���graytrans�����õ�ǰDIB���ݶ�graytrans��ʼ��
	GrayTrans graytrans(pDib->GetDimensions(), pDib->m_nBitCount,
		pDib->m_lpColorTable, pDib->m_pImgData);
	graytrans.Clearlinemid(thresh);//��ʱ��Ҫȥ���Ĳ����Ǻ�ɫ��0������Ҫȥ���Ĳ����ǰ�ɫ��255���������200���ص���ͨ��
	//�½���ͼ����ʾ�ָ���
	CMainFrame* pFrame = (CMainFrame *)(AfxGetApp()->m_pMainWnd);
	pFrame->SendMessage(WM_COMMAND, ID_FILE_NEW);
	CDemoView* pView = (CDemoView*)pFrame->MDIGetActive()->GetActiveView();
	CDemoDoc* pDocNew = pView->GetDocument();
	ImgCenterDib *dibNew = pDocNew->GetPDib();
	dibNew->ReplaceDib(graytrans.GetDimensions(), graytrans.m_nBitCountOut, graytrans.m_lpColorTableOut, graytrans.m_pImgDataOut);
	pView->OnInitialUpdate1();
	//pDocNew->SetModifiedFlag(TRUE);
	pDocNew->UpdateAllViews(pView);

}




