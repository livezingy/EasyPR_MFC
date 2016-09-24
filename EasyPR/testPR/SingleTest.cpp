
// testPRDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "testPR.h"
#include "SingleTest.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include "resource.h"
#include "easypr.h"
#include "TestEasyPr.h"


using namespace easypr;
using namespace cv;
using namespace std;
using namespace api;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestPRDlg 对话框



CSingleTest::CSingleTest(CWnd* pParent /*=NULL*/)
: CDialogEx(CSingleTest::IDD, pParent)
{
	
}

void CSingleTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSingleTest, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON1, &CSingleTest::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CSingleTest::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(Result_BUTTON, &CSingleTest::OnBnClickedButton)
	ON_BN_CLICKED(PROCESS_BUTTON, &CSingleTest::OnBnClickedProcessButton)
END_MESSAGE_MAP()


// CtestPRDlg 消息处理程序
BOOL CSingleTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	GetClientRect(&m_dlgRect);

	//initial the CComboBox
	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(0, _T("plate locate"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(1, _T("plate judge"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(2, _T("plate detect"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(3, _T("chars segment"));

	((CComboBox*)GetDlgItem(IDC_COMBO1))->SetCurSel(0);

	
	namedWindow("view", 1);
	HWND hWnd = (HWND)cvGetWindowHandle("view");
	HWND hParent = ::GetParent(hWnd);
	::SetParent(hWnd, GetDlgItem(SRC_STATIC)->m_hWnd);
	::ShowWindow(hParent, SW_HIDE);
	::ShowWindow(hWnd, SW_HIDE);
	

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSingleTest::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		if (m_srcPath != "")
		{
			CRect   rectPC;
			CWnd *pWnd = NULL;
			pWnd = GetDlgItem(SRC_STATIC);
			pWnd->GetClientRect(&rectPC);

			CDC *pDc = NULL;
			pDc = pWnd->GetDC();

			//draw the picture in the Picture Control 
			m_sImage.DrawToHDC(pDc->m_hDC, rectPC);

			ReleaseDC(pDc);
		}
	}
}


//load image from specified path and show it in the picture box
void CSingleTest::OnBnClickedButton1()
{
	CFileDialog openfile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
	INT_PTR result = openfile.DoModal();

	int cx, cy;		
	CRect rectPC;
	
	//get the size of Picture Control 
	GetDlgItem(SOURCE_IMAGE)->GetWindowRect(&rectPC);

	if (result == IDOK)
	{
		//Get image path
		CString tmpSrcPath = openfile.GetPathName();

		USES_CONVERSION;
		string tmps_srcPath(W2A(tmpSrcPath));
		m_srcPath = tmps_srcPath;
		//load the selected image 
		m_sImage.Load(m_srcPath.c_str());

		cx = m_sImage.Width() - rectPC.Width();
		cy = m_sImage.Height() - rectPC.Height();
	}
	else
	{
		cx = 0;
		cy = 0;
	}

		
	//Get the width and Height of the image

	/*
	CRect testRec;
	CTestEasyPr *TestEasyPr = CTestEasyPr::m_testDlg;
	TestEasyPr->GetWindowRect(testRec);
	TestEasyPr->SetWindowPos(NULL, 0, 0, testRec.Width() + cx, testRec.Height() + cy, SWP_NOZORDER | SWP_NOMOVE);
	*/

	CPoint topLeft = m_dlgRect.TopLeft();
	CPoint bottomRight = { (m_dlgRect.Width() + cx), (m_dlgRect.Height() + cy) };

	m_dlgRect.SetRect(topLeft, bottomRight);

	//SWP_NOZORDER:忽略第一个参数
	//SWP_NOMOVE:忽略参数x,y，维持位置不变
	CWnd::SetWindowPos(NULL, 0, 0, m_dlgRect.Width(), m_dlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

	ChangeSize(SOURCE_IMAGE, cx, cy);
	ChangeSize(SRC_STATIC, cx, cy);
	//ChangeSize(IDC_EASYPRTAB, cx, cy);

	GetDlgItem(SRC_STATIC)->GetWindowRect(&rectPC);
		
	//Get the size of the Picture
	CWnd *pWnd = NULL;
	pWnd = GetDlgItem(SRC_STATIC);
	pWnd->GetClientRect(&rectPC);

	CDC *pDc = NULL;
	pDc = pWnd->GetDC();
		
	//draw the picture in the Picture Control 
	m_sImage.DrawToHDC(pDc->m_hDC, rectPC);

	ReleaseDC(pDc);
	//}

}

void CSingleTest::ChangeSize(UINT nID, int x, int y)  //nID为控件ID，x,y分别为对话框的当前长和宽
{
	CWnd *pWnd;
	pWnd = GetDlgItem(nID);
	if (pWnd != NULL)     //判断是否为空，因为在窗口创建的时候也会调用OnSize函数，但是此时各个控件还没有创建，Pwnd为空
	{
		CRect rec;

		pWnd->GetWindowRect(&rec);  //获取控件变化前的大小
		ScreenToClient(&rec);   //将控件大小装换位在对话框中的区域坐标

		rec.left = rec.left;  //按照比例调整空间的新位置
		rec.top = rec.top;
		rec.bottom = rec.bottom + y;
		rec.right = rec.right + x;
		pWnd->MoveWindow(rec);   //伸缩控件
	}
}

int CSingleTest::test_plate_locate()
{
	cv::Mat src = imread(m_srcPath);

	vector<cv::Mat> resultVec;
	CPlateLocate plate;

	int result = plate.plateLocate(src, resultVec);
	if (result == 0) 
	{
		cv::Mat resultMat,comMat;
		size_t num = resultVec.size();
		size_t j = 1;
		comMat = resultVec[0];
		while (j < num)
		{
			resultMat = resultVec[j];
			vconcat(comMat, resultMat, comMat);
			j++;
		}

		imshow("view", comMat);
	}

	return result;
}

int CSingleTest::test_plate_judge()
{
	cv::Mat src = imread(m_srcPath);

	vector<cv::Mat> matVec;

	vector<cv::Mat> resultVec;

	CPlateLocate lo;
	lo.setDebug(1);
	lo.setLifemode(true);

	int resultLo = lo.plateLocate(src, matVec);

	if (0 != resultLo) return -1;

	int resultJu = PlateJudge::instance()->plateJudge(matVec, resultVec);

	if (0 != resultJu) return -1;

	cv::Mat resultMat, comMat;
	size_t num = resultVec.size();
	size_t j = 1;
	comMat = resultVec[0];
	while (j < num)
	{
		resultMat = resultVec[j];
		vconcat(comMat, resultMat, comMat);
		j++;
	}

	imshow("view", comMat);

	return resultJu;
}

int CSingleTest::test_plate_detect()
{
	cv::Mat src = imread(m_srcPath);

	vector<CPlate> resultVec;
	CPlateDetect pd;
	pd.setPDLifemode(true);

	int result = pd.plateDetect(src, resultVec,0,true);

	return result;
}

int CSingleTest::test_chars_segment() {

	cv::Mat src = cv::imread(m_srcPath);

	std::vector<cv::Mat> resultVec;
	vector<CPlate> plateVec;
	CCharsSegment plate;
	CPlateDetect pd;
	int result;
	cv::Mat resultMat, comMat;

	int resultPD = pd.plateDetect(src, plateVec);

	size_t numPlate = plateVec.size();

	for (size_t j = 0; j < numPlate; j++)
	{
		CPlate item = plateVec.at(j);

		Mat plateMat = item.getPlateMat();

		result = plate.charsSegment(plateMat, resultVec);

		if (result == 0) {

			size_t num = resultVec.size();
			size_t i = 1;
			comMat = resultVec[0];
			while (i < num)
			{
				resultMat = resultVec[i];
				hconcat(comMat, resultMat, comMat);
				i++;
			}
		}
	}

	imshow("view", comMat);

	return result;
}

void CSingleTest::ShowIplImage(IplImage* img) {
	CDC* pDC = GetDC();
	HDC hDC = pDC->GetSafeHdc(); 
	CRect rect; 
	rect.SetRect(0, 0, img->width, img->height); 
	CvvImage cimg; 
	cimg.CopyOf(img); 
	cimg.DrawToHDC(hDC, &rect); 
	ReleaseDC(pDC);
}



void CSingleTest::DrawPicToHDC(IplImage* iplimg, UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(ID)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(iplimg);
	cimg.DrawToHDC(hDC, &rect);
	ReleaseDC(pDC);
}


//choose the plate process to observer
void CSingleTest::OnCbnSelchangeCombo1()
{

}

void CSingleTest::OnBnClickedProcessButton()
{
	HWND hWnd = (HWND)cvGetWindowHandle("view");

	int tmpCPos = ((CComboBox*)GetDlgItem(IDC_COMBO1))->GetCurSel();

	switch (tmpCPos)
	{
	case 0:
		test_plate_locate();
		::ShowWindow(hWnd, SW_SHOW);
		break;
	case 1:
		test_plate_judge();
		::ShowWindow(hWnd, SW_SHOW);
		break;
	case 2:
		test_plate_detect();
		break;
	case 3:
		test_chars_segment();
		::ShowWindow(hWnd, SW_SHOW);
		break;
	default:
		break;

	}

}

//Get the recognition result of the loaded plate
void CSingleTest::OnBnClickedButton()
{
	CPlateRecognize pr;
	pr.setLifemode(true);
	pr.setDebug(false);
	pr.setMaxPlates(4);
	pr.setDetectType(easypr::PR_DETECT_CMSER);

	vector<CPlate> plateVec;
	Mat src = imread(m_srcPath);//m_srcPath是std::string类型，记录用户当前选中的待识别图片的路径
	int result = pr.plateRecognize(src, plateVec);

	if (result == 0)
	{
		size_t num = plateVec.size();

		CString resultStr;

		for (size_t j = 0; j < num; j++)
		{
			resultStr += plateVec[j].getPlateStr().c_str();//string to constant char*
		}

		SetDlgItemText(RESULT_EDIT2, resultStr);
	}
}
