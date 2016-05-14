
// d2dgdiView.h : Cd2dgdiView ��Ľӿ�
//

#pragma once

#include "Graphics.h"

class Cd2dgdiView : public CView
{
protected: // �������л�����
	Cd2dgdiView();
	DECLARE_DYNCREATE(Cd2dgdiView)

// ����
public:
	Cd2dgdiDoc* GetDocument() const;

// ����
public:
	void OnGraphicsDraw(Utility::CGraphics& gs);

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cd2dgdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnAfxWmDraw2d(WPARAM wParam, LPARAM lParam);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // d2dgdiView.cpp �еĵ��԰汾
inline Cd2dgdiDoc* Cd2dgdiView::GetDocument() const
   { return reinterpret_cast<Cd2dgdiDoc*>(m_pDocument); }
#endif

