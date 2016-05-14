
// d2dgdiView.h : Cd2dgdiView 类的接口
//

#pragma once

#include "Graphics.h"

class Cd2dgdiView : public CView
{
protected: // 仅从序列化创建
	Cd2dgdiView();
	DECLARE_DYNCREATE(Cd2dgdiView)

// 特性
public:
	Cd2dgdiDoc* GetDocument() const;

// 操作
public:
	void OnGraphicsDraw(Utility::CGraphics& gs);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cd2dgdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // d2dgdiView.cpp 中的调试版本
inline Cd2dgdiDoc* Cd2dgdiView::GetDocument() const
   { return reinterpret_cast<Cd2dgdiDoc*>(m_pDocument); }
#endif

