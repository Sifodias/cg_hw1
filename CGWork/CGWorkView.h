// CGWorkView.h : interface of the CCGWorkView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
#define AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "gl\gl.h"    // Include the standard CGWork  headers
#include "gl\glu.h"   // Add the utility library


#include "Light.h"
#include "Camera.h"
#include "Model.h"
#include "Mat.h"
#include <bitset>
#include <unordered_set>
#include <unordered_map>
#include "CGWorkDoc.h"
#include "cPNG.h"
#include "PngWrapper.h"
#include "Animation.h"

class CCGWorkView : public CView
{
public:
	class CRenderer
	{
	public:
		mat4 view_transform;

		CCGWorkView* parent;
		BYTE* bitmap;
		CCamera camera;
		enum { SQUELETON, SOLID };
		int rendering_type;
		enum { FLAT, GOURAUD, PHONG };
		int shading_type;

		void set_pixel(POINT P, const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint = false);
		void draw_pixel(POINT p, COLORREF c);
		void draw_line(const vec3& v1, const vec3& v2, COLORREF color, bool forcePrint = false);
		void calculate_left(const vec3& v1, const vec3& v2, vector<pair<int, float>>& points, int min_y);
		void calculate_right(const vec3& v1, const vec3& v2, vector<pair<int, float>>& points, int min_y);
		COLORREF calculate_light(const vec3& point, const vec3& normal, const COLORREF& base);
		COLORREF multiply(COLORREF color, float k);
		COLORREF add(COLORREF c1, COLORREF c2);
		void apply_perspective(vec4& v);
		vec3 cast(const vec3& v);
		void downsample(BYTE* buffer, BYTE* ss_buffer, int h, int w, int factor);
		void set_filter(float* rate, float** weights);

		bool draw_bbox;
		bool draw_silouhette;
		bool draw_polygon_normals;
		bool draw_vertice_normals;
		bool invert_polygon_normals;
		bool invert_vertice_normals;
		bool draw_polygon_included_normals;
		bool draw_vertice_included_normals;

		bool backface_culling;

		CRenderer(CCGWorkView* parent);
		~CRenderer();
		void set_bitmap_dimensions(const BITMAPINFO& info);
		void set_bitmap_dimensions(int width, int height);
		void get_bitmap(CDC* context);
		void draw_bitmap(CDC* context);
		void set_camera(const CCamera& camera);
		void draw_model(CModel& model);

		float get_x(vec3 v1, vec3 v2, int y);
		void draw_flat(const CModel& model);
		void draw_gouraud(const CModel& model);
		void draw_phong(const CModel& model);
		void draw_edges(CModel& model);
		void draw_normals(const CModel& model);
		void draw_normal(const vec3& origin, const vec3& direction, COLORREF color);
		void draw_bounding_box(const CModel& model);

		CRect screen;
		vector<bool> bitFlag;
		std::vector<std::vector<float>> z_buffer;
		int mouse_x, mouse_y;
		bool select_highlighted_pol;
		COLORREF background_color;
		COLORREF highlight_polygon;
		COLORREF normals_color;
		COLORREF wireframe_color;
		COLORREF bbox_color;
		COLORREF silouhette_color;

		LightParams lights[LIGHT_NUM];
		AmbiantLightParams ambiant;

		int ss_factor;
		AAFilter filter_type;
	};

	class CScene
	{
	public:
		vector<CCamera> cameras;
		int current_camera;
		Animation animator;
		bool recording;
		bool playing;
		
		CScene(CCGWorkView* parent);
		void add_model(const CModel& model);
		void add_camera(const CCamera& camera);
		void update_projection(int projection_type);
		CCamera& get_current_camera();
		void draw_background();
		void draw_fog();

		void update(CCGWorkView* app, int mouse_dx);
		void draw();
		void drawZBuffer();
		void screenshot(CCGWorkView* parent, float width, float height);
		void drawMotionBlur();

		CRenderer renderer;
		bool display_z_buffer;

		bool display_fog;
		bool display_motionBlur;
		double motionBlur_t;
		double minBoundFog;
		double maxBoundFog;
		double fogIntensity;
		COLORREF fog_color;
		bool fog_on_bg;

		vector<CModel> models;
		int active_model;
		BYTE* lastFrame;
		int sizeLastFrame;

		PngWrapper background_image;
		bool isBackgroundLoaded;
		bool isBackgroundStretched;
	};

protected: // create from serialization only
	CCGWorkView();
	DECLARE_DYNCREATE(CCGWorkView)

// Attributes
public:
	CCGWorkDoc* GetDocument();

// Operations
public:
	int mouse_pos_x;
	bool clicking;

	bool X;
	bool Y;
	bool Z;
	Transformation m_nAction;				// Rotate, Translate, Scale
	int m_nView;				// Orthographic, perspective
	bool m_bIsPerspective;			// is the view perspective

	Space transform_context;

private:
	CString m_strItdFileName;		// file name of IRIT data

	int m_nLightShading;			// shading: Flat, Gouraud.

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCGWorkView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCGWorkView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BOOL InitializeCGWork();
	BOOL SetupViewingFrustum(void);
	BOOL SetupViewingOrthoConstAspect(void);

	virtual void RenderScene();


	HGLRC    m_hRC;			// holds the Rendering Context
	CDC*     m_pDC;			// holds the Device Context
	int m_WindowWidth;		// hold the windows width
	int m_WindowHeight;		// hold the windows height
	double m_AspectRatio;		// hold the fixed Aspect Ration

	HBITMAP m_pDbBitMap;
	CDC* m_pDbDC;
	BITMAPINFO BMInfo;
	void update_draw_bitmap();

public:
	CScene scene;



// Generated message map functions
protected:
	//{{AFX_MSG(CCGWorkView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileLoad();
	afx_msg void OnViewOrthographic();
	afx_msg void OnUpdateViewOrthographic(CCmdUI* pCmdUI);
	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);
	afx_msg void OnActionRotate();
	afx_msg void OnUpdateActionRotate(CCmdUI* pCmdUI);
	afx_msg void OnActionScale();
	afx_msg void OnUpdateActionScale(CCmdUI* pCmdUI);
	afx_msg void OnActionTranslate();
	afx_msg void OnUpdateActionTranslate(CCmdUI* pCmdUI);
	afx_msg void OnAxisX();
	afx_msg void OnUpdateAxisX(CCmdUI* pCmdUI);
	afx_msg void OnAxisY();
	afx_msg void OnUpdateAxisY(CCmdUI* pCmdUI);
	afx_msg void OnAxisZ();
	afx_msg void OnUpdateAxisZ(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingFlat();
	afx_msg void OnUpdateLightShadingFlat(CCmdUI* pCmdUI);
	afx_msg void OnLightShadingGouraud();
	afx_msg void OnUpdateLightShadingGouraud(CCmdUI* pCmdUI);
	afx_msg void OnLightConstants();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPolygonIncluded();
	afx_msg void OnVertexIncluded();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPolygonCalculated();
	afx_msg void OnVertexCalculated();
	afx_msg void OnUpdatePolygonIncluded(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePolygonCalculated(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVertexIncluded(CCmdUI *pCmdUI);
	afx_msg void OnUpdateVertexCalculated(CCmdUI *pCmdUI);
	afx_msg void OnActionTransformobject();
	afx_msg void OnUpdateActionTransformobject(CCmdUI *pCmdUI);
	afx_msg void OnActionTransformview();
	afx_msg void OnUpdateActionTransformview(CCmdUI *pCmdUI);
	afx_msg void OnNormalPlaneToggle();
	afx_msg void OnUpdateNormalPlaneToggle(CCmdUI *pCmdUI);
	afx_msg void OnNormalVerticeToggle();
	afx_msg void OnUpdateNormalVerticeToggle(CCmdUI *pCmdUI);
	afx_msg void OnShowBbox();
	afx_msg void OnUpdateShowBbox(CCmdUI *pCmdUI);
	afx_msg void OnOptionsPerspectivecontrol();
	float mouse_sensitivity;
	afx_msg void OnOptionsMousesensitivity();
	afx_msg void OnOptionsColors();
	afx_msg void OnMfineness();
	afx_msg void OnRenderSqueleton();
	afx_msg void OnUpdateRenderSqueleton(CCmdUI *pCmdUI);
	afx_msg void OnRenderOnscreen();
	afx_msg void OnUpdateRenderOnscreen(CCmdUI *pCmdUI);
	afx_msg void OnRenderFlat();
	afx_msg void OnUpdateRenderFlat(CCmdUI *pCmdUI);
	afx_msg void OnShadingPhong();
	afx_msg void OnUpdateShadingPhong(CCmdUI *pCmdUI);
	afx_msg void OnRenderGouraud();
	afx_msg void OnUpdateRenderGouraud(CCmdUI *pCmdUI);
	afx_msg void OnRenderPhong();
	afx_msg void OnUpdateRenderPhong(CCmdUI *pCmdUI);
	afx_msg void OnBackfaceCulling();
	afx_msg void OnUpdateBackfaceCulling(CCmdUI *pCmdUI);
	afx_msg void OnZBuffer();
	afx_msg void OnUpdateZBuffer(CCmdUI *pCmdUI);
	afx_msg void OnPolygonInverted();
	afx_msg void OnUpdatePolygonInverted(CCmdUI *pCmdUI);
	afx_msg void OnVertexInverted();
	afx_msg void OnUpdateVertexInverted(CCmdUI *pCmdUI);
	afx_msg void OnRenderTofile();
	afx_msg void OnFileLoadbackground();
	afx_msg void OnBgRepeat();
	afx_msg void OnUpdateBgRepeat(CCmdUI *pCmdUI);
	afx_msg void OnBgStretch();
	afx_msg void OnUpdateBgStretch(CCmdUI *pCmdUI);
	afx_msg void OnSilouhette();
	afx_msg void OnUpdateSilouhette(CCmdUI *pCmdUI);
	afx_msg void OnAntiNoaa();
	afx_msg void OnUpdateAntiNoaa(CCmdUI *pCmdUI);
	afx_msg void OnAntiSsaax3();
	afx_msg void OnUpdateAntiSsaax3(CCmdUI *pCmdUI);
	afx_msg void OnAntiSsaax5();
	afx_msg void OnUpdateAntiSsaax5(CCmdUI *pCmdUI);
	afx_msg void OnAntiBoxfilter();
	afx_msg void OnUpdateAntiBoxfilter(CCmdUI *pCmdUI);
	afx_msg void OnAntiTrianglefilter();
	afx_msg void OnUpdateAntiTrianglefilter(CCmdUI *pCmdUI);
	afx_msg void OnAntiGaussianfilter();
	afx_msg void OnUpdateAntiGaussianfilter(CCmdUI *pCmdUI);
	afx_msg void OnAntiSincfilter();
	afx_msg void OnUpdateAntiSincfilter(CCmdUI *pCmdUI);
	afx_msg void OnRecord();
	afx_msg void OnUpdateRecord(CCmdUI *pCmdUI);
	afx_msg void OnPlay();
	afx_msg void OnUpdatePlay(CCmdUI *pCmdUI);
	afx_msg void OnPause();
	afx_msg void OnUpdatePause(CCmdUI *pCmdUI);
	afx_msg void OnStop();
	afx_msg void OnUpdateStop(CCmdUI *pCmdUI);
	afx_msg void OnAnimationSpeed();
	afx_msg void OnAnimationSaveanimation();
	afx_msg void OnOptionsFog();
	afx_msg void OnOptionsMotionblur();
	afx_msg void OnClearView();
};

#ifndef _DEBUG  // debug version in CGWorkView.cpp
inline CCGWorkDoc* CCGWorkView::GetDocument()
   { return (CCGWorkDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CGWORKVIEW_H__5857316D_EA60_11D5_9FD5_00D0B718E2CD__INCLUDED_)
