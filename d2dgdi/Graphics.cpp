#include "stdafx.h"
#include "Graphics.h"

namespace Utility
{
	class CDirect2DGraphicsContext
		:public LowLevelGraphicsContext
	{
	public:
		CDirect2DGraphicsContext(unsigned long pRenderTarget)
			:m_pRenderTarget((CHwndRenderTarget*)pRenderTarget)
		{
			ASSERT_VALID(m_pRenderTarget);
			m_pTextBrush = new CD2DSolidColorBrush(m_pRenderTarget, D2D1::ColorF(Gdiplus::Color::White));
			m_pFillBrush = new CD2DSolidColorBrush(m_pRenderTarget, D2D1::ColorF(Gdiplus::Color::White));
			m_pLineBrush = new CD2DSolidColorBrush(m_pRenderTarget, D2D1::ColorF(Gdiplus::Color::Black));
			m_pTextFormat = new CD2DTextFormat(m_pRenderTarget, _T("Arial"), 24);
			m_pTextFormat->Get()->SetTextAlignment(m_align);
			m_pTextFormat->Get()->SetParagraphAlignment(m_lineAlign);
		}

	public:
		virtual ~CDirect2DGraphicsContext()
		{
			if (nullptr != m_pTextBrush)
			{
				delete m_pTextBrush;
				m_pTextBrush = nullptr;
			}

			if (nullptr != m_pFillBrush)
			{
				delete m_pFillBrush;
				m_pFillBrush = nullptr;
			}

			if (nullptr != m_pLineBrush)
			{
				delete m_pLineBrush;
				m_pLineBrush = nullptr;
			}

			if (nullptr != m_pTextFormat)
			{
				delete m_pTextFormat;
				m_pTextFormat = nullptr;
			}
		}

	public:
		virtual void SetTextColor(unsigned long color)
		{
			m_pTextBrush->SetColor(D2D1::ColorF(color));
		}


		virtual void SetFillColor(unsigned long color)
		{
			m_pFillBrush->SetColor(D2D1::ColorF(color));
		}

		virtual void SetLineColor(unsigned long color)
		{
			m_pLineBrush->SetColor(D2D1::ColorF(color));
		}

		virtual void SetLineWidth(float width)
		{
			m_lineWidth = width;
		}

		virtual void SetFont(const CString& szFontFamilyName, float fontSize)
		{
			if (nullptr != m_pTextFormat)
			{
				delete m_pTextFormat;
				m_pTextFormat = nullptr;
			}

			m_pTextFormat = new CD2DTextFormat(m_pRenderTarget, szFontFamilyName, fontSize);
			m_pTextFormat->Get()->SetTextAlignment(m_align);
			m_pTextFormat->Get()->SetParagraphAlignment(m_lineAlign);
		}

		virtual void SetAlignment(StringAlignment align)
		{
			switch (align)
			{
			case Utility::StringAlignmentNear:
				m_align = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
				break;
			case Utility::StringAlignmentCenter:
				m_align = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER;
				break;
			case Utility::StringAlignmentFar:
				m_align = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_TRAILING;
				break;
			default:
				break;
			}

			m_pTextFormat->Get()->SetTextAlignment(m_align);
		}

		virtual void SetLineAlignment(StringAlignment align)
		{
			switch (align)
			{
			case Utility::StringAlignmentNear:
				m_lineAlign = DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
				break;
			case Utility::StringAlignmentCenter:
				m_lineAlign = DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER;
				break;
			case Utility::StringAlignmentFar:
				m_lineAlign = DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_FAR;
				break;
			default:
				break;
			}

			m_pTextFormat->Get()->SetParagraphAlignment(m_lineAlign);
		}

	public:
		virtual void DrawString(const CString& szText, float x, float y, float width, float height)
		{
			m_pRenderTarget->DrawText(szText, CD2DRectF(x, y, x + width, y + height), m_pTextBrush, m_pTextFormat);
		}

		virtual void DrawRectangle(float x, float y, float width, float height)
		{
			m_pRenderTarget->DrawRectangle(CD2DRectF(x, y, x + width, y + height), m_pLineBrush, m_lineWidth);
		}

		virtual void DrawLine(float x1, float y1, float x2, float y2)
		{
			m_pRenderTarget->DrawLine(CD2DPointF(x1, y1), CD2DPointF(x2, y2), m_pLineBrush, m_lineWidth);
		}

		virtual void DrawEllipse(float x, float y, float width, float height)
		{
			m_pRenderTarget->DrawEllipse(CD2DEllipse(CD2DRectF(x, y, x + width, y + height)), m_pLineBrush, m_lineWidth);
		}

		virtual void FillRectangle(float x, float y, float width, float height)
		{
			m_pRenderTarget->FillRectangle(CD2DRectF(x, y, x + width, y + height), m_pFillBrush);
		}

		virtual void FillEllipse(float x, float y, float width, float height)
		{
			m_pRenderTarget->FillEllipse(CD2DEllipse(CD2DRectF(x, y, x + width, y + height)), m_pFillBrush);
		}

		virtual void Clear(unsigned long color)
		{
			m_pRenderTarget->Clear(D2D1::ColorF(color));
		}

	private:
		CHwndRenderTarget * m_pRenderTarget = nullptr;
		CD2DTextFormat* m_pTextFormat = nullptr;
		CD2DSolidColorBrush* m_pTextBrush = nullptr;
		CD2DSolidColorBrush* m_pFillBrush = nullptr;
		CD2DSolidColorBrush* m_pLineBrush = nullptr;
		float m_lineWidth = 1.0f;
		DWRITE_TEXT_ALIGNMENT m_align = DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING;
		DWRITE_PARAGRAPH_ALIGNMENT m_lineAlign = DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	};

	class CGdiplusGraphicsContext
		:public LowLevelGraphicsContext
	{
	public:
		CGdiplusGraphicsContext(unsigned long pRenderTarget)
			:m_gs((HDC)pRenderTarget),
			m_textBrush(Gdiplus::Color::White),
			m_fillBrush(Gdiplus::Color::White),
			m_linePen(Gdiplus::Color::White)
		{
			Gdiplus::FontFamily  fontFamily(_T("Arial"));
			m_pFont = new Gdiplus::Font(&fontFamily, 24, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		}

	public:
		virtual ~CGdiplusGraphicsContext()
		{
			if (nullptr != m_pFont)
			{
				delete m_pFont;
				m_pFont = nullptr;
			}
		}

	public:
		virtual void SetTextColor(unsigned long color)
		{
			m_textBrush.SetColor(color);
		}


		virtual void SetFillColor(unsigned long color)
		{
			m_fillBrush.SetColor(color);
		}

		virtual void SetLineColor(unsigned long color)
		{
			m_linePen.SetColor(color);
		}

		virtual void SetLineWidth(float width)
		{
			m_linePen.SetWidth(width);
		}

		virtual void SetFont(const CString& szFontFamilyName, float fontSize)
		{
			if (nullptr != m_pFont)
			{
				delete m_pFont;
				m_pFont = nullptr;
			}

			Gdiplus::FontFamily  fontFamily(szFontFamilyName);
			m_pFont = new Gdiplus::Font(&fontFamily, fontSize, Gdiplus::FontStyleRegular, Gdiplus::UnitPixel);
		}

		virtual void SetAlignment(StringAlignment align)
		{
			m_sFormat.SetAlignment((Gdiplus::StringAlignment)align);
		}

		virtual void SetLineAlignment(StringAlignment align)
		{
			m_sFormat.SetLineAlignment((Gdiplus::StringAlignment)align);
		}

	public:
		virtual void DrawString(const CString& szText, float x, float y, float width, float height)
		{
			m_gs.DrawString(szText.GetString(), -1, m_pFont, Gdiplus::RectF(x, y, width, height), &m_sFormat, &m_textBrush);
		}

		virtual void DrawRectangle(float x, float y, float width, float height)
		{
			m_gs.DrawRectangle(&m_linePen, x, y, width, height);
		}

		virtual void DrawLine(float x1, float y1, float x2, float y2)
		{
			m_gs.DrawLine(&m_linePen, x1, y1, x2, y2);
		}

		virtual void DrawEllipse(float x, float y, float width, float height)
		{
			m_gs.DrawEllipse(&m_linePen, x, y, width, height);
		}

		virtual void FillRectangle(float x, float y, float width, float height)
		{
			m_gs.FillRectangle(&m_fillBrush, x, y, width, height);
		}

		virtual void FillEllipse(float x, float y, float width, float height)
		{
			m_gs.FillEllipse(&m_fillBrush, x, y, width, height);
		}

		virtual void Clear(unsigned long color)
		{
			m_gs.Clear(color);
		}

	private:
		Gdiplus::Graphics m_gs;
		Gdiplus::SolidBrush m_textBrush;
		Gdiplus::SolidBrush m_fillBrush;
		Gdiplus::Pen m_linePen;
		Gdiplus::Font * m_pFont = nullptr;
		Gdiplus::StringFormat m_sFormat;
	};

	CGraphics::CGraphics(unsigned long pRenderTarget, RenderType renderType)
	{
		m_renderType = renderType;
		switch (renderType)
		{
		case Utility::Gdiplus:
			m_context = new CGdiplusGraphicsContext(pRenderTarget);
			break;
		case Utility::Direct2D:
			m_context = new CDirect2DGraphicsContext(pRenderTarget);
			break;
		default:
			break;
		}
	}

	CGraphics::~CGraphics()
	{
		if (nullptr != m_context)
		{
			delete m_context;
			m_context = nullptr;
		}
	}

	void CGraphics::SetTextColor(unsigned long color)
	{
		m_context->SetTextColor(color);
	}

	void CGraphics::SetFillColor(unsigned long color)
	{
		m_context->SetFillColor(color);
	}

	void CGraphics::SetLineColor(unsigned long color)
	{
		m_context->SetLineColor(color);
	}

	void CGraphics::SetLineWidth(float width)
	{
		m_context->SetLineWidth(width);
	}

	void CGraphics::SetFont(const CString& szFontFamilyName, float fontSize)
	{
		m_context->SetFont(szFontFamilyName, fontSize);
	}

	void CGraphics::SetAlignment(StringAlignment align)
	{
		m_context->SetAlignment(align);
	}

	void CGraphics::SetLineAlignment(StringAlignment align)
	{
		m_context->SetLineAlignment(align);
	}

	void CGraphics::DrawString(const CString& szText, float x, float y, float width, float height)
	{
		m_context->DrawString(szText, x, y, width, height);
	}

	void CGraphics::DrawRectangle(float x, float y, float width, float height)
	{
		m_context->DrawRectangle(x, y, width, height);
	}

	void CGraphics::DrawLine(float x1, float y1, float x2, float y2)
	{
		m_context->DrawLine(x1, y1, x2, y2);
	}

	void CGraphics::DrawEllipse(float x, float y, float width, float height)
	{
		m_context->DrawEllipse(x, y, width, height);
	}

	void CGraphics::FillRectangle(float x, float y, float width, float height)
	{
		m_context->FillRectangle(x, y, width, height);
	}

	void CGraphics::FillEllipse(float x, float y, float width, float height)
	{
		m_context->FillEllipse(x, y, width, height);
	}

	void CGraphics::Clear(unsigned long color)
	{
		m_context->Clear(color);
	}
}

