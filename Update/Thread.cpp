#include "Thread.h"
#include  "UpdateInfo.h"
#include "XmlDocument.h"

///////////////////////////////////////////////////////////////////////////
//
//  ������־��ͼƬ��Ŀ¼��
//   update.exe����Ŀ¼\update\change_log
//
//
//  ���ݵ��ļ��ڣ�
//      update.exe����Ŀ¼\update\backup  
//
///
///	ע����Ǹ�����Ϻ�ɾ��update�ļ��м����ļ�����
//	�������ļ�
//
//
// ע����·��������ʱ������ϢʱwParam����Ϊ102 
//
//



HWND  hWndCallBk ;

#define  BUFFER_SZIE    1024
//unzip�����ļ�·������ʹ��'/'
E_ZipContent CheckZipItem(LPCTSTR lptName)
{
	LPCTSTR pp = _tcsrchr(lptName, _T('\\'));
	if (_tcsrchr(lptName, _T('\\'))[1] == _T('\0'))
	{
		return E_Dir;
	}
	return E_File;
}

//�ص������� ���ؽ���
void  DownloadProgress(unsigned long total, unsigned long part)
{
	int val = (int)((double)part /total * 100);
	PostMessage(hWndCallBk, WM_ONDOWNLOADPROGRESS, (WPARAM)val, 0 );
	//printf("progress: %i%%\n", val);
}

//����Post���ص�Xml�ַ���
BOOL ParseVersionXml(UpdateInfo* pUpdateInfo, const tString& tstrXml)
{
	::CoInitialize(NULL);
	//MessageBox(NULL, tstrXml.c_str(), _T("xml����"), MB_OK);
	CXmlDocument XmlDoc;
	CXmlNodeList  VerNodeList, ChangelogNodeList; 
	CXmlNode RootNode,  xmlNode, xmlSubNode;
	BOOL bRet = XmlDoc.LoadXml(tstrXml.c_str());
	if (!bRet)
		//MessageBox(NULL, _T("����ʧ��"), _T("xml����"), MB_OK);
		return  bRet;
	bRet = XmlDoc.SelectSingleNode(_T("response"), RootNode);
	if (!bRet)
		return bRet;

	bRet = RootNode.SelectSingleNode(_T("window"), xmlNode);
	if (bRet)
	{
		pUpdateInfo->m_size.cy = xmlNode.GetAttributeInt(_T("height"));
		pUpdateInfo->m_size.cx = xmlNode.GetAttributeInt(_T("width"));
	}
	else
		return  FALSE;
	bRet = RootNode.SelectSingleNode(_T("product"), xmlNode);
	if(bRet)
	{
		UnicodeToTString(pUpdateInfo->m_pProductName, xmlNode.GetAttribute(_T("name")).c_str());
	}
	else
		return FALSE;

	bRet = RootNode.SelectNodes(_T("version"),  VerNodeList);
	if (!bRet)
		return bRet;


		
	int x = VerNodeList.GetLength();
	for (int i = 0 ; i < VerNodeList.GetLength(); ++i)
	{
		if (VerNodeList.GetItem(i, xmlNode))
		{
			Version* pVer = new Version;
			W2C(pVer->m_strUrl,  (xmlNode.GetAttribute(_T("url"))).c_str());
			//pVer->m_strUrl = xmlNode.GetAttribute(_T("url"));
			UnicodeToTString(pVer->m_tstrVersion, (xmlNode.GetAttribute(_T("version"))).c_str());
			//pVer->m_tstrVersion = xmlNode.GetAttribute(_T("version"));
			UnicodeToTString(pVer->m_tstrTime, (xmlNode.GetAttribute(_T("create_at"))).c_str());
			//pVer->m_tstrTime = xmlNode.GetAttribute(_T("create_at"));
			bRet = xmlNode.SelectNodes(_T("change_log"), ChangelogNodeList);
			if (!bRet)
				return bRet;
			for (int j = 0; j < ChangelogNodeList.GetLength(); ++j)
			{
				if (ChangelogNodeList.GetItem(j, xmlSubNode))
				{
					Change_log* pChangeLog = new Change_log;
					UnicodeToTString(pChangeLog->m_tstrTitle, (xmlSubNode.GetAttribute(_T("title"))).c_str());
					//pChangeLog->m_tstrTitle = xmlSubNode.GetAttribute(_T("title"));
					UnicodeToTString(pChangeLog->m_tstrdescribe, (xmlSubNode.GetAttribute(_T("description")).c_str()));
					//pChangeLog->m_tstrdescribe = xmlSubNode.GetAttribute(_T("description"));
					W2C(pChangeLog->m_strImageUrl, (xmlSubNode.GetAttribute(_T("image")).c_str()));
					//pChangeLog->m_strImageUrl = xmlSubNode.GetAttribute(_T("image"));
					pVer->m_vcChangelogs.push_back(pChangeLog);
				}
			}
			pUpdateInfo->m_vcVersion.push_back(pVer);
		}
	}
::CoUninitialize();
	return bRet;
}


//�̺߳���
//���ز��������ԣ� ��callbk �ص������м�ص�ǰ���صĽ���
unsigned _stdcall DownLoadUpdatePackage(void* para)
{
	TCHAR tszFileDir[MAX_FILENAME_SIZE+1] = {0x0};
	GetCurDir(tszFileDir, MAX_FILENAME_SIZE);
	_tcscat(tszFileDir, _T("\\update"));
	char* pszFileDir = NULL;
	T2C(&pszFileDir, tszFileDir);
	download_para* p = (download_para*)para;
	BOOL bRet = FALSE;
	tString tStrZipName;

	//����
	for (vector<Version*>::iterator it = p->pUpdateInfo->m_vcVersion.begin(); it != p->pUpdateInfo->m_vcVersion.end(); ++it)
	{
		bRet = http_downloadfile((*it)->m_strUrl.c_str(), pszFileDir, NULL,false, p->pcalbk)?TRUE:FALSE;
		if (bRet)
		{
			TCHAR tszPackagePath[MAX_FILENAME_SIZE + 1] = {0x0};

			GetFileNameFromURL((*it)->m_strUrl.c_str(), tStrZipName);
			_stprintf_s(tszPackagePath,  _T("%s\\%s"), tszFileDir,tStrZipName.c_str());
			p->pUpdateInfo->m_vcPackageName.push_back(tszPackagePath);
		}
		else
		{
			//���°�װ������ʧ��
			tString tstrError = _T("���°�װ������ʧ��,URL:");
			tString tstrUrl;
			StringTotString(tstrUrl,(*it)->m_strUrl.c_str() );
			tstrError += tstrUrl;
			p->pUpdateError->SendErrorMsg(tstrError.c_str());
			PostMessage(hWndCallBk, WM_ONDOWNLOADPROGRESS, 102, 0 );
			return 1;
		}
	}
	
	SAFE_ARRYDELETE(pszFileDir);
	return bRet?0:1;
}

//����֮ǰ����Ҫ���µ��ļ����б���
BOOL BackUpFile(LPCTSTR lptExistingFilePath, UpdateInfo* pUpdateInfo,  LPCTSTR lptBackUpDir)
{
	LPCTSTR lptFileName = _tcsrchr(lptExistingFilePath, _T('\\'));
	TCHAR tszBackUpPath[MAX_FILENAME_SIZE + 1] = {0x0};
	GetCurDir(tszBackUpPath, MAX_FILENAME_SIZE);
	_tcscat(tszBackUpPath, _T("\\update\\backup"));
	//�ж�Ŀ¼���ڲ�
	if (!FindFile(tszBackUpPath))
	{
		CreateDir(tszBackUpPath);
	}
	
	_tcscat(tszBackUpPath, lptFileName);
#if 0 
	//���ַ���Ҳ�ɸ����ļ�
	HANDLE hIn, hOut;
	DWORD nIn, nOut;
	hIn = CreateFile(lptExistingFilePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);

	if (hIn == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		OutputLastError(GetLastError());
#endif
		return  FALSE;
	}
	hOut = CreateFile(tszNewFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
	if (hOut == INVALID_HANDLE_VALUE)
	{
#ifdef _DEBUG
		OutputLastError(GetLastError());
#endif
		return  FALSE;
	}
	CHAR buffer[2048];
	while (ReadFile(hIn, buffer, 2048,&nIn, NULL) && nIn > 0)
	{
		WriteFile(hOut, buffer, nIn, &nOut, NULL);
		if (nIn != nOut)
		{
#ifdef _DEBUG
			OutputLastError(GetLastError());
#endif
			DeleteFile(tszNewFileName);
			return  FALSE;
		}
	}
	return TRUE;
#endif
	
	if  (CopyFile(lptExistingFilePath, tszBackUpPath, FALSE))
	{
		BackUpInfo* pBackUpInfo = new BackUpInfo;
		pBackUpInfo->tstrBackUpPath = tszBackUpPath;
		pBackUpInfo->tstrOldPath = lptExistingFilePath;
		pUpdateInfo->m_vcBackUpInfo.push_back(pBackUpInfo);
		return TRUE;
	}
	return FALSE;
}

//����ĺ����� ������ʧ��ʱ��ԭ���ɵİ汾
BOOL RestoreToOldVersion(UpdateInfo* pUpdateInfo)
{
	for (vector<BackUpInfo*>::iterator it = pUpdateInfo->m_vcBackUpInfo.begin(); it != pUpdateInfo->m_vcBackUpInfo.end(); ++it)
	{
		if (!CopyFile((*it)->tstrBackUpPath.c_str(), (*it)->tstrOldPath.c_str(), FALSE))
		{
			//����ʧ�ܣ����ʱ��ͻ��˿��ܻᱻ��
			return FALSE;
		}
	}
	return TRUE;
}

//������ɻ���ʧ�ܺ�ɾ��
BOOL Clear()
{
	TCHAR tszUpdateFolderPath[MAX_FILENAME_SIZE + 1] = {0x0};
	GetCurDir(tszUpdateFolderPath, MAX_FILENAME_SIZE);
	_tcscat(tszUpdateFolderPath, _T("\\update"));
	return DeleteFolder(tszUpdateFolderPath);
}

//��Ӧchang_log��ͼƬ������
unsigned _stdcall DownloadUpdateImages(void* para)
{
	//WaitForSingleObject(p->hHttpRequestThread, INFINITE);
	imagesdownload_para* p = (imagesdownload_para*)(para);
	TCHAR  tszFileDir[MAX_FILENAME_SIZE + 1] = {0x0};
	GetCurDir(tszFileDir, MAX_FILENAME_SIZE );
	//��ǰĿ¼��updateĿ¼���� 
	_tcscat(tszFileDir, _T("\\update\\change_log"));
	if (!FindFile(tszFileDir))
	{
		if (!CreateDir(tszFileDir))
		{
			tString tstrError = _T("����Ŀ¼ʧ�ܣ� ���ƣ�");
			tstrError += tszFileDir;
			p->pUpdateError->SendErrorMsg(tstrError.c_str());
			return 1;
		}
	}

	char* pszFileDir = NULL;
	T2C(&pszFileDir, tszFileDir);
	
	
	for (vector<Version*>::iterator itVer = p->pUpdateInfo->m_vcVersion.begin(); itVer != p->pUpdateInfo->m_vcVersion.end(); ++itVer)
	{
		for (vector<Change_log*>::iterator itChangelog = (*itVer)->m_vcChangelogs.begin(); itChangelog != (*itVer)->m_vcChangelogs.end(); ++itChangelog)
		{

			BOOL bRet = http_downloadfile((*itChangelog)->m_strImageUrl.c_str(), pszFileDir, NULL,false, NULL)?TRUE:FALSE;

			//�洢���ص�ͼƬ����
			if (bRet)
			{
				TCHAR tszImagePath[MAX_FILENAME_SIZE + 1] = {0x0};
				tString tstrName;
				GetFileNameFromURL((*itChangelog)->m_strImageUrl.c_str(), tstrName);
				_stprintf_s(tszImagePath, _T("%s\\%s"), tszFileDir,tstrName.c_str());
				
				p->pUpdateInfo->m_vcImageName.push_back(tszImagePath);
			}
			else
			{
				//������־ͼƬ����ʧ��
				tString tstrError = _T("������־ͼƬ����ʧ�ܣ�URL:");
				tString tstrUrl;
				StringTotString(tstrUrl, (*itChangelog)->m_strImageUrl.c_str());
				tstrError += tstrUrl;
				p->pUpdateError->SendErrorMsg(tstrUrl.c_str());
			}
		}
	}

	PostMessage(p->hWnd, WM_BEGINUPDATE, 0, 0);
	SAFE_ARRYDELETE(pszFileDir);
	return 0;
}

//�̺߳���
//���صİ�װ�� �� ���н�ѹ
//�˺������˰汾��û���õ�
unsigned _stdcall UnzipUpdatePackage(void* para)
{
	unzipthread_para* p = (unzipthread_para*)para;

	WaitForSingleObject(p->hThread, INFINITE);

	for (vector<tString>::iterator it = p->pUpdateInfo->m_vcPackageName.begin(); it != p->pUpdateInfo->m_vcPackageName.end(); ++it)
	{
		HZIP hz = OpenZip(it->c_str(), NULL);
		if (hz == NULL)
		{
			OutputDebugString(_T("Open zip files failed \n"));
			//p->pUpdateInfo->
			return 0;
		}
		ZIPENTRY ze ;
		GetZipItem(hz,  -1, &ze);
		int nItems = ze.index;
		//SetUnzipBaseDir()   //Ĭ���ǵ�ǰĿ¼
		for (int i= 0; i < nItems; ++i)
		{
			ZIPENTRY zeItem;
			GetZipItem(hz, i, &zeItem);
			UnzipItem(hz, i, zeItem.name );
		}
		CloseZip(hz);
	}
	
	return 0;
}

//��ѹʱ�������ʾ����
//
//
//ֱ�ӽ�ѹ����ǰĿ¼���ǣ�
//��Ҫע������ڸ��ǵ�ǰ�ļ�֮ǰ��Ҫ���ݾɵ��ļ���
//������ֹ�ڰ�װʧ�ܺ��޷���ԭ
unsigned _stdcall UnzipUpdatePackageWithProgress(void* para)
{
	unzipthread_para* p = (unzipthread_para*)para;
	//�ȴ������̵߳Ľ���
	WaitForSingleObject(p->hThread, INFINITE);

	DWORD httpRequestThreadExitCode = -1;
	GetExitCodeThread(p->hThread, &httpRequestThreadExitCode);
	if (httpRequestThreadExitCode != 0)
	{
		return 0;
	}
	for (vector<tString>::iterator it = p->pUpdateInfo->m_vcPackageName.begin(); it != p->pUpdateInfo->m_vcPackageName.end(); ++it)
	{
		HZIP hz = OpenZip(it->c_str(), 0);
		if (hz == NULL)
		{
			//��ѹ���ļ�ʧ��
			//Clear();
			p->pUpdateError->SendErrorMsg(_T("��ѹ���ļ�ʧ��"));
			RestoreToOldVersion(p->pUpdateInfo);
			PostMessage(p->hWnd, WM_ONINSTALLSTATE, WPARAM(102), 0);
			OutputDebugString(_T("OpenZip Failed \n"));
			return  1;
		}
		ZIPENTRY ze;
		GetZipItem(hz, -1, &ze);

		int nItemNum = ze.index;
		DWORD tot = 0;

		//����ѹ�����ܴ�С
		for (int i = 0; i < nItemNum; ++i)
		{
			GetZipItem(hz, i, &ze);
			tot += ze.unc_size;
		}

		DWORD countall = 0;
		//TCHAR szFileDir[MAX_FILENAME_SIZE +1] = {0x0};
		//GetCurDir(szFileDir, MAX_FILENAME_SIZE + 1);

		for (int i= 0; i < nItemNum; ++i)
		{
			GetZipItem(hz, i, &ze);

			//��ѹ�����ļ�·��
			TCHAR tszFileName[MAX_FILENAME_SIZE + 1] = {0x0};
			_stprintf_s(tszFileName, _T("%s\\%s"), p->tszDir, ze.name);
			_tstrreplace(tszFileName, _T('/'), _T('\\'));

			//ע������ڸ���ԭ�оɵ��ļ���ʱ���Ƚ��ɵ��ļ�����
			if(!BackUpFile(tszFileName, p->pUpdateInfo))
			{
				tString tstrError = _T("�����ļ�ʧ�ܣ��ļ���:");
				tstrError += tszFileName;
				p->pUpdateError->SendErrorMsg(tstrError.c_str());
			}

			if ( CheckZipItem(tszFileName) == E_Dir)
			{
				//ѹ���ļ��п��ܴ��ڿյ��ļ���
				//ϵͳ�Ҳ���ָ����·�����ļ������ļ��в�����
				TCHAR tszItemDir[MAX_PATH+1] = {0x0};
				GetDirFromZipFileItem(tszFileName, tszItemDir, MAX_PATH + 1);

				//�����ļ�Ŀ¼��zip��item����Ϊ�ļ����ļ��У�
				CreateDir(tszItemDir);
				continue;;
			}
			else
			{
				//����һ����ѹ�ļ��� �洢ѹ���ļ�����
				HANDLE hf = CreateFile(tszFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);


				if (INVALID_HANDLE_VALUE == hf)
				{
					OutputDebugString(_T("CreateFile Failed \n"));
					DWORD dwError= GetLastError();
#ifdef _DEBUG
					//OutputLastError(dwError);
#endif
					if (dwError == 3)
					{
						//ϵͳ�Ҳ���ָ����·�����ļ������ļ��в�����
						TCHAR tszItemDir[MAX_PATH+1] = {0x0};
						GetDirFromZipFileItem(tszFileName, tszItemDir, MAX_PATH + 1);

						//�����ļ�Ŀ¼��zip��item����Ϊ�ļ����ļ��У�
						if (CreateDir(tszItemDir))
						{
							hf = CreateFile(tszFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
						}
						else
						{
#ifdef _DEBUG
							OutputLastError(GetLastError());
#endif
						}
					}	
				}

				//��������ļ�������Ч�Ļ�ֱ�ӽ�����һ�ν�ѹ
				if (hf == INVALID_HANDLE_VALUE)
				{
#ifdef _DEBUG
					OutputLastError(GetLastError());
#endif
					tString tstrError = _T("��ѹʱ�����ļ�ʧ��,�ļ�����");
					tstrError += tszFileName;
					p->pUpdateError->SendErrorMsg(tstrError.c_str());
					PostMessage(p->hWnd, WM_ONINSTALLSTATE, WPARAM(102), 0);
					CloseZip(hz);
					return 1;
					//continue;
				}

				char buf[ZIP_BUF];
				DWORD  countfile = 0;

				for (ZRESULT zr = ZR_MORE; zr== ZR_MORE;)
				{
					zr = UnzipItem(hz, i, buf, ZIP_BUF);
					unsigned long bufsize = ZIP_BUF;
					if (zr == ZR_OK)
					{
						bufsize = ze.unc_size - countfile;
					}
					else if (zr != ZR_MORE)
					{
						bufsize=0;
					}
					DWORD writ;
					WriteFile(hf, buf,bufsize, &writ, 0);

					countfile += bufsize;
					countall += bufsize;

					//�����ѹ����,�������ÿ��ѹ�����Ľ���
					int nProgress = (int)(100.0*((double)countall)/((double)tot));
					PostMessage(p->hWnd, WM_ONINSTALLSTATE, (WPARAM)nProgress, 0);
					//����淢����Ϣ ��ʾ����
				}

				CloseHandle(hf);
			}
		}
		CloseZip(hz);
	}


	//��ѹ��ɣ� ��ѹ��ɺ�ɾ�����µ���ʱ�ļ��С�
	//Clear();
	PostMessage(p->hWnd, WM_ONINSTALLSTATE, WPARAM(101), 0);
	return 0;
}

// BOOL CheckUpdate(const char* url)
// {
// 	return TRUE;
// }

void GetDirFromZipFileItem(LPCTSTR lptPath, LPTSTR lptDir, int nSize)
{
// 	_tcsrchr(lptPath, _T('/'));
// 	if (lptName+1 == _T('\0'))
// 	{
// 		return E_Dir;
// 	}
	_tcscpy_s(lptDir,_tcslen(lptPath)+1, lptPath);

	_tcsrchr(lptDir, _T('\\'))[0] = _T('\0');

}

unsigned _stdcall HttpRequest(void* para)
{
	httprequest_para* p = (httprequest_para*)(para);
	//��������
	LPCTSTR lpszAccept[] =
	{
			// ��Ӧͷ
		_T("*/*"), //�����κζ���
		NULL       //ע����NULL��β
	};
	TCHAR szHeader[] =
	{
		// ����ύ���Ǳ�,��ô��� MIME һ��Ҫ��!
		_T("Content-Type: application/x-www-form-urlencoded\r\n")
	};

	HINTERNET  hInet = InternetOpen(_T("UpdateClient"), INTERNET_OPEN_TYPE_DIRECT, NULL, INTERNET_INVALID_PORT_NUMBER, 0);
	if (hInet == NULL)
	{
#if _DEBUG
		OutputLastError(GetLastError());
#endif 
		p->pUpdateError->SendErrorMsg(_T("HttpRequest,InternetOpen Method Failed "));
		return 1;
	}
	HINTERNET hConn = InternetConnect(hInet, p->serverName.c_str(), p->port, NULL, NULL, INTERNET_SERVICE_HTTP, 0 ,1 );
	if (hConn == NULL)
	{
#if _DEBUG
		OutputLastError(GetLastError());
#endif 
		p->pUpdateError->SendErrorMsg(_T("HttpRequest, InternetConnect Method Failed"));
		return 1;
	}
	HINTERNET hPost = HttpOpenRequest(hConn, _T("POST"), p->filePath.c_str(), HTTP_VERSION, NULL, (LPCTSTR*)lpszAccept, INTERNET_FLAG_DONT_CACHE, 1);
	if (hPost == NULL)
	{
#if _DEBUG
		OutputLastError(GetLastError());
#endif 
		p->pUpdateError->SendErrorMsg(_T("HttpRequest, HttpOpenRequest Method Failed"));
		return 1;
	}

	if (!HttpSendRequest(hPost, szHeader, _tcslen(szHeader), (LPVOID)(p->postdata.c_str()), p->postdata.length()))
	{
#if _DEBUG
		OutputLastError(GetLastError());
#endif 
		p->pUpdateError->SendErrorMsg(_T("HttpRequest, HttpSendRequest Method Failed"));
		return 1;
	}

	unsigned long lByteRead;

	char szBuffer[BUFFER_SZIE+1] = {0x0};

	std::string strReponse;
	do 
	{
		if (!InternetReadFile(hPost,  szBuffer, BUFFER_SZIE, &lByteRead))
		{
#if _DEBUG
			OutputLastError(GetLastError());
#endif 
			p->pUpdateError->SendErrorMsg(_T("HttpRequest, InternetReadFile Failed"));
			return 1;
		}
		szBuffer[lByteRead] = 0x0;
		strReponse += szBuffer;
	} while (lByteRead > 0);

	tString tstrReponse;
	Utf8TotString(tstrReponse, strReponse.c_str());
//	MessageBox(NULL, tstrReponse.c_str(), _T("Response"), MB_OK);
	if(!ParseVersionXml(p->pUpdateInfo, tstrReponse))
	{
		p->pUpdateError->SendErrorMsg(_T("Parse XMl Failed"));
		return 2;
	}

	Clear();
	// 0 ���ر�ʾ����
	// 1 ��ʾpost ���̿��ܳ�����
	// 2 ��ʾ���ص�xml ���ܲ���ȷ
	return  0;
}