// CChangeExtensionDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Multiple-Image-Editor.h"
#include "afxdialogex.h"
#include "CChangeExtensionDlg.h"
#include <comdef.h>
#include <afxpriv.h>
#include <string>
#include "CFileManager.h"


// CChangeExtensionDlg 대화 상자

IMPLEMENT_DYNAMIC(CChangeExtensionDlg, CDialogEx)

CChangeExtensionDlg::CChangeExtensionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHANGE_EXTENSION_DIALOG, pParent)
{

}

CChangeExtensionDlg::~CChangeExtensionDlg()
{
}

/**
 * \brief
 * \param imageFormat 이미지 포맷 변경
 */
void CChangeExtensionDlg::ChangeExtension(GUID imageFormat)
{
	if (m_FileListBox.GetCount() > 0)
	{
		CString filePath;

		CImage originalImage;

		CString* paths = new CString[m_FileListBox.GetCount()];
		bool* isSuccess = new bool[m_FileListBox.GetCount()];

		for (int i = 0; i < m_FileListBox.GetCount(); ++i)
		{
			m_FileListBox.GetText(i, filePath);
			filePath.AppendChar('a');
			paths[i] = filePath;
			isSuccess[i] = false;
		}

		for (int i = 0; i < m_FileListBox.GetCount(); ++i)
		{
			m_FileListBox.GetText(i, filePath);

			try
			{
				originalImage.Load(filePath);

				CString path = filePath.Left(filePath.ReverseFind('.'));
				CString oldExtension = filePath.Right(filePath.GetLength() - filePath.ReverseFind('.') - 1);
				CString extension;
				if (imageFormat == Gdiplus::ImageFormatBMP)
				{
					extension = "bmp";
				}
				else if (imageFormat == Gdiplus::ImageFormatPNG)
				{
					extension = "png";
				}
				else if (imageFormat == Gdiplus::ImageFormatJPEG)
				{
					extension = "jpg";
				}
				else if (imageFormat == Gdiplus::ImageFormatTIFF)
				{
					extension = "tiff";
				}
				else
				{
					extension = "png";
				}

				if(extension.CompareNoCase(oldExtension) == 0)
				{
					originalImage.Destroy();
					continue;
				}

				CString newPath;
				newPath.Format(_T("%s.%s"), path, extension);

				originalImage.Save(newPath, imageFormat);

				isSuccess[i] = true;

				originalImage.Destroy();
			}
			catch (...)
			{
				originalImage.Destroy();
			}
		}
		for (int i = 0; i < m_FileListBox.GetCount(); ++i)
		{
			if (isSuccess[i])
			{
				/*wchar_t* wchar_str;
				char* char_str;
				int char_str_len;

				wchar_str = T2W(paths[i].GetBuffer(0));

				char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
				char_str = new char[char_str_len];

				WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);*/

				//remove(char_str);
				CFileManager::RemoveFile(paths[i]);
			}
		}

		delete[] isSuccess;
		delete[] paths;
		MessageBox(L"파일 확장자 변경에 성공하였습니다", L"결과", MB_OK);
		m_FileListBox.ResetContent();
	}
}

void CChangeExtensionDlg::CreateHorizontalScroll()
{
	CString str; CSize sz; int dx = 0;
	CDC* pDC = m_FileListBox.GetDC();
	TEXTMETRIC tm;

	pDC->GetTextMetrics(&tm);

	for (int i = 0; i < m_FileListBox.GetCount(); i++)
	{
		m_FileListBox.GetText(i, str);
		sz = pDC->GetTextExtent(str);

		sz.cx += tm.tmAveCharWidth;
		sz.cx += 170;

		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_FileListBox.ReleaseDC(pDC);

	if (m_FileListBox.GetHorizontalExtent() < dx)
	{
		m_FileListBox.SetHorizontalExtent(dx);
		ASSERT(m_FileListBox.GetHorizontalExtent() == dx);
	}
}

void CChangeExtensionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_FileListBox);
}


BEGIN_MESSAGE_MAP(CChangeExtensionDlg, CDialogEx)
	ON_BN_CLICKED(IDC_ALL_RESET_BUTTON, &CChangeExtensionDlg::OnBnClickedAllResetButton)
	ON_BN_CLICKED(IDC_SELECT_DELECT_BUTTON, &CChangeExtensionDlg::OnBnClickedSelectDelectButton)
	ON_BN_CLICKED(IDC_CHANGE_BMP_BUTTON, &CChangeExtensionDlg::OnBnClickedChangeBmpButton)
	ON_BN_CLICKED(IDC_CHANGE_PNG_BUTTON, &CChangeExtensionDlg::OnBnClickedChangePngButton)
	ON_BN_CLICKED(IDC_CHANGE_JPG_BUTTON, &CChangeExtensionDlg::OnBnClickedChangeJpgButton)
	ON_BN_CLICKED(IDC_CHANGE_TIFF_BUTTON, &CChangeExtensionDlg::OnBnClickedChangeTiffButton)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CChangeExtensionDlg 메시지 처리기


void CChangeExtensionDlg::OnBnClickedAllResetButton()
{
	m_FileListBox.ResetContent();
}


void CChangeExtensionDlg::OnBnClickedSelectDelectButton()
{
	if (LB_ERR != m_FileListBox.GetCurSel())
	{
		m_FileListBox.DeleteString(m_FileListBox.GetCurSel());
	}

}

void CChangeExtensionDlg::OnBnClickedChangeBmpButton()
{
	ChangeExtension(Gdiplus::ImageFormatBMP);
}

void CChangeExtensionDlg::OnBnClickedChangePngButton()
{
	ChangeExtension(Gdiplus::ImageFormatPNG);
}

void CChangeExtensionDlg::OnBnClickedChangeJpgButton()
{
	ChangeExtension(Gdiplus::ImageFormatJPEG);
}

void CChangeExtensionDlg::OnBnClickedChangeTiffButton()
{
	ChangeExtension(Gdiplus::ImageFormatTIFF);
}


void CChangeExtensionDlg::OnDropFiles(HDROP hDropInfo)
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

		while ((nIndex = m_FileListBox.FindStringExact(-1, filePath)) != LB_ERR)
		{
			m_FileListBox.DeleteString(nIndex);
		}
		m_FileListBox.AddString(filePath);
	}

	::DragFinish(hDropInfo);

	CreateHorizontalScroll();

	CDialogEx::OnDropFiles(hDropInfo);
}
