#pragma once
#include "CGWorkView.h"

// BlurDlg dialog

class BlurDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BlurDlg)

public:
	BlurDlg(CCGWorkView* parent, CWnd* pParent = nullptr);   // standard constructor
	virtual ~BlurDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BLUR_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	CCGWorkView* pview;
	CButton enable;
	CEdit factor;

	BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
