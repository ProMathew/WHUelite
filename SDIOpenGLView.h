
// SDIOpenGLView.h : CSDIOpenGLView 类的接口
//

#pragma once


class CSDIOpenGLView : public CView
{
protected: // 仅从序列化创建
	CSDIOpenGLView();
	DECLARE_DYNCREATE(CSDIOpenGLView)

// 特性
public:
	CSDIOpenGLDoc* GetDocument() const;

// 操作
public:

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
	virtual ~CSDIOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	BOOL RenderScene(void);
	BOOL SetupPixelFormat(void);
	void SetLogicalPalette(void);
	BOOL InitializeOpenGL(CDC* pDC);
private:
	HGLRC m_hRC;			//OpenGL 绘制描述表
	HPALETTE m_hPalette;	//OpenGL 调色板
	CDC*	m_pDC;			//OpenGL 设备描述表
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

#ifndef _DEBUG  // SDIOpenGLView.cpp 中的调试版本
inline CSDIOpenGLDoc* CSDIOpenGLView::GetDocument() const
   { return reinterpret_cast<CSDIOpenGLDoc*>(m_pDocument); }
#endif

