
// testPRDlg.h : 头文件
#include <string>
#include "cvvImage.h"
#include "easypr.h"


using namespace easypr;
using namespace cv;
using namespace std;
using namespace api;

#pragma once


// CtestPRDlg 对话框
class CSingleTest : public CDialogEx
{
// 构造
public:
	CSingleTest(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTPR_SINGLE };

	CPlateRecognize m_single_pr;
	CPlateDetect m_single_pd;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


	

// 实现
protected:
	
	CRect m_dlgRect;
	std::string m_srcPath;
	CvvImage  m_sImage;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedButton();
	afx_msg void OnBnClickedProcessButton();

	void ChangeSize(UINT nID, int x, int y);
	void ShowIplImage(IplImage* img);
	void DrawPicToHDC(IplImage* iplimg, UINT ID);
	IplImage* resizeImage();

private:
	HICON m_hIcon;
	int test_plate_locate();
	int test_plate_judge();
	int test_plate_detect();
	int test_plate_recognize();
	int test_chars_segment();
	int test_gray_Binary();
	afx_msg void OnPaint();
	
};
