
// SDIOpenGLView.cpp : CSDIOpenGLView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

// CSDIOpenGLView ����/����

CSDIOpenGLView::CSDIOpenGLView()
	: m_bLDown(false)
	, m_xRotate(0.f)
	, m_yRotate(0.f)
	, m_fScale(1.f)
	, m_bShowMode(false)
{
	// TODO: �ڴ˴���ӹ������

}

CSDIOpenGLView::~CSDIOpenGLView()
{
}

BOOL CSDIOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	return CView::PreCreateWindow(cs);
}

// CSDIOpenGLView ����

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


// CSDIOpenGLView ��ӡ

BOOL CSDIOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CSDIOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CSDIOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CSDIOpenGLView ���

#ifdef _DEBUG
void CSDIOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIOpenGLDoc* CSDIOpenGLView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIOpenGLDoc)));
	return (CSDIOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CSDIOpenGLView ��Ϣ�������


BOOL CSDIOpenGLView::RenderScene(void)
{
	::SwapBuffers(m_pDC->GetSafeHdc());
	return TRUE;
}


BOOL CSDIOpenGLView::SetupPixelFormat(void)
{
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(pfd),				// pdf �ṹ�Ĵ�С
		1,							// �汾��
		PFD_DRAW_TO_WINDOW |		// ֧���ڴ����л�ͼ
		PFD_SUPPORT_OPENGL |		// ֧�� OpenGL
		PFD_DOUBLEBUFFER,			// ˫����ģʽ
		PFD_TYPE_RGBA,				// RGBAģʽ
		24,							// 24λ��ɫ���
		0,0,0,0,0,0,				// ������ɫλ
		0,							// û�з�͸���Ȼ���
		0,							// ������λλ
		0,							// �޸��ӻ���
		0,0,0,0,					// �����ۼ�λ
		32,							// 32λ��Ȼ���
		0,							// ��ģ�建��
		0,							// �帨������
		PFD_MAIN_PLANE,				// ����
		0,							// ����
		0,0,0						// ���Բ㣬�ɼ��Ժ������ģ
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
	//��ʼ�� OpenGL ����
	m_pDC = pDC;
	SetupPixelFormat();
	//���ɻ���������
	m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc());
	//���õ�ǰ����������
	::wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC);
	glEnable(GL_DEPTH_TEST);
	return TRUE;
}


int CSDIOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;


	//��ʼ�� OpenGL
	m_pDC = new CClientDC(this);
	InitializeOpenGL(m_pDC);

	return 0;
}


void CSDIOpenGLView::OnDestroy()
{
	CView::OnDestroy();


	//ɾ����ɫ�����Ⱦ������
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

	// ��Ӵ�������ʱ��ͼ��任����
	
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CMenu menuPopup;
	if (menuPopup.CreatePopupMenu()) {
		CPoint pt(point);
		ClientToScreen(&pt);
		//menuPopup.AppendMenuW(MF_STRING,ID_FILE_OPEN,L"���ļ�");
		menuPopup.AppendMenuW(MF_STRING,ID_SHOW_VIEW,L"���ģʽ");
		menuPopup.AppendMenuW(MF_STRING,ID_SHOW_CHANGE,L"����ģʽ");
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
