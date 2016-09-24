#pragma once
#include <string>

// CBatchTest 对话框

class CBatchTest : public CDialogEx
{
	DECLARE_DYNAMIC(CBatchTest)

public:
	CBatchTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBatchTest();

// 对话框数据
	enum { IDD = IDD_TESTPR_BATCH };

protected:
	CListCtrl* m_BatchList;
	CString m_folderPath;
	CString m_plateResult;
	std::string m_srcPath;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void processPlate(CString platePath);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedChooseBtn();
	afx_msg void OnBnClickedStartBtn();
};
