#include "Global.h"

//���ֽ�ת��Ϊ���ֽ�
void C2W(WCHAR** dest, const char* src)
{
	if (src == NULL)
	{
		return ;
	}

	size_t alen = strlen(src) + 1;
	size_t  ulen = (size_t)MultiByteToWideChar(CP_ACP, 0, src,alen,NULL, 0 )+1;

	*dest = new WCHAR[ulen];
	::MultiByteToWideChar(CP_ACP, 0, src, alen, *dest, ulen);
}

//���ֽ�ת��Ϊ���ֽ�
void W2C(char** dest, const WCHAR *src)
{
	if(src == NULL)
		return ;
	size_t len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0 , NULL, NULL);
	if (len == 0)
	{
		return;
	}
	*dest = new char[len];
	WideCharToMultiByte( CP_ACP, 0, src, -1, *dest, len, NULL, NULL );

}

void W2C(string& dest,const WCHAR *src)
{
	char* pStr = NULL;
	W2C(&pStr, src);
	dest = pStr;
	SAFE_ARRYDELETE(pStr);
}
//char �ַ���ת��Ϊtchar�ַ���
// void C2T(TCHAR** dest, const char* src)
// {
// #ifdef _UNICODE
// 	if (src == NULL)
// 	{
// 		return ;
// 	}
// 
// 	size_t alen = strlen(src) + 1;
// 	size_t  ulen = (size_t)MultiByteToWideChar(CP_ACP, 0, src,alen,NULL, 0 )+1;
// 
// 	*dest = new WCHAR[ulen];
// 	::MultiByteToWideChar(CP_ACP, 0, src, alen, *dest, ulen);
// #else 
// 	//���ֽ�TCHAR���ǡ������
// 	int len = strlen(src)+1;
// 	*dest = new char[len];
// 	strcpy(*dest, src);
// #endif
// }

void C2T(TCHAR** dest, const char* src)
{
#ifdef _UNICODE
	C2W(dest, src);
#else 
	//���ֽ�TCHAR���ǡ������
	int len = strlen(src)+1;
	*dest = new char[len];
	strcpy(*dest, src);
#endif
}

void T2C(char** dest, const TCHAR* src)
{
#ifdef _UNICODE
	W2C(dest, src);
#else
	int len = _tcslen(src) + 1;
	*dest = new TCHAR[len];
	strcpy(*dest, src);
#endif
}

//ɾ���ַ���ǰ��Ŀո�
char * Trim( char * str )
{     
	int len = strlen(str);    
	int i = 0;     
	int j = 1;    

	//ͳ���ַ���ǰ�˿ո���  
	while ( *(str + i) == ' ')  
	{      
		i++;   
	} 

	//ͳ���ַ�����˿ո���  
	while ( *(str + len - j) == ' ')  
	{      
		j++;
	}    

	//���¼����޼�����ַ�����  
	len = len - i - j + 1;    
	char* newStr = new char[len + 1];  
	for (int p = 0; p < len; p++)   
	{      
		*(newStr + p) = *(str + i + p); 
	}     
	newStr[len] = '\0';    
	return newStr; 
}

//��ȡ��ǰĿ¼
void  GetCurDir(LPTSTR lpStr, int nSize)
{
	GetModuleFileName(NULL, lpStr, nSize); 
	(_tcsrchr(lpStr, _T('\\')))[0] = 0;//ɾ���ļ�����ֻ���·��
}


//��Url�����ص��ļ���
BOOL GetDownLoadZipFileName(const char* url, TCHAR* tstrFname)
{
	const char* p = strrchr(url, '/');

	if (p && strlen(p) < MAX_PATH)
	{
		++p;
		TCHAR*  lptFname = NULL;
		C2T(&lptFname, p);
		_tcscpy(tstrFname,lptFname);
		SAFE_ARRYDELETE(lptFname);
		return TRUE;
	}
	return FALSE;
}

// void T2C(char** dest, const TCHAR* src)
// {
// 	if(src == NULL)
// 		return ;
// #ifdef _UNICODE
// 	size_t len = WideCharToMultiByte(CP_ACP, 0, src, -1, NULL, 0 , NULL, NULL);
// 	if (len == 0)
// 	{
// 		return;
// 	}
// 	*dest = new char[len];
// 	WideCharToMultiByte( CP_ACP, 0, src, -1, *dest, len, NULL, NULL );
// #else
// 	int len = _tcslen(src) + 1;
// 	*dest = new TCHAR[len];
// 	strcpy(*dest, src);
// #endif
// }

BOOL T2C(char*dest, int nSize, const TCHAR* src )
{
	BOOL bRet = FALSE;
	char* pStr = NULL;
	T2C(&pStr, src);
	if (nSize < strlen(pStr)+1)
	{
		//need more buffer 
		bRet = FALSE;
	}
	else
	{
		strcpy(dest, pStr);
		bRet = TRUE;
	}
	return bRet;
}

BOOL FindFile(LPCTSTR lptPath)
{
	WIN32_FIND_DATA wfd;
	HANDLE hFind;
	hFind = FindFirstFile(lptPath, &wfd);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(_T("Invalid file handle�� can't find file or file directory\n"));
		return FALSE;
	}
	else
	{
		//int i =  wfd.dwFileAttributes & FILE_ATTRIBUTE_ARCHIVE; //�����ж��ļ�����
		FindClose(hFind);
		return TRUE;
	}
}

BOOL CreateDir(LPCTSTR lptPath)
{

	//TCHAR* tszPath = IPath::s_tszPath;
	TCHAR tszPath[MAX_PATH+1] = {0x0};
	_tcscpy_s(tszPath, lptPath);
	TCHAR* ptszTok = _tcstok(tszPath, _T("\\"));

	TCHAR tszTempPath[MAX_PATH] = {'\0'};

	while(ptszTok != NULL)
	{
		int pos = 0; //��¼��ǰ�ļ���һ��Ŀ¼����λ�ã��ַ����У�
		if (tszTempPath[0] == '\0')
		{
			_tcscpy_s(tszTempPath, ptszTok);
		}
		else
		{
			pos = _tcslen(tszTempPath);
			_stprintf_s(tszTempPath, _T("%s\\%s"), tszTempPath, ptszTok);
		}


		//res 0: �ļ����ڣ�
		//	  1�������ɹ� 
		//	  -1������ʧ��
		int res = 0; 
		if (!CreateDirectory(tszTempPath, NULL))
		{
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				//�ļ����Ѿ�����
				res = 0;
			else
				//����ʧ��
				res = -1;
		}
		else
			res = 1;

		if (res >= 0)
		{
			//�����ɹ� �����ļ�����
			ptszTok =  _tcstok(NULL, _T("\\"));
		}
		else
		{
			//����ʧ�ܺ� ������� ɾ�� ֮ǰ�Ѿ�������
			tszTempPath[pos] = '\0';
			RemoveDirectory(tszTempPath);
			return FALSE;

		}
	}

	return TRUE;

}

void _tstrreplace(LPTSTR lptStr, TCHAR tSrc, TCHAR tDest)
{
	while(*lptStr != _T('\0'))
	{
		if (*lptStr == tSrc)
		{
			*lptStr  = tDest;
		}
		++lptStr;
	}
}


void tStringToString(std::string& strDest, const tString& tstrSrc)
{
	char* pszStr = NULL;
	T2C(&pszStr, tstrSrc.c_str());
	strDest = pszStr;
	SAFE_ARRYDELETE(pszStr);
}

void tStringToString(std::string& strDest, LPCTSTR lpctSrc)
{
	char* pszStr = NULL;
	T2C(&pszStr, lpctSrc);
	strDest = pszStr;
	SAFE_ARRYDELETE(pszStr);
}

void StringTotString(tString& tstrDest, const std::string& strSrc)
{
	TCHAR* pwszStr = NULL;
	C2T(&pwszStr, strSrc.c_str());
	tstrDest = pwszStr;
	SAFE_ARRYDELETE(pwszStr);
}

void StringTotString(tString& tstrDest, LPCSTR lpcSrc)
{
	TCHAR* pwszStr = NULL;
	C2T(&pwszStr, lpcSrc);
	tstrDest = pwszStr;
	SAFE_ARRYDELETE(pwszStr);
}

void UnicodeToTString(tString& tstrDest, const WCHAR* lpwSrc)
{
#if _UNICODE
	tstrDest = lpwSrc;
#else
	char* pStr = NULL;
	W2C(&pStr, lpwSrc);
	tstrDest = lpwSrc;
	SAFE_ARRYDELETE(pStr);
#endif
}


void Utf8ToUnicode(WCHAR** dest,const char* src)
{
	//ASSERT(dest!= NULL || src != NULL);
	int unicodeLen = ::MultiByteToWideChar( CP_UTF8, 0, src, -1, NULL, 0 ) + 1;  

	*dest = new WCHAR[unicodeLen];
	//memset(*dest, 0x0, (unicodeLen + 1)*sizeof(WCHAR));
	MultiByteToWideChar(CP_UTF8, 0, src, -1, *dest, unicodeLen);

}


void Utf8TotString(tString &strDest, const char* src)
{
	WCHAR* wstr = NULL;
	Utf8ToUnicode(&wstr, src);
#ifdef _UNICODE
	strDest =  wstr;
#else
	char* str = NULL;
	W2C(&str, wstr);
	strDest = str;
	SAFE_ARRYDELETE(str);
#endif
	SAFE_ARRYDELETE(wstr);

}

void UnicodeToUtf8(char** dest , const WCHAR* src)
{
	//ASSERT(dest!= NULL || src != NULL);
	int len = -1;
	len = WideCharToMultiByte(CP_UTF8, 0, src, -1, 0, 0, 0, 0)+1;
	*dest = new char[len+1];
	::WideCharToMultiByte(CP_UTF8, 0, src, -1,*dest, len, 0, 0);
}

void AnsiToUtf8(char** dest, const char* src)
{
	//ASSERT(dest!= NULL || src != NULL);
	WCHAR* pwszStr = NULL;
	C2W(&pwszStr, src);
	UnicodeToUtf8(dest, pwszStr);
	SAFE_ARRYDELETE(pwszStr);
}

// void TcharToUtf8(char** dest, const TCHAR* src)

void tStringToUtf8(string &tstrDest,  LPCTSTR src)
{
	char* p = NULL;
#ifdef _UNICODE
	UnicodeToUtf8(&p, src);
#else
	AnsiToUtf8(&p , src);
#endif
	tstrDest = p;
	SAFE_ARRYDELETE(p);
}

//��ȡһ���ַ����������ַ����м���ַ���
tString GetBetweenString(const TCHAR * pStr, const TCHAR * pStart, const TCHAR * pEnd)
{
	tString strText;

	if (NULL == pStr || NULL == pStart || NULL == pEnd)
		return _T("");

	int nStartLen = _tcslen(pStart);

	const TCHAR * p1 = _tcsstr(pStr, pStart);
	if (NULL == p1)
		return _T("");

	const TCHAR * p2 = _tcsstr(p1+nStartLen, pEnd);
	if (NULL == p2)
		return _T("");

	int nLen = p2-(p1+nStartLen);
	if (nLen <= 0)
		return _T("");

	TCHAR * lpText = new TCHAR[nLen+1];
	if (NULL == lpText)
		return _T("");

	memset(lpText, 0, (nLen+1)*sizeof(TCHAR));
	_tcsncpy(lpText, p1+nStartLen, nLen);
	strText = lpText;
	delete []lpText;

	return strText;
}

#ifdef _DEBUG
void OutputLastError(DWORD dwError)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
		);
	OutputDebugString((LPCTSTR)lpMsgBuf);
	LocalFree( lpMsgBuf );
}

#endif


BOOL GetFileNameFromURL(const char *url, tString& tstrName)
{
	// �ҵ����һ�� /
	const  char *p = strrchr(url, '/');

	// �������һ�� / ����ļ���
	if(p)
	{
		p++;
		StringTotString(tstrName, p);
		return true;
	}
	else
	{
		return false;
	}
}



BOOL DeleteFolder(LPCTSTR lpstrFolder)
{
	TCHAR tszFind[MAX_PATH] = {0x0};
	WIN32_FIND_DATA wfd;
	BOOL bRet ;
	_tcscpy_s(tszFind, MAX_PATH, lpstrFolder);
	_tcscat_s(tszFind, _T("\\*.*"));

	HANDLE hFind  = FindFirstFile(tszFind, &wfd);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		return FALSE;
	}
	bRet = TRUE;
	//�����ļ��н���ɾ��
	while (bRet)
	{
		bRet = FindNextFile(hFind, &wfd);
		//���ǵ�ǰĿ¼���߸�Ŀ¼
		if (!bRet)
		{
			break;
		}
		_tcscpy_s(tszFind, MAX_PATH, lpstrFolder);
		_tcscat_s(tszFind, _T("\\"));
		_tcscat_s(tszFind, wfd.cFileName);

		if (wfd.cFileName[0] != _T('.'))
		{
#if _DEBUG
			TCHAR tszPath[MAX_PATH] = {0x0};
			wsprintf(tszPath,_T("%s\\%s\n"), lpstrFolder, wfd.cFileName);
			OutputDebugString(tszPath);
#endif	
			if (wfd.dwFileAttributes &  FILE_ATTRIBUTE_DIRECTORY)
			{
				//��ͨĿ¼
				DeleteFolder(tszFind);
			}
			else
			{
				if (!DeleteFile(tszFind))
				{
					OutputDebugStringA("Delete fie failed \n");
					FindClose(hFind);
					return FALSE;
				}
				//�ļ�
			}
		}
		else
			continue;
	}
	FindClose(hFind);
	BOOL bb =   RemoveDirectory(lpstrFolder);
	if (!bb)
	{
#ifdef _DEBUG
		OutputLastError(GetLastError());
#endif
	}
	return bb;
}


BOOL GetUrlFromCommandline(tString& tstrPostUrl)
{
	tstrPostUrl = GetCommandLine(); //������exe·��
	int nPos = tstrPostUrl.find(_T("http://"));
	if (nPos != tString::npos)
	{
		tstrPostUrl.erase(0, nPos);
		return TRUE;
	}
	return FALSE;
	//ȥ��ǰ��exe·��ʣ�µ�url
	///tString tstrPostUrl = _tcsrchr(lptstrCommand, _T(' '))+1;
}
