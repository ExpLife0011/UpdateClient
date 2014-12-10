#pragma once
#include  "BaseWnd.h"
#include "Thread.h"
#include "UpdateInfo.h"
// /*
// <?xml version="1.0" encoding="UTF-8" standalone="true"?>
// <response>
// 	<success>true</success>
// 	<version url="http://10.0.40.18:8081/download/liuliu_0.0.2.msi" version="0.0.2" create_at="2014-09-10 10:50:42">
// 		<change_log title="����1" description="��Լ������" image="http://sdfasdf.jpg"/>
// 		<change_log title="����2" description="��˹��" image="http://asdfasdf.png"/>
// 	</version>
// 	<version url="http://10.0.40.18:8081/download/liuliu_0.0.3.msi" version="0.0.3" create_at="2014-09-10 10:50:51">
// 		<change_log title="�ش����" description="��˹�ٷ����ط���˹�ٷ�" image="http://sdfasdfasd.gif"/>
// 	</version>
// </response>
// */
// 
// //ÿ���汾����Ǹ��µ���ϸ��Ϣ
// class Change_log
// {
// public:
// 	Change_log();
// 	~Change_log();
// public:
// 	tString m_tstrTitle;
// 	tString m_tstrdescribe;
// 	tString m_strImageUrl;
// };
// 
// //�汾�����
// class Version
// {
// public:	
// 	Version();
// 	~Version();
// public:
// 	vector<Change_log*>  m_vcChangelogs;
// 	tString             m_tstrTime;
// 	tString             m_strUrl;
// 	tString             m_tstrVersion;
// };
// 
// 
// 
// class CUpdateInfo
// {
// public:
// 	CUpdateInfo();
// 	~CUpdateInfo();
// public:
// 
// 
// };


class CUpdateWnd:public CBaseWnd
{
public:
	CUpdateWnd(void);
	~CUpdateWnd(void);

protected:
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	UINT GetClassStyle() const ;
	virtual CDuiString GetSkinFolder();
	virtual void       InitWindow();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(TNotifyUI& msg);
	void OnBtn_UpdateOK(TNotifyUI& msg);
	void OnBtn_UpdateCancel(TNotifyUI& msg);
	void OnBtn_DownloadCancel(TNotifyUI& msg);
	void OnBtn_InstallOK(TNotifyUI& msg);
	void OnBtn_InstallError(TNotifyUI& msg);
	virtual void OnFinalMessage(HWND hWnd);
	LRESULT OnDownloadProgress(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnInstallState(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnMouseHover(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnBeginUpdate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam/*, BOOL& bHanded*/);
	//LRESULT OnBeginInstallation(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetUpdateTips();
private:
	void Close();
	void Exit();
	void SwitchChangeLog(int nIndex); //��־������ͼƬ���л���
	void AddButtons();   //����������Ϣ�еĸ�����Ϣ��̬������ť�ؼ�
	void InitUpdateInfo();  //�����򴫹����Ĳ�������

	void PreUpdate();     // ��ʽ���ذ�װ֮ǰ��Ҫ��������
	void OnSelectChanged(TNotifyUI& msg);
	//BOOL ParseVersionXml(const tString& tstrXml);
	
	int GetProcessInstances(LPCTSTR lptName); //���ؽ���ʵ���� 0 ��ʾϵͳ�޸ý���
	void  KillProcess();  //��ֹ�������������Ľ���
	void StartProcess();
private:
	//�������صİ�װ������
	int					   m_nDownloadNum;
	vector<CControlUI*>    m_vcLogBtn;
	int					   m_nChangelogIndex;
	UpdateInfo*            m_pUpdateInfo;
	httprequest_para*	   m_pHttpRequestPara;
	imagesdownload_para*   m_pImagesDownloadPara;
	HANDLE				   m_hHttpRequestThread;
	HANDLE				   m_hImagesDownloadThread; // 
	vector<tString>        m_vcPicPath;
	download_para*         m_pDownloadPara;
	unzipthread_para*      m_pUnzipPara;
	CVerticalLayoutUI*	   m_pUpdateView;
	CVerticalLayoutUI*     m_pArrowBtnView;
	CVerticalLayoutUI*	   m_pDownloadView;
	CVerticalLayoutUI*	   m_pInstallView;
	CVerticalLayoutUI*     m_pCompleteView;
	CVerticalLayoutUI*	   m_pErrorView;
	CVerticalLayoutUI*     m_pBkgroudContainer;
	CVerticalLayoutUI*     m_pLoadingView;
	CControlGifUI*		   m_pGifControl;
	CProgressUI*           m_pDownloadProgress;
	CProgressUI*           m_pUnzipProgress;
	CLabelUI*			   m_pTitle;
	CLabelUI*			   m_pDownloadTip;
	CButtonUI*             m_pCloseBtn;
	HANDLE                 m_hDownloadThread;
	HANDLE				   m_hUnzipThread;
	vector<DWORD>		   m_vcHProcess;
	//TCHAR				   m_tszVerName[MAX_PATH+1];  //��װ��ȥ��׺����
	UpdateError*				m_pUpdateError;
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual LPCTSTR GetResourceID() const;
	//LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

