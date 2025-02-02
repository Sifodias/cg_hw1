// BlurDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CGWork.h"
#include "BlurDlg.h"
#include "afxdialogex.h"


// BlurDlg dialog

IMPLEMENT_DYNAMIC(BlurDlg, CDialogEx)

BlurDlg::BlurDlg(CCGWorkView* parent, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BLUR_DLG, pParent), pview(parent)
{

}

BlurDlg::~BlurDlg()
{
}

void BlurDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BLUR_ENABLE, enable);
	DDX_Control(pDX, IDC_BLUR_FACTOR, factor);
}

BOOL BlurDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	enable.SetCheck(pview->scene.display_motionBlur);
	CString str;
	str.Format(L"%.2f", pview->scene.motionBlur_t);
	factor.SetWindowText(str);

	return 0;
}


BEGIN_MESSAGE_MAP(BlurDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &BlurDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// BlurDlg message handlers


void BlurDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	pview->scene.display_motionBlur = enable.GetCheck() == BST_CHECKED;

	CString str;
	factor.GetWindowTextW(str);
	pview->scene.motionBlur_t = std::stof(str.GetString());
}
