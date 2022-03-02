// CChangeNameDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Multiple-Image-Editor.h"
#include "afxdialogex.h"
#include "CChangeNameDlg.h"

#include <string>
#include <iostream>
#include <io.h>
#include <fstream>
#include <atlstr.h>
using namespace std;


// CChangeNameDlg 대화 상자

IMPLEMENT_DYNAMIC(CChangeNameDlg, CDialogEx)

CChangeNameDlg::CChangeNameDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGE_NAME_DIALOG, pParent)
	, m_FileNameEdit(_T(""))
{
}

CChangeNameDlg::~CChangeNameDlg()
{
}

void CChangeNameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_NameDialogFileListView);
	DDX_Text(pDX, IDC_EDIT1, m_FileNameEdit);
	DDV_MaxChars(pDX, m_FileNameEdit, 100);
	GetDlgItem(IDC_CHANGE_APPLY_BUTTON)->EnableWindow(FALSE);
	DDX_Control(pDX, IDC_NAME_EDIT, m_NameEdit);
	m_IsRemoveText = false;
}


BEGIN_MESSAGE_MAP(CChangeNameDlg, CDialogEx)
	ON_WM_DROPFILES()
	//	ON_LBN_SELCHANGE(IDC_LIST1, &CChangeNameDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_SELECT_DELECT_BUTTON, &CChangeNameDlg::SelectDelect)
	ON_BN_CLICKED(IDC_ALL_RESET_BUTTON, &CChangeNameDlg::AllDelect)
	ON_BN_CLICKED(IDC_MOVE_UP_BUTTON, &CChangeNameDlg::MoveUpButton)
	ON_BN_CLICKED(IDC_MOVE_DOWN_BUTTON, &CChangeNameDlg::MoveDownButton)
	ON_BN_CLICKED(IDC_CHANGE_APPLY_BUTTON, &CChangeNameDlg::OnBnClickedChangeApplyButton)
	ON_BN_CLICKED(IDC_HELPBUTTON, &CChangeNameDlg::OnBnClickedHelpButton)
	ON_EN_CHANGE(IDC_NAME_EDIT, &CChangeNameDlg::OnEnChangeNameEdit)
END_MESSAGE_MAP()


// CChangeNameDlg 메시지 처리기

/**
 * \brief
 * \param hDropInfo 파일을 드래그 드롭했을때 리스트 박스에 표시
 */
void CChangeNameDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR FileName[MAX_PATH] = { 0, };
	UINT count = DragQueryFile(hDropInfo, 0xFFFFFFFF, FileName, MAX_PATH);

	int nIndex;

	for (UINT i = 0; i < count; i++)
	{
		DragQueryFile(hDropInfo, i, FileName, MAX_PATH);

		CString filePath = static_cast<LPCTSTR>(FileName);

		CString aaa = filePath.Right(filePath.GetLength() - filePath.ReverseFind('.'));
		CString abc = filePath.Left(filePath.ReverseFind('\\') + 1);

		// 이미지인지 확인해주고 맞으면 리스트에 추가
		CImage image;
		HRESULT result = image.Load(filePath);
		if (!(result == S_OK || result == NOERROR))
		{
			continue;
		}

		while ((nIndex = m_NameDialogFileListView.FindStringExact(-1, filePath)) != LB_ERR)
		{
			m_NameDialogFileListView.DeleteString(nIndex);
		}
		m_NameDialogFileListView.AddString(filePath);
	}

	::DragFinish(hDropInfo);

	CreateHorizontalScroll();

	CheckAbleChangeName();

	CDialogEx::OnDropFiles(hDropInfo);
}

void CChangeNameDlg::CreateHorizontalScroll()
{
	CString str; CSize sz; int dx = 0;
	CDC* pDC = m_NameDialogFileListView.GetDC();
	TEXTMETRIC tm;

	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < m_NameDialogFileListView.GetCount(); i++)
	{
		m_NameDialogFileListView.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		sz.cx += tm.tmAveCharWidth;
		sz.cx += 170;

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_NameDialogFileListView.ReleaseDC(pDC);

	if (m_NameDialogFileListView.GetHorizontalExtent() < dx)
	{
		m_NameDialogFileListView.SetHorizontalExtent(dx);
		ASSERT(m_NameDialogFileListView.GetHorizontalExtent() == dx);
	}
}

void CChangeNameDlg::SelectDelect()
{
	if (LB_ERR != m_NameDialogFileListView.GetCurSel())
	{
		m_NameDialogFileListView.DeleteString(m_NameDialogFileListView.GetCurSel());
	}

	CheckAbleChangeName();
}

void CChangeNameDlg::AllDelect()
{
	m_NameDialogFileListView.ResetContent();
	GetDlgItem(IDC_CHANGE_APPLY_BUTTON)->EnableWindow(FALSE);
}

void CChangeNameDlg::MoveUpButton()
{
	MoveFileSequence(1, m_NameDialogFileListView.GetCount() - 1, true);
}


void CChangeNameDlg::MoveDownButton()
{
	MoveFileSequence(0, m_NameDialogFileListView.GetCount() - 2, false);
}

void CChangeNameDlg::MoveFileSequence(int startIndex, int endIndex, bool isUp)
{
	const int selIndex = m_NameDialogFileListView.GetCurSel();
	if (selIndex == LB_ERR)
	{
		return;
	}

	if ((startIndex > selIndex) || (selIndex > endIndex))
	{
		return;
	}

	CString str;
	m_NameDialogFileListView.GetText(selIndex, str);
	m_NameDialogFileListView.DeleteString(selIndex);
	m_NameDialogFileListView.InsertString(selIndex + (isUp ? -1 : 1), str);
	m_NameDialogFileListView.SetCurSel(selIndex + (isUp ? -1 : 1));
}

void CChangeNameDlg::FileReName(CString filePath, CString newName)
{
	CString tmp = filePath.Left(filePath.ReverseFind('.'));
	CString extension = filePath.Right(filePath.GetLength() - filePath.ReverseFind('.'));
	CString path = filePath.Left(filePath.ReverseFind('\\') + 1);
	//int first = tmp.ReverseFind('\\') + 1;
	//CString fileName = filePath.Mid(first, tmp.GetLength() - first);

	CString fileName;
	int index = 0;
	while (true)
	{
		CString tmp;
		if (index > 0)
		{
			tmp.Format(_T(" (%i)"), index);
		}
		else
		{
			tmp = "";
		}
		fileName.Format(_T("%s%s%s%s"), path, newName, tmp, extension);

		wchar_t* wchar_str;
		char* char_str;
		int char_str_len;

		wchar_str = fileName.GetBuffer(fileName.GetLength());

		char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
		char_str = new char[char_str_len];

		WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);

		if (!((_access(char_str, 0)) != -1))
		{
			break;
		}
		index++;
	}

	CFile::Rename(filePath, fileName);
}

void CChangeNameDlg::CheckAbleChangeName()
{
	if ((m_NameEdit.GetWindowTextLengthW() > 0) && (m_NameDialogFileListView.GetCount() > 0))
	{
		GetDlgItem(IDC_CHANGE_APPLY_BUTTON)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHANGE_APPLY_BUTTON)->EnableWindow(FALSE);
	}
}

/**
 * \brief 이름 적용 버튼
 */
void CChangeNameDlg::OnBnClickedChangeApplyButton()
{
	setlocale(LC_ALL, "");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < m_NameDialogFileListView.GetCount(); ++i)
	{
		CString oldName;
		m_NameDialogFileListView.GetText(i, oldName);

		CString newName;
		m_NameEdit.GetWindowTextW(newName);

		const wchar_t* p_str = L"{num}";

		wchar_t m_intToStr[256];
		swprintf_s(m_intToStr, L"%d", i);

		newName.Replace(p_str, m_intToStr);

		FileReName(oldName, newName);
	}
	MessageBox(L"이름 변경에 성공하였습니다", L"결과", MB_OK);
	m_NameDialogFileListView.ResetContent();
	CString emptyStr;
	emptyStr = "";
	m_NameEdit.SetWindowTextW(emptyStr);
	GetDlgItem(IDC_CHANGE_APPLY_BUTTON)->EnableWindow(FALSE);
}

/**
 * \brief 도움말 버튼
 */
void CChangeNameDlg::OnBnClickedHelpButton()
{
	MessageBox(L"예시) 강아지이미지{num}번\n결과값\n강아지이미지0번\n강아지이미지1번\n강아지이미지2번\n강아지이미지3번", L"도움말", MB_OK);
}


void CChangeNameDlg::OnEnChangeNameEdit()
{
	CString nameEditText;
	m_NameEdit.GetWindowTextW(nameEditText);

	// 입력 취소해야할 문자
	CString cancelKeyword[9];
	cancelKeyword[0] = "\\";
	cancelKeyword[1] = "/";
	cancelKeyword[2] = ":";
	cancelKeyword[3] = "*";
	cancelKeyword[4] = "?";
	cancelKeyword[5] = '"';
	cancelKeyword[6] = "<";
	cancelKeyword[7] = ">";
	cancelKeyword[8] = "|";

	if (!m_IsRemoveText)
	{
		m_IsRemoveText = true;

		for (int i = 0; i < 9; i++)
		{
			const int index = nameEditText.Find(cancelKeyword[i]);
			if (index != -1)
			{
				int start, end;
				m_NameEdit.GetSel(start, end);
				nameEditText.Remove(*cancelKeyword[i]);
				m_NameEdit.SetWindowTextW(nameEditText);
				m_NameEdit.SetSel(start - 1, end - 1);
				break;
			}
		}
	}
	else
	{
		m_IsRemoveText = false;
	}

	CheckAbleChangeName();
}
