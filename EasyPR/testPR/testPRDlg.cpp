
// testPRDlg.cpp : 实现文件
//


#include "stdafx.h"
#include "testPR.h"
#include "testPRDlg.h"
#include "afxdialogex.h"
#include <atlconv.h>
#include "resource.h"
#include "easypr.h"
#include "cvvImage.h"

using namespace easypr;
using namespace cv;
using namespace std;
using namespace api;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CtestPRDlg 对话框



CtestPRDlg::CtestPRDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CtestPRDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtestPRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestPRDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CtestPRDlg::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CtestPRDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(Result_BUTTON, &CtestPRDlg::OnBnClickedButton)
	ON_BN_CLICKED(PROCESS_BUTTON, &CtestPRDlg::OnBnClickedProcessButton)
END_MESSAGE_MAP()


// CtestPRDlg 消息处理程序

BOOL CtestPRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

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

void CtestPRDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestPRDlg::OnPaint()
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
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestPRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//load image from specified path and show it in the picture box
void CtestPRDlg::OnBnClickedButton1()
{
	CFileDialog openfile(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL);

	if (openfile.DoModal())
	{
		int cx, cy;
		CvvImage  image;
	    CRect   rectPC;

		//Get image path
		CString tmpSrcPath = openfile.GetPathName();

		USES_CONVERSION;
		string tmps_srcPath(W2A(tmpSrcPath));
		m_srcPath = tmps_srcPath;
		//load the selected image 
		image.Load(m_srcPath.c_str());
		//image.load

		//get the size of Picture Control 
		GetDlgItem(SRC_STATIC)->GetWindowRect(&rectPC);
		
		//Get the width and Height of the image
		cx = image.Width() - rectPC.Width();
		cy = image.Height() - rectPC.Height();

		CPoint topLeft = m_dlgRect.TopLeft();
		CPoint bottomRight = { (m_dlgRect.Width() + cx), (m_dlgRect.Height() + cy) };

		m_dlgRect.SetRect(topLeft, bottomRight);
		CWnd::SetWindowPos(NULL, 0, 0, m_dlgRect.Width(), m_dlgRect.Height(), SWP_NOZORDER | SWP_NOMOVE);

		
		ChangeSize(SOURCE_IMAGE, cx, cy);
		ChangeSize(SRC_STATIC, cx, cy);

		GetDlgItem(SRC_STATIC)->GetWindowRect(&rectPC);

		//Get the size of the Picture
		CWnd *pWnd = NULL;
		pWnd = GetDlgItem(SRC_STATIC);
		pWnd->GetClientRect(&rectPC);

		CDC *pDc = NULL;
		pDc = pWnd->GetDC();
		
		//draw the picture in the Picture Control 
		image.DrawToHDC(pDc->m_hDC, rectPC);

		ReleaseDC(pDc);
	}
}

void CtestPRDlg::ChangeSize(UINT nID, int x, int y)  //nID为控件ID，x,y分别为对话框的当前长和宽
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

int CtestPRDlg::test_plate_locate()
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

int CtestPRDlg::test_plate_judge()
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

int CtestPRDlg::test_plate_detect() 
{
	cv::Mat src = imread(m_srcPath);

	vector<CPlate> resultVec;
	CPlateDetect pd;
	pd.setPDLifemode(true);

	int result = pd.plateDetect(src, resultVec,0,true);

	return result;
}

int CtestPRDlg::test_chars_segment() {

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


//choose the plate process to observer
void CtestPRDlg::OnCbnSelchangeCombo1()
{

}

void CtestPRDlg::OnBnClickedProcessButton()
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
void CtestPRDlg::OnBnClickedButton()
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
