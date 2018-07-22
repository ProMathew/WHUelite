
// SDIOpenGLView.h : CSDIOpenGLView ��Ľӿ�
//

#pragma once


class CSDIOpenGLView : public CView
{
protected: // �������л�����
	CSDIOpenGLView();
	DECLARE_DYNCREATE(CSDIOpenGLView)

// ����
public:
	CSDIOpenGLDoc* GetDocument() const;

// ����
public:

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
	virtual ~CSDIOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL RenderScene(void);
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL InitializeOpenGL(CDC* pDC);
private:
	HGLRC m_hRC;			//OpenGL ����������
	HPALETTE m_hPalette;	//OpenGL ��ɫ��
	CDC*	m_pDC;			//OpenGL �豸������
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	bool m_bLDown;
	CPoint m_PtDown;
	float m_xRotate;
	float m_yRotate;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
private:
	float m_fScale;
public:
	bool m_bShowMode;
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnShowChange();
	afx_msg void OnShowView();
};

#ifndef _DEBUG  // SDIOpenGLView.cpp �еĵ��԰汾
inline CSDIOpenGLDoc* CSDIOpenGLView::GetDocument() const
   { return reinterpret_cast<CSDIOpenGLDoc*>(m_pDocument); }
#endif

