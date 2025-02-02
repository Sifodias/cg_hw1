#pragma once
#include "CGWorkView.h"

// AnimationSpeedDlg dialog

class AnimationSpeedDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AnimationSpeedDlg)

public:
	AnimationSpeedDlg(CCGWorkView* parent, CWnd* pParent = nullptr);   // standard constructor
	virtual ~AnimationSpeedDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATION_SPEED };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CCGWorkView* parent;
	CEdit speed;
	int speed_value;

	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
