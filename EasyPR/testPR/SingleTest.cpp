
// testPRDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "testPR.h"
#include "SingleTest.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include "resource.h"

#include "TestEasyPr.h"

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

	((CComboBox*)GetDlgItem(IDC_COMBO1))->InsertString(4, _T("gray and binary"));

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
			IplImage* pShowImg = resizeImage();

			DrawPicToHDC(pShowImg, SRC_STATIC);
		}
	}
}


//load image from specified path and show it in the picture box
void CSingleTest::OnBnClickedButton1()
{
	CFileDialog openfile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);
	INT_PTR result = openfile.DoModal();

	if (result == IDOK)
	{		
		//Get image path
		CString tmpSrcPath = openfile.GetPathName();

		USES_CONVERSION;
		string tmps_srcPath(W2A(tmpSrcPath));
		m_srcPath = tmps_srcPath;

		IplImage* pShowImg = resizeImage();

		DrawPicToHDC(pShowImg, SRC_STATIC);

	}
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

IplImage* CSingleTest::resizeImage()
{
	IplImage* pSrcImage = cvLoadImage(m_srcPath.c_str(), CV_LOAD_IMAGE_UNCHANGED);
	IplImage* pDstImage = NULL;

	CRect rectPC;
	//get the size of Picture Control 
	GetDlgItem(SRC_STATIC)->GetWindowRect(&rectPC);

	double fWidth = (double)(rectPC.Width()) / (pSrcImage->width);
	double fHeight = (double)(rectPC.Height()) / (pSrcImage->height);

	if ((fWidth < 1) || (fHeight < 1))
	{
		double fScale = (fWidth > fHeight) ? fHeight:fWidth;

		CvSize czSize;              //目标图像尺寸  
		//计算目标图像大小  
		czSize.width = pSrcImage->width * fScale;
		czSize.height = pSrcImage->height * fScale;

		//创建图像并缩放  
		pDstImage = cvCreateImage(czSize, pSrcImage->depth, pSrcImage->nChannels);
		cvResize(pSrcImage, pDstImage, CV_INTER_AREA);
	}
	else
	{
		pDstImage = pSrcImage;
	}

	return pDstImage;
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
	size_t num = resultVec.size();
	if ((result == 0) && (num > 0))
	{
		cv::Mat resultMat,comMat;
		
		size_t j = 1;
		comMat = resultVec[0];
		while (j < num)
		{
			resultMat = resultVec[j];
			vconcat(comMat, resultMat, comMat);
			j++;
		}
		if ((comMat.rows > 0) && (comMat.cols > 0))
		{
			imshow("view", comMat);
		}
		
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
	int resultLo = lo.plateLocate(src, matVec);

	if (0 != resultLo) return -1;

	int resultJu = PlateJudge::instance()->plateJudge(matVec, resultVec);

	if (0 != resultJu) return -1;

	cv::Mat resultMat, comMat;
	size_t num = resultVec.size();

	if (num > 0)
	{

		size_t j = 1;

		comMat = resultVec[0];
		while (j < num)
		{
			resultMat = resultVec[j];
			vconcat(comMat, resultMat, comMat);
			j++;
		}
		if ((comMat.rows > 0) && (comMat.cols > 0))
		{
			imshow("view", comMat);
		}
	}

	return resultJu;
}

int CSingleTest::test_plate_detect()
{
	cv::Mat src = imread(m_srcPath);

	vector<CPlate> resultVec;

	int result = m_single_pd.plateDetect(src, resultVec);
	size_t num = resultVec.size();

	if ((0 == result) && (num > 0))
	{
		cv::Mat resultMat, comMat;
		size_t j = 1;
		comMat = resultVec[0].getPlateMat();
		while (j < num)
		{
			resultMat = resultVec[j].getPlateMat();
			vconcat(comMat, resultMat, comMat);
			j++;
		}
		if ((comMat.rows > 0) && (comMat.cols > 0))
		{
			imshow("view", comMat);
		}
	}

	return result;
}

int CSingleTest::test_chars_segment() {

	cv::Mat src = cv::imread(m_srcPath);

	std::vector<cv::Mat> resultVec;
	vector<CPlate> plateVec;
	CCharsSegment plate;
	int result = 1;
	cv::Mat newMat, comHMat, comVMat;


	int resultPD = m_single_pd.plateDetect(src, plateVec);

	if (resultPD != 0)
	{
		return resultPD;
	}

	size_t numPlate = plateVec.size();

	for (size_t j = 0; j < numPlate; j++)
	{
		CPlate item = plateVec.at(j);

		Mat plateMat = item.getPlateMat();

		result = plate.charsSegment(plateMat, resultVec);

		if (result == 0) {

			size_t num = resultVec.size();
			size_t i = 1;
			comHMat = resultVec[0];
			while (i < num)
			{
				newMat = resultVec[i];
				hconcat(comHMat, newMat, comHMat);
				i++;
			}
		
		}
		if (j == 0)
		{
			comVMat = comHMat;
		}
		else
		{
			vconcat(comVMat, comHMat, comVMat);
		}
		
	}

	if ((comVMat.rows > 0) && (comVMat.cols > 0))
	{
		imshow("view", comVMat);
	}
	

	return result;
}


int CSingleTest::test_gray_Binary()
{
	cv::Mat src = cv::imread(m_srcPath);

	std::vector<cv::Mat> resultVec;
	vector<CPlate> plateVec;
	CCharsSegment plate;
	int result = 1;
	cv::Mat grayMat, binaMat, comHMat, comVMat;
	

	int resultPD = m_single_pd.plateDetect(src, plateVec);

	if (resultPD != 0)
	{
		return resultPD;
	}

	size_t numPlate = plateVec.size();

	for (size_t j = 0; j < numPlate; j++)
	{
		CPlate item = plateVec.at(j);

		Mat plateMat = item.getPlateMat();

		cvtColor(plateMat, grayMat, CV_BGR2GRAY);

		Color color;
		if (item.getPlateLocateType() == CMSER) {
			color = item.getPlateColor();
		}
		else {
			int w = plateMat.cols;
			int h = plateMat.rows;
			Mat tmpMat = plateMat(Rect_<double>(w * 0.1, h * 0.1, w * 0.8, h * 0.8));
			color = getPlateType(tmpMat, true);
		}
		binaMat = grayMat.clone();
		spatial_ostu(binaMat, 8, 2, color);

		hconcat(grayMat, binaMat, comHMat);

		
		if (j == 0)
		{
			comVMat = comHMat;
		}
		else
		{
			vconcat(comVMat, comHMat, comVMat);
		}

	}

	if ((comVMat.rows > 0) && (comVMat.cols > 0))
	{
		imshow("view", comVMat);
	}


	return result;
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

	case 4:
		test_gray_Binary();
		::ShowWindow(hWnd, SW_SHOW);
		break;
	default:
		break;

	}

}

//Get the recognition result of the loaded plate
void CSingleTest::OnBnClickedButton()
{
	vector<CPlate> plateVec;
	Mat src = imread(m_srcPath);//m_srcPath是std::string类型，记录用户当前选中的待识别图片的路径
	int result = m_single_pr.plateRecognize(src, plateVec);

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
