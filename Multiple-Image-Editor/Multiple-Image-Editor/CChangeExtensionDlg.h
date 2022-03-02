#pragma once
#include "afxdialogex.h"


// CChangeExtensionDlg 대화 상자

class CChangeExtensionDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeExtensionDlg)

public:
	CChangeExtensionDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CChangeExtensionDlg();
private:
	void ChangeExtension(GUID imageFormat);
	void CreateHorizontalScroll();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGE_EXTENSION_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAllResetButton();
	afx_msg void OnBnClickedSelectDelectButton();
	afx_msg void OnBnClickedChangeBmpButton();
	afx_msg void OnBnClickedChangePngButton();
	afx_msg void OnBnClickedChangeJpgButton();
	afx_msg void OnBnClickedChangeTiffButton();
private:
	CListBox m_FileListBox;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
