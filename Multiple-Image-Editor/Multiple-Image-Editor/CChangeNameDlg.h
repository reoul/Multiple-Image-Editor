#pragma once
#include "afxdialogex.h"


// CChangeNameDlg 대화 상자

class CChangeNameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeNameDlg)

public:
	CChangeNameDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CChangeNameDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGE_NAME_DIALOG };
#endif

private:
	void MoveFileSequence(int startIndex, int endIndex, bool isUp);
	void FileReName(CString filePath, CString newName);
	void CheckAbleChangeName();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
	void CreateHorizontalScroll();
//	afx_msg void OnLbnSelchangeList1();
private:
	CListBox m_NameDialogFileListView;
public:
	afx_msg void SelectDelect();
	afx_msg void AllDelect();
	afx_msg void MoveUpButton();
	afx_msg void MoveDownButton();
private:
//	CEdit m_FileNameEdit;
public:
	CString m_FileNameEdit;
	afx_msg void OnBnClickedChangeApplyButton();
	afx_msg void OnBnClickedHelpButton();
	CEdit m_NameEdit;
	bool m_IsRemoveText;
	afx_msg void OnEnChangeNameEdit();
};
