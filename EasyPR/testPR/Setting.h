#include "SingleTest.h"
#include "BatchTest.h"

#pragma once


// CSetting 对话框

class CSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CSetting)

public:
	CSetting(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetting();

	int m_detect_Type = 0;

	bool m_bLifeMode = false;
// 对话框数据
	enum { IDD = SETTING_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	CSingleTest m_single;
	CBatchTest m_batch;
public:
	afx_msg void OnBnClickedOk();
};
