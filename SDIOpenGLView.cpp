
// SDIOpenGLView.cpp : CSDIOpenGLView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "SDIOpenGL.h"
#endif

#include "SDIOpenGLDoc.h"
#include "SDIOpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSDIOpenGLView

IMPLEMENT_DYNCREATE(CSDIOpenGLView, CView)

BEGIN_MESSAGE_MAP(CSDIOpenGLView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_SHOW_CHANGE, &CSDIOpenGLView::OnShowChange)
	ON_COMMAND(ID_SHOW_VIEW, &CSDIOpenGLView::OnShowView)
END_MESSAGE_MAP()

// CSDIOpenGLView 构造/析构

CSDIOpenGLView::CSDIOpenGLView()
	: m_bLDown(false)
	, m_xRotate(0.f)
	, m_yRotate(0.f)
	, m_fScale(1.f)
	, m_bShowMode(false)
{
	// TODO: 在此处添加构造代码

}

CSDIOpenGLView::~CSDIOpenGLView()
{
}

BOOL CSDIOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

// CSDIOpenGLView 绘制

void CSDIOpenGLView::OnDraw(CDC* pDC)
{
	CSDIOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(m_xRotate,0.f,1.f,0.f);
	glRotatef(m_yRotate,1.f,0.f,0.f);
	glScalef(m_fScale,m_fScale,m_fScale);
	glCallList(1);
	glPopMatrix();
	RenderScene();
	auto pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	if(pStatus)
		pStatus->SetPaneText(0,CString(pDoc->m_Obj.getString().c_str()));
}


// CSDIOpenGLView 打印

BOOL CSDIOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CSDIOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CSDIOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CSDIOpenGLView 诊断

#ifdef _DEBUG
void CSDIOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIOpenGLDoc* CSDIOpenGLView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIOpenGLDoc)));
	return (CSDIOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CSDIOpenGLView 消息处理程序


BOOL CSDIOpenGLView::RenderScene(void)
{
	::SwapBuffers(m_pDC->GetSafeHdc());
	return TRUE;
}


BOOL CSDIOpenGLView::SetupPixelFormat(void)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(pfd),				// pdf 结构的大小
		1,							// 版本号
		PFD_DRAW_TO_WINDOW |		// 支持在窗口中绘图
		PFD_SUPPORT_OPENGL |		// 支持 OpenGL
		PFD_DOUBLEBUFFER,			// 双缓冲模式
		PFD_TYPE_RGBA,				// RGBA模式
		24,							// 24位颜色深度
		0,0,0,0,0,0,				// 忽略颜色位
		0,							// 没有非透明度缓存
		0,							// 忽略移位位
		0,							// 无附加缓存
		0,0,0,0,					// 忽略累加位
		32,							// 32位深度缓存
		0,							// 五模板缓存
		0,							// 五辅助缓存
		PFD_MAIN_PLANE,				// 主层
		0,							// 保留
		0,0,0						// 忽略层，可见性和损毁掩模
	};

	int pixelformat = ::ChoosePixelFormat(m_pDC->GetSafeHdc(),&pfd);
	::SetPixelFormat(m_pDC->GetSafeHdc(),pixelformat,&pfd);
	if (pfd.dwFlags & PFD_NEED_PALETTE)
		SetLogicalPalette();
	return TRUE;
}


void CSDIOpenGLView::SetLogicalPalette(void)
{
	struct {
		WORD Version;
		WORD NumberOfEntries;
		PALETTEENTRY aEntries[256];
	} logicalPalette = { 0x300,256 };

	BYTE reds[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE greens[] = { 0, 36, 72, 109, 145, 182, 218, 255 };
	BYTE blues[] = { 0, 85, 170, 255 };

	for (int i = 0; i < 256; ++ i) {
		logicalPalette.aEntries[i].peRed = 
			reds[i & 7];
		logicalPalette.aEntries[i].peRed = 
			greens[(i >> 3) & 7];
		logicalPalette.aEntries[i].peRed = 
			blues[(i >> 6) & 3];
		logicalPalette.aEntries[i].peFlags = 0;
	}

	m_hPalette = CreatePalette((LOGPALETTE*)(&logicalPalette));
}


BOOL CSDIOpenGLView::InitializeOpenGL(CDC* pDC)
{
	//初始化 OpenGL 场景
	m_pDC = pDC;
	SetupPixelFormat();
	//生成绘制描述表
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//设置当前绘制描述表
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	glEnable(GL_DEPTH_TEST);
	return TRUE;
}


int CSDIOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	//初始化 OpenGL
	m_pDC = new CClientDC(this);
	InitializeOpenGL(m_pDC);

	return 0;
}


void CSDIOpenGLView::OnDestroy()
{
	CView::OnDestroy();


	//删除调色板和渲染上下文
	::wglMakeCurrent(0,0);
	::wglDeleteContext(m_hRC);
	if (m_hPalette)
		DeleteObject(m_hPalette);
	if (m_pDC)
		delete m_pDC;
}


void CSDIOpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// 添加窗口缩放时的图像变换函数
	
	if (cx && cy) {
		glViewport(0,0,cx,cy);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(75,static_cast<float>(cx) / static_cast<float>(cy),1,1000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(0.f,0.f,10.f,0.f,0.f,0.f,0.f,1.f,0.f);
	}
}


BOOL CSDIOpenGLView::OnEraseBkgnd(CDC* pDC)
{
	
	return TRUE;
}


void CSDIOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLDown = true;
	m_PtDown = point;

	CView::OnLButtonDown(nFlags, point);
}


void CSDIOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLDown = false;

	CView::OnLButtonUp(nFlags, point);
}


void CSDIOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	CSDIOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	if (m_bLDown) {
		m_xRotate += (point.x - m_PtDown.x) * 0.5f;
		m_yRotate += (point.y - m_PtDown.y) * 0.5f;
		m_PtDown = point;
		CWnd::Invalidate();
	}
	auto pStatus = (CStatusBar*)AfxGetApp()->m_pMainWnd->GetDescendantWindow(ID_VIEW_STATUS_BAR);
	if(pStatus)
		pStatus->SetPaneText(0,CString(pDoc->m_Obj.getString().c_str()));
	CView::OnMouseMove(nFlags, point);
}


BOOL CSDIOpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CSDIOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc) {
		if (m_bShowMode)
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		if (zDelta > 0)
			if (m_bShowMode)
				pDoc->m_Obj.Radio(true);
			else
				m_fScale *= 1.25f;
		else
			if (m_bShowMode)
				pDoc->m_Obj.Radio(false);
			else
				m_fScale *= 0.8f;
		CWnd::Invalidate();
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CSDIOpenGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CMenu menuPopup;
	if (menuPopup.CreatePopupMenu()) {
		CPoint pt(point);
		ClientToScreen(&pt);
		//menuPopup.AppendMenuW(MF_STRING,ID_FILE_OPEN,L"打开文件");
		menuPopup.AppendMenuW(MF_STRING,ID_SHOW_VIEW,L"浏览模式");
		menuPopup.AppendMenuW(MF_STRING,ID_SHOW_CHANGE,L"调整模式");
		menuPopup.TrackPopupMenu(TPM_LEFTALIGN,pt.x,pt.y,this);
	}
	CView::OnRButtonDown(nFlags, point);
}


void CSDIOpenGLView::OnShowChange()
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	m_bShowMode = true;
	CSDIOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (pDoc)
		pDoc->m_Obj.partition();
	CWnd::Invalidate();
}


void CSDIOpenGLView::OnShowView()
{
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	m_bShowMode = false;
}
