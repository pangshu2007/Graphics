
// d2dgdiView.cpp : Cd2dgdiView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "d2dgdi.h"
#endif

#include "d2dgdiDoc.h"
#include "d2dgdiView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cd2dgdiView

IMPLEMENT_DYNCREATE(Cd2dgdiView, CView)

BEGIN_MESSAGE_MAP(Cd2dgdiView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cd2dgdiView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_REGISTERED_MESSAGE(AFX_WM_DRAW2D, &Cd2dgdiView::OnAfxWmDraw2d)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CREATE()
END_MESSAGE_MAP()

// Cd2dgdiView ����/����

Cd2dgdiView::Cd2dgdiView()
{
	// Enable D2D support for this window:
	EnableD2DSupport();
}

Cd2dgdiView::~Cd2dgdiView()
{
}

BOOL Cd2dgdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cd2dgdiView ����

void Cd2dgdiView::OnDraw(CDC* pDC)
{
	Cd2dgdiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Utility::CGraphics gs((unsigned long)pDC->GetSafeHdc(), Utility::Gdiplus);
	OnGraphicsDraw(gs);
}


// Cd2dgdiView ��ӡ


void Cd2dgdiView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cd2dgdiView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cd2dgdiView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cd2dgdiView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}

void Cd2dgdiView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cd2dgdiView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cd2dgdiView ���

#ifdef _DEBUG
void Cd2dgdiView::AssertValid() const
{
	CView::AssertValid();
}

void Cd2dgdiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cd2dgdiDoc* Cd2dgdiView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cd2dgdiDoc)));
	return (Cd2dgdiDoc*)m_pDocument;
}
#endif //_DEBUG


// Cd2dgdiView ��Ϣ�������


afx_msg LRESULT Cd2dgdiView::OnAfxWmDraw2d(WPARAM wParam, LPARAM lParam)
{
	Utility::CGraphics gs((unsigned long)lParam, Utility::RenderType::Direct2D);
	OnGraphicsDraw(gs);

	//CHwndRenderTarget* pRenderTarget = (CHwndRenderTarget*)lParam;
	


	return TRUE;
}

void Cd2dgdiView::OnGraphicsDraw(Utility::CGraphics& gs)
{
	gs.Clear(Utility::Color::White);

	CRect rc;
	GetClientRect(&rc);

	int row = 10;
	int col = 15;
	float width = (float)rc.Width() / (float)col;
	float height = (float)rc.Height() / (float)row;
	// ��ʾ���ֺͱ���
	CString str;
	CString szFormat;
	szFormat.Format(_T("%%0.%df"), 0);

	gs.SetFont(_T("Arial"), 50);
	gs.SetAlignment(Utility::StringAlignment::StringAlignmentCenter);
	gs.SetLineAlignment(Utility::StringAlignment::StringAlignmentCenter);

	static int data = 0;
	data++;

	for (int i = 0; i<row; i++)
	{
		Gdiplus::RectF rcf((Gdiplus::REAL)rc.left, (Gdiplus::REAL)rc.top + i*height, width, height);
		for (int j = 0; j<col; j++)
		{
			float showData = data + i + j;

			auto color = ((unsigned long)showData) & 0xFF;
			gs.SetFillColor(Utility::CGraphics::MakeColor(0xFF, color, color, color));
			gs.FillRectangle(rcf.X, rcf.Y, rcf.Width, rcf.Height);

			str.Format(szFormat, showData);
			color = 0xFF - color;
			gs.SetTextColor(Utility::CGraphics::MakeColor(0xFF, color, color, color));
			gs.DrawString(str.GetString(), rcf.X, rcf.Y, rcf.Width, rcf.Height);
			rcf.X += width;
		}
	}
}



BOOL Cd2dgdiView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void Cd2dgdiView::OnTimer(UINT_PTR nIDEvent)
{
	Invalidate(FALSE);

	CView::OnTimer(nIDEvent);
}


int Cd2dgdiView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(1, 1000 / 30, NULL);

	return 0;
}
