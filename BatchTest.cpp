// BatchTest.cpp : 实现文件
//

#include "stdafx.h"
#include "testPR.h"
#include "BatchTest.h"
#include "afxdialogex.h"
#include "easypr.h"

using namespace easypr;
using namespace cv;
using namespace std;
using namespace api;
// CBatchTest 对话框

IMPLEMENT_DYNAMIC(CBatchTest, CDialogEx)

CBatchTest::CBatchTest(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBatchTest::IDD, pParent)
{

}

CBatchTest::~CBatchTest()
{
}

void CBatchTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CBatchTest, CDialogEx)
	ON_BN_CLICKED(BATCH_CHOOSE_BTN, &CBatchTest::OnBnClickedChooseBtn)
	ON_BN_CLICKED(BATCH_START_BTN, &CBatchTest::OnBnClickedStartBtn)
END_MESSAGE_MAP()

BOOL CBatchTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_BatchList = ((CListCtrl*)GetDlgItem(BTACH_SHOW_LIST));

	//获得原有风格
	DWORD dwStyle = ::GetWindowLong(m_BatchList->m_hWnd, GWL_STYLE);
	dwStyle &= ~(LVS_TYPEMASK);
	dwStyle &= ~(LVS_EDITLABELS);
	//设置新风格
	SetWindowLong(m_BatchList->m_hWnd, GWL_STYLE, dwStyle | LVS_REPORT | LVS_NOLABELWRAP | LVS_SHOWSELALWAYS);
	//设置扩展风格
	DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	ListView_SetExtendedListViewStyleEx(m_BatchList->m_hWnd, styles, styles);
	/*
	其中LVS_EX_FULLROWSELECT　就是前面说得整行选中
		LVS_EX_GRIDLINES　网格线（只适用与report风格的listctrl）
		LVS_EX_CHECKBOXES　前面加个checkbox
		*/
	m_BatchList->SetExtendedStyle(m_BatchList->GetExtendedStyle() | LVS_EX_SUBITEMIMAGES);

	m_BatchList->DeleteAllItems();//清空
	m_BatchList->InsertColumn(0, _T("file name"));//添加列
	m_BatchList->InsertColumn(1, _T("plate Detect"));
	m_BatchList->InsertColumn(2, _T("get Plate Color"));
	m_BatchList->InsertColumn(3, _T("chars Recognise"));
	m_BatchList->SetColumnWidth(0, 135);//设置列宽
	m_BatchList->SetColumnWidth(1, 135);
	m_BatchList->SetColumnWidth(2, 135);//设置列宽
	m_BatchList->SetColumnWidth(3, 135);

	//BATCH_PATH_EDIT
	

	/*
 
	m_BatchList->InsertItem(1, _T("Item 1_1"));//插入行
	m_BatchList->SetItemText(1, 1, _T("Item 1_2"));//设置该行的不同列的显示字符
	m_BatchList->SetItemText(1, 2, _T("Item 1_3"));
	*/


	return TRUE;
}
// CBatchTest 消息处理程序


void CBatchTest::OnBnClickedChooseBtn()
{
	TCHAR           szFolderPath[MAX_PATH] = { 0 };
	CString         strFolderPath = TEXT("");

	BROWSEINFO      sInfo;
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));
	sInfo.pidlRoot = 0;
	sInfo.lpszTitle = _T("Please choose a folder:");
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_EDITBOX;
	sInfo.lpfn = NULL;

	// 显示文件夹选择对话框  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);
	if (lpidlBrowse != NULL)
	{
		// 取得文件夹名  
		if (::SHGetPathFromIDList(lpidlBrowse, szFolderPath))
		{
			m_folderPath = szFolderPath;
			GetDlgItem(BATCH_PATH_EDIT)->SetWindowTextW(m_folderPath);
		}
	}
	if (lpidlBrowse != NULL)
	{
		::CoTaskMemFree(lpidlBrowse);
	}
}


void CBatchTest::OnBnClickedStartBtn()
{
	CFileFind fileFinder;//类CFileFind执行本地文件查找  
	CString strPicFile = TEXT("");//  
	CString strFilePath;//保存文件路径  
	CString strFileName;//保存文件名  
	int nIndex = 0;
	do
	{
		if (m_folderPath.Right(1) == TEXT("\\"))
		{
			int nPos = m_folderPath.ReverseFind(TEXT('\\'));
			m_folderPath = m_folderPath.Left(nPos);
		}
		strPicFile.Format(TEXT("%s\\%s"), m_folderPath, TEXT("*.jpg"));//只选择jpg格式的图片  
		BOOL bWorking = fileFinder.FindFile(strPicFile);
		while (bWorking)
		{
			bWorking = fileFinder.FindNextFile();
			if (fileFinder.IsDots())//IsDots判断是否为点,由CFileFind对象引用IsDots的意思是：这是一个目录并且不是这个目录本身或者上层目录  
			{
				continue;
			}
			strFilePath = fileFinder.GetFilePath();//图片的完整路径  
			strFileName = fileFinder.GetFileName();//图片文件的名字  
			if (fileFinder.IsDirectory())//检查是否是文件夹，是返回true，不是返回false  
			{
				//继续遍历目录  
				continue;
			}
			else
			{
				int nPos = strFilePath.ReverseFind(TEXT('.'));
				CString strExt = strFilePath.Right(strFilePath.GetLength() - nPos - 1);
				if (strExt.CompareNoCase(TEXT("jpg")) == 0 ||
					strExt.CompareNoCase(TEXT("jpeg")) == 0 ||
					strExt.CompareNoCase(TEXT("bmp")) == 0)
				{
					//要进行的批量操作  
					processPlate(strFilePath);

					m_BatchList->InsertItem(nIndex, strFileName);//插入行
					m_BatchList->SetItemText(nIndex, 1, m_plateResult);//设置该行的不同列的显示字符
					nIndex++;
				}
			}
		}
	} while (fileFinder.FindNextFile());

	fileFinder.Close();
}


void CBatchTest::processPlate(CString platePath)
{
	std::vector<CPlate> plateVec;
	USES_CONVERSION;
	string tmps_srcPath(W2A(platePath));
	cv::Mat src = imread(tmps_srcPath);
	CPlateDetect pd;
	CCharsRecognise cr;
	int resultPD = pd.plateDetect(src, plateVec);

	if (resultPD == 0)
	{

		size_t num = plateVec.size();
		int index = 0;

		for (size_t j = 0; j < num; j++)
		{
			CPlate item = plateVec.at(j);
			Mat plateMat = item.getPlateMat();

			Color color = item.getPlateColor();
			if (color == UNKNOWN)
			{
				color = getPlateType(plateMat, true);
				item.setPlateColor(color);
			}

			std::string plateColor = cr.getPlateColor(color);


			std::string plateIdentify = "";
			int resultCR = cr.charsRecognise(item, plateIdentify);

			m_plateResult = plateIdentify.c_str();
		}
	}
}