#include <windows.h>
#include <tchar.h>

#include "UpdateWnd.h"

#include "resource.h"


// ȷ����http����
BOOL Isttp()
{
	tString tstrPostUrl;
	GetUrlFromCommandline(tstrPostUrl);
	TCHAR str[5] = _T("");

	// 
	_tcsncpy_s(str, tstrPostUrl.c_str(),  4);
	

	// 
	for(TCHAR *p = str; *p; p++)
		*p = tolower(*p);

	return !_tcsicmp(_T("http"), str);
}


//���������������
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE /*hPrevInstance*/,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	//::CoInitialize(NULL);
	//MessageBox(NULL, _T("hrllo"), _T("Update"), MB_OKCANCEL);
	//ͨ��main�����������жϵ��ô˳���
	
	if (!Isttp())
	{
		return 0;
	}
//	LPCTSTR lptstr = GetCommandLine();

 //	MessageBox(NULL, lptstr, _T("TISHI"), MB_OK);
// 	if (!Isttp(lpCmdLine))
// 
// 	{
// #ifdef _DEBUG
// 	MessageBox(NULL, lptstr, _T("TISHI"), MB_OK);
// #endif
// 		return 0;
// 	}


	//////////////////////////////////////////////////////////////////////////
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath() /*+ _T("skin\\")*/);

	CUpdateWnd *pUpdateWnd = new CUpdateWnd;
	if (pUpdateWnd == NULL)
	{
		return 0;
	}
	pUpdateWnd->Create(NULL, _T("�Զ�����"), UI_WNDSTYLE_DIALOG,WS_EX_WINDOWEDGE);
	pUpdateWnd->CenterWindow();
		pUpdateWnd->SetIcon(IDI_ICON1);
	pUpdateWnd->ShowModal();

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
	//::CoUninitialize();
	return 0;
}