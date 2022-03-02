#include "pch.h"
#include "CFileManager.h"

void CFileManager::RemoveFile(CString strFilePath)
{
	CString file = L"";
	file += strFilePath;

	SHFILEOPSTRUCT FileOp = { 0 };
	char szTemp[MAX_PATH];

	wchar_t* wchar_str;
	char* char_str;
	int char_str_len;

	wchar_str = strFilePath.GetBuffer(strFilePath.GetLength());

	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	char_str = new char[char_str_len];

	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);

	strcpy_s(szTemp, char_str);

	FileOp.hwnd = NULL;
	FileOp.wFunc = FO_DELETE; // 삭제 속성 설정
	FileOp.pFrom = NULL;
	FileOp.pTo = NULL;
	// 확인메시지가 안뜨도록 설정
	FileOp.fFlags = FOF_ALLOWUNDO | FOF_NOCONFIRMATION | FOF_NOERRORUI;
	FileOp.fAnyOperationsAborted = false;
	FileOp.hNameMappings = NULL;
	FileOp.lpszProgressTitle = NULL;
	USES_CONVERSION;
	WCHAR* w = wchar_str;
	w[strFilePath.GetLength() - 1] = '\0';
	w[strFilePath.GetLength()] = '\0';
	FileOp.pFrom = w;

	SHFileOperation(&FileOp); // 삭제 작업

	delete[] char_str;
}
