// CGWorkView.cpp : implementation of the CCGWorkView class
//
#include "stdafx.h"
#include "CGWork.h"

extern CCGWorkApp theApp;

#include "CGWorkDoc.h"
#include "CGWorkView.h"

#include <iostream>
using std::cout;
using std::endl;
#include "PerspectiveDlg.h"
#include "LightingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "PngWrapper.h"
#include "iritSkel.h"
#include "Globals.h"

// For Status Bar access
#include "MainFrm.h"

#include <set>
#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include "DialMouseSens.h"
#include "ColorsSelect.h"
#include "DiaFineNess.h"
#include "DiaScreenshot.h"
#include "AnimationSpeedDlg.h"
#include "FogDlg.h"
#include "BlurDlg.h"

// Use this macro to display text messages in the status bar.
#define STATUS_BAR_TEXT(str) (((CMainFrame*)GetParentFrame())->getStatusBar().SetWindowText(str))


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView

IMPLEMENT_DYNCREATE(CCGWorkView, CView)

BEGIN_MESSAGE_MAP(CCGWorkView, CView)
	//{{AFX_MSG_MAP(CCGWorkView)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_FILE_LOAD, OnFileLoad)
	ON_COMMAND(ID_VIEW_ORTHOGRAPHIC, OnViewOrthographic)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORTHOGRAPHIC, OnUpdateViewOrthographic)
	ON_COMMAND(ID_VIEW_PERSPECTIVE, OnViewPerspective)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PERSPECTIVE, OnUpdateViewPerspective)
	ON_COMMAND(ID_ACTION_ROTATE, OnActionRotate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_ROTATE, OnUpdateActionRotate)
	ON_COMMAND(ID_ACTION_SCALE, OnActionScale)
	ON_UPDATE_COMMAND_UI(ID_ACTION_SCALE, OnUpdateActionScale)
	ON_COMMAND(ID_ACTION_TRANSLATE, OnActionTranslate)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSLATE, OnUpdateActionTranslate)
	ON_COMMAND(ID_AXIS_X, OnAxisX)
	ON_UPDATE_COMMAND_UI(ID_AXIS_X, OnUpdateAxisX)
	ON_COMMAND(ID_AXIS_Y, OnAxisY)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Y, OnUpdateAxisY)
	ON_COMMAND(ID_AXIS_Z, OnAxisZ)
	ON_UPDATE_COMMAND_UI(ID_AXIS_Z, OnUpdateAxisZ)
	ON_COMMAND(ID_LIGHT_SHADING_FLAT, OnLightShadingFlat)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_FLAT, OnUpdateLightShadingFlat)
	ON_COMMAND(ID_LIGHT_SHADING_GOURAUD, OnLightShadingGouraud)
	ON_UPDATE_COMMAND_UI(ID_LIGHT_SHADING_GOURAUD, OnUpdateLightShadingGouraud)
	ON_COMMAND(ID_LIGHT_CONSTANTS, OnLightConstants)
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_COMMAND(ID_POLYGON_INCLUDED, &CCGWorkView::OnPolygonIncluded)
	ON_COMMAND(ID_VERTEX_INCLUDED, &CCGWorkView::OnVertexIncluded)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_POLYGON_CALCULATED, &CCGWorkView::OnPolygonCalculated)
	ON_COMMAND(ID_VERTEX_CALCULATED, &CCGWorkView::OnVertexCalculated)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_INCLUDED, &CCGWorkView::OnUpdatePolygonIncluded)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_CALCULATED, &CCGWorkView::OnUpdatePolygonCalculated)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_INCLUDED, &CCGWorkView::OnUpdateVertexIncluded)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_CALCULATED, &CCGWorkView::OnUpdateVertexCalculated)
	ON_COMMAND(ID_ACTION_TRANSFORMOBJECT, &CCGWorkView::OnActionTransformobject)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSFORMOBJECT, &CCGWorkView::OnUpdateActionTransformobject)
	ON_COMMAND(ID_ACTION_TRANSFORMVIEW, &CCGWorkView::OnActionTransformview)
	ON_UPDATE_COMMAND_UI(ID_ACTION_TRANSFORMVIEW, &CCGWorkView::OnUpdateActionTransformview)
	ON_COMMAND(ID_NORMAL_PLANE_TOGGLE, &CCGWorkView::OnNormalPlaneToggle)
	ON_UPDATE_COMMAND_UI(ID_NORMAL_PLANE_TOGGLE, &CCGWorkView::OnUpdateNormalPlaneToggle)
	ON_COMMAND(ID_NORMAL_VERTICE_TOGGLE, &CCGWorkView::OnNormalVerticeToggle)
	ON_UPDATE_COMMAND_UI(ID_NORMAL_VERTICE_TOGGLE, &CCGWorkView::OnUpdateNormalVerticeToggle)
	ON_COMMAND(ID_SHOW_BBOX, &CCGWorkView::OnShowBbox)
	ON_UPDATE_COMMAND_UI(ID_SHOW_BBOX, &CCGWorkView::OnUpdateShowBbox)
	ON_COMMAND(ID_OPTIONS_PERSPECTIVECONTROL, &CCGWorkView::OnOptionsPerspectivecontrol)
	ON_COMMAND(ID_OPTIONS_MOUSESENSITIVITY, &CCGWorkView::OnOptionsMousesensitivity)
	ON_COMMAND(ID_OPTIONS_COLORS, &CCGWorkView::OnOptionsColors)
	ON_COMMAND(ID_MFINENESS, &CCGWorkView::OnMfineness)
	ON_COMMAND(ID_RENDER_SQUELETON, &CCGWorkView::OnRenderSqueleton)
	ON_UPDATE_COMMAND_UI(ID_RENDER_SQUELETON, &CCGWorkView::OnUpdateRenderSqueleton)
	ON_COMMAND(ID_RENDER_ONSCREEN, &CCGWorkView::OnRenderOnscreen)
	ON_UPDATE_COMMAND_UI(ID_RENDER_ONSCREEN, &CCGWorkView::OnUpdateRenderOnscreen)
	ON_COMMAND(ID_RENDER_FLAT, &CCGWorkView::OnRenderFlat)
	ON_UPDATE_COMMAND_UI(ID_RENDER_FLAT, &CCGWorkView::OnUpdateRenderFlat)
	ON_COMMAND(ID_SHADING_PHONG, &CCGWorkView::OnShadingPhong)
	ON_UPDATE_COMMAND_UI(ID_SHADING_PHONG, &CCGWorkView::OnUpdateShadingPhong)
	ON_COMMAND(ID_RENDER_GOURAUD, &CCGWorkView::OnRenderGouraud)
	ON_UPDATE_COMMAND_UI(ID_RENDER_GOURAUD, &CCGWorkView::OnUpdateRenderGouraud)
	ON_COMMAND(ID_RENDER_PHONG, &CCGWorkView::OnRenderPhong)
	ON_UPDATE_COMMAND_UI(ID_RENDER_PHONG, &CCGWorkView::OnUpdateRenderPhong)
	ON_COMMAND(ID_BACKFACE_CULLING, &CCGWorkView::OnBackfaceCulling)
	ON_UPDATE_COMMAND_UI(ID_BACKFACE_CULLING, &CCGWorkView::OnUpdateBackfaceCulling)
	ON_COMMAND(ID_Z_BUFFER, &CCGWorkView::OnZBuffer)
	ON_UPDATE_COMMAND_UI(ID_Z_BUFFER, &CCGWorkView::OnUpdateZBuffer)
	ON_COMMAND(ID_POLYGON_INVERTED, &CCGWorkView::OnPolygonInverted)
	ON_UPDATE_COMMAND_UI(ID_POLYGON_INVERTED, &CCGWorkView::OnUpdatePolygonInverted)
	ON_COMMAND(ID_VERTEX_INVERTED, &CCGWorkView::OnVertexInverted)
	ON_UPDATE_COMMAND_UI(ID_VERTEX_INVERTED, &CCGWorkView::OnUpdateVertexInverted)
	ON_COMMAND(ID_RENDER_TOFILE, &CCGWorkView::OnRenderTofile)
	ON_COMMAND(ID_FILE_LOADBACKGROUND, &CCGWorkView::OnFileLoadbackground)
	ON_COMMAND(ID_BG_REPEAT, &CCGWorkView::OnBgRepeat)
	ON_UPDATE_COMMAND_UI(ID_BG_REPEAT, &CCGWorkView::OnUpdateBgRepeat)
	ON_COMMAND(ID_BG_STRETCH, &CCGWorkView::OnBgStretch)
	ON_UPDATE_COMMAND_UI(ID_BG_STRETCH, &CCGWorkView::OnUpdateBgStretch)
	ON_COMMAND(ID_SILOUHETTE, &CCGWorkView::OnSilouhette)
	ON_UPDATE_COMMAND_UI(ID_SILOUHETTE, &CCGWorkView::OnUpdateSilouhette)
	ON_COMMAND(ID_ANTI_NOAA, &CCGWorkView::OnAntiNoaa)
	ON_UPDATE_COMMAND_UI(ID_ANTI_NOAA, &CCGWorkView::OnUpdateAntiNoaa)
	ON_COMMAND(ID_ANTI_SSAAX3, &CCGWorkView::OnAntiSsaax3)
	ON_UPDATE_COMMAND_UI(ID_ANTI_SSAAX3, &CCGWorkView::OnUpdateAntiSsaax3)
	ON_COMMAND(ID_ANTI_SSAAX5, &CCGWorkView::OnAntiSsaax5)
	ON_UPDATE_COMMAND_UI(ID_ANTI_SSAAX5, &CCGWorkView::OnUpdateAntiSsaax5)
	ON_COMMAND(ID_ANTI_BOXFILTER, &CCGWorkView::OnAntiBoxfilter)
	ON_UPDATE_COMMAND_UI(ID_ANTI_BOXFILTER, &CCGWorkView::OnUpdateAntiBoxfilter)
	ON_COMMAND(ID_ANTI_TRIANGLEFILTER, &CCGWorkView::OnAntiTrianglefilter)
	ON_UPDATE_COMMAND_UI(ID_ANTI_TRIANGLEFILTER, &CCGWorkView::OnUpdateAntiTrianglefilter)
	ON_COMMAND(ID_ANTI_GAUSSIANFILTER, &CCGWorkView::OnAntiGaussianfilter)
	ON_UPDATE_COMMAND_UI(ID_ANTI_GAUSSIANFILTER, &CCGWorkView::OnUpdateAntiGaussianfilter)
	ON_COMMAND(ID_ANTI_SINCFILTER, &CCGWorkView::OnAntiSincfilter)
	ON_UPDATE_COMMAND_UI(ID_ANTI_SINCFILTER, &CCGWorkView::OnUpdateAntiSincfilter)
	ON_COMMAND(ID_RECORD, &CCGWorkView::OnRecord)
	ON_UPDATE_COMMAND_UI(ID_RECORD, &CCGWorkView::OnUpdateRecord)
	ON_COMMAND(ID_PLAY, &CCGWorkView::OnPlay)
	ON_UPDATE_COMMAND_UI(ID_PLAY, &CCGWorkView::OnUpdatePlay)
	ON_COMMAND(ID_STOP, &CCGWorkView::OnStop)
	ON_UPDATE_COMMAND_UI(ID_STOP, &CCGWorkView::OnUpdateStop)
		ON_COMMAND(ID_ANIMATION_SPEED, &CCGWorkView::OnAnimationSpeed)
		ON_COMMAND(ID_ANIMATION_SAVEANIMATION, &CCGWorkView::OnAnimationSaveanimation)
		ON_COMMAND(ID_OPTIONS_FOG, &CCGWorkView::OnOptionsFog)
		ON_COMMAND(ID_OPTIONS_MOTIONBLUR, &CCGWorkView::OnOptionsMotionblur)
		ON_COMMAND(ID_CLEAR_VIEW, &CCGWorkView::OnClearView)
		END_MESSAGE_MAP()


// A patch to fix GLaux disappearance from VS2005 to VS2008
void auxSolidCone(GLdouble radius, GLdouble height) {
	GLUquadric *quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);
	gluCylinder(quad, radius, 0.0, height, 20, 20);
	gluDeleteQuadric(quad);
}

/////////////////////////////////////////////////////////////////////////////
// CCGWorkView construction/destruction

CCGWorkView::CCGWorkView() :
	scene(this)
{
	// Set default values
	X = true;
	Y = false;
	Z = false;
	m_nAction = ROTATION;
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	transform_context = MODEL;
	clicking = false;

	m_nLightShading = ID_LIGHT_SHADING_FLAT;

	m_pDbBitMap = NULL;
	m_pDbDC = NULL;
	mouse_sensitivity = 0.5f;
}

CCGWorkView::~CCGWorkView()
{
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView diagnostics

#ifdef _DEBUG
void CCGWorkView::AssertValid() const
{
	CView::AssertValid();
}

void CCGWorkView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGWorkDoc* CCGWorkView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGWorkDoc)));
	return (CCGWorkDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView Window Creation - Linkage of windows to CGWork

BOOL CCGWorkView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// An CGWork window must be created with the following
	// flags and must NOT include CS_PARENTDC for the
	// class style.

	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}



int CCGWorkView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	InitializeCGWork();

	return 0;
}


// This method initialized the CGWork system.
BOOL CCGWorkView::InitializeCGWork()
{
	m_pDC = new CClientDC(this);

	if (NULL == m_pDC) { // failure to get DC
		::AfxMessageBox(CString("Couldn't get a valid DC."));
		return FALSE;
	}

	CRect r;
	GetClientRect(&r);
	m_pDbDC = new CDC();
	m_pDbDC->CreateCompatibleDC(m_pDC);
	SetTimer(1, 1, NULL);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);

	// Code
	update_draw_bitmap();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView message handlers


void CCGWorkView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (0 >= cx || 0 >= cy) {
		return;
	}

	// save the width and height of the current window
	m_WindowWidth = cx;
	m_WindowHeight = cy;

	// compute the aspect ratio
	// this will keep all dimension scales equal
	m_AspectRatio = (GLdouble)m_WindowWidth / (GLdouble)m_WindowHeight;

	CRect r;
	GetClientRect(&r);
	DeleteObject(m_pDbBitMap);
	m_pDbBitMap = CreateCompatibleBitmap(m_pDC->m_hDC, r.right, r.bottom);
	m_pDbDC->SelectObject(m_pDbBitMap);
	update_draw_bitmap();
}


BOOL CCGWorkView::SetupViewingFrustum(void)
{
	return TRUE;
}


// This viewing projection gives us a constant aspect ration. This is done by
// increasing the corresponding size of the ortho cube.
BOOL CCGWorkView::SetupViewingOrthoConstAspect(void)
{
	return TRUE;
}

void CCGWorkView::update_draw_bitmap()
{
	BMInfo = { 0 };
	BMInfo.bmiHeader.biSize = sizeof(BMInfo.bmiHeader);
	GetDIBits(*m_pDbDC, m_pDbBitMap, 0, 0, nullptr, &BMInfo, DIB_RGB_COLORS);
	BMInfo.bmiHeader.biBitCount = 32;
	BMInfo.bmiHeader.biCompression = BI_RGB;
	BMInfo.bmiHeader.biHeight = abs(BMInfo.bmiHeader.biHeight);
	scene.renderer.set_bitmap_dimensions(BMInfo);
}

BOOL CCGWorkView::OnEraseBkgnd(CDC* pDC)
{
	// Windows will clear the window with the background color every time your window 
	// is redrawn, and then CGWork will clear the viewport with its own background color.


	return true;
}



/////////////////////////////////////////////////////////////////////////////
// CCGWorkView drawing
/////////////////////////////////////////////////////////////////////////////

void CCGWorkView::OnDraw(CDC* pDC)
{
	static float theta = 0.0f;
	CCGWorkDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CRect r;
	GetClientRect(&r);
	CDC *pDCToUse = /*m_pDC*/m_pDbDC;

	POINT mouse;
	if (scene.playing) {
		if (scene.animator.restart) {
			// First frame, restart the animation
			scene.animator.restart = false;
			scene.models[scene.active_model].model_transform = scene.animator.start_model_mat;
			scene.renderer.view_transform = scene.animator.start_view_mat;
			for (CModel& model : scene.models) {
				model.apply_transform(scene.renderer.view_transform);
			}
		}
		else {
			bool is_view;
			mat4 transform = scene.animator.get_transform(&is_view, mouse_sensitivity);
			if (!is_view) {
				// model transform
				scene.models[scene.active_model].transform_model(transform, scene.renderer.view_transform);
			}
			else {
				scene.renderer.view_transform = transform * scene.renderer.view_transform;
				for (CModel& model : scene.models) {
					model.apply_transform(scene.renderer.view_transform);
				}
			}
		}
	}
	else {
		if (GetCursorPos(&mouse) && clicking) {
			int diff = mouse.x - mouse_pos_x;
			if (scene.recording) {
				scene.animator.acc += diff;
			}
			scene.update(this, diff);
		}
	}
	mouse_pos_x = mouse.x;
	
	scene.renderer.get_bitmap(pDCToUse);
	scene.renderer.set_camera(scene.cameras[scene.current_camera]);

	int ss_factor = scene.renderer.ss_factor;

	if (ss_factor > 1) {
		BYTE* buffer = scene.renderer.bitmap;
		int h = scene.renderer.screen.Height();
		int w = scene.renderer.screen.Width();
		scene.renderer.set_bitmap_dimensions(h * ss_factor, w * ss_factor);
		for (int y = 0; y < h * ss_factor; y++) {
			for (int x = 0; x < w * ss_factor; x++) {
				unsigned int offset = 4 * (y * w * ss_factor + x);
				scene.renderer.bitmap[offset] = GetBValue(scene.renderer.background_color);
				scene.renderer.bitmap[offset + 1] = GetGValue(scene.renderer.background_color);
				scene.renderer.bitmap[offset + 2] = GetRValue(scene.renderer.background_color);
			}
		}

		scene.draw();

		BYTE* ss_buffer = scene.renderer.bitmap;
		scene.renderer.downsample(buffer, ss_buffer, h, w, ss_factor);

		update_draw_bitmap(); // Reset the bitmap to screen size
		delete[] scene.renderer.bitmap;
		scene.renderer.bitmap = buffer;
	}
	else {
		scene.draw();
	}
	
	scene.renderer.draw_bitmap(pDCToUse);
	m_pDC->BitBlt(r.left, r.top, r.Width(), r.Height(), pDCToUse, r.left, r.top, SRCCOPY);

	theApp.OnIdle(-1);
}


/////////////////////////////////////////////////////////////////////////////
// CCGWorkView CGWork Finishing and clearing...

void CCGWorkView::OnDestroy()
{
	CView::OnDestroy();

	// delete the DC
	if (m_pDC) {
		delete m_pDC;
	}

	if (m_pDbDC) {
		delete m_pDbDC;
	}
}



/////////////////////////////////////////////////////////////////////////////
// User Defined Functions

void CCGWorkView::RenderScene() {
	// do nothing. This is supposed to be overriden...

	return;
}


void CCGWorkView::OnFileLoad()
{
	TCHAR szFilters[] = _T("IRIT Data Files (*.itd)|*.itd|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("itd"), _T("*.itd"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		reset_current_model();
		m_strItdFileName = dlg.GetPathName();		// Full path and filename
		PngWrapper p;
		CGSkelProcessIritDataFiles(m_strItdFileName, 1);
		CModel model = get_current_model();
		model.setup_model(scene.renderer.view_transform);
		scene.add_model(model);

		Invalidate();	// force a WM_PAINT for drawing.
	}

}





// VIEW HANDLERS ///////////////////////////////////////////

// Note: that all the following Message Handlers act in a similar way.
// Each control or command has two functions associated with it.

void CCGWorkView::OnViewOrthographic()
{
	m_nView = ID_VIEW_ORTHOGRAPHIC;
	m_bIsPerspective = false;
	scene.update_projection(m_nView);
	Invalidate();		// redraw using the new view.
}

void CCGWorkView::OnUpdateViewOrthographic(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_ORTHOGRAPHIC);
}

void CCGWorkView::OnViewPerspective()
{
	m_nView = ID_VIEW_PERSPECTIVE;
	m_bIsPerspective = true;
	scene.update_projection(m_nView);
	Invalidate();
}

void CCGWorkView::OnUpdateViewPerspective(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nView == ID_VIEW_PERSPECTIVE);
}




// ACTION HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnActionRotate()
{
	m_nAction = ROTATION;
}

void CCGWorkView::OnUpdateActionRotate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == ROTATION);
}

void CCGWorkView::OnActionTranslate()
{
	m_nAction = TRANSLATION;
}

void CCGWorkView::OnUpdateActionTranslate(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == TRANSLATION);
}

void CCGWorkView::OnActionScale()
{
	m_nAction = SCALING;
}

void CCGWorkView::OnUpdateActionScale(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_nAction == SCALING);
}




// AXIS HANDLERS ///////////////////////////////////////////


// Gets calles when the X button is pressed or when the Axis->X menu is selected.
// The only thing we do here is set the ChildView member variable m_nAxis to the 
// selected axis.
void CCGWorkView::OnAxisX()
{
	X = !X;
}

// Gets called when windows has to repaint either the X button or the Axis pop up menu.
// The control is responsible for its redrawing.
// It sets itself disabled when the action is a Scale action.
// It sets itself Checked if the current axis is the X axis.
void CCGWorkView::OnUpdateAxisX(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(X);
}

void CCGWorkView::OnAxisY()
{
	Y = !Y;
}

void CCGWorkView::OnUpdateAxisY(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Y);
}


void CCGWorkView::OnAxisZ()
{
	Z = !Z;
}

void CCGWorkView::OnUpdateAxisZ(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(Z);
}

// LIGHT SHADING HANDLERS ///////////////////////////////////////////

void CCGWorkView::OnLightShadingFlat()
{
	scene.renderer.shading_type = CRenderer::FLAT;
}

void CCGWorkView::OnUpdateLightShadingFlat(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.shading_type == CRenderer::FLAT);
}


void CCGWorkView::OnLightShadingGouraud()
{
	scene.renderer.shading_type = CRenderer::GOURAUD;
}

void CCGWorkView::OnUpdateLightShadingGouraud(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.shading_type == CRenderer::GOURAUD);
}

// LIGHT SETUP HANDLER ///////////////////////////////////////////

void CCGWorkView::OnLightConstants()
{
	LightingDlg dlg(this);
	if (dlg.DoModal() == IDOK) {
		for (int i = 0; i < LIGHT_NUM; i++) {
			scene.renderer.lights[i] = dlg.lights[i];
		}
		scene.renderer.ambiant = dlg.ambiant;
		Invalidate();
	}
}

void CCGWorkView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CView::OnTimer(nIDEvent);
	if (nIDEvent == 1)
		Invalidate();
}





void CCGWorkView::OnLButtonDown(UINT nFlags, CPoint point)
{
	clicking = true;
	CView::OnLButtonDown(nFlags, point);
}

void CCGWorkView::OnLButtonUp(UINT nFlags, CPoint point)
{
	clicking = false;
	CView::OnLButtonUp(nFlags, point);
	scene.renderer.mouse_x = point.x;
	scene.renderer.mouse_y = point.y;
	scene.renderer.select_highlighted_pol = true;
	if (scene.recording) {
		scene.animator.add_keyframe(X, Y, Z, m_nAction, transform_context);
	}
}

void CCGWorkView::OnPolygonIncluded()
{
	scene.renderer.draw_polygon_included_normals = true;
}

void CCGWorkView::OnPolygonCalculated()
{
	scene.renderer.draw_polygon_included_normals = false;
}

void CCGWorkView::OnVertexIncluded()
{
	scene.renderer.draw_vertice_included_normals = true;
}

void CCGWorkView::OnVertexCalculated()
{
	scene.renderer.draw_vertice_included_normals = false;
}

void CCGWorkView::OnUpdatePolygonIncluded(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_polygon_included_normals);
}

void CCGWorkView::OnUpdatePolygonCalculated(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!scene.renderer.draw_polygon_included_normals);
}

void CCGWorkView::OnUpdateVertexIncluded(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_vertice_included_normals);
}

void CCGWorkView::OnUpdateVertexCalculated(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(!scene.renderer.draw_vertice_included_normals);
}

void CCGWorkView::OnActionTransformobject()
{
	transform_context = MODEL;
}

void CCGWorkView::OnUpdateActionTransformobject(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(transform_context == MODEL);
}


void CCGWorkView::OnActionTransformview()
{
	transform_context = VIEW;
}


void CCGWorkView::OnUpdateActionTransformview(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(transform_context == VIEW);
}


void CCGWorkView::OnNormalPlaneToggle()
{
	scene.renderer.draw_polygon_normals = !scene.renderer.draw_polygon_normals;
}


void CCGWorkView::OnUpdateNormalPlaneToggle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_polygon_normals);
}


void CCGWorkView::OnNormalVerticeToggle()
{
	scene.renderer.draw_vertice_normals = !scene.renderer.draw_vertice_normals;
}


void CCGWorkView::OnUpdateNormalVerticeToggle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_vertice_normals);
}


void CCGWorkView::OnShowBbox()
{
	scene.renderer.draw_bbox = !scene.renderer.draw_bbox;
}


void CCGWorkView::OnUpdateShowBbox(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_bbox);
}


void CCGWorkView::OnOptionsPerspectivecontrol()
{
	PerspectiveDlg dlg(nullptr, scene.get_current_camera().d);
	if (dlg.DoModal() == IDOK) {
		scene.get_current_camera().set_depth(dlg.depth);
	}
}



void CCGWorkView::OnOptionsMousesensitivity()
{
	DialMouseSens dlg(this);
	if (dlg.DoModal() == IDOK) {
		mouse_sensitivity = dlg.mouse_sensitivity;
	}
}


void CCGWorkView::OnOptionsColors()
{
	ColorsSelect dlg(this);
	dlg.DoModal();
}




void CCGWorkView::OnMfineness()
{
	DiaFineNess dlg;
	dlg.DoModal();
	changeFineNess(dlg.fineNess);
}


void CCGWorkView::OnRenderSqueleton()
{
	scene.renderer.rendering_type = CRenderer::SQUELETON;
}


void CCGWorkView::OnUpdateRenderSqueleton(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.rendering_type == CRenderer::SQUELETON);
}


void CCGWorkView::OnRenderOnscreen()
{
	scene.renderer.rendering_type = CRenderer::SOLID;
}


void CCGWorkView::OnUpdateRenderOnscreen(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.rendering_type == CRenderer::SOLID);
}


void CCGWorkView::OnRenderFlat()
{
	scene.renderer.shading_type = CRenderer::FLAT;
	scene.renderer.rendering_type = CRenderer::SOLID;
}


void CCGWorkView::OnUpdateRenderFlat(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.rendering_type == CRenderer::SOLID && scene.renderer.shading_type == CRenderer::FLAT);
}


void CCGWorkView::OnShadingPhong()
{
	scene.renderer.shading_type = CRenderer::PHONG;
}


void CCGWorkView::OnUpdateShadingPhong(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.shading_type == CRenderer::PHONG);
}


void CCGWorkView::OnRenderGouraud()
{
	scene.renderer.shading_type = CRenderer::GOURAUD;
	scene.renderer.rendering_type = CRenderer::SOLID;
}


void CCGWorkView::OnUpdateRenderGouraud(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.rendering_type == CRenderer::SOLID && scene.renderer.shading_type == CRenderer::GOURAUD);
}


void CCGWorkView::OnRenderPhong()
{
	scene.renderer.shading_type = CRenderer::PHONG;
	scene.renderer.rendering_type = CRenderer::SOLID;
}


void CCGWorkView::OnUpdateRenderPhong(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.rendering_type == CRenderer::SOLID && scene.renderer.shading_type == CRenderer::PHONG);
}


void CCGWorkView::OnBackfaceCulling()
{
	scene.renderer.backface_culling = !scene.renderer.backface_culling;
}


void CCGWorkView::OnUpdateBackfaceCulling(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.backface_culling);
}


void CCGWorkView::OnZBuffer()
{
	scene.display_z_buffer = !scene.display_z_buffer;
}


void CCGWorkView::OnUpdateZBuffer(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.display_z_buffer);
}


void CCGWorkView::OnPolygonInverted()
{
	scene.renderer.invert_polygon_normals = !scene.renderer.invert_polygon_normals;
}


void CCGWorkView::OnUpdatePolygonInverted(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.invert_polygon_normals);
}


void CCGWorkView::OnVertexInverted()
{
	scene.renderer.invert_vertice_normals = !scene.renderer.invert_vertice_normals;
}


void CCGWorkView::OnUpdateVertexInverted(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.invert_vertice_normals);
}


void CCGWorkView::OnRenderTofile()
{
	DiaScreenshot dlg(this);
	dlg.DoModal();
}


void CCGWorkView::OnFileLoadbackground()
{
	TCHAR szFilters[] = _T("PNG Data Files (*.png)|*.png|All Files (*.*)|*.*||");

	CFileDialog dlg(TRUE, _T("png"), _T("*.png"), OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilters);

	if (dlg.DoModal() == IDOK) {
		CString path = dlg.GetPathName();
		CT2A ascii(path);
		scene.background_image.SetFileName(ascii.m_psz);
		scene.background_image.ReadPng();
		scene.isBackgroundLoaded = true;

		Invalidate();	// force a WM_PAINT for drawing.
	}
}


void CCGWorkView::OnBgRepeat()
{
	scene.isBackgroundStretched = false;
}


void CCGWorkView::OnUpdateBgRepeat(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.isBackgroundLoaded && !scene.isBackgroundStretched);
}


void CCGWorkView::OnBgStretch()
{
	scene.isBackgroundStretched = true;
}


void CCGWorkView::OnUpdateBgStretch(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.isBackgroundLoaded && scene.isBackgroundStretched);
}


void CCGWorkView::OnSilouhette()
{
	scene.renderer.draw_silouhette = !scene.renderer.draw_silouhette;
}


void CCGWorkView::OnUpdateSilouhette(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.draw_silouhette);
}


void CCGWorkView::OnAntiNoaa()
{
	scene.renderer.ss_factor = 1;
}


void CCGWorkView::OnUpdateAntiNoaa(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.ss_factor == 1);
}


void CCGWorkView::OnAntiSsaax3()
{
	scene.renderer.ss_factor = 3;
}


void CCGWorkView::OnUpdateAntiSsaax3(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.ss_factor == 3);
}


void CCGWorkView::OnAntiSsaax5()
{
	scene.renderer.ss_factor = 5;
}


void CCGWorkView::OnUpdateAntiSsaax5(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.ss_factor == 5);
}


void CCGWorkView::OnAntiBoxfilter()
{
	scene.renderer.filter_type = BOX;
}


void CCGWorkView::OnUpdateAntiBoxfilter(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.filter_type == BOX);
}


void CCGWorkView::OnAntiTrianglefilter()
{
	scene.renderer.filter_type = TRIANGLE;
}


void CCGWorkView::OnUpdateAntiTrianglefilter(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.filter_type == TRIANGLE);
}


void CCGWorkView::OnAntiGaussianfilter()
{
	scene.renderer.filter_type = GAUSSIAN;
}


void CCGWorkView::OnUpdateAntiGaussianfilter(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.filter_type == GAUSSIAN);
}


void CCGWorkView::OnAntiSincfilter()
{
	scene.renderer.filter_type = SINC;
}


void CCGWorkView::OnUpdateAntiSincfilter(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.renderer.filter_type == SINC);
}


void CCGWorkView::OnRecord()
{
	if (scene.models.size() > 0 && !scene.playing) {
		scene.recording = !scene.recording;
		if (scene.recording) {
			scene.animator.start_recording(scene.models[scene.active_model].model_transform, scene.renderer.view_transform);
		}
	}
}


void CCGWorkView::OnUpdateRecord(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.recording);
}


void CCGWorkView::OnPlay()
{
	if (scene.models.size() > 0) {
		scene.recording = false;
		scene.playing = true;
		scene.animator.restart = true;
	}
}


void CCGWorkView::OnUpdatePlay(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(scene.playing);
}

void CCGWorkView::OnStop()
{
	if (scene.models.size() > 0 && scene.playing) {
		scene.playing = false;
		scene.animator.next_keyframe = 0;
		scene.animator.current_step = 0;
		scene.models[scene.active_model].model_transform = scene.animator.start_model_mat;
		scene.renderer.view_transform = scene.animator.start_view_mat;
		for (CModel& model : scene.models) {
			model.apply_transform(scene.renderer.view_transform);
		}
	}
}


void CCGWorkView::OnUpdateStop(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}


void CCGWorkView::OnAnimationSpeed()
{
	AnimationSpeedDlg dlg(this);
	if (dlg.DoModal() == IDOK) {
		if (scene.models.size() > 0 && scene.playing) {
			scene.animator.next_keyframe = 0;
			scene.animator.current_step = 0;
			scene.animator.restart = true;
			scene.models[scene.active_model].model_transform = scene.animator.start_model_mat;
			scene.renderer.view_transform = scene.animator.start_view_mat;
			for (CModel& model : scene.models) {
				model.apply_transform(scene.renderer.view_transform);
			}
		}
	}
}


void CCGWorkView::OnAnimationSaveanimation()
{
	if (scene.animator.keyframes.size() == 0) {
		return;
	}

	CFolderPickerDialog dlg(nullptr, OFN_FILEMUSTEXIST, this, sizeof(OPENFILENAME));

	if (dlg.DoModal() != IDOK) {
		return;
	}

	CString path = dlg.GetPathName();
	int i = 0;

	scene.animator.next_keyframe = 0;
	scene.animator.current_step = 0;
	scene.models[scene.active_model].model_transform = scene.animator.start_model_mat;
	scene.renderer.view_transform = scene.animator.start_view_mat;
	for (CModel& model : scene.models) {
		model.apply_transform(scene.renderer.view_transform);
	}

	while (true) {
		i++;
		CString filepath;
		filepath.Format(_T("%s\\%d.png"), path, i);
		CT2A ascii(filepath);

		int height = scene.renderer.screen.Height();
		int width = scene.renderer.screen.Width();
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				unsigned int offset = 4 * (y * width + x);
				scene.renderer.bitmap[offset] = GetBValue(scene.renderer.background_color);
				scene.renderer.bitmap[offset + 1] = GetGValue(scene.renderer.background_color);
				scene.renderer.bitmap[offset + 2] = GetRValue(scene.renderer.background_color);
			}
		}
		scene.draw();

		PngWrapper res(ascii.m_psz, width, height);
		res.InitWritePng();


		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				unsigned int offset = 4 * (y * width + x);
				int B = scene.renderer.bitmap[offset];
				int G = scene.renderer.bitmap[offset + 1];
				int R = scene.renderer.bitmap[offset + 2];

				res.SetValue(x, height - y - 1, SET_RGB(R, G, B));
			}
		}
		res.WritePng();

		if (scene.animator.restart) {
			break;
		}
		
		bool is_view;
		mat4 transform = scene.animator.get_transform(&is_view, mouse_sensitivity);
		if (!is_view) {
			// model transform
			scene.models[scene.active_model].transform_model(transform, scene.renderer.view_transform);
		}
		else {
			scene.renderer.view_transform = transform * scene.renderer.view_transform;
			for (CModel& model : scene.models) {
				model.apply_transform(scene.renderer.view_transform);
			}
		}
	}
	scene.animator.next_keyframe = 0;
	scene.animator.current_step = 0;
	scene.models[scene.active_model].model_transform = scene.animator.start_model_mat;
	scene.renderer.view_transform = scene.animator.start_view_mat;
	for (CModel& model : scene.models) {
		model.apply_transform(scene.renderer.view_transform);
	}
	Invalidate();
}


void CCGWorkView::OnOptionsFog()
{
	FogDlg dlg(this);
	if (dlg.DoModal() == IDOK) {

	}
}


void CCGWorkView::OnOptionsMotionblur()
{
	BlurDlg dlg(this);
	if (dlg.DoModal() == IDOK) {

	}
}


void CCGWorkView::OnClearView()
{
	scene.models.clear();
}
