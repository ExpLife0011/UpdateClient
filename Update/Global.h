#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <windows.h>
#include <tchar.h>
#include <string>
#include  <WinInet.h>
#pragma comment(lib, "wininet.lib")
//�ڴ��ͷź�
using namespace std;
#if defined(UNICODE)
typedef std::basic_string<wchar_t,std::char_traits<wchar_t>,std::allocator<wchar_t> > tString;
#else
typedef std::basic_string<char,std::char_traits<char>,std::allocator<char> >		  tString;
#endif
#define SAFE_ARRYDELETE(x) if(NULL!=x){delete[] x;x = NULL;}

#define SAFE_DELETE(x) if(NULL!=x){delete x; x =NULL;}

#define  WM_ONDOWNLOADPROGRESS  WM_USER + 100

#define  WM_ONINSTALLSTATE     WM_USER + 101

//#define  WM_ONINSTALL           WM_USER + 102 

//ɾ���ַ���ǰ��Ŀո�
char * Trim( char * str );

//��ȡ��ǰĿ¼
void  GetCurDir(LPTSTR lpStr, int nSize);

//��Url�����ص��ļ���
BOOL GetDownLoadZipFileName(const char* url, TCHAR* tstrFname);

//���ֽ�ת��Ϊ���ֽ�
void C2W(WCHAR** dest, const char* src);

//���ֽ�ת��Ϊ���ֽ�
void W2C(char** dest, const WCHAR *src);

void W2C(string& dest,const WCHAR *src);

//char �ַ���ת��Ϊtchar�ַ���
void C2T(TCHAR** dest, const char* src);

void T2C(char** dest, const TCHAR* src);

BOOL T2C(char*dest, int nSize, const TCHAR* src );

//�����ļ�����Ŀ¼�Ƿ����
BOOL FindFile(LPCTSTR lptPath) ; 

BOOL CreateDir(LPCTSTR lptPath); //����Ŀ¼

void tStringToString(std::string& strDest, const tString& tstrSrc);

void tStringToString(std::string& strDest, LPCTSTR lpctSrc);

void StringTotString(tString& tstrDest, const std::string& strSrc);

void StringTotString(tString& tstrDest, LPCSTR lpcSrc);

void UnicodeToTString(tString& tstrDest, const WCHAR* lpwSrc);

void Utf8ToUnicode(WCHAR** dest,const char* src);

void Utf8TotString(tString &strDest, const char* src);

void UnicodeToUtf8(char** dest , const WCHAR* src);

void AnsiToUtf8(char** dest, const char* src);

// void TcharToUtf8(char** dest, const TCHAR* src)
// {
// #ifdef _UNICODE
// 	UnicodeToUtf8(dest, src);
// #else
// 	AnsiToUtf8(dest, src);
// #endif
// }

void tStringToUtf8(string &tstrDest,  LPCTSTR src);

tString GetBetweenString(const TCHAR * pStr, const TCHAR * pStart, const TCHAR * pEnd);
//�ַ������ַ��滻
void _tstrreplace(LPTSTR lptStr, TCHAR tSrc, TCHAR tDest);

#ifdef _DEBUG
void OutputLastError(DWORD dwError);
#endif

//��url�еõ�http���������ļ�����
BOOL GetFileNameFromURL(const char *url, tString& tstrName);

//ɾ�������ļ��У��������е����ļ����ļ��е�
BOOL DeleteFolder(LPCTSTR lpstrFolder);

BOOL GetUrlFromCommandline(tString& tstrPostUrl);

#endif