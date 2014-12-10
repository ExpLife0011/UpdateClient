#ifndef _THREAD_H
#define _THREAD_H
#include "Global.h"
#include <process.h>
#include  <string>
using namespace std;
#include "unzip.h"
#include "UpdateInfo.h"
#include "..\\HtttpDownload\\Download.h"
#ifdef _DEBUG
#pragma  comment(lib, "HtttpDownload_d.lib")
#else
#pragma  comment(lib, "HtttpDownload.lib")
#endif

/*
	����diulib��ʹ����xunzip��ѹѹ���ļ��� �ᵼ�£��˴�ʹ����һ��Unzip�������ռ�
*/
using namespace  Unzip;

#define ZIP_BUF  16384   //��ѹzipÿ�ν�ѹ�Ĵ�С

#define WM_BEGINUPDATE    WM_USER + 1

typedef void  (*download_callback) (unsigned long , unsigned long);


//���°������̲߳���
struct download_para
{
	HWND   hWnd;
	//std::string url ;
	UpdateInfo*  pUpdateInfo;
	download_callback pcalbk;
	UpdateError* pUpdateError;
};

//��ѹ���°��̲߳���
struct unzipthread_para
{
	HWND    hWnd;
	TCHAR   tszDir[MAX_FILENAME_SIZE+1];
	HANDLE hThread;
	//TCHAR	tszZipName[MAX_FILENAME_SIZE+1];
	UpdateInfo*  pUpdateInfo;
	UpdateError* pUpdateError;
};

//http�����̲߳���
struct httprequest_para
{
	tString  serverName;
	tString  filePath;
	string   postdata;// ע��utf8����
	int      port;	  //�����url�д��˿ڵĻ���û���˿�ΪĬ��80
	//tString  tstrReponse;   //Post ����get�ķ���ֵ
	UpdateInfo*  pUpdateInfo;
	UpdateError* pUpdateError;
};

 struct imagesdownload_para
 {
	 HWND        hWnd;
	HANDLE       hHttpRequestThread;
 	UpdateInfo*  pUpdateInfo;
	UpdateError* pUpdateError;
 };

enum E_ZipContent
{
	E_Dir = 0,
	E_File
};

E_ZipContent CheckZipItem(LPCTSTR lptName);

//�ص������� ���ؽ���
void  DownloadProgress(unsigned long total, unsigned long part);

//����post���ص�Xml
BOOL ParseVersionXml(UpdateInfo* pUpdateInfo, const tString& tstrXml);

//�ļ����ƺ���
BOOL BackUpFile(LPCTSTR lptExistingFileName, UpdateInfo* pUpdateInfo, LPCTSTR lptBackUpDir = NULL);

//��װ����ԭ�����θ���֮ǰ�İ汾
BOOL RestoreToOldVersion();

//������ɻ���ʧ�ܣ� ������򴴽����ļ����Լ����ص�һЩ�ļ�
BOOL Clear();

//�̺߳���
//���ظ��°�֮ǰ�����������Ϣ��ص�һЩͼƬ
unsigned _stdcall DownloadUpdateImages(void* para);

//�̺߳���
//���ز��������ԣ� ��callbk �ص������м�ص�ǰ���صĽ���
unsigned _stdcall DownLoadUpdatePackage(void* para);

//�̺߳���
//���صİ�װ�� �� ���н�ѹ
//ֱ�ӽ�ѹ����ǰĿ¼���Ǽ���
unsigned _stdcall UnzipUpdatePackage(void* para);

//��ѹʱ�������ʾ����
unsigned _stdcall UnzipUpdatePackageWithProgress(void* para);

//ѹ���ļ��д��ڶ༶Ŀ¼ʱ���߿�Ŀ¼ʱ�� ��ѹʱ�������޸�Ŀ¼��Ҫ��ȡѹ���ļ���Ŀ¼���ٴ���
void GetDirFromZipFileItem(LPCTSTR lptPath, LPTSTR lptDir, int nSize);
//BOOL CheckUpdate(const char* url);

//
unsigned _stdcall HttpRequest(void* para);

#endif