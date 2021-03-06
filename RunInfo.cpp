// RunInfo.cpp : implementation file

//#include <io.h>
#include "stdafx.h"
#include "PT.h"
#include "RunInfo.h"
#include "winsock2.h"
#include "shlwapi.h"
#include "Tlhelp32.h"
#include <time.h>
#include <io.h>
#include <afxwin.h>
#include <iostream>


#include "Ambit_Dut.h"
#pragma comment(lib,"ambit_config.lib")
#pragma comment(lib,"ambit_dut.lib")
// CRunInfo dialog


#define SL00    	_T("SL00 Recive SFC data error... ")

#define SR00    	_T("SR00 SFC response error... ")
#define SY45    	_T("SY45 Read ini or tab file(Ambit_Test_flow.ini&ambit_steup .ini) failed...	")
#define SY00    	_T("SY00 Inner runtime error...	")
#define SY05		_T("SY05 Connect with server&&copy file failed...")
#define SY06		_T("SY06 Connect Mydas server&&insert data failed...")
#define SY10        _T("SY10 Creat file path failed...")
#define SY15        _T("SY15 Compare product name failed...")

#define IN40    	_T("IN40 Connect IQlex tester fail...")
#define WT00    	_T("WT00 Send DUT command wait respond timeout...	")
#define IN10     	_T("IN10 Ping DUT failed...    ")
#define IN20     	_T("IN20 Telnet into DUT failed...    ")
#define IN12     	_T("IN12 Befor download test image ping DUT failed...    ")
#define IN11     	_T("IN11 Befor RF test ping DUT failed...    ")
#define	IN30		_T("IN30 Find Test flow failed ...	")

#define EV11    	_T("EV11 5G 11b EVM fail...	")
#define EV12    	_T("EV12 5G 11g EVM fail...	")
#define EV14    	_T("EV14 5G 11n EVM fail...	")

#define PR11    	_T("PR11 5G 11b PER fail...	")
#define PR12    	_T("PR12 5G 11g PER fail...	")
#define PR14    	_T("PR14 5G 11n PER fail...	")

#define PR15    	_T("PR15 5G 11b Higt Power lever PER fail...	")
#define PR16    	_T("PR16 5G 11g Higt Power lever PER fail...	")
#define PR18    	_T("PR18 5G 11n Higt Power lever PER fail...	")

#define SM11    	_T("SM11 5G 11b MASK fail...	")
#define SM12    	_T("SM12 5G 11g MASK fail...	")
#define SM14    	_T("SM14 5G 11n MASK fail...	")

#define PW11    	_T("PW01 5G 11b POWER fail...	")
#define PW12    	_T("PW02 5G 11g POWER fail...	")
#define PW14    	_T("PW14 5G 11n POWER fail...	")

#define EV01    	_T("EV01 2.4G 11b EVM fail...	")
#define EV02    	_T("EV02 2.4G 11g EVM fail...	")
#define EV04    	_T("EV04 2.4G 11n EVM fail...	")

#define PR01    	_T("PR01 2.4G 11b PER fail...	")
#define PR02    	_T("PR02 2.4G 11g PER fail...	")
#define PR04    	_T("PR04 2.4G 11n PER fail...	")

#define PR05    	_T("PR05 2.4G 11b Higt Power lever PER fail...	")
#define PR06    	_T("PR06 2.4G 11g Higt Power lever PER fail...	")
#define PR08    	_T("PR08 2.4G 11n Higt Power lever PER fail...	")

#define SM01    	_T("SM01 2.4G 11b MASK fail...	")
#define SM02    	_T("SM02 2.4G 11g MASK fail...	")
#define SM04    	_T("SM04 2.4G 11n MASK fail...	")

#define PW01    	_T("PW01 2.4G 11b POWER fail...	")
#define PW02    	_T("PW02 2.4G 11g POWER fail...	")
#define PW04    	_T("PW04 2.4G 11n POWER fail...	")

#define RF01   	    _T("RF01 Read test log file fail...	")
#define DL00   	    _T("DL00 Download test image fail...	")


#define RR00   	    _T("RR00 Run RegisterTechnologyDll fail...	")  
#define RG00   	    _T("RG00 Run GLOBAL_SETTINGS fail...")
#define RL00   	    _T("RL00 Run LOAD_PATH_LOSS_TABLE fail...	")
#define RI00   	    _T("RI00 Run INSERT_DUT fail...	")
#define UN00   	    _T("UN00 Read test log unknown error...	")

#define RR01   	    _T("RR01 Run IQlite_Demo.exe fail...	")  


#define BUF_LENGH     1024
#define UM_SOCK     WM_USER+4
#define WM_COMET    WM_USER+1
#define WM_PASS     WM_USER+63
#define WM_FAIL     WM_USER+64

bool OnSFIS = FALSE;
static CString strRunInfo;
char ServerDir_LOG_PRODUCT[BUF_LENGH]="";
char ServerDir_LOG_PRODUCT_TIME[BUF_LENGH]="";
char ServerDir_LOG_PRODUCT_PASS[BUF_LENGH]="";
char ServerDir_LOG_PRODUCT_FAIL[BUF_LENGH]="";

//char Log[BUF_LENGH] = ""; //Recode server file path
char RecordDir_LOG[BUF_LENGH]="";
char RecordDir_LOG_PRODUCT[BUF_LENGH]="";
char RecordDir_LOG_PRODUCT_TIME[BUF_LENGH]="";
char RecordDir_LOG_PRODUCT_PASS[BUF_LENGH]="";
char RecordDir_LOG_PRODUCT_FAIL[BUF_LENGH]="";
static CString ProductName;
CString          g_data;

/*add for SFIS*/
int SN_LEN = 0;
int PN_LEN = 0;
int PC_LEN = 0;
int PASSFLAG_LEN = 4;
int	FAILFLAG_LEN = 4;
int RN_LEN = 2;
char	SFIS_Begin[BUF_LENGH] = "";
char	SFIS_Pass[BUF_LENGH] = "";
char	SFIS_Fail[BUF_LENGH] = "";
/*End*/

fstream fs;
static INIT_INFO m_Mydas;
void *pInstMydas;
SYSTEMTIME filetime;
vector<string> g_EVM,g_PER,g_MASK;

CRunInfo *pRunInfo = NULL;
vector<string> g_Recorder;

char FailValue[20] = "";
char FailItem[BUF_LENGH] = "";
char g_MyDAS[BUF_LENGH*4] = "";

IMPLEMENT_DYNAMIC(CRunInfo, CDialog)
char szRecordFilePath  [MAXBUFSIZE] = "";
bool IsGeneralTx=true;
bool IsGeneralRx=true;
bool IsGeneralTxForDualBand=true;
bool IsGeneralRxForDualBand=true;
int ItemNum = 0;
char g_ErrDef[BUF_LENGH] = "";
HANDLE hDev=NULL;
bool SendDatatoMYDAS(char* DetailLog,char* ErrDef,char* SendDataBuf);
DWORD WINAPI LinkDataBase(void);
HANDLE hThread;
HANDLE hThread1;
void Run_Test(const ReturnInfo *pReturnInfo);
void RecordDetailLog(char  *szRecordFilePath);
typedef struct _Item_Data
{
	string evm;
	string power;
	string evm0;
	string evm1;
	string power0;
	string power1;
	string freq;
	string mask;
	
	string per;
}Item_Data;
map<string,Item_Data> lk;
CRunInfo::CRunInfo(CWnd* pParent )
	: CDialog(CRunInfo::IDD, pParent)
	, m_strSN(_T(""))
	, m_strPcName(_T(""))
	, m_strMAC(_T(""))
	, m_strCom(_T(""))
	, m_strSfisStatus(_T(""))
	, m_strTestResult(_T(""))
	, m_strProductName(_T(""))
	, m_strTestTime(_T(""))
	, m_strErroCode(_T(""))
	, m_strConnectTime(0)
	, m_strConnectTimeN(0)
	, m_strConnectTimeC(0)
{
	m_TestTimeCount= 0;
	m_blIsOpenSfis= false;  
	m_blIsSfis = false;
	pRunInfo = this;
	m_hEvt = NULL;
	pInstMydas = NULL;
	m_hEvt = CreateEvent(NULL,true,false,NULL);
}

CRunInfo::~CRunInfo()
{
	try
	{
		char buf[BUF_LEN]="";
		if(NULL != m_hEvt)
			CloseHandle(m_hEvt);
		DeleteSpecifyExe("epi_ttcp.exe");
		DeleteSpecifyExe("ping.exe");
		DeleteSpecifyExe("IQlite_Demo.exe");
		DeleteSpecifyExe("Netgear_TFTP_Server.exe");		
		//devenv.exe
	}
	catch(...)
	{
		AfxMessageBox("CRunInfo fail");
	}
}

void CRunInfo::DoDataExchange(CDataExchange* pDX)
{
	try
	{
		CDialog::DoDataExchange(pDX);
		DDX_Text(pDX, IDC_EDIT_SN, m_strSN);
		DDX_Text(pDX, IDC_EDIT_PCNAME, m_strPcName);
		DDX_Text(pDX, IDC_EDIT_MAC, m_strMAC);
		DDX_CBString(pDX, IDC_EDIT_COM, m_strCom);
		DDX_Text(pDX, IDC_SFIS_STATUS, m_strSfisStatus);
		DDX_Text(pDX, IDC_RESULT, m_strTestResult);
		DDX_Text(pDX, IDC_PRODUCTNAME, m_strProductName);
		DDX_Text(pDX, IDC_TESTTIME, m_strTestTime);/*110309 delete by FaHong for RD request*/
		DDX_Text(pDX, IDC_ERRORCODE, m_strErroCode);
		DDX_Text(pDX, IDC_EDIT_CONNECT, m_strConnectTime);
		DDX_Text(pDX, IDC_EDIT_NET, m_strConnectTimeN);
		DDX_Text(pDX, IDC_EDIT_Cable, m_strConnectTimeC);
	}
	catch(...)
	{
		AfxMessageBox("DoDataExchange fail");
	}
}


BEGIN_MESSAGE_MAP(CRunInfo, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BTN_START, &CRunInfo::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CRunInfo::OnBnClickedBtnQuit)
	ON_CBN_SELCHANGE(IDC_COM, &CRunInfo::OnCbnSelchangeCom)
	ON_MESSAGE(WM_COMET,CRunInfo::OnRecvComData)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_PASS,CRunInfo::OnPass)
	ON_MESSAGE(WM_FAIL,CRunInfo::OnFail)
	ON_MESSAGE(UM_SOCK ,CRunInfo::OnRecvEthernetData)
END_MESSAGE_MAP()




HBRUSH CRunInfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	try
	{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(IDC_PRODUCTNAME == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Midft);
		pDC->SetTextColor(RGB(255,255,0));
		pDC->SetBkMode(TRANSPARENT);
		return m_Grancybh;
	}
	if(IDC_PASS_COUNT == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Minft);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	if(IDC_FAIL_COUNT == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Minft);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}
	if(IDC_STATION == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Midft);
		pDC->SetTextColor(RGB(255,255,0));
		pDC->SetBkMode(TRANSPARENT);
		return m_Grancybh;
	}
	if(IDC_RESULT==pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Maxft);
		pDC->SetBkMode(TRANSPARENT);

		if(!strcmp("StandBy", m_strTestResult))
		{
			pDC->SetTextColor(RGB(255,255,0));
			return m_Grancybh;
		}
		if(!strcmp("RUN", m_strTestResult))
		{
			pDC->SetTextColor(RGB(0,255,0));
			return m_Yellowbh;
		}
		if(!strcmp("PASS", m_strTestResult))
		{
			pDC->SetTextColor(RGB(0,0,255));
			return m_Greenbh;
		}
		if(!strcmp("FAIL", m_strTestResult))
		{
			pDC->SetTextColor(RGB(255,255,255));
			return m_Redbh;
		}
	}
	if(IDC_SFIS_STATUS==pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Minft);
		pDC->SetBkMode(TRANSPARENT);
		if(!strcmp("SFIS OFF", m_strSfisStatus))
		{
			pDC->SetTextColor(RGB(255,255,0));
			return m_Redbh;
		}
		if(!strcmp("SFIS ON", m_strSfisStatus))
		{
			pDC->SetTextColor(RGB(255,255,0));
			return m_Greenbh;		
		}
	}
	if(IDC_PASS == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Minft);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	if(IDC_FAIL == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Minft);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}
	if(IDC_TESTTIME == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_Minft);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	if(IDC_ERROR == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_SupMinft);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkMode(TRANSPARENT);
	}
	if(IDC_ERRORCODE == pWnd->GetDlgCtrlID())
	{
		pDC->SelectObject(&m_SupMinft);
		pDC->SetTextColor(RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
	}
	catch(...)
	{
		AfxMessageBox("OnCtlColor fail");	
		return 0 ;
	}
}

BOOL CRunInfo::OnInitDialog()
{
	try
	{
		CDialog::OnInitDialog();

		/*Debug*/
		/*GetSFISData("SN(25)+HH(25)+PASS(4)" , NULL);*/
		/*End*/


		char szINITPath[MINBUFSIZE]="";		
		GetCurrentDirectory(sizeof(szINITPath),szINITPath);
		lstrcat(szINITPath,"\\init.ini");


		DeleteSpecifyExe("ping");
		//check IQ_demo is exist or not,if exist close
		WritePrivateProfileString("TESTER_CONTROL","CTRL_MODE","999",".\\TestStartUp.ini");
		Sleep(500);

		//run TFTP server
		GetPrivateProfileString("General","DownLoadImageFlag","0",INI.DLI.szDownLoadImageFlag,MAX_PATH,".\\init.ini");	
		if(strstr( INI.DLI.szDownLoadImageFlag ,"1") != NULL )
		{
			ShellExecute(NULL, "open", ".\\Netgear_TFTP_Server.exe ", "", " ",SW_MINIMIZE);
		}
		m_SupMinft.CreatePointFont(100, _T("Arial"));
		m_Minft.CreatePointFont(200, _T("Arial"));
		m_Midft.CreatePointFont(350, _T("Arial"));
		m_Maxft.CreatePointFont(1000,_T("Arial"));

		m_Redbh.CreateSolidBrush(RGB(255,0,0));
		m_Greenbh.CreateSolidBrush(RGB(0,255,0));
		m_Yellowbh.CreateSolidBrush(RGB(255,255,0));
		m_Grancybh.CreateSolidBrush(RGB(82,64,111));

		m_ComSfis.SetParentWnd(m_hWnd);
		char szProductName[30] = "";	
		gethostname(szProductName,30);
		_strupr_s(szProductName,30);

		m_strPcName = szProductName;
		if(m_strPcName.GetLength() > 12)
		{
			m_strPcName.SetAt(12, '\0');
		}
		else
		{
			int len = m_strPcName.GetLength();
			for(int i = len; i<12; i++)
				m_strPcName+='-';
		}


		((CComboBox*)GetDlgItem(IDC_COM))->AddString(_T("COM1"));
		((CComboBox*)GetDlgItem(IDC_COM))->AddString(_T("COM2"));
		((CComboBox*)GetDlgItem(IDC_COM))->AddString(_T("COM3"));
		((CComboBox*)GetDlgItem(IDC_COM))->AddString(_T("COM4"));
		((CComboBox*)GetDlgItem(IDC_COM))->AddString(_T("COM5"));
		m_strCom = _T("COM2");

		((CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_GOLDENIP))->SetAddress(192,168,0,1);

		m_strTestResult = _T("StandBy");

		m_strSfisStatus = _T("SFIS OFF");

		UpdateData(false);

		((CEdit*)GetDlgItem(IDC_EDIT_CONNECT))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_EDIT_NET))->EnableWindow(false);
		((CEdit*)GetDlgItem(IDC_EDIT_Cable))->EnableWindow(false);


		/*Need add a flag read or not*/           //   --->6/7 ok

		
		GetPrivateProfileString("ConnectControl","ConnectcontrolFlag","0" ,INI.CC.szConnectControlFlag, MAX_PATH,szINITPath);

		if(strstr(INI.CC.szConnectControlFlag , "1") != NULL)
		{
			HKEY   hk;
			DWORD  dwDisp;

			
			char RegBuf[MAX_PATH] = "";
			GetPrivateProfileString("ConnectControl","DutName","0" ,INI.CC.szDutName, MAX_PATH,szINITPath);			
			sprintf_s(RegBuf , MAX_PATH , "SYSTEM\\CurrentControlSet\\Services\\%s" , INI.CC.szDutName);
			if(RegCreateKeyEx(HKEY_LOCAL_MACHINE,
				RegBuf,
				0,
				NULL,
				REG_OPTION_NON_VOLATILE,
				KEY_WRITE | KEY_QUERY_VALUE,
				NULL,
				&hk,
				&dwDisp))
			{
				DisplayRunTimeInfo("Create registry fail\r\n");
			}
			else
			{
				if(dwDisp == REG_CREATED_NEW_KEY)
				{
					DisplayRunTimeInfo("Create new key\r\n");
					RegCloseKey(hk);
				}
				else
				{
					DisplayRunTimeInfo("Read old key\r\n");
					char szCOM[256] = "";
					char szFile[256] = "";
					char szConnectTime [256] = "";
					char szConnectTimeN [256] = "";
					char szConnectTimeC [256] = "";

					DWORD dwBuf1 = 256,dwBuf2 = 256,dwBuf3 = 256,dwBuf4 = 256,dwBuf5 = 256;
					if(RegQueryValueEx(hk,"COM",NULL,NULL,(LPBYTE)szCOM,&dwBuf1)
						||RegQueryValueEx(hk,"FILETIME",NULL,NULL,(LPBYTE)szFile,&dwBuf2)
						||RegQueryValueEx(hk,"CONNECTTIME",NULL,NULL,(LPBYTE)szConnectTime,&dwBuf3)
						||RegQueryValueEx(hk,"CONNECTTIMEN",NULL,NULL,(LPBYTE)szConnectTimeN,&dwBuf4)
						||RegQueryValueEx(hk,"CONNECTTIMEC",NULL,NULL,(LPBYTE)szConnectTimeC,&dwBuf5))
					{
						DisplayRunTimeInfo("Get Info from registry fail\r\n");
						RegCloseKey(hk);
					}
					else
					{
						iCOM = atoi(szCOM);
						((CComboBox*)GetDlgItem(IDC_EDIT_COM))->SetCurSel(iCOM);
						iConnectTimeN= atoi(szConnectTimeN);
						iConnectTime = atoi(szConnectTime);
						iConnectTimeC= atoi(szConnectTimeC);
						m_strConnectTime = iConnectTime;
						m_strConnectTimeN = iConnectTimeN;
						m_strConnectTimeC = iConnectTimeC;
						UpdateData(false);
						m_FileTime = szFile;

						RegCloseKey(hk);
					}			
				}
			}

			int itime = 0;
			int itimeN = 0;
			int itimeC = 0;
			GetPrivateProfileString("ConnectControl","ConnectTimePower","3000" ,INI.CC.szTestTime, MAX_PATH,szINITPath);
			GetPrivateProfileString("ConnectControl","ConnectTimeNet","3000" ,INI.CC.szTestTimeN, MAX_PATH,szINITPath);
			GetPrivateProfileString("ConnectControl","ConnectTimeCable","3000" ,INI.CC.szTestTimeC, MAX_PATH,szINITPath);

			itime = atoi(INI.CC.szTestTime);
			itimeN = atoi(INI.CC.szTestTimeN);
			itimeC = atoi(INI.CC.szTestTimeC);

			if(m_strConnectTime <itime && m_strConnectTime >(itime-50))
			{
				AfxMessageBox("電源線即將超過管控上限,請TE及時更換電源線!\r\n Please call the TE to change the Power cable!");
			}

			else if(m_strConnectTime >= itime)
			{
				if(IDOK == m_ConnectTime.DoModal())
				{
					CString strPassWord = m_ConnectTime.m_PassWord;
					if(strPassWord.IsEmpty())
					{
						AfxMessageBox(_T("電源線超過管控上限,請更換并輸入正確的密碼！\r\n Please input the right password!"));
						::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
						return 0;

					}

					else if(strPassWord == "foxconn168!")
					{
						m_strConnectTime = 0;
						UpdateData(false);
					}

					else
					{
						::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
						return 0;
					}

				}
				else
				{
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
					return 0;
				}
			}

			if(m_strConnectTimeN <itimeN && m_strConnectTimeN >(itimeN-50))
			{
				AfxMessageBox("RJ45即將超過管控上限,請TE及時更換RJ45線！\r\n Please call the TE to change the RJ45!");
			}
			else if(m_strConnectTimeN >= itimeN)
			{
				if(IDOK == m_ConnectTimeN.DoModal())
				{
					CString strPassWord = m_ConnectTimeN.m_PassWord;
					if(strPassWord.IsEmpty())
					{
						AfxMessageBox(_T("RJ45超過管控上限,請更換RJ45并輸入正確的密碼！\r\n Please input the right password!"));
						::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
						return 0;
					}

					else if(strPassWord == "foxconn168!")
					{
						m_strConnectTimeN = 0;
						UpdateData(false);
					}

					else
					{
						::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
						return 0;
					}
				}
				else
				{
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
					return 0;
				}
			}


			if(m_strConnectTimeC <itimeC && m_strConnectTimeC >(itimeC-50))
			{
				AfxMessageBox("CONNECTOR即將超過管控上限,請TE及時跟換CONNECTOR!\r\n Please call the TE to change the CONNECTOR!");
			}

			else if(m_strConnectTimeC >= itimeC)
			{
				if(IDOK == m_ConnectTimeC.DoModal())
				{
					CString strPassWord = m_ConnectTimeC.m_PassWord;
					if(strPassWord.IsEmpty())
					{
						AfxMessageBox(_T("CONNECTOR超過管控上限,請更換CONNECTOR并輸入正確的密碼！\r\n Please input the right password!"));
						::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
						return 0;
					}
					else if(strPassWord == "foxconn168!")
					{
						m_strConnectTimeC = 0;
						UpdateData(false);
					}
					else
					{
						::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
						return 0;
					}
				}
				else
				{
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
					return 0;
				}
			}
		}

		//char szTestSetupPath[MAX_PATH] = "";
		//char szLocalTestSetupPath[MAX_PATH] = "";
		//char szSerTestSetupPath[MAX_PATH] = "";			
		//GetPrivateProfileString("FilePath","ServerInitFilePath","0" ,INI.FP.szPath, MAX_PATH,szINITPath);  /*init.ini path at server*/
		GetPrivateProfileString("FilePath","ServerFilePath1","0" ,INI.FP.szPath1, MAX_PATH,szINITPath);/*2gconfig.txt path at server*/
		//GetPrivateProfileString("FilePath","ServerFilePath2","0" ,INI.FP.szPath2, MAX_PATH,szINITPath);/*5gconfig.txt path at server*/
		//GetPrivateProfileString("FilePath","ServerFilePath3","0" ,INI.FP.szPath3, MAX_PATH,szINITPath);/*SFIS.txt path at server*/
		//GetPrivateProfileString("FilePath","ServerFilePath4","0" ,INI.FP.szPath4, MAX_PATH,szINITPath);/*File path4 at server*/
		//GetPrivateProfileString("FilePath","ServerFilePath5","0" ,szPath5, MAX_PATH,szINITPath);/*File path5 at server*/
		//GetPrivateProfileString("General","TestBybat","0" ,INI.GEN.szTestByBat, MAX_PATH,szINITPath);/*3*3 need test by BAT file*/		
		if(IDYES==AfxMessageBox(_T("是否要加入SFIS\nSFIS ON/OFF?"),MB_YESNO|MB_ICONQUESTION))
		{
			//char DefaultIP[MAX_PATH] = "";
			//char RetryNum[MAX_PATH] = "";
			//char PowerSpec[MAX_PATH] = "";
			//char SerTestFlow[MAX_PATH] = "";
			//char SerTestSetup[MAX_PATH] ="";
			//char Bat[MAX_PATH] = "";

			//char szLocalTestFlowPath[MAX_PATH] = "";
			//char szTestFlowPath[MAX_PATH] = "";
			//char szLocalBatPath[MAX_PATH] = "";


			m_blIsSfis = true;
			((CEdit*)GetDlgItem(IDC_EDIT_SN))->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_BTN_START))->EnableWindow(false);
			((CButton*)GetDlgItem(IDC_BTN_QUIT))->EnableWindow(false);

			char COM[256] = "";
			sprintf_s(COM,256,"com%d",iCOM);
			if(!m_ComSfis.Open(COM, 9600))
			{
				AfxMessageBox("默認連接SFIS的COM1口不存在或者被其他應用程序占用\n請從新選擇COM與SFIS連接\nThe default COM1 don't exit or is used by other application!\nPls choose COM port again");			
				return TRUE;
			}
			m_ComSfis.SendData(("Hello\r\n"), (int)strlen("Hello\r\n"));

			Sleep(1000);
			char pBuf[MAXBUFSIZE] = "";
			m_ComSfis.ReadData(pBuf, sizeof(MAXBUFSIZE), 2000);

			//AfxMessageBox(pBuf);
			if( (strstr(pBuf, "ERRO") == NULL) && (strstr(pBuf, "PASS") == NULL))
			{
				AfxMessageBox(pBuf);
				AfxMessageBox(_T("請從新選擇COM與SFIS連接\n"));
				m_ComSfis.Close();
				return TRUE;
			}
			else
			{
				m_strSfisStatus = _T("SFIS ON");
				UpdateData(false);
				m_blIsOpenSfis = true;
				AfxBeginThread(ReadComForSfis, &m_ComSfis);				
			}

		}
		else
		{
			if(IDOK == m_IsStartSfis.DoModal())
			{
				CString  strName = m_IsStartSfis.m_ProductName;
				if(strName.IsEmpty())
				{
					AfxMessageBox(_T("請輸入机种名稱,不能為空"));
					::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
				}
				else
				{
					m_strProductName = m_IsStartSfis.m_ProductName;
					_strupr_s(m_strProductName.GetBuffer(m_strProductName.GetLength()), m_strProductName.GetLength()+1);
					UpdateData(false);
				}
			}
			else
			{
				::PostMessage(GetParent()->GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
			}
		}

		ReadIniFile();
		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	}
	catch(...)
	{
		AfxMessageBox("On InitDialog fail");
		return 0;

	}
}


void CRunInfo::SendSfisResultAndShowFail(char* ErroCode)
{
	try
	{
		char szErrorCode[MINBUFSIZE] = "";
		strncpy_s(szErrorCode, MINBUFSIZE, ErroCode, 4);
		if(m_blIsOpenSfis)
		{	
			char szSendSfisData[MINBUFSIZE] = "";
			char szMac[20] = "";
			DisplayRunTimeInfo("INI.GEN.szSFCProductLength=");
			DisplayRunTimeInfo(INI.GEN.szSFCProductLength);
			DisplayRunTimeInfo("\r\n");
			if(!strcmp(INI.GEN.szSFCProductLength , "25"))
			{
				sprintf_s(szSendSfisData, MINBUFSIZE, "%-25s%-12s%-4s\r\n", m_strSN, m_strPcName, szErrorCode);
				szSendSfisData[25+12+4+2] = '\0';
			}	
			else if(!strcmp(INI.GEN.szSFCProductLength , "12"))
			{
				sprintf_s(szSendSfisData, MINBUFSIZE, "%-25s%-12s%-12s%-4s\r\n", m_strSN.GetBuffer(m_strSN.GetLength()), szMac, m_strPcName.GetBuffer(m_strPcName.GetLength()),szErrorCode);
				szSendSfisData[25+12+12+4+2] = '\0';
			}
			else
			{
				//do nothing
			}

			DisplayRunTimeInfo("szSendSfisData=");
			DisplayRunTimeInfo(szSendSfisData);
			DisplayRunTimeInfo("\r\n");
			m_ComSfis.SendData(szSendSfisData, (int)strlen(szSendSfisData));
		}
		TestFail(ErroCode);		
	}
	catch(...)
	{ 
		AfxMessageBox("SendSfisResultAndShowFail fail");
	}
}

void CRunInfo::SendSfisResultAndShowPass()
{
	try
	{
		if(m_blIsOpenSfis)	
		{
			DisplayTestInfo("INI.GEN.szDisableWireless=",INI.GEN.szDisableWireless);				
			if(OnSFIS && (strstr(INI.GEN.szDisableWireless , "1")))
			{
				BOOL bRunTelnet = FAIL;
				CString strTemp;
				char szRunTelnetExeFullName[MAXBUFSIZE] = "";

				CString  strArpInfo;
				TCHAR    szArp[MINBUFSIZE] = "";
				char     szMac[MINBUFSIZE] = "";

				DisplayTestInfo("INI.GEN.szDefaultIP=",INI.GEN.szDefaultIP);

				//sprintf_s(szArp,"arp -a %s",INI.GEN.szDefaultIP);
				//RunSpecifyExeAndRead(strArpInfo,szArp,true);
				//GetMac(strArpInfo.GetBuffer(strArpInfo.GetLength()+1),szMac);
				//_strupr_s(szMac,MINBUFSIZE);

				////sprintf_s(szRunTelnetExeFullName, MAXBUFSIZE, "telnetenable.exe %s %s Gearguy Geardog",INI.GEN.szDefaultIP,szMac);
				//sprintf_s(szRunTelnetExeFullName, MAXBUFSIZE, "telnetenable.exe %s %s Gearguy Geardog",INI.GEN.szDefaultIP,"000000000001");
				//for(int i = 0; i<6; i++)
				//{
				//	if(RunSpecifyExeAndRead(strTemp,szRunTelnetExeFullName,false))
				//	{
				//		bRunTelnet = PASS;
				//		break;
				//	}
				//	else
				//		Sleep(1000);
				//}
				//if(bRunTelnet == FAIL)
				//{
				//	TestFail(IN20);
				//	return ;
				//}

				ResetEvent(pRunInfo->m_hEvt);
				if(!pRunInfo->InitSocket())
				{
					TestFail(IN20);
					return ;
				}

				int iTimeout=100;	
				while(iTimeout--)
				{
					if(strstr(pRunInfo->m_strStoreEthernetData,"BusyBox") == NULL)
					{
						Sleep(100);
					}	
					else
						break;
				}
				if(!iTimeout)
				{
					TestFail(IN20);
					return ;
				}

				SendDutCommand("nvram set wla_wlanstate=Disable" , "#" , 1000);
				SendDutCommand("nvram set wlg_wlanstate=Disable" , "#" , 1000);		
				SendDutCommand("nvram set boot_wait=off" , "#" , 1000);
				SendDutCommand("nvram commit" , "#" , 3000);

			}

			char szSendSfisData[MINBUFSIZE] = "";
			char szMac[20] = "";
			DisplayRunTimeInfo("INI.GEN.szSFCProductLength=");
			DisplayRunTimeInfo(INI.GEN.szSFCProductLength);
			DisplayRunTimeInfo("\r\n");
			if(!strcmp(INI.GEN.szSFCProductLength , "25"))
			{
				sprintf_s(szSendSfisData, MINBUFSIZE, "%-25s%-12s\r\n", m_strSN.GetBuffer(m_strSN.GetLength()), m_strPcName.GetBuffer(m_strPcName.GetLength()));
				szSendSfisData[25+12+2] = '\0';
			}
			else if(!strcmp(INI.GEN.szSFCProductLength , "12"))
			{
				sprintf_s(szSendSfisData, MINBUFSIZE, "%-25s%-12s%-12s\r\n", m_strSN.GetBuffer(m_strSN.GetLength()), szMac, m_strPcName.GetBuffer(m_strPcName.GetLength()));
				szSendSfisData[25+12+12+2] = '\0';
			}
			else
			{
				//do nothing
			}

			DisplayRunTimeInfo("szSendSfisData=");
			DisplayRunTimeInfo(szSendSfisData);
			DisplayRunTimeInfo("\r\n");

			m_ComSfis.SendData(szSendSfisData, (int)strlen(szSendSfisData));	
		}
		TestPass();
	}
	catch(...)
	{
		AfxMessageBox("SendSfisResultAndShowPass fail");
	}
}


void CRunInfo::TestFail(char* ErroCode)
{
	try
	{
		bool bPost = true;
		if((strstr(ErroCode,"SL") != NULL) ||
		   (strstr(ErroCode,"SY") != NULL) ||
		   (strstr(ErroCode,"SR") != NULL) )
		{
			bPost = false;
		}
		if(bPost)
		{
			/*Add Mydas*/
			DisplayRunTimeInfo("INI.MY.szPostMydas=");
			DisplayRunTimeInfo(INI.MY.szPostMydas);
			DisplayRunTimeInfo("\r\n");
			DisplayTestInfo("INI.MY.szMydasDebug=" , INI.MY.szMydasDebug);
			if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
			{
				if(LinkDataBase())
				{
					char temp[1024] = "";
					CString strErrInfo = ErroCode;
					sprintf_s(temp,1024,"%s,%s,|",strErrInfo.Trim().Left(4),strErrInfo.Trim().Mid(5,strErrInfo.GetLength()-5).Trim());
					strcpy_s(g_ErrDef,BUF_LENGH,temp);
					SYSTEMTIME SystemTime;
					GetSystemTime(&SystemTime);
					char time[BUF_LENGH] = "";
					sprintf_s(time,BUF_LENGH,"%d-%d-%d %d:%d:%d",SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
					char temp2[BUF_LENGH] = "";
					sprintf_s(temp2,BUF_LENGH,"%s,0,%s,V0.0.1,%s,%s,%s,",m_strSN.Trim(),szRecordFilePath,pRunInfo->m_strPcName,pRunInfo->m_strTestTime,time);

					sprintf_s(g_MyDAS,BUF_LENGH*4,"%s," , g_MyDAS);

					DisplayTestInfo("Failed mydas detail test data=" , g_MyDAS);				
					DisplayTestInfo("Failed mydas error info data=" , g_ErrDef);
					DisplayTestInfo("Failed mydas main define data=" , temp2);

					if(!SendDatatoMYDAS(g_MyDAS,g_ErrDef,temp2))
					{
						pRunInfo->SendSfisResultAndShowFail(SY05);
						return;
					}
				}
				memset(g_MyDAS , 0 , sizeof(g_MyDAS));
			}

			/*End*/
		}
		char sztemp[1024] = "";
		CString strErrInfo = ErroCode;
		sprintf_s(sztemp,1024,"%s",strErrInfo.Trim().Left(4));

		//Rename test log file
	
		char szLog_allFilePath[MAXBUFSIZE] = "";
		GetCurrentDirectory(sizeof(szLog_allFilePath),szLog_allFilePath);	
		lstrcat(szLog_allFilePath,"\\log\\log_all.txt");

		if(PathFileExists(szLog_allFilePath))
		{
			char szRecordFilePath_Server  [MAXBUFSIZE] = "";
			char szRecordFilePath[MAXBUFSIZE] = "";
			CTime    Time = CTime::GetCurrentTime();
			CString  strFileName = pRunInfo->m_strSN.Trim()+Time.Format("_%Y-%m-%d-%H-%M-%S");
			sprintf_s(szRecordFilePath, MAXBUFSIZE, ".\\log\\%s_%s.txt", strFileName ,sztemp);			
			rename(".\\log\\log_all.txt",szRecordFilePath);	

			//copy log file to server
			DisplayTestInfo("INI.FP.szRecodeServerFlag=" , INI.FP.szRecodeServerFlag);	
			if(strstr(INI.FP.szRecodeServerFlag , "1") != NULL )
			{
				sprintf_s(szRecordFilePath_Server, MAXBUFSIZE, "%s\\%s.txt", ServerDir_LOG_PRODUCT_FAIL, strFileName);
				CopyFile( szRecordFilePath , szRecordFilePath_Server , FALSE );		
			}

		}

		/*Recode fail test UI log*/
		char szRecordFailFilePath_UI  [MAXBUFSIZE] = "";
		sprintf_s(szRecordFailFilePath_UI, MAXBUFSIZE, "D:\\Logs\\FAIL\\%s_%s.txt", m_strSN , sztemp );
		fsFail.open (szRecordFailFilePath_UI   ,fstream::out|fstream::app);
		fsFail<<strRunInfo.GetBuffer(strRunInfo.GetLength())<<endl;
		fsFail.close();
		/*End*/


		KillTimer(1);
		IsDisplayErrorCode(true);
		++m_strConnectTime;
		++m_strConnectTimeN;
		++m_strConnectTimeC;
		m_strTestResult = "FAIL";
		m_strErroCode = ErroCode;

		m_strSN = "";
		m_strMAC = "";

		UpdateData(false);



		if(!m_blIsSfis)
		{
			((CButton*)GetDlgItem(IDC_BTN_START))->EnableWindow(true);
			((CButton*)GetDlgItem(IDC_BTN_QUIT))->EnableWindow(true);
		}

		DisplayTestInfo("INI.TF.szTestWiFiCommonFlag=" , INI.TF.szTestWiFiCommonFlag);	
		if(strstr(INI.TF.szTestWiFiCommonFlag , "1"))
		{

			char buf[BUF_LENGH]="";
			/////////////////DisConnect IQ//////
			memset(buf,0,BUF_LENGH);
			printf_s("DisConnect IQ tester_________");
			printf_s("\r\n");
			if(!ambit_Disconnect_DUT(buf,BUF_LENGH))
			{
				printf_s(buf);
				printf_s("\r\n");
			}
			else
			{
				DisplayRunTimeInfo("ambit_DISCONNECT_IQ_TESTER return(OK)");
				printf_s("\r\n\r\n");
			}
			//////////////ambit_REMOVE_DUT//////////
			memset(buf,0,BUF_LENGH);
			printf_s("ambit_REMOVE_DUT__________");
			printf_s("\r\n");
			if(!ambit_REMOVE_DUT(buf,BUF_LENGH))
			{
				printf_s(buf);
				printf_s("\r\n");			
			}
			else
			{
				DisplayRunTimeInfo("ambit_REMOVE_DUT return(OK)");
				printf_s("\r\n");
			}
		}		
		/////////////////~CRunInfo///////////	
		if(NULL != m_hEvt)
			CloseHandle(m_hEvt);			
		return;

	}
	catch(...)
	{
		AfxMessageBox("TestFail fail");
	}
}


void CRunInfo::TestPass()
{
	try
	{		
		/*Add Mydas*/
		DisplayRunTimeInfo("INI.MY.szPostMydas=");
		DisplayRunTimeInfo(INI.MY.szPostMydas);
		DisplayRunTimeInfo("\r\n");
		DisplayRunTimeInfo("INI.MY.szMydasDebug=");
		DisplayRunTimeInfo(INI.MY.szMydasDebug);
		DisplayRunTimeInfo("\r\n");
		if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
		{
			if(LinkDataBase())
			{
				char temp[1024] = "";
				sprintf_s(temp,1024,",,|");
				strcpy_s(g_ErrDef,BUF_LENGH,temp);
				SYSTEMTIME SystemTime;
				GetSystemTime(&SystemTime);
				char time[BUF_LENGH] = "";
				sprintf_s(time,BUF_LENGH,"%d-%d-%d %d:%d:%d",SystemTime.wYear,SystemTime.wMonth,SystemTime.wDay,SystemTime.wHour,SystemTime.wMinute,SystemTime.wSecond);
				char temp2[BUF_LENGH] = "";
				sprintf_s(temp2,BUF_LENGH,"%s,1,%s,V0.0.1,%s,%s,%s,",pRunInfo->m_strSN.Trim(),szRecordFilePath,pRunInfo->m_strPcName,pRunInfo->m_strTestTime,time);
				
				sprintf_s(g_MyDAS,BUF_LENGH*4,"%s," , g_MyDAS);				
				
				DisplayTestInfo("Passed mydas detail test data=" , g_MyDAS);				
				DisplayTestInfo("Passed mydas error info data=" , g_ErrDef);
				DisplayTestInfo("Passed mydas main define data=" , temp2);
				
				if(!SendDatatoMYDAS(g_MyDAS,g_ErrDef,temp2))
				{
					pRunInfo->SendSfisResultAndShowFail(SY06);
					return;
				}
			}
			memset(g_MyDAS , 0 , sizeof(g_MyDAS));
		}		
		/*End*/


		//Rename test log		
		
		char szLog_allFilePath[MAXBUFSIZE] = "";
		GetCurrentDirectory(sizeof(szLog_allFilePath),szLog_allFilePath);	
		lstrcat(szLog_allFilePath,"\\log\\log_all.txt");
		if(PathFileExists(szLog_allFilePath))		
		{
			char szRecordFilePath_Server  [MAXBUFSIZE] = "";
			char szRecordFilePath[MAXBUFSIZE] = "";
			CTime    Time = CTime::GetCurrentTime();
			CString  strFileName = pRunInfo->m_strSN.Trim()+Time.Format("_%Y-%m-%d-%H-%M-%S");
			sprintf_s(szRecordFilePath, MAXBUFSIZE, ".\\log\\%s.txt",strFileName );			
			rename(".\\log\\log_all.txt",szRecordFilePath);	

			//copy log file to server
			DisplayTestInfo("INI.FP.szRecodeServerFlag=" , INI.FP.szRecodeServerFlag);	
			if(strstr(INI.FP.szRecodeServerFlag , "1") != NULL )
			{
				sprintf_s(szRecordFilePath_Server, MAXBUFSIZE, "%s\\%s.txt", ServerDir_LOG_PRODUCT_PASS  , strFileName);
				CopyFile( szRecordFilePath , szRecordFilePath_Server , FALSE );
			}
		}



		
		/*Recode fail test UI log*/
		char szRecordPassFilePath_UI  [MAXBUFSIZE] = "";		

		sprintf_s(szRecordPassFilePath_UI, MAXBUFSIZE, "D:\\Logs\\PASS\\%s.txt", m_strSN);
		fsPass.open (szRecordPassFilePath_UI   ,fstream::out|fstream::app);
		fsPass<<strRunInfo.GetBuffer(strRunInfo.GetLength())<<endl;
		fsPass.close();
		/*End*/

		SetFlagForIsSameDut(false);
		IsGeneralTx=true;
		IsGeneralRx=true;
		IsGeneralTxForDualBand=true;
		IsGeneralRxForDualBand=true;
		SetFlagForIsSameDut(false);
		IsGeneralTx=true;
		IsGeneralRx=true;
		IsGeneralTxForDualBand=true;
		IsGeneralRxForDualBand=true;

		KillTimer(1);
		m_strTestResult = "PASS";
		m_strSN = "";
		m_strMAC = "";
		++m_strConnectTime;
		++m_strConnectTimeN;
		++m_strConnectTimeC;
		UpdateData(false);

		if(!m_blIsSfis)
		{
			((CButton*)GetDlgItem(IDC_BTN_START))->EnableWindow(true);
			((CButton*)GetDlgItem(IDC_BTN_QUIT))->EnableWindow(true);
		}

		DisplayTestInfo("INI.TF.szTestWiFiCommonFlag=" , INI.TF.szTestWiFiCommonFlag);	
		if(strstr(INI.TF.szTestWiFiCommonFlag , "1"))
		{

			////////////DisConnect IQ//////////
			char buf[BUF_LENGH]="";
			memset(buf,0,BUF_LENGH);
			printf_s("DisConnect IQ tester__________");
			printf_s("\r\n");
			if(!ambit_Disconnect_DUT(buf,BUF_LENGH))
			{
				printf_s(buf);
				printf_s("\r\n");
			}
			else
			{
				DisplayRunTimeInfo("ambit_DISCONNECT_IQ_TESTER return(OK)");
				printf_s("\r\n");
			}
			////////////Remove Dut//////////
			memset(buf,0,BUF_LENGH);
			printf_s("ambit_REMOVE_DUT____________");
			printf_s("\r\n");
			if(!ambit_REMOVE_DUT(buf,BUF_LENGH))
			{
				printf_s(buf);
				printf_s("\r\n");			
			}
			else
			{
				DisplayRunTimeInfo("ambit_REMOVE_DUT return(OK)");
				printf_s("\r\n");
			}
			////////////~CRunInfo////////////
		}
		if(NULL != m_hEvt)
			CloseHandle(m_hEvt);

				
		return;
	}
	catch(...)
	{
		AfxMessageBox("TestPass fail");
	}
}

void CRunInfo::OnTimer(UINT_PTR nIDEvent)
{
	try
	{
		// TODO: Add your message handler code here and/or call default
		m_TestTimeCount+=1;
		CString strTimeCount = m_TestTimeCount.Format("%M:%S");	
		m_strTestTime = strTimeCount;
		UpdateData(false);
		CDialog::OnTimer(nIDEvent);
	}
	catch(...)
	{
		AfxMessageBox("OnTimer fail");
	}
}

int CRunInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  Add your specialized creation code here
	return 0;
}


void CRunInfo::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

//	CDialog::OnOK();
}



void CRunInfo::DisplayRunTimeInfo(const char* pRunInfo)
{
	try
	{
	//static CString strRunInfo;
	strRunInfo += pRunInfo;
	if(pRunInfo == NULL)
	{
		strRunInfo.Empty();
	}
	SetDlgItemText(IDC_EDIT_RUNINFO,strRunInfo);
	int a = ((CEdit*)GetDlgItem(IDC_EDIT_RUNINFO))->GetLineCount();
	((CEdit*)GetDlgItem(IDC_EDIT_RUNINFO))->LineScroll(a,0);
	}
	catch(...)
	{
		AfxMessageBox("DisplayRunTimeInfo fail");
	}
}

void CRunInfo::IsDisplayErrorCode(bool Flag)
{
	try
	{
		if(Flag)
		{
			((CStatic*)GetDlgItem(IDC_ERRORCODE))->ShowWindow(SW_SHOW);
		}
		else
		{
			((CStatic*)GetDlgItem(IDC_ERRORCODE))->ShowWindow(SW_HIDE);
		}
	}
	catch(...)
	{
		AfxMessageBox("IsDisplayErrorCode fail");
	}
}



LRESULT  CRunInfo::OnPass(WPARAM wParam,LPARAM lParam)
{
	try
	{
		char pBuf [MINBUFSIZE] = "";
		strcpy_s(pBuf, MINBUFSIZE, (char*)lParam);
		memset((char*)lParam, 0 , MINBUFSIZE);
		SendSfisResultAndShowPass();
		return 1;
	}
	catch(...)
	{
		AfxMessageBox("OnPass fail");
		return 0;
	}
}

LRESULT  CRunInfo::OnFail(WPARAM wParam,LPARAM lParam)
{
	try
	{
		char pBuf [MINBUFSIZE] = "";
		strcpy_s(pBuf, MINBUFSIZE, (char*)lParam);
		memset((char*)lParam, 0 , MINBUFSIZE);
		SendSfisResultAndShowFail(pBuf);
		return 1;
	}
	catch(...)
	{
		AfxMessageBox("OnFail fail");
		return 0;
	}
}

LRESULT  CRunInfo::OnRecvComData(WPARAM wParam,LPARAM lParam)
{
	char pBuf [MINBUFSIZE] = "";
	strcpy_s(pBuf, MINBUFSIZE, (char*)lParam);

	DisplayTestInfo("Recive SFIS data:",pBuf);
	DisplayRunTimeInfo("\r\n\r\n");
	memset((char*)lParam, 0 , MINBUFSIZE);
	int len = (int)strlen(pBuf);
	if(len < 25 + 12 + 4)
	{
		TestFail(SR00);
		return 0;
	}
	else if(len == 25+25+4+2)//25 start
	{
		if(!strcmp(m_strTestResult , "RUN"))
			return 0;
		char szSN[MINBUFSIZE] = "";
		char szStatus[MINBUFSIZE] = "";
		char szProductName[MINBUFSIZE] = "";
		strncpy_s(szSN, MINBUFSIZE, pBuf, 25);
		strncpy_s(szProductName, MINBUFSIZE, pBuf+25, 25);
		strncpy_s(szStatus, MINBUFSIZE, pBuf+50, 4);
		m_strSN = szSN;
		UpdateData(false);
		m_strProductName = szProductName;
		static bool bIniProduct = true ;
		if(bIniProduct)
		{
			bIniProduct = false;
			ProductName = m_strProductName;
		}
		m_strProductName.Trim();
		if(strcmp(ProductName, szProductName))
		{
			TestFail(SY15);
			return 0;
		}
		UpdateData(false);
		if(strncmp(szStatus, "PASS", 4))
			return 0;
		OnBnClickedBtnStart();
	}
	else if(len == 25+12+4+2)
	{
		if(!strcmp(INI.GEN.szSFCProductLength , "25"))//25 pass
		{
			if(strstr(pBuf, "PASS") == NULL)
			{
				TestFail(SR00);	
				return 0;
			}
		}
		else if(!strcmp(INI.GEN.szSFCProductLength , "12"))//12 start
		{
			if(!strcmp(m_strTestResult , "RUN"))
				return 0;
			char szSN[MINBUFSIZE] = "";
			char szStatus[MINBUFSIZE] = "";
			char szProductName[MINBUFSIZE] = "";
			strncpy_s(szSN, MINBUFSIZE, pBuf, 25);
			strncpy_s(szProductName, MINBUFSIZE, pBuf+25, 12);
			strncpy_s(szStatus, MINBUFSIZE, pBuf+37, 4);
			m_strSN = szSN;
			UpdateData(false);
			m_strProductName = szProductName;
			static bool bIniProduct = true ;
			if(bIniProduct)
			{
				bIniProduct = false;
				ProductName = m_strProductName;
			}
			m_strProductName.Trim();

			//AfxMessageBox(ProductName);
			//AfxMessageBox(szProductName);

			if(strcmp(ProductName, szProductName))
			{
				TestFail(SY15);
				return 0;
			}
			UpdateData(false);
			if(strncmp(szStatus, "PASS", 4))
				return 0;
			OnBnClickedBtnStart();
		}
		else
		{
			//do nothing
		}
	}
	else if(len == 25+12+4+4+2)//25 fail
	{
		if(strstr(pBuf, "PASS") == NULL)
		{
			TestFail(SR00);
			return 0;
		}
	}
	else if(len == 25+12+12+4+2)//12 pass
	{
		if(strstr(pBuf, "PASS") == NULL)
		{
			TestFail(SR00);
			return 0;
		}
	}
	else if(len == 25+12+12+4+4+2)//12 fail
	{
		if(strstr(pBuf, "PASS") == NULL)
		{
			TestFail(SR00);
			return 0;
		}
	}
	else
	{
		if(strcmp(m_strTestResult , "RUN"))
		{		
			TestFail(SL00);
		}
		return 0;
		//do nothing
	}
	return 1;
}

UINT __cdecl  CRunInfo::ReadComForSfis(LPVOID param)
{
	try
	{
		CCom    *SfisCom = (CCom*)param;
		DWORD   dwEvt;
		DWORD   dwErrorFlags;
		COMSTAT ComStat;
		char    pReadBuf[MINBUFSIZE] = "";

		SetCommMask(SfisCom->m_hCom, EV_RXCHAR);

		while(1)
		{
			WaitCommEvent(SfisCom->m_hCom, &dwEvt, NULL);
			if((dwEvt & EV_RXCHAR) == EV_RXCHAR)
			{
				ClearCommError(SfisCom->m_hCom ,&dwErrorFlags,&ComStat);
				if(ComStat.cbInQue>0)
				{
					SfisCom->ReadData(pReadBuf, MINBUFSIZE, 1200);
					::PostMessage(SfisCom->m_hParentWnd, WM_COMET, 0, (LPARAM)pReadBuf);
				}
			}
		}
		return 1;
	}
	catch(...)
	{
		AfxMessageBox("ReadComForSfis fail");
		return 0;
	}
}

void GeneralCmdForDualBand(vector<string> &cmdList,bool IsTxCmd)
{
	try
	{
		cmdList.clear();

		if(IsTxCmd)
		{
			if(IsGeneralTxForDualBand)
			{
				IsGeneralTxForDualBand=false;
				cmdList.push_back("wl -i eth2 radio on\n");
				cmdList.push_back("wl -i eth2 mpc 0\n");
				cmdList.push_back("wl -i eth2 down\n");
				cmdList.push_back("wl -i eth2 country ALL\n");
				cmdList.push_back("wl -i eth2 wsec 0\n");
				cmdList.push_back("wl -i eth2 interference 0\n");
				cmdList.push_back("wl -i eth2 band a\n");
				cmdList.push_back("wl -i eth2 frameburst 0\n");
				cmdList.push_back("wl -i eth2 ampdu 0\n");
				cmdList.push_back("wl -i eth2 bi 100\n");
				cmdList.push_back("wl -i eth2 frameburst 1\n");
				cmdList.push_back("wl -i eth2 mimo_bw_cap 1\n");
				cmdList.push_back("wl -i eth2 up\n");
			}
		}
		else
		{
			if(IsGeneralRxForDualBand)
			{
				IsGeneralRxForDualBand=false;
				cmdList.push_back("wl -i eth2 radio on\n");
				cmdList.push_back("wl -i eth2 mpc 0\n");
				cmdList.push_back("wl -i eth2 down\n");
				cmdList.push_back("wl -i eth2 country ALL\n");
				cmdList.push_back("wl -i eth2 wsec 0\n");
				cmdList.push_back("wl -i eth2 interference 0\n");
				cmdList.push_back("wl -i eth2 band a\n");
				cmdList.push_back("wl -i eth2 bi 65535\n");
				cmdList.push_back("wl -i eth2 frameburst 0\n");
				cmdList.push_back("wl -i eth2 mimo_bw_cap 1\n");
				cmdList.push_back("wl -i eth2 up\n");
			}
		}
	}
	catch(...)
	{
		AfxMessageBox("GeneralCmdForDualBand fail");
	}
}

void GeneralCmdForSingleBand(vector<string> &cmdList,bool IsTxCmd)
{
	
	try
	{
		cmdList.clear();

		if(IsTxCmd)
		{
			if(IsGeneralTx)
			{
				IsGeneralTx=false;
				cmdList.push_back("wl radio on\n");
				cmdList.push_back("wl mpc 0\n");
				cmdList.push_back("wl down\n");
				cmdList.push_back("wl country ALL\n");
				cmdList.push_back("wl wsec 0\n");
				cmdList.push_back("wl interference 0\n");
				cmdList.push_back("wl band b\n");
				cmdList.push_back("wl frameburst 0\n");
				cmdList.push_back("wl ampdu 0\n");
				cmdList.push_back("wl bi 100\n");
				cmdList.push_back("wl frameburst 1\n");
				cmdList.push_back("wl mimo_bw_cap 1\n");
				cmdList.push_back("wl up\n");
			}
		}
		else
		{
			if(IsGeneralRx)
			{
				IsGeneralRx=false;
				cmdList.push_back("wl radio on\n");
				cmdList.push_back("wl mpc 0\n");
				cmdList.push_back("wl down\n");
				cmdList.push_back("wl country ALL\n");
				cmdList.push_back("wl wsec 0\n");
				cmdList.push_back("wl interference 0\n");
				cmdList.push_back("wl band b\n");
				cmdList.push_back("wl bi 65535\n");
				cmdList.push_back("wl frameburst 0\n");
				cmdList.push_back("wl mimo_bw_cap 1\n");
				cmdList.push_back("wl up\n");
			}
		}
	}
	catch(...)
	{
		AfxMessageBox("GeneralCmdForSingleBand fail");
	}
}

void CRunInfo::DeleteSpecifyExe(char* exeName)
{
	
	try
	{
		if(exeName == NULL)
			return;
		HANDLE hProcess = INVALID_HANDLE_VALUE;
		PROCESSENTRY32 pe;
		pe.dwSize = sizeof(PROCESSENTRY32);
		hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
		if(INVALID_HANDLE_VALUE == hProcess)
			return;
		BOOL fPOK = Process32First(hProcess,&pe);
		for(; fPOK;fPOK = Process32Next(hProcess,&pe))
		{
			char szVal1[100]="";
			char szVal2[100]="";
			sprintf_s(szVal1,sizeof(szVal1),"%s",pe.szExeFile);
			sprintf_s(szVal2,sizeof(szVal2),"%s",exeName);
			_strupr_s(szVal1,100);
			_strupr_s(szVal2,100);
			if(strstr(szVal1,szVal2) != NULL)
			{
				HANDLE hPr = OpenProcess( PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID );
				if( hPr == NULL )
					return;
				else
				{
					TerminateProcess(hPr,0);
				}
			}
		}
		if(INVALID_HANDLE_VALUE != hProcess)
			CloseHandle(hProcess);
	}
	catch(...)
	{
		AfxMessageBox("DeleteSpecifyExe fail");
	}
}

int CRunInfo::Run_WiFiTest(CString RunBATName , 
						  CString IQLogFile , 
						  CString IQLogPath ,
						  CString IQTestFinishFlagFile ,
						  CString BATFilePath ,
						 // CString ServeBATPath ,
						  CString WindowsName 
						  )
/*use example 
*Run189_5GTest("5GRun.bat" ,
				"C:\\Program Files\\LitePoint\\IQfact_plus\\IQfact_plus_Broadcom_4331_TELNET_1.0.14.Eng1\\Log\\log_all.txt" ,
				"C:\\Program Files\\LitePoint\\IQfact_plus\\IQfact_plus_Broadcom_4331_TELNET_1.0.14.Eng1\\Log" ,
				"C:\\Program Files\\LitePoint\\IQfact_plus\\IQfact_plus_Broadcom_4331_TELNET_1.0.14.Eng1\\done.txt" ,
				"C:\\Program Files\\LitePoint\\IQfact_plus\\IQfact_plus_Broadcom_4331_TELNET_1.0.14.Eng1\\5GRun.bat",
				//"F:\\lsy\\Test\\量產PT\\PT_U12H181\\U12H181_1220\\5GRun.bat",
				"U12H181"
				)

*/
{
	char szbuf[128];
	int iTestResult=0;
	bool bFindFail=false;

	char szIQLogFile[MINBUFSIZE] = "" ;
	char szIQLogPath[MINBUFSIZE] = "" ;
	char szIQTestFinishFlagFile[MINBUFSIZE] = "" ;
	sprintf_s( szIQLogFile , MINBUFSIZE , "%s" , IQLogFile);
	sprintf_s( szIQLogPath , MINBUFSIZE , "%s" , IQLogPath);
	sprintf_s( szIQTestFinishFlagFile , MINBUFSIZE , "%s" , IQTestFinishFlagFile);

	//1.check IQTestFinishFlagFile
	
	if(PathFileExists(szIQTestFinishFlagFile))
	{
		DeleteFile(szIQTestFinishFlagFile);
	}
	//2.check logfile
	if(PathFileExists(szIQLogFile))
	{
		DeleteFile(szIQLogFile);
	}
	//3.check BAT file at system32 is new
	CString strBat = BATFilePath ;
	char szBATPath[MINBUFSIZE] = "";
	sprintf_s(szBATPath , MINBUFSIZE , "C:\\WINDOWS\\system32\\%s" , RunBATName);
	if(PathFileExists(szBATPath))
	{
		DeleteFile(szBATPath);
	}
	
	char szProPath[MINBUFSIZE]="";	
	GetCurrentDirectory(sizeof(szProPath),szProPath);
	sprintf_s(szProPath, MINBUFSIZE, "%s\\%s", szProPath , RunBATName);
	
	CopyFile(szProPath,szBATPath,true);
	Sleep(1000);

	// 4.start Run bat file, check the WindowsName exits
	HWND hwndfind;	
	int TimeOut = 50;

	hwndfind = ::FindWindow(NULL,WindowsName);
	if(::IsWindow(hwndfind))
	{
		::PostMessage(hwndfind,WM_CHAR,VK_RETURN,0);
		DisplayRunTimeInfo("WiFi Testing Start...\r\n");
	}
	else
	{
		char szRun[MINBUFSIZE] =""; 
		sprintf_s(szRun, MINBUFSIZE, "cmd.exe /C %s", RunBATName);
		ShellExecute(NULL, "open", "c:\\WINDOWS\\system32\\cmd.exe ", szRun, " ",SW_HIDE);
		Sleep(500);
		hwndfind = ::FindWindow(NULL,WindowsName);
		if(!::IsWindow(hwndfind))
		{
			DisplayRunTimeInfo("Failed to run ");
			DisplayRunTimeInfo(RunBATName);
			DisplayRunTimeInfo(" ...\r\n");
			return 99;
		}
	}

	//5.wait test finish 
	
	TimeOut = 150000;
	while(TimeOut--)
	{
		//check whether have test window
		/*if(!::IsWindow(hwndfind))
		{
			DisplayRunTimeInfo("test finised\r\n"); 
			break;
		}*/
		//when test finish flag file exist ,but windows don't close , close it
		if(PathFileExists(szIQTestFinishFlagFile))
		{
			/*for(int iTimeout=0;iTimeout<5;iTimeout++)
			{
				if(!::IsWindow(hwndfind))
				{
					break;
				}
				DisplayRunTimeInfo("test windows not close\r\n"); 
				Sleep(1000);
			}
			::PostMessage(hwndfind,WM_CLOSE,0,0);*/
			break;
		}
		else
			Sleep(100);
	}

	DisplayRunTimeInfo("WiFi Testing finished!\r\n");

	//check test finish falg file
	char buf1[128];
	memset(buf1 ,0 , 128);
	sprintf_s(buf1,sizeof(buf1) ,"%d",TimeOut);
	DisplayTestInfo("iTimeout = ",buf1);

	if(!PathFileExists(szIQTestFinishFlagFile))
	{
		DisplayRunTimeInfo("Test finish flag file not exist\r\n");
		return 99;	
	}
	while(_access(szIQTestFinishFlagFile,04) !=0)
	{
		Sleep(100);
		continue;
	}

	Sleep(3500);
	//6.analyze test result

	//int iBeginShow=0;	
	FILE *ff;
	char buf[102400] = {0};
	fopen_s(&ff,szIQLogFile,"r");
	while(!feof(ff))
	{		
		memset(buf,0,102400);
		fgets(buf,102400,ff);
		if( strstr(buf,"Test Function Time") != NULL )  
		{ 
			DisplayRunTimeInfo("Find \"Test Function Time\" in \"szIQLogFile\" OK!\r\n");
			break;	
		}
		else
		{			
			continue;
		}
	}
	fclose(ff);	


	try
	{
		fopen_s(&ff,szIQLogFile,"r");	
		while(!feof(ff))
		{
			memset(buf,0,102400);
			fgets(buf,102400,ff);			
			//DisplayRunTimeInfo( buf );
			if(strstr(buf,"P A S S"))  
			{ 
				iTestResult = 1;
			}
			else if(strstr(buf,"*  F A I L  *")) 
			{ 
				bFindFail = true;
			}
			else
			{
				//do nothing
			}
			if(bFindFail)
			{


				if(strstr(buf,"-Failed")!=NULL)
				{
					//add 110718
					DisplayRunTimeInfo(buf);
					DisplayRunTimeInfo("\r\n");
					//end
					if(strstr(buf,"24")!=NULL)
					{

						if( strstr(buf,"CONNECT_IQ_TESTER") )        {iTestResult=5;}
						else if( strstr(buf,"INSERT_DUT") )          {iTestResult=6;}
						else if( strstr(buf,"INITIALIZE_DUT") )      {iTestResult=7;}

						else if( strstr(buf,"EVM") && strstr(buf,"MCS"))       {iTestResult=8;}//EV04
						else if( strstr(buf,"EVM") && strstr(buf,"OFDM"))       {iTestResult=9;}//EV02
						else if( strstr(buf,"EVM") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=10;}//EV01

						else if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=11;}//PR04
						else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=12;}//PR02
						else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=13;}//PR01

						else if( strstr(buf,"MASK") && strstr(buf,"MCS"))       {iTestResult=21;}//SM04
						else if( strstr(buf,"MASK") && strstr(buf,"OFDM"))       {iTestResult=21;}//SM02
						else if( strstr(buf,"MASK") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=23;}//SM01

						else if( strstr(buf,"DISCONNECT_IQ_TESTER")) {iTestResult=14;}
						else if( strstr(buf,"REMOVE_DUT") )          {iTestResult=15;}
						else                                         {iTestResult=16;}
					}
					else
					{
						if( strstr(buf,"CONNECT_IQ_TESTER") )        {iTestResult=5;}
						else if( strstr(buf,"INSERT_DUT") )          {iTestResult=6;}
						else if( strstr(buf,"INITIALIZE_DUT") )      {iTestResult=7;}

						else if( strstr(buf,"EVM") && strstr(buf,"MCS"))       {iTestResult=41;}//EV14
						else if( strstr(buf,"EVM") && strstr(buf,"OFDM"))       {iTestResult=42;}//EV12
						else if( strstr(buf,"EVM") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=43;}//EV11

						else if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=44;}//PR14
						else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=45;}//PR12
						else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=46;}//PR11

						else if( strstr(buf,"MASK") && strstr(buf,"MCS"))       {iTestResult=51;}//SM14
						else if( strstr(buf,"MASK") && strstr(buf,"OFDM"))       {iTestResult=52;}//SM12
						else if( strstr(buf,"MASK") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=53;}//SM11

						else if( strstr(buf,"DISCONNECT_IQ_TESTER")) {iTestResult=14;}
						else if( strstr(buf,"REMOVE_DUT") )          {iTestResult=15;}
						else                                         {iTestResult=16;}
					}

				}
			}
			if(strstr(buf,"Function Time"))
			{
				char szBuf[1024];
				sprintf_s(szBuf , 1024 , "test Function Time:%s\r\n" , buf);
				DisplayRunTimeInfo(szBuf);
				break;
			}

		}
		fclose(ff);	
	}
	catch (CFileException *msg)
	{
		if( msg->m_cause == CFileException::fileNotFound )
		{
			DisplayRunTimeInfo("CFileException::fileNotFound\r\n");
		}
		else if( msg->m_cause == CFileException::accessDenied )
		{
			DisplayRunTimeInfo("CFileException::accessDenied\r\n"); 
		}
		else if( msg->m_cause == CFileException::badPath )
		{
			DisplayRunTimeInfo("CFileException::badPath\r\n"); 
		}		
		else if( msg->m_cause == CFileException::tooManyOpenFiles )
		{
			DisplayRunTimeInfo("CFileException::tooManyOpenFiles\r\n"); 
		}		
		else if( msg->m_cause == CFileException::endOfFile )
		{
			DisplayRunTimeInfo("CFileException::endOfFile\r\n"); 
		}	
		else
		{

		}
	}	

	memset(szbuf ,0 , 128);
	sprintf_s(szbuf,sizeof(szbuf) ,"%d",iTestResult);
	DisplayTestInfo("iTestResult = ",szbuf);
	memset(szbuf ,0 , 128);

	//For record test log file
	char szRecordFilePath[MAXBUFSIZE] = "";
	CTime    Time = CTime::GetCurrentTime();
	CString  strFileName = pRunInfo->m_strSN.Trim()+Time.Format("_%Y-%m-%d-%H-%M-%S");
	sprintf_s(szRecordFilePath, MAXBUFSIZE, "%s\\%s_%s.txt", szIQLogPath, strFileName , WindowsName);
	rename(szIQLogFile,szRecordFilePath);


	/*Add Mydas*/
	//20110809 change 2G/5G used different config.txt marked test items

	DisplayTestInfo("INI.MY.szPostMydas=",INI.MY.szPostMydas);
	DisplayTestInfo("INI.MY.szMydasDebug=",INI.MY.szMydasDebug);	
	
	if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
	{
		DisplayTestInfo("szRecordFilePath=",szRecordFilePath);
		if(strstr(szRecordFilePath , "5G") != NULL)
		{
			char szResult[4012]="";
			DisplayTestInfo("Begin Get 5G Data","");
			int iRet = _dxGetData5g(szResult,szRecordFilePath);
			sprintf_s(szbuf,sizeof(szbuf) ,"%d",iRet);
			DisplayTestInfo("iTestResult = ",szbuf);
			DisplayTestInfo("5G szResult:",szResult);
			switch (iRet)
			{
			case ERR_OK:
				printf("%s\n",szResult);
				strcat_s(g_MyDAS,BUF_LENGH,szResult);
				break;
			case ERR_NO_CONFIG_FILE:
				printf("No 5gconfig.txt under exe path!\n");
				break;
			case ERR_NO_TEST_ITEM:
				printf("No test items in 5gconfig.txt!\n");
				break;
			default:
				break;
			}
			DisplayTestInfo("Detail test data 5G Bat test collected=" , g_MyDAS);		
		}
		else if(strstr(szRecordFilePath , "2G") != NULL)
		{
			char szResult[4012]="";
			DisplayTestInfo("Begin Get 2G Data","");
			int iRet = _dxGetData2g(szResult,szRecordFilePath);
			sprintf_s(szbuf,sizeof(szbuf) ,"%d",iRet);
			DisplayTestInfo("iTestResult = ",szbuf);
			DisplayTestInfo("2G szResult:",szResult);
			switch (iRet)
			{
			case ERR_OK:
				printf("%s\n",szResult);
				strcat_s(g_MyDAS,BUF_LENGH,szResult);
				break;
			case ERR_NO_CONFIG_FILE:
				printf("No 2gconfig.txt under exe path!\n");
				break;
			case ERR_NO_TEST_ITEM:
				printf("No test items in 2gconfig.txt!\n");
				break;
			default:
				break;
			}
			DisplayTestInfo("Detail test data 2G Bat test collected=" , g_MyDAS);		
		}
		else
		{
			DisplayTestInfo("Error information=" , "Test log file name(in .bat is test window name(title) ) should include \"2G\" or \"5G\"\r\n,please check!");		
		}
	}		
	/*End*/


	

	/*when IQ INITIALIZE_DUT fail，open 5G test program again（total 3 times）*/
	//if(iTestResult == 7)/* iTestResult = 7:INITIALIZE_DUT IQ fail*/
	//{
	//	int i = 0;

	//	for(;i < 3;i++)
	//	{
	//		goto __LOOP;
	//	}
	//	if(i >= 3)
	//	{
	//		return iTestResult;
	//	}
	//}
	/*end*/

	return iTestResult;

}


int CRunInfo::Get5gTestErorCode(char* szLine)
{
   /*
--------------------------------------------------------------------
                         *************
                         *  F A I L  *
                         *************								
1.GLOBAL_SETTINGS                               :0.000 s
2.LOAD_PATH_LOSS_TABLE                          :0.000 s
3.CONNECT_IQ_TESTER                             :0.000 s           <-Failed
4.INSERT_DUT                                    :0.000 s           <-Skipped
5.INITIALIZE_DUT                                :0.000 s           <-Skipped
6.TX_VERIFY_EVM 5280 MCS7 HT20                  :0.000 s           <-Skipped
7.TX_VERIFY_EVM 5280 MCS15 HT20                 :0.000 s           <-Skipped
8.TX_VERIFY_EVM 5310 MCS7 HT40                  :0.000 s           <-Skipped
9.TX_VERIFY_EVM 5310 MCS15 HT40                 :0.000 s           <-Skipped
10.TX_VERIFY_EVM 5500 MCS7 HT20                 :0.000 s           <-Skipped
11.TX_VERIFY_EVM 5500 MCS15 HT20                :0.000 s           <-Skipped
12.TX_VERIFY_EVM 5755 MCS7 HT40                 :0.000 s           <-Skipped
13.TX_VERIFY_EVM 5755 MCS15 HT40                :0.000 s           <-Skipped
14.DISCONNECT_IQ_TESTER                         :0.000 s           <-Skipped
15.REMOVE_DUT                                   :0.000 s           <-Skipped
*/
	return 0;
}

void CRunInfo::OnBnClickedBtnStart()
{
	try
	{	
	/*Debug for Mydas*/
	/*AfxMessageBox("5 G Pass data");
	
	char szResult1[1024]="";
	int iRet = _dxGetData5g(szResult1,"F:\\temp\\ZFH\\187 110906 log\\PASS\\SH862RO_2011-09-06-11-15-47_U12H187_5G.txt");
	switch (iRet)
	{
	case ERR_OK:
		printf("%s\n",szResult1);
		
		strcat_s(g_MyDAS,BUF_LENGH,szResult1);
		AfxMessageBox(szResult1);
		AfxMessageBox(g_MyDAS);
		break;
	case ERR_NO_CONFIG_FILE:
		printf("No config.txt under exe path!\n");
		break;
	case ERR_NO_TEST_ITEM:
		printf("No test items in config.txt!\n");
		break;
	default:
		break;
	}*/
	
	/*AfxMessageBox("5 G Fail data");
	
	char szResult2[1024]="";
	iRet = _dxGetData5g(szResult2,"F:\\temp\\ZFH\\H189 PT MYDAS LOG\\SH42V4R_FAIL_U12H189_5G.txt");
	switch (iRet)
	{
	case ERR_OK:
		printf("%s\n",szResult2);
		
		strcat_s(g_MyDAS,BUF_LENGH,szResult2);
		AfxMessageBox(szResult2);
		AfxMessageBox(g_MyDAS);
		break;
	case ERR_NO_CONFIG_FILE:
		printf("No config.txt under exe path!\n");
		break;
	case ERR_NO_TEST_ITEM:
		printf("No test items in config.txt!\n");
		break;
	default:
		break;
	}
	
	AfxMessageBox("2G Pass data");
	
	char szResult3[1024]="";
	iRet = _dxGetData2g(szResult3,"F:\\temp\\ZFH\\H189 PT MYDAS LOG\\SH42UOC_PASS-20_U12H189_2G.txt");
	switch (iRet)
	{
	case ERR_OK:
		printf("%s\n",szResult3);		
		strcat_s(g_MyDAS,BUF_LENGH,szResult3);
		AfxMessageBox(szResult3);
		AfxMessageBox(g_MyDAS);
		break;
	case ERR_NO_CONFIG_FILE:
		printf("No config.txt under exe path!\n");
		break;
	case ERR_NO_TEST_ITEM:
		printf("No test items in config.txt!\n");
		break;
	default:
		break;
	}
	
	AfxMessageBox("2G Fail data");	
	char szResult4[1024]="";
	iRet = _dxGetData2g(szResult4,"F:\\temp\\ZFH\\H189 PT MYDAS LOG\\SH42VWR_FAIL-57_U12H189_2G.txt");
	switch (iRet)
	{
	case ERR_OK:
		printf("%s\n",szResult4);
		
		strcat_s(g_MyDAS,BUF_LENGH,szResult4);
		AfxMessageBox(szResult4);
		AfxMessageBox(g_MyDAS);
		break;
	case ERR_NO_CONFIG_FILE:
		printf("No config.txt under exe path!\n");
		break;
	case ERR_NO_TEST_ITEM:
		printf("No test items in config.txt!\n");
		break;
	default:
		break;
	}

	AfxMessageBox("g_MyDAS");
	AfxMessageBox(g_MyDAS);
	TestFail(SY05);
	return;*/
	/*End*/

	
		/*Debug for read log file*/
	//FILE *ff;
	//int iTestResult=0;
	//bool bFindFail=false;
	//char buf[102400] = {0};
	//
	//fopen_s(&ff,"F:\\temp\\ZFH\H187\\H187 PT log\\SH06XBK_2011-07-14-14-39-37_U12H187_5G.txt","r");	
	//while(!feof(ff))
	//{
	//	memset(buf,0,102400);
	//	fgets(buf,102400,ff);			
	//	//DisplayRunTimeInfo( buf );
	//	if(strstr(buf,"P A S S"))  
	//	{ 
	//		iTestResult = 1;
	//	}
	//	else if(strstr(buf,"*  F A I L  *")) 
	//	{ 
	//		bFindFail = true;
	//	}
	//	else
	//	{
	//		//do nothing
	//	}
	//	if(bFindFail)
	//	{
	//		if(strstr(buf,"-Failed")!=NULL)
	//		{
	//			if( strstr(buf,"CONNECT_IQ_TESTER") )        {iTestResult=5;}
	//			else if( strstr(buf,"INSERT_DUT") )          {iTestResult=6;}
	//			else if( strstr(buf,"INITIALIZE_DUT") )      {iTestResult=7;}

	//			else if( strstr(buf,"EVM") && strstr(buf,"MCS"))       {iTestResult=8;}//EV04
	//			else if( strstr(buf,"EVM") && strstr(buf,"OFDM"))       {iTestResult=9;}//EV02
	//			else if( strstr(buf,"EVM") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=10;}//EV01

	//			else if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=11;}//PR14
	//			else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=12;}//PR12
	//			else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=13;}//PR11

	//			else if( strstr(buf,"MASK") && strstr(buf,"MCS"))       {iTestResult=21;}//SM14
	//			else if( strstr(buf,"MASK") && strstr(buf,"OFDM"))       {iTestResult=21;}//SM12
	//			else if( strstr(buf,"MASK") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=23;}//SM11

	//			else if( strstr(buf,"DISCONNECT_IQ_TESTER")) {iTestResult=14;}
	//			else if( strstr(buf,"REMOVE_DUT") )          {iTestResult=15;}
	//			else                                         {iTestResult=16;}
	//		}
	//	}
	//	if(strstr(buf,"Function Time"))
	//	{
	//		char szBuf[1024];
	//		sprintf_s(szBuf , 1024 , "test Function Time:%s\r\n" , buf);
	//		DisplayRunTimeInfo(szBuf);
	//		break;
	//	}
	//	
	//}
	//fclose(ff);
	/*End*/

	//Debug get Power lever
	//int i = GetItemPower("[Step 010] RX_VERIFY_PER 2472 OFDM-54 HT20 #-89 &1000 @1    	<-- Failed");
	//int j = GetItemPower("TX_VERIFY_EVM 5190 MCS7 HT40 #12 $-5 @1");
	//end 
		
		if(NULL == m_hEvt)	
		{
			m_hEvt = CreateEvent(NULL,true,false,NULL);
		}

		SetFlagForIsSameDut(true);
		g_Recorder.clear();
		UpdateData();
		if(m_strSN.IsEmpty())
			return;
		static BOOL bInit = TRUE;
		DisplayRunTimeInfo("INI.TF.szTestWiFiCommonFlag=");
		DisplayRunTimeInfo(INI.TF.szTestWiFiCommonFlag);
		DisplayRunTimeInfo("\r\n");
		//20110714 add
		if(strstr(INI.TF.szTestWiFiCommonFlag , "1"))
		{
			if(bInit)
			{
				bInit=FALSE;
				if(!ambit_InitTestFlow("Ambit_test_flow.ini"))
				{
					SendSfisResultAndShowFail(SY45);
					return;
				}
			}
		}
		//add end
		DisplayRunTimeInfo(NULL);
		IsDisplayErrorCode(false);
		m_TestTimeCount = 0;
		m_strTestResult = "RUN";
		m_strTestTime = "00:00";
		SetTimer(1,1000,NULL);
		UpdateData(false);
		CTime      Time = CTime::GetCurrentTime();
		CString    strFileName = Time.Format("%Y%m%d");
		memset(RecordDir_LOG ,0,BUF_LENGH);
		memset(RecordDir_LOG_PRODUCT,0,BUF_LENGH);
		memset(RecordDir_LOG_PRODUCT_TIME,0,BUF_LENGH);
		memset(RecordDir_LOG_PRODUCT_PASS,0,BUF_LENGH);
		memset(RecordDir_LOG_PRODUCT_FAIL,0,BUF_LENGH);

		DisplayTestInfo("INI.FP.szStoreSrvPath=" , INI.FP.szStoreSrvPath);

		sprintf_s(RecordDir_LOG ,BUF_LENGH, INI.FP.szStoreSrvPath);
		
		sprintf_s(RecordDir_LOG_PRODUCT,BUF_LENGH,"%s\\%s" ,RecordDir_LOG,m_strProductName.Trim());
		sprintf_s(RecordDir_LOG_PRODUCT_TIME,BUF_LENGH,"%s\\%s" ,RecordDir_LOG_PRODUCT,strFileName.Trim());
		sprintf_s(RecordDir_LOG_PRODUCT_PASS,BUF_LENGH,"%s\\PASS" ,RecordDir_LOG_PRODUCT_TIME );
		sprintf_s(RecordDir_LOG_PRODUCT_FAIL,BUF_LENGH,"%s\\FAIL" ,RecordDir_LOG_PRODUCT_TIME );

		if(OnSFIS)
		{
			memset(ServerDir_LOG_PRODUCT,0,BUF_LENGH);
			memset(ServerDir_LOG_PRODUCT_TIME,0,BUF_LENGH);
			memset(ServerDir_LOG_PRODUCT_PASS,0,BUF_LENGH);
			memset(ServerDir_LOG_PRODUCT_FAIL,0,BUF_LENGH);

			sprintf_s(ServerDir_LOG_PRODUCT,MAX_PATH,"%s\\%s",INI.FP.Log,m_strProductName.Trim());
			sprintf_s(ServerDir_LOG_PRODUCT_TIME,MAX_PATH,"%s\\%s",ServerDir_LOG_PRODUCT,strFileName.Trim());
			sprintf_s(ServerDir_LOG_PRODUCT_PASS , BUF_LENGH,"%s\\PASS",ServerDir_LOG_PRODUCT_TIME );
			sprintf_s(ServerDir_LOG_PRODUCT_FAIL , BUF_LENGH,"%s\\FAIL",ServerDir_LOG_PRODUCT_TIME );

			DisplayRunTimeInfo("INI.FP.Log=");
			DisplayRunTimeInfo(INI.FP.Log);
			DisplayRunTimeInfo("\r\n");
			if(!PathIsDirectory(INI.FP.Log))
			{
				CreateDirectory(INI.FP.Log,NULL);
			}
			if(!PathIsDirectory(ServerDir_LOG_PRODUCT))
			{	
				CreateDirectory(ServerDir_LOG_PRODUCT,NULL);
			}
			if(!PathIsDirectory(ServerDir_LOG_PRODUCT_TIME))
			{	
				CreateDirectory(ServerDir_LOG_PRODUCT_TIME,NULL);
			}
			if(!PathIsDirectory(ServerDir_LOG_PRODUCT_PASS))
			{
				CreateDirectory(ServerDir_LOG_PRODUCT_PASS,NULL);
			}
			if(!PathIsDirectory(ServerDir_LOG_PRODUCT_FAIL))
			{
				CreateDirectory(ServerDir_LOG_PRODUCT_FAIL,NULL);
			}
		}
		
		if(!PathIsDirectory(RecordDir_LOG))
		{
			CreateDirectory(RecordDir_LOG,NULL);
		}
		if(!PathIsDirectory(RecordDir_LOG_PRODUCT))
		{
			CreateDirectory(RecordDir_LOG_PRODUCT,NULL);
		}
		if(!PathIsDirectory(RecordDir_LOG_PRODUCT_TIME))
		{
			CreateDirectory(RecordDir_LOG_PRODUCT_TIME,NULL);
		}
		if(!PathIsDirectory(RecordDir_LOG_PRODUCT_PASS))
		{
			CreateDirectory(RecordDir_LOG_PRODUCT_PASS,NULL);
		}
		if(!PathIsDirectory(RecordDir_LOG_PRODUCT_FAIL))
		{
			CreateDirectory(RecordDir_LOG_PRODUCT_FAIL,NULL);
		}
		if(!m_blIsSfis)
		{
			((CButton*)GetDlgItem(IDC_BTN_START))->EnableWindow(FALSE);
			((CButton*)GetDlgItem(IDC_BTN_QUIT))->EnableWindow(FALSE);
		}
		
	
		CreateDirectory("D:\\Logs",NULL);
	
		CreateDirectory("D:\\Logs\\PASS",NULL);	

		CreateDirectory("D:\\Logs\\FAIL",NULL);	



		DeleteSpecifyExe("ping");
		
		AfxBeginThread(FunctionTestRun, this);
	}
	catch(...)
	{
		AfxMessageBox("OnbnClickBtnStart fail" );
	}
}




UINT __cdecl  CRunInfo::FunctionTestRun(LPVOID parameter)
{
	try
	{


		CRunInfo *pt = (CRunInfo *)parameter;
		CString szBuf11;
		char szBuf12[256];
		sprintf_s(szBuf12,"arp -d %s",pt->INI.GEN.szDefaultIP);
		pt->RunSpecifyExeAndRead(szBuf11,szBuf12,true);

		if(!pt->PingSpecifyIP(pt->INI.GEN.szDefaultIP  , 2))
		{
			pt->SendSfisResultAndShowFail(IN12);
			return 0;
		}

		
		pt->DisplayTestInfo("INI.DLI.szDownLoadImageFlag=",pt->INI.DLI.szDownLoadImageFlag);
		if(strstr(pt->INI.DLI.szDownLoadImageFlag , "1") != NULL)
		{	
			//Add download test image
			int iRtyTime = 3 ;
			while(iRtyTime--)
			{
				if(pt->Funtion_Downlaod_image())
				{
					pt->DisplayRunTimeInfo("Download test image OK!\r\n");
					break;
				}
				else
				{	
					pt->DisplayRunTimeInfo("Download test image failed!\r\n");
					continue;
				}
			}


			if( iRtyTime<0 )
			{
				pt->SendSfisResultAndShowFail(DL00);
				return 0;
			}			

			CString szBuf1;
			char szBuf2[256];
			sprintf_s(szBuf2,"arp -d %s",pt->INI.GEN.szDefaultIP);
			pt->RunSpecifyExeAndRead(szBuf1,szBuf2,true);

			if(!pt->PingSpecifyIP(pt->INI.GEN.szDefaultIP  , 2))
			{
				pt->SendSfisResultAndShowFail(IN11);
				return 0;
			}
		}

		GetPrivateProfileString("TestFlow","IQDemoRunFlag","1" , pt->INI.TF.szIQDemoRunFlag , MAX_PATH,".\\init.ini");
		
		if(strstr(pt->INI.TF.szIQDemoRunFlag , "1" ) != NULL)
		{
			//1.check IQ_Demo is exist or not
			HWND hwndfind;
			hwndfind = ::FindWindow(NULL,"IQlite_Demo.exe");
			//IQ_Demo rerun not exist,need rerun again 
			if(!(::IsWindow(hwndfind)))
			{
				pt->DisplayRunTimeInfo("IQlite_Demo.exe not exist,run again...\r\n");
				ShellExecute(NULL, "open", ".\\IQlite_Demo.exe ", "", " ",SW_MINIMIZE);
				Sleep(1000);
				hwndfind = ::FindWindow(NULL,"IQlite_Demo.exe");
				////IQ_Demo rerun failed
				if(!(::IsWindow(hwndfind)))
				{
					pt->DisplayRunTimeInfo("IQlite_Demo.exe run again failed,please check is there have IQlite_Demo.exe...\r\n");
					AfxMessageBox("IQlite_Demo.exe run failed,please check is there have IQlite_Demo.exe\r\n請檢查程序目錄下有IQlite_Demo.exe文件存在");
					pt->SendSfisResultAndShowFail(RR01);
					return 0;
				}
				//IQ_Demo rerun ok 
				else
				{
					pt->DisplayRunTimeInfo("IQlite_Demo.exe run again OK...\r\n");
				}

			}
			//IQ_Demo have exist
			else
			{
				pt->DisplayRunTimeInfo("IQlite_Demo.exe have running...\r\n");
			}
		
			pt->DisplayRunTimeInfo("WiFi Testing Start...\r\n");
			//2.run IQ_Demo.exe test RF wifi
			int iRes = pt->Run_WiFiTest_IQDemo();
			pt->DisplayRunTimeInfo("Run WiFiTest end!\r\n");
			switch(iRes)
			{
			case 0:
				pt->SendSfisResultAndShowFail(RF01);
				return 0;
			case 1:
				break;

			case 8:
				pt->SendSfisResultAndShowFail(EV04);
				return 0;
			case 9:
				pt->SendSfisResultAndShowFail(EV02);
				return 0;
			case 10:
				pt->SendSfisResultAndShowFail(EV01);
				return 0;
			case 11:
				pt->SendSfisResultAndShowFail(PR04);
				return 0;
			case 12:
				pt->SendSfisResultAndShowFail(PR02);
				return 0;
			case 13:
				pt->SendSfisResultAndShowFail(PR01);
				return 0;
			case 14:
				pt->SendSfisResultAndShowFail(PR08);
				return 0;
			case 15:
				pt->SendSfisResultAndShowFail(PR06);
				return 0;
			case 16:
				pt->SendSfisResultAndShowFail(PR05);
				return 0;
			case 21:
				pt->SendSfisResultAndShowFail(SM04);
				return 0;
			case 22:
				pt->SendSfisResultAndShowFail(SM02);
				return 0;
			case 23:
				pt->SendSfisResultAndShowFail(SM01);
				return 0;
			case 31:
				pt->SendSfisResultAndShowFail(PW04);
				return 0;
			case 32:
				pt->SendSfisResultAndShowFail(PW02);
				return 0;
			case 33:
				pt->SendSfisResultAndShowFail(PW01);
				return 0;
			case 41:
				pt->SendSfisResultAndShowFail(EV14);
				return 0;
			case 42:
				pt->SendSfisResultAndShowFail(EV12);
				return 0;
			case 43:
				pt->SendSfisResultAndShowFail(EV11);
				return 0;
			case 44:
				pt->SendSfisResultAndShowFail(PR14);
				return 0;
			case 45:
				pt->SendSfisResultAndShowFail(PR12);
				return 0;
			case 46:
				pt->SendSfisResultAndShowFail(PR11);
				return 0;
			case 47:
				pt->SendSfisResultAndShowFail(PR18);
				return 0;
			case 48:
				pt->SendSfisResultAndShowFail(PR16);
				return 0;
			case 49:
				pt->SendSfisResultAndShowFail(PR15);
				return 0;
			case 51:
				pt->SendSfisResultAndShowFail(SM14);
				return 0;
			case 52:
				pt->SendSfisResultAndShowFail(SM12);
				return 0;
			case 53:
				pt->SendSfisResultAndShowFail(SM11);
				return 0;
			case 61:
				pt->SendSfisResultAndShowFail(PW14);
				return 0;
			case 62:
				pt->SendSfisResultAndShowFail(PW12);
				return 0;
			case 63:
				pt->SendSfisResultAndShowFail(PW11);
				return 0;

				//need change error code
			case 90:
				pt->SendSfisResultAndShowFail(RR00);
				return 0;
			case 91:
				pt->SendSfisResultAndShowFail(RG00);
				return 0;
			case 92:
				pt->SendSfisResultAndShowFail(RL00);
				return 0;
			case 93:
				pt->SendSfisResultAndShowFail(IN40);
				return 0;
			case 94:
				pt->SendSfisResultAndShowFail(RI00);
				return 0;
			case 95:
				pt->SendSfisResultAndShowFail(IN20);
				return 0;

			default:
				break;
			}		
		}


		static BOOL FirstConnect = TRUE;
		static BOOL FirstRead = TRUE;	

		char szReturn[BUF_LENGH]="";
		char DisplayTime[BUF_LENGH]="";	

		/*Add for read .ini to test 5G or not*/
		pt->DisplayRunTimeInfo("INI.GEN.szTestByBat=");
		pt->DisplayRunTimeInfo(pt->INI.GEN.szTestByBat);
		pt->DisplayRunTimeInfo("\r\n");
		if(strstr(pt->INI.GEN.szTestByBat , "1"))
		{
			pt->DisplayRunTimeInfo("INI.TF.szTest5G=");
			pt->DisplayRunTimeInfo(pt->INI.TF.szTest5G);
			pt->DisplayRunTimeInfo("\r\n");
			if(strstr(pt->INI.TF.szTest5G , "1"))
			{
				pt->DisplayRunTimeInfo("BEGINING 5G WIFI test!\r\n");

				pt->DisplayRunTimeInfo("INI.T5G.RunBATName_5G=");
				pt->DisplayRunTimeInfo(pt->INI.T5G.RunBATName_5G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T5G.IQLogFile_5G=");
				pt->DisplayRunTimeInfo(pt->INI.T5G.IQLogFile_5G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T5G.IQLogPath_5G=");
				pt->DisplayRunTimeInfo(pt->INI.T5G.IQLogPath_5G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T5G.IQTestFinishFlagFile_5G=");
				pt->DisplayRunTimeInfo(pt->INI.T5G.IQTestFinishFlagFile_5G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T5G.BATFilePath_5G=");
				pt->DisplayRunTimeInfo(pt->INI.T5G.BATFilePath_5G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T5G.WindowsName_5G=");
				pt->DisplayRunTimeInfo(pt->INI.T5G.WindowsName_5G);
				pt->DisplayRunTimeInfo("\r\n");
		
				int iRes = pt->Run_WiFiTest( pt->INI.T5G.RunBATName_5G, 
					pt->INI.T5G.IQLogFile_5G,
					pt->INI.T5G.IQLogPath_5G,
					pt->INI.T5G.IQTestFinishFlagFile_5G,
					pt->INI.T5G.BATFilePath_5G,
					//pt->INI.T5G.ServeBATPath_5G,
					pt->INI.T5G.WindowsName_5G
					);
				pt->DisplayRunTimeInfo("5G WIFI test end!\r\n");
				switch(iRes)
				{
				case 0:
					pt->SendSfisResultAndShowFail(RF01);
					return 0;
				case 1:
					break;
				case 2:
				case 3:
				case 4:
					pt->SendSfisResultAndShowFail(IN10);
					return 0;
					// break;
				case 5:
					pt->SendSfisResultAndShowFail(IN40); 
					return 0;
					//break;
				case 6:
					pt->SendSfisResultAndShowFail(IN20);
					return 0;
					// break;
				case 7:
					pt->SendSfisResultAndShowFail(IN10);
					return 0;
					// break;
				case 8:
					pt->SendSfisResultAndShowFail(EV04);
					return 0;
				case 9:
					pt->SendSfisResultAndShowFail(EV02);
					return 0;
				case 10:
					pt->SendSfisResultAndShowFail(EV01);
					return 0;
				case 11:
					pt->SendSfisResultAndShowFail(PR04);
					return 0;
				case 12:
					pt->SendSfisResultAndShowFail(PR02);
					return 0;
				case 13:
					pt->SendSfisResultAndShowFail(PR01);
					return 0;
				case 14:
				case 15:
					pt->SendSfisResultAndShowFail(IN40);
					return 0;
				case 16:
					pt->SendSfisResultAndShowFail(UN00);
					return 0;
				case 21:
					pt->SendSfisResultAndShowFail(SM04);
					return 0;
				case 22:
					pt->SendSfisResultAndShowFail(SM02);
					return 0;
				case 23:
					pt->SendSfisResultAndShowFail(SM01);
					return 0;
				case 41:
					pt->SendSfisResultAndShowFail(EV14);
					return 0;
				case 42:
					pt->SendSfisResultAndShowFail(EV12);
					return 0;
				case 43:
					pt->SendSfisResultAndShowFail(EV11);
					return 0;
				case 44:
					pt->SendSfisResultAndShowFail(PR14);
					return 0;
				case 45:
					pt->SendSfisResultAndShowFail(PR12);
					return 0;
				case 46:
					pt->SendSfisResultAndShowFail(PR11);
					return 0;
				case 51:
					pt->SendSfisResultAndShowFail(SM14);
					return 0;
				case 52:
					pt->SendSfisResultAndShowFail(SM12);
					return 0;
				case 53:
					pt->SendSfisResultAndShowFail(SM11);
					return 0;
				case 98:
					pt->SendSfisResultAndShowFail(SY10);
					return 0;
				case 99:
					pt->SendSfisResultAndShowFail(SY10);
					return 0;
				default:
					break;
				}
			}
			pt->DisplayRunTimeInfo("INI.TF.szTest2G=");
			pt->DisplayRunTimeInfo(pt->INI.TF.szTest2G);
			pt->DisplayRunTimeInfo("\r\n");
			if(strstr(pt->INI.TF.szTest2G , "1")) 
			{
				pt->DisplayRunTimeInfo("BEGINING 2G WIFI test!\r\n");

				pt->DisplayRunTimeInfo("INI.T2G.RunBATName_2G=");
				pt->DisplayRunTimeInfo(pt->INI.T2G.RunBATName_2G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T2G.IQLogFile_2G=");
				pt->DisplayRunTimeInfo(pt->INI.T2G.IQLogFile_2G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T2G.IQLogPath_2G=");
				pt->DisplayRunTimeInfo(pt->INI.T2G.IQLogPath_2G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T2G.IQTestFinishFlagFile_2G=");
				pt->DisplayRunTimeInfo(pt->INI.T2G.IQTestFinishFlagFile_2G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T2G.BATFilePath_2G=");
				pt->DisplayRunTimeInfo(pt->INI.T2G.BATFilePath_2G);
				pt->DisplayRunTimeInfo("\r\n");
				pt->DisplayRunTimeInfo("INI.T2G.WindowsName_2G=");
				pt->DisplayRunTimeInfo(pt->INI.T2G.WindowsName_2G);
				pt->DisplayRunTimeInfo("\r\n");

				int iRes = pt->Run_WiFiTest( pt->INI.T2G.RunBATName_2G, 
					pt->INI.T2G.IQLogFile_2G,
					pt->INI.T2G.IQLogPath_2G,
					pt->INI.T2G.IQTestFinishFlagFile_2G,
					pt->INI.T2G.BATFilePath_2G,
					//pt->INI.T2G.ServeBATPath_2G,
					pt->INI.T2G.WindowsName_2G
					);
				pt->DisplayRunTimeInfo("2G WIFI test end!\r\n");
				switch(iRes)
				{
				case 0:
					pt->SendSfisResultAndShowFail(RF01);
					return 0;
				case 1:
					break;
				case 2:
				case 3:
				case 4:
					pt->SendSfisResultAndShowFail(IN10);
					return 0;
					// break;
				case 5:
					pt->SendSfisResultAndShowFail(IN40); 
					return 0;
					//break;
				case 6:
					pt->SendSfisResultAndShowFail(IN20);
					return 0;
					// break;
				case 7:
					pt->SendSfisResultAndShowFail(IN10);
					return 0;
					// break;
				case 8:
					pt->SendSfisResultAndShowFail(EV04);
					return 0;
				case 9:
					pt->SendSfisResultAndShowFail(EV02);
					return 0;
				case 10:
					pt->SendSfisResultAndShowFail(EV01);
					return 0;
				case 11:
					pt->SendSfisResultAndShowFail(PR04);
					return 0;
				case 12:
					pt->SendSfisResultAndShowFail(PR02);
					return 0;
				case 13:
					pt->SendSfisResultAndShowFail(PR01);
					return 0;
				case 14:
				case 15:
					pt->SendSfisResultAndShowFail(IN40);
					return 0;
				case 16:
					pt->SendSfisResultAndShowFail(UN00);
					return 0;
				case 21:
					pt->SendSfisResultAndShowFail(SM04);
					return 0;
				case 22:
					pt->SendSfisResultAndShowFail(SM02);
					return 0;
				case 23:
					pt->SendSfisResultAndShowFail(SM01);
					return 0;
				case 41:
					pt->SendSfisResultAndShowFail(EV14);
					return 0;
				case 42:
					pt->SendSfisResultAndShowFail(EV12);
					return 0;
				case 43:
					pt->SendSfisResultAndShowFail(EV11);
					return 0;
				case 44:
					pt->SendSfisResultAndShowFail(PR14);
					return 0;
				case 45:
					pt->SendSfisResultAndShowFail(PR12);
					return 0;
				case 46:
					pt->SendSfisResultAndShowFail(PR11);
					return 0;
				case 51:
					pt->SendSfisResultAndShowFail(SM14);
					return 0;
				case 52:
					pt->SendSfisResultAndShowFail(SM12);
					return 0;
				case 53:
					pt->SendSfisResultAndShowFail(SM11);
					return 0;
				case 98:
					pt->SendSfisResultAndShowFail(SY10);
					return 0;
				case 99:
					pt->SendSfisResultAndShowFail(SY10);
					return 0;
				default:
					break;
				}
			}

			pt->DisplayRunTimeInfo("WiFi test by bat finished!!!");

		}
		/*End*/


		pt->SendSfisResultAndShowPass();
		return 1;
	}
	catch(...)
	{
		AfxMessageBox("FunctionTestRun fail" );
		return 0;
	}
}
void CRunInfo::OnBnClickedBtnQuit()
{
	//TODO: Add your control notification handler code here
	if(m_Socket!=INVALID_SOCKET)
		closesocket(m_Socket);
	WSACleanup();
	if(m_blIsSfis)
		m_ComSfis.Close();

	WritePrivateProfileString("TESTER_CONTROL","CTRL_MODE","999",".\\TestStartUp.ini");
	//	m_ComTest.Close();
}

void CRunInfo::OnCbnSelchangeCom()
{
	// TODO: Add your control notification handler code here
	try
	{
		if(!m_blIsOpenSfis && m_blIsSfis)
		{
			UpdateData();
			if(!m_ComSfis.Open(m_strCom.GetBuffer(m_strCom.GetLength()), 9600))
			{
				AfxMessageBox("選擇的COM口不存在或者被其他應用程序占用\n請從新選擇COM與SFIS連接\nThe select COM port don't exit or is used by other application!\nPls choose COM port again");
				return;
			}

			m_ComSfis.SendData(("Hello\r\n"), (int)strlen("Hello\r\n"));

			char pBuf[MINBUFSIZE] = "";
			m_ComSfis.ReadData(pBuf, sizeof(pBuf), 2000);

			if( (strstr(pBuf, "ERRO") == NULL) && (strstr(pBuf, "PASS") == NULL))
			{
				AfxMessageBox(_T("請從新選擇COM與SFIS連接\n"));
				m_ComSfis.Close();
				return ;
			}
			else
			{
				m_strSfisStatus = "SFIS ON";
				UpdateData(false);
				m_blIsOpenSfis = true;
				OnSFIS = true;
				ReadIniFile();
				AfxBeginThread(ReadComForSfis, &m_ComSfis);
				//do nothing
			}


		}
	}
	catch(...)
	{
		AfxMessageBox("OnCbnSelchangeCom fail" );
	}

}




void CRunInfo::OnDestroy()
{
	CDialog::OnDestroy();
}



void Run_Test(const ReturnInfo *pReturnInfo/*,const ReturnValueCollection *pReturnValue*/)
{
	try
	{
		ASSERT(pRunInfo!=NULL);
		if(pReturnInfo->bFinish)
		{
			char szRecordFilePath_Server  [MAXBUFSIZE] = "";
			//char szRecordServerPath [MAXBUFSIZE] = "";
			CTime    Time = CTime::GetCurrentTime();
			CString  strFileName = pRunInfo->m_strSN.Trim()+Time.Format("(%Y%m%d)");
			if(pReturnInfo->Result == PASS)
			{
				/*Recode log*/
				sprintf_s(szRecordFilePath, MAXBUFSIZE, "%s\\%s.txt", RecordDir_LOG_PRODUCT_PASS, strFileName);
				fs.open (szRecordFilePath,fstream::out|fstream::app);
				for(vector<string>::iterator it=g_Recorder.begin();it!=g_Recorder.end();++it)
				{
					fs<<it->c_str();
				}
				fs<<"Total test time:"<<pRunInfo->m_strTestTime<<"\r\n";
				fs<<"Test PC name:"<<pRunInfo->m_strPcName<<"\r\n";
				fs<<endl;
				fs.close();
				/*Recode server log*/
				pRunInfo->DisplayTestInfo("INI.FP.szRecodeServerFlag=",pRunInfo->INI.FP.szRecodeServerFlag);	
				if(OnSFIS && strstr(pRunInfo->INI.FP.szRecodeServerFlag , "1"))
				{
					sprintf_s(szRecordFilePath_Server, MAXBUFSIZE, "%s\\%s.txt", ServerDir_LOG_PRODUCT_PASS, strFileName);
					fs.open (szRecordFilePath_Server,fstream::out|fstream::app);
					for(vector<string>::iterator it=g_Recorder.begin();it!=g_Recorder.end();++it)
					{
						fs<<it->c_str();
					}
					fs<<"Total test time:"<<pRunInfo->m_strTestTime<<"\r\n";
					fs<<"Test PC name:"<<pRunInfo->m_strPcName<<"\r\n";
					fs<<endl;
					fs.close();
				}

				pRunInfo->DisplayTestInfo("INI.MY.szPostMydas=",pRunInfo->INI.MY.szPostMydas);	
				pRunInfo->DisplayTestInfo("INI.MY.szMydasDebug=",pRunInfo->INI.MY.szMydasDebug);
				if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
				{
					/*add 2.4G mydas*/
					char szResult[1024]="";
					int iRet = _dxGetData_f(szResult,szRecordFilePath);
					switch (iRet)
					{
					case ERR_OK:
						printf("%s\n",szResult);

						strcat_s(g_MyDAS,BUF_LENGH,szResult);

						break;
					case ERR_NO_CONFIG_FILE:
						printf("No config.txt under exe path!\n");
						break;
					case ERR_NO_TEST_ITEM:
						printf("No test items in config.txt!\n");
						break;
					default:
						break;
					}
					/*End*/
				}
				pRunInfo->DisplayTestInfo("Detail test data Foxconn funtion collected=" , g_MyDAS);
				//pRunInfo->SendSfisResultAndShowPass();
				return;
			}
			else if(pReturnInfo->Result == FAIL)
			{
				/*Recode log*/
				sprintf_s(szRecordFilePath, MAXBUFSIZE, "%s\\%s.txt", RecordDir_LOG_PRODUCT_FAIL, strFileName);
				fs.open (szRecordFilePath,fstream::out|fstream::app);
				for(vector<string>::iterator it=g_Recorder.begin();it!=g_Recorder.end();++it)
				{
					fs<<it->c_str();
				}
				fs<<"Total test time:"<<pRunInfo->m_strTestTime<<"\r\n";
				fs<<"Test PC name:"<<pRunInfo->m_strPcName<<"\r\n";
				fs<<endl;
				fs.close();
				/*Recode server log*/
				pRunInfo->DisplayRunTimeInfo(pRunInfo->INI.FP.szRecodeServerFlag);
				pRunInfo->DisplayRunTimeInfo("\r\n");
				if(OnSFIS && strstr(pRunInfo->INI.FP.szRecodeServerFlag , "1"))
				{
					sprintf_s(szRecordFilePath_Server, MAXBUFSIZE, "%s\\%s.txt", ServerDir_LOG_PRODUCT_FAIL, strFileName);
					fs.open (szRecordFilePath_Server,fstream::out|fstream::app);
					for(vector<string>::iterator it=g_Recorder.begin();it!=g_Recorder.end();++it)
					{
						fs<<it->c_str();
					}
					fs<<"Total test time:"<<pRunInfo->m_strTestTime<<"\r\n";
					fs<<"Test PC name:"<<pRunInfo->m_strPcName<<"\r\n";
					fs<<endl;
					fs.close();
				}
				pRunInfo->DisplayTestInfo("INI.MY.szPostMydas=",pRunInfo->INI.MY.szPostMydas);	
				pRunInfo->DisplayTestInfo("INI.MY.szMydasDebug=",pRunInfo->INI.MY.szMydasDebug);	
				if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
				{
					/*add 2.4G mydas*/
					char szResult[1024]="";
					int iRet = _dxGetData_f(szResult,szRecordFilePath);
					switch (iRet)
					{
					case ERR_OK:
						printf("%s\n",szResult);
						strcat_s(g_MyDAS,BUF_LENGH,szResult);
						break;
					case ERR_NO_CONFIG_FILE:
						printf("No config.txt under exe path!\n");
						break;
					case ERR_NO_TEST_ITEM:
						printf("No test items in config.txt!\n");
						break;
					default:
						break;
					}
					/*End*/
				}
				pRunInfo->DisplayTestInfo("Detail test data Foxconn funtion collected=" , g_MyDAS);
				pRunInfo->SendSfisResultAndShowFail((char*)pReturnInfo->ErrorDes);
				return;
			}
			else if(pReturnInfo->Result == ERRORR)
			{
				/*Recode log*/
				sprintf_s(szRecordFilePath, MAXBUFSIZE, "%s\\%s.txt", RecordDir_LOG_PRODUCT_FAIL, strFileName);
				fs.open (szRecordFilePath,fstream::out|fstream::app);
				for(vector<string>::iterator it=g_Recorder.begin();it!=g_Recorder.end();++it)
				{
					fs<<it->c_str();
				}
				fs<<"Total test time:"<<pRunInfo->m_strTestTime<<"\r\n";
				fs<<"Test PC name:"<<pRunInfo->m_strPcName<<"\r\n";
				fs<<endl;
				fs.close();
				/*Recode server log*/
				pRunInfo->DisplayRunTimeInfo(pRunInfo->INI.FP.szRecodeServerFlag);
				pRunInfo->DisplayRunTimeInfo("\r\n");
				if(OnSFIS && strstr(pRunInfo->INI.FP.szRecodeServerFlag , "1"))
				{
					sprintf_s(szRecordFilePath_Server, MAXBUFSIZE, "%s\\%s.txt", ServerDir_LOG_PRODUCT_FAIL, strFileName);
					fs.open (szRecordFilePath_Server,fstream::out|fstream::app);
					for(vector<string>::iterator it=g_Recorder.begin();it!=g_Recorder.end();++it)
					{
						fs<<it->c_str();
					}
					fs<<"Total test time:"<<pRunInfo->m_strTestTime<<"\r\n";
					fs<<"Test PC name:"<<pRunInfo->m_strPcName<<"\r\n";
					fs<<endl;
					fs.close();
				}
				pRunInfo->DisplayTestInfo("INI.MY.szPostMydas=",pRunInfo->INI.MY.szPostMydas);	
				pRunInfo->DisplayTestInfo("INI.MY.szMydasDebug=",pRunInfo->INI.MY.szMydasDebug);
				if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
				{
					/*add 2.4G mydas*/
					char szResult[1024]="";
					int iRet = _dxGetData_f(szResult,szRecordFilePath);
					switch (iRet)
					{
					case ERR_OK:
						printf("%s\n",szResult);
						strcat_s(g_MyDAS,BUF_LENGH,szResult);
						break;
					case ERR_NO_CONFIG_FILE:
						printf("No config.txt under exe path!\n");
						break;
					case ERR_NO_TEST_ITEM:
						printf("No test items in config.txt!\n");
						break;
					default:
						break;
					}
					/*End*/
				}
				pRunInfo->DisplayTestInfo("Detail test data Foxconn funtion collected=" , g_MyDAS);
				pRunInfo->SendSfisResultAndShowFail((char*)pReturnInfo->ErrorDes);
				return;
			}
			else
			{
				//do nothing
			}
		}
		else
		{	
			clock_t finish=clock();
			double dbtem = (double)(finish -(clock_t)(pReturnInfo->beginTestTime)) / CLOCKS_PER_SEC;
			char DisplayTime[BUF_LENGH]="";
			sprintf_s(DisplayTime,BUF_LENGH,"[#] Time:  %2.1f seconds\r\n\r\n",dbtem);
			pRunInfo->DisplayRunTimeInfo((char*)"\r\n");
			pRunInfo->DisplayRunTimeInfo((char*)pReturnInfo->TestItem);
			g_Recorder.push_back((char*)pReturnInfo->TestItem);
			pRunInfo->DisplayRunTimeInfo((char*)"\r\n");
			g_Recorder.push_back((char*)"\r\n");

			for( vector<string>::const_iterator iter = pReturnInfo->RunInfo.begin(); iter != pReturnInfo->RunInfo.end();++iter)
			{
				pRunInfo->DisplayRunTimeInfo(iter->c_str());
				g_Recorder.push_back(iter->c_str());
			}
			pRunInfo->DisplayRunTimeInfo(DisplayTime);
			g_Recorder.push_back(DisplayTime);
		}
	}
	catch(...)
	{
		AfxMessageBox("Run_Test fail");
	}
}

void GetItemData(char* item,char *sep,char *outBuf)
{
	try
	{
		char *token=NULL;
		char *Context;
		token=strtok_s(item,sep,&Context);
		if(token!=NULL)
		{
			token=strtok_s(NULL,sep,&Context);
		}
		strcpy_s(outBuf,BUF_LENGH,token);
	}
	catch(...)
	{
		AfxMessageBox("GetItemData fail");
	}
}

LRESULT  CRunInfo::OnRecvEthernetData(WPARAM wParam,LPARAM lParam)
{
	switch(LOWORD(lParam)) 
	{
	case FD_READ:
		m_strStoreEthernetData.Empty();
		WSABUF  ReceiveBuff;
		ReceiveBuff.buf = new char[1024];
		ReceiveBuff.len = 1024;
		memset(ReceiveBuff.buf,0,1024);
		DWORD NumberOfBytesRecvd = 0;
		DWORD dwflag = 0;
		m_strStoreEthernetData.Empty();
		Sleep(500);
		WSARecv(m_Socket,&ReceiveBuff,1,&NumberOfBytesRecvd,&dwflag,NULL,NULL);
		DisplayRunTimeInfo(ReceiveBuff.buf);
		m_strStoreEthernetData = ReceiveBuff.buf;
		g_data += ReceiveBuff.buf;
		delete []ReceiveBuff.buf;
		SetEvent(m_hEvt);
		break;
	}
	
	return 0;
}
void CRunInfo::SendEthernetCmd(char* SendData)
{
	CStringA strSendData;
	DWORD    dwNumber;
	WSABUF   sendbuf;
	sendbuf.buf=new char[300];
	sendbuf.len=300;
	memset(sendbuf.buf,0,300);
	
	strSendData  =SendData;
	strSendData +="\n";
	
	strcpy_s(sendbuf.buf,300,strSendData.GetBuffer(strSendData.GetLength()+1));
	
	WSASend(m_Socket,&sendbuf,1,&dwNumber,0,NULL,NULL);
	
	delete []sendbuf.buf;
}
int CRunInfo::RunSpecifyExeAndRead(CString& strArpOutInfo,char* RunFileName,bool bIsShow )
{
	strArpOutInfo.Empty();
	SECURITY_ATTRIBUTES sat;
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION pinfo;
	char buffer[1024] = "";
	DWORD byteRead =0;
	HANDLE hReadPipeARP,hWritePipeARP;
	sat.nLength=sizeof(SECURITY_ATTRIBUTES);
	sat.bInheritHandle=true;
	sat.lpSecurityDescriptor=NULL;
	if(!CreatePipe(&hReadPipeARP,&hWritePipeARP,&sat,NULL))
	{
		DisplayRunTimeInfo("Create pipe fail");
		return 0;
	}
	startupinfo.cb=sizeof(STARTUPINFO);
	GetStartupInfo(&startupinfo);
	startupinfo.hStdError =hWritePipeARP;
	startupinfo.hStdOutput=hWritePipeARP;
	startupinfo.hStdInput = hReadPipeARP;
	startupinfo.dwFlags=STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	startupinfo.wShowWindow=SW_HIDE;
	if(!CreateProcess(NULL,RunFileName,NULL, NULL, TRUE, NULL, NULL, NULL,&startupinfo,&pinfo))
	{
		char error[MINBUFSIZE] = "";
		sprintf_s(error, MAXBUFSIZE, "Run %s application fail", RunFileName);
		DisplayRunTimeInfo(error);
		return 0;
	}
	if(WAIT_TIMEOUT == WaitForSingleObject(pinfo.hProcess,20000))
	{
		TerminateProcess(pinfo.hProcess,1);
		CloseHandle(hWritePipeARP);
		CloseHandle(pinfo.hProcess);
		CloseHandle(pinfo.hThread);
		CloseHandle(hReadPipeARP);
		char error[MINBUFSIZE] = "";
		sprintf_s(error, MAXBUFSIZE, "Run %s application can't complete", RunFileName);
		return -1;
	}
	CloseHandle(hWritePipeARP);
	CloseHandle(pinfo.hProcess);
	CloseHandle(pinfo.hThread);
	while(TRUE)
	{
		memset(buffer, 0, 1024);
		Sleep(100);
		//BOOL bResult = ReadFile(hReadPipeARP,buffer,1024,&byteRead,NULL);
		if( byteRead == 0 ) 
			break;
		strArpOutInfo+=buffer;
		Sleep(200);
		if(bIsShow)
			DisplayRunTimeInfo(buffer);	
	}
	CloseHandle(hReadPipeARP);
	return 1;
}
bool CRunInfo::InitSocket()
{
	sockaddr_in  SocketAddr;
	m_Socket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,0);
	if(INVALID_SOCKET ==m_Socket)
	{
		DisplayRunTimeInfo("Create socket fail\n");
		return FALSE;
	}	
	SocketAddr.sin_family = AF_INET;
	SocketAddr.sin_addr.S_un.S_addr = inet_addr(INI.GEN.szDefaultIP);
	SocketAddr.sin_port = htons(23);
	int revt = connect(m_Socket,(sockaddr*)&SocketAddr,sizeof(sockaddr));
	if(SOCKET_ERROR==revt)
	{
		DisplayRunTimeInfo("Bind sock fail\n");
		return FALSE;
	}
	if(SOCKET_ERROR ==WSAAsyncSelect(m_Socket,m_hWnd,UM_SOCK,FD_READ))
	{
		DisplayRunTimeInfo("Create network event fail\n ");
		return FALSE;
	}
	return TRUE;
}
bool CRunInfo::ReadIniFile()
{
	DisplayRunTimeInfo("Begin read init.ini file!! \r\n ");
	char szProPath[MINBUFSIZE]="";
	char szProPath1[MINBUFSIZE]="";
	//char szProPath2[MINBUFSIZE]="";
	//char szProPath3[MINBUFSIZE]="";
	//char szProPath4[MINBUFSIZE]="";
	//char szProPath5[MINBUFSIZE]="";
	m_strProductName.Trim();
	GetCurrentDirectory(sizeof(szProPath),szProPath);
	GetCurrentDirectory(sizeof(szProPath1),szProPath1);
	//GetCurrentDirectory(sizeof(szProPath2),szProPath2);
	//GetCurrentDirectory(sizeof(szProPath3),szProPath3);
	//GetCurrentDirectory(sizeof(szProPath4),szProPath4);
	//GetCurrentDirectory(sizeof(szProPath5),szProPath5);

	lstrcat(szProPath,"\\init.ini");
	lstrcat(szProPath1,"\\AutoDownLoad.bat");
	//lstrcat(szProPath2,"\\5gconfig.txt");
	//lstrcat(szProPath3,"\\Ambit_test_flow.ini");
	//lstrcat(szProPath4,"\\AutoDownLoad.bat");
	

	if(OnSFIS)
	{
		//DisplayRunTimeInfo("INI.FP.szPath4=");
		//DisplayRunTimeInfo(INI.FP.szPath4);
		//DisplayRunTimeInfo("\r\n");


		//if(!CopyFile(INI.FP.szPath4,szProPath4 , false))  //Need change path //--->6/7 ok
		//{
		//	AfxMessageBox("Copy AutoDownLoad.bat failed!\r\n 請确定server是否映射到本地F:盤,請關閉程式!\n");
		//	SendSfisResultAndShowFail(SY05);
		//	return false;
		//}
		//else
		//{
		//	//do nothing 
		//}


		//DisplayRunTimeInfo("INI.FP.szPath=");
		//DisplayRunTimeInfo(INI.FP.szPath);
		//DisplayRunTimeInfo("\r\n");


		//if(!CopyFile(INI.FP.szPath,szProPath , false))  //Need change path //--->6/7 ok
		//{
		//	AfxMessageBox("Copy init.ini failed!\r\n 請确定server是否映射到本地F:盤,請關閉程式!\n");
		//	SendSfisResultAndShowFail(SY05);
		//	return false;
		//}
		//else
		//{
		//	//do nothing 
		//}


		//DisplayRunTimeInfo("INI.MY.szPostMydas=");
		//DisplayRunTimeInfo(INI.MY.szPostMydas);
		//DisplayRunTimeInfo("\r\n");
		//if( strstr(INI.MY.szPostMydas , "1") )
		//{
		//	DisplayRunTimeInfo(INI.TF.szTestWiFiCommonFlag);
		//	DisplayRunTimeInfo("\r\n");
		//	if( strstr(INI.TF.szTestWiFiCommonFlag , "1") )
		//	{
		//		DisplayRunTimeInfo(INI.FP.szPath1);
		//		DisplayRunTimeInfo("\r\n");
		//		if(!CopyFile(INI.FP.szPath1,szProPath1 , false)) //Need change path //--->6/7 ok 
		//		{
		//			AfxMessageBox("Copy 2gconfig.txt failed!!\r\n 請确定server是否映射到本地F:盤,請關閉程式!\n");
		//			SendSfisResultAndShowFail(SY05);
		//			return false;
		//		}
		//		else
		//		{
		//			//do nothing 
		//		}
		//	}
		//	else
		//	{
		//		//do nothing 
		//	}
		//	DisplayRunTimeInfo("INI.GEN.szTestByBat=");
		//	DisplayRunTimeInfo(INI.GEN.szTestByBat);
		//	DisplayRunTimeInfo("\r\n");
		//	if( strstr(INI.GEN.szTestByBat , "1") )
		//	{
		//		DisplayRunTimeInfo("INI.FP.szPath2=");
		//		DisplayRunTimeInfo(INI.FP.szPath2);
		//		DisplayRunTimeInfo("\r\n");
		//		if(!CopyFile(INI.FP.szPath2,szProPath2 , false)) //Need change path //--->6/7 ok
		//		{
		//			AfxMessageBox("Copy 5gconfig.txt failed!!\r\n 請确定server是否映射到本地F:盤,請關閉程式!\n");
		//			SendSfisResultAndShowFail(SY05);
		//			return false;
		//		}
		//		else
		//		{
		//			//do nothing 
		//		}
		//	}
		//	else
		//	{
		//		//do nothing 
		//	}
		//}
		//else
		//{
		//	//do nothing 
		//}
		//DisplayRunTimeInfo("INI.FP.szPath3=");
		//DisplayRunTimeInfo(INI.FP.szPath3);
		//DisplayRunTimeInfo("\r\n");
		//if(!CopyFile(INI.FP.szPath3,szProPath3 , false))  //Need change path //--->6/7 ok
		//{
		//	AfxMessageBox("Copy Ambit_test_flow.ini failed!\r\n 請确定server是否映射到本地F:盤,請關閉程式!\n");
		//	SendSfisResultAndShowFail(SY05);
		//	return false;
		//}
		//else
		//{
		//	//do nothing 
		//}
		DisplayRunTimeInfo("INI.FP.szPath1=");
		DisplayRunTimeInfo(INI.FP.szPath1);
		DisplayRunTimeInfo("\r\n");
		if(!CopyFile(INI.FP.szPath1,szProPath1 , false))  //Need change path //--->6/7 ok
		{
			AfxMessageBox("Copy AutoDownLoad.bat failed!\r\n 請确定server是否映射到本地F:盤,請關閉程式!\n");
			SendSfisResultAndShowFail(SY05);
			return false;
		}
		else
		{
			//do nothing 
		}

		if(!WinExec("AutoDownLoad.bat",SW_HIDE))
		{
			AfxMessageBox("AutoDownLoad.bat Copy file failed!!\r\n 請确定server是否映射到本地,請關閉程式!\n");
			SendSfisResultAndShowFail(SY05);
			return false;
		}
	}
	else
	{
		//do nothing 
	}


	GetPrivateProfileString("General","DeaultIP","192.168.0.1",INI.GEN.szDefaultIP, MINBUFSIZE,szProPath);
	GetPrivateProfileString("General","DisableWirelessFlag","0",INI.GEN.szDisableWireless, MINBUFSIZE,szProPath);
	GetPrivateProfileString("General","SFCProductLength","25",INI.GEN.szSFCProductLength,MAX_PATH,szProPath);
	GetPrivateProfileString("General","TestBybat","0" ,INI.GEN.szTestByBat, MAX_PATH,szProPath);/*3*3 need test by BAT file*/		
	
	GetPrivateProfileString("MYDAS","PostMydas","0",INI.MY.szPostMydas,MAX_PATH,szProPath);	
	GetPrivateProfileString("MYDAS","MYDAS_Debug","0",INI.MY.szMydasDebug,MAX_PATH,szProPath);
	GetPrivateProfileString("General","DownLoadImageFlag","0",INI.DLI.szDownLoadImageFlag,MAX_PATH,szProPath);	

	//GetPrivateProfileString("FilePath","ServerInitFilePath","0" ,INI.FP.szPath, MAX_PATH,szProPath);  /*init.ini path at server*/
	//GetPrivateProfileString("FilePath","ServerFilePath1","0" ,INI.FP.szPath1, MAX_PATH,szProPath);/*2gconfig.txt path at server*/
	//GetPrivateProfileString("FilePath","ServerFilePath2","0" ,INI.FP.szPath2, MAX_PATH,szProPath);/*5gconfig.txt path at server*/
	//GetPrivateProfileString("FilePath","ServerFilePath3","0" ,INI.FP.szPath3, MAX_PATH,szProPath);/*SFIS.txt path at server*/
	//GetPrivateProfileString("FilePath","ServerFilePath4","0" ,INI.FP.szPath4, MAX_PATH,szProPath);/*File path4 at server*/
	GetPrivateProfileString("FilePath","RecordFilePath","0",INI.FP.szStoreSrvPath,MAX_PATH,szProPath);
	GetPrivateProfileString("FilePath","RecordFileServerFalg","0",INI.FP.szRecodeServerFlag,MAX_PATH,szProPath);
	GetPrivateProfileString("FilePath","RecordFilePathServer","0",INI.FP.Log,MAX_PATH,szProPath);
	/*SFIS*/
	GetPrivateProfileString("SFIS","SFIS_B","",INI.SFC.szSFCBegin, MINBUFSIZE,szProPath);//SFIS begin string
	GetPrivateProfileString("SFIS","SFIS_P","",INI.SFC.szSFCPass, MINBUFSIZE,szProPath);//SFIS pass string
	GetPrivateProfileString("SFIS","SFIS_F","",INI.SFC.szSFCFail, MINBUFSIZE,szProPath);//SFIS fail string
	/*End*/	
	GetPrivateProfileString("TestFlow","FoxWiFiTestFlag","0",INI.TF.szTestWiFiCommonFlag,MAX_PATH,szProPath);	
	GetPrivateProfileString("TestFlow","5GBatTestFlag","0",INI.TF.szTest5G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("TestFlow","2GBatTestFlag","0",INI.TF.szTest2G, MINBUFSIZE,szProPath);

	GetPrivateProfileString("5GBatTest","RunBATName_5G","0",INI.T5G.RunBATName_5G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("5GBatTest","IQLogFile_5G","0",INI.T5G.IQLogFile_5G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("5GBatTest","IQLogPath_5G","0",INI.T5G.IQLogPath_5G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("5GBatTest","IQTestFinishFlagFile_5G","0",INI.T5G.IQTestFinishFlagFile_5G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("5GBatTest","BATFilePath_5G","0",INI.T5G.BATFilePath_5G,MAX_PATH,szProPath);
	GetPrivateProfileString("5GBatTest","ServeBATPath_5G","0",INI.T5G.ServeBATPath_5G,MAX_PATH,szProPath);
	GetPrivateProfileString("5GBatTest","WindowsName_5G","0",INI.T5G.WindowsName_5G,MAX_PATH,szProPath);
	
	
	GetPrivateProfileString("2GBatTest","RunBATName_2G","0",INI.T2G.RunBATName_2G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("2GBatTest","IQLogFile_2G","0",INI.T2G.IQLogFile_2G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("2GBatTest","IQLogPath_2G","0",INI.T2G.IQLogPath_2G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("2GBatTest","IQTestFinishFlagFile_2G","0",INI.T2G.IQTestFinishFlagFile_2G, MINBUFSIZE,szProPath);
	GetPrivateProfileString("2GBatTest","BATFilePath_2G","",INI.T2G.BATFilePath_2G,MAX_PATH,szProPath);
	GetPrivateProfileString("2GBatTest","ServeBATPath_2G","0",INI.T2G.ServeBATPath_2G,MAX_PATH,szProPath);
	GetPrivateProfileString("2GBatTest","WindowsName_2G","0",INI.T2G.WindowsName_2G,MAX_PATH,szProPath);

	
	
	DisplayRunTimeInfo("read init.ini file finish!!\r\n ");
	return true;
}
bool CRunInfo::RunTelnetExe(char* GetMacValue)
{
	char szRunTelnetExeFullName[MAXBUFSIZE] = "";
	DisplayRunTimeInfo(INI.GEN.szDefaultIP);
	DisplayRunTimeInfo("\r\n");
	sprintf_s(szRunTelnetExeFullName, MAXBUFSIZE, "telnetenable %s %s Gearguy Geardog", INI.GEN.szDefaultIP, GetMacValue);
	
	STARTUPINFO si;
	GetStartupInfo(&si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;

	if(!CreateProcess(NULL,szRunTelnetExeFullName,NULL,NULL,FALSE,0,NULL,NULL,&si,&pi))
	{
		DisplayRunTimeInfo("Not found Telnetenble.exe\n Test over");
		return false;
	}

	if(WAIT_TIMEOUT == WaitForSingleObject(pi.hProcess,5000))
	{
			TerminateProcess(pi.hProcess,1);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);
			return false;
	}
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	return true;
}

DWORD WINAPI LinkDataBase(void)
{
	if(pInstMydas != NULL)
	{
		if(PTSC_GetLinkStatus(pInstMydas))
		{
			return 1;
		}
	}

	char szdutName[32]; //Mydas DutName
	char szsvip[32];	  //mydas IP
	char szTestDaigVersion[32];  //Mydas DaigVersion
	char szTestTitleVersion[32];  //Mydas TitleVersion
	char productName[15];  //Mydas ProductName
	memset(szsvip,0,sizeof(szsvip));
	memset(szdutName,0,sizeof(szdutName));
	memset(szTestDaigVersion,0,sizeof(szTestDaigVersion));
	memset(szTestTitleVersion,0,sizeof(szTestTitleVersion));	
	memset(productName,0,sizeof(productName));

	char szPath[MAX_PATH] = "";
	char szFullPath[MAX_PATH] = "";	
	GetCurrentDirectory(MAX_PATH, szPath);
	sprintf_s(szFullPath, MAX_PATH, "%s\\init.ini", szPath);

	GetPrivateProfileString("MYDAS","MYDAS_IP","10.117.34.106",szsvip,32,szFullPath);
	GetPrivateProfileString("MYDAS","MYDAS_DutName","0",szdutName,32,szFullPath);
	GetPrivateProfileString("MYDAS","MYDAS_DaigVersion","0",szTestDaigVersion,32,szFullPath);
	GetPrivateProfileString("MYDAS","MYDAS_TitleVersion","0",szTestTitleVersion,32,szFullPath);

	strcpy_s(m_Mydas.Ip, sizeof(m_Mydas.Ip),szsvip);
	strcpy_s(m_Mydas.ProductPN, sizeof(m_Mydas.ProductPN),pRunInfo->m_strProductName.Trim());
	strcpy_s(m_Mydas.Product,sizeof(productName),szdutName);
	strcpy_s(m_Mydas.ComName, sizeof(m_Mydas.ComName),pRunInfo ->m_strPcName);
	strcpy_s(m_Mydas.TestStaName, sizeof(m_Mydas.TestStaName),"PT");
	strcpy_s(m_Mydas.DaigVersion, sizeof(m_Mydas.DaigVersion),szTestDaigVersion);
	strcpy_s(m_Mydas.TitleVersion,sizeof(m_Mydas.TitleVersion),szTestTitleVersion);

	if (!PTSC_Connect(&hDev,&m_Mydas))
	{
		return 0;
	}

	return 1;
}

bool SendDatatoMYDAS(char* DetailLog,char* ErrDef,char* SendDataBuf)
{
	HANDLE hEventRev=CreateEvent( 
		NULL,         // default security attributes
		TRUE,         // auto-reset event
		FALSE,         // initial state is unsignaled
		NULL  // object name
		); 

	PTSC_SetSendData(hDev,DetailLog,(unsigned int)strlen(DetailLog),0);
	PTSC_SetSendData(hDev,ErrDef,(unsigned int)strlen(ErrDef),1);
	PTSC_SetSendData(hDev,SendDataBuf,(unsigned int)strlen(SendDataBuf),2);
	PTSC_Send(hDev);

	CloseHandle(hEventRev);

	return TRUE;
}

void RecordDetailLog(char  *szRecordFilePath)
{
	lk.clear();
	CFile rfile;
	CString strline;
	if(rfile.Open(szRecordFilePath,CFile::modeRead))
	{
		CArchive ar(&rfile,CArchive::load);
		pair<string,Item_Data> p;
		char outBuf[20]="";
		while(ar.ReadString(strline))
		{
			memset(outBuf,0,20);
			if(strstr(strline,"Verify_Tx_EVM")!=NULL
				||strstr(strline,"Verify_Tx_MASK")!=NULL
				||strstr(strline,"Verify_Rx")!=NULL)
			{
				int len = strline.GetLength();
				strline.SetAt(len-1,0);
				p.first=strline;
				memset(FailValue,0,20);
				continue;
			}
			if(strline.IsEmpty())
			{
				continue;
			}
			else if(strstr(strline,"[#] EVM ")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":dB",outBuf);
				p.second.evm=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] POWER ")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":dBm",outBuf);
				p.second.power=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] EVM0 ")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":dB",outBuf);
				p.second.evm0=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] POWER0 ")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":dB",outBuf);
				p.second.power0=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] EVM1 ")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":dBm",outBuf);
				p.second.evm1=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] POWER1 ")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":dBm",outBuf);
				p.second.power1=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] MASK")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":%",outBuf);
				p.second.mask=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] PER")!=NULL)
			{
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":%",outBuf);
				p.second.per=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			if(strstr(strline,"[#] FREQ")!=NULL)
			{
				AfxMessageBox("FREQ");
				BOOL flag = FALSE;
				if(strstr(strline,"Fail") != NULL)
				{
					flag = TRUE;
				}
				GetItemData(strline.GetBuffer(strline.GetLength()),":ppm",outBuf);
				p.second.freq=outBuf;
				if(flag == TRUE)
				{
					strcpy_s(FailValue,20,outBuf);
				}
				continue;
			}
			else if(strstr(strline,"[#] Time:")!=NULL)
			{
				map<string,Item_Data>::iterator itPower = lk.find(p.first);
				if(itPower!=lk.end())
				{
					lk.erase(itPower);
				}
				lk.insert(p);
				continue;
			}
			else
			{
				continue;
			}
		}
		ar.Close();
		rfile.Close();	
		memset(g_MyDAS,0,BUF_LENGH);
		for(map<string,Item_Data>::iterator it=lk.begin();it!=lk.end();it++)
		{

			if(strstr(it->first.c_str(),"Verify_Tx_EVM")!=NULL)
			{

				if(strstr(it->first.c_str(),"FREQ")!=NULL)
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.freq.c_str());
					strcat_s(g_MyDAS,BUF_LENGH," ");
				}
				if(
					strstr(it->first.c_str(),"MCS8") != NULL||
					strstr(it->first.c_str(),"MCS9") != NULL||
					strstr(it->first.c_str(),"MCS10")!= NULL||
					strstr(it->first.c_str(),"MCS11")!= NULL||
					strstr(it->first.c_str(),"MCS12")!=NULL||
					strstr(it->first.c_str(),"MCS13")!=NULL||
					strstr(it->first.c_str(),"MCS14")!=NULL||
					strstr(it->first.c_str(),"MCS15")!=NULL
				  )
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.evm.c_str());
					strcat_s(g_MyDAS,BUF_LENGH," ");
					strcat_s(g_MyDAS,BUF_LENGH,it->second.power.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}
				else if(
						(strstr(it->first.c_str(),"MCS0")!=NULL)||
						(strstr(it->first.c_str(),"MCS1")!=NULL)||
						(strstr(it->first.c_str(),"MCS2")!=NULL)||
						(strstr(it->first.c_str(),"MCS3")!=NULL)||
						(strstr(it->first.c_str(),"MCS4")!=NULL)||
						(strstr(it->first.c_str(),"MCS5")!=NULL)||
						(strstr(it->first.c_str(),"MCS6")!=NULL)||
						(strstr(it->first.c_str(),"MCS7")!=NULL)||
						(strstr(it->first.c_str(),"11M")!=NULL) ||
						(strstr(it->first.c_str(),"1M")!=NULL)  ||
						(strstr(it->first.c_str(),"54M")!=NULL)	||
						(strstr(it->first.c_str(),"2M")!=NULL)	||
						(strstr(it->first.c_str(),"5.5M")!=NULL)||
						(strstr(it->first.c_str(),"22M")!=NULL)	||
						(strstr(it->first.c_str(),"36M")!=NULL)	||
						(strstr(it->first.c_str(),"6M")!=NULL)	||
						(strstr(it->first.c_str(),"9M")!=NULL)	||
						(strstr(it->first.c_str(),"12M")!=NULL)	||
						(strstr(it->first.c_str(),"18M")!=NULL)	||
						(strstr(it->first.c_str(),"24M")!=NULL)	
					   )
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.evm0.c_str());
					strcat_s(g_MyDAS,BUF_LENGH," ");
					strcat_s(g_MyDAS,BUF_LENGH,it->second.power0.c_str());
					strcat_s(g_MyDAS,BUF_LENGH," ");
					strcat_s(g_MyDAS,BUF_LENGH,it->second.evm1.c_str());
					strcat_s(g_MyDAS,BUF_LENGH," ");
					strcat_s(g_MyDAS,BUF_LENGH,it->second.power1.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}

				/*
				if(it->second.evm.c_str() != "")
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.evm.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}
				if(it->second.evm0.c_str() != "")
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.evm0.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}
				if(it->second.evm1.c_str() != "")
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.evm1.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}

				if(it->second.power.c_str() != "")
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.power.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}
				if(it->second.power0.c_str() != "")
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.power0.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}
				if(it->second.power1.c_str() != "")
				{
					strcat_s(g_MyDAS,BUF_LENGH,it->second.power1.c_str());
					strcat_s(g_MyDAS,BUF_LENGH,",");
				}*/

			//	AfxMessageBox(g_MyDAS);
			}
			else if(strstr(it->first.c_str(),"Verify_Tx_MASK")!=NULL)
			{
				strcat_s(g_MyDAS,BUF_LENGH,it->second.mask.c_str());
				strcat_s(g_MyDAS,BUF_LENGH,",");
			}
			else if(strstr(it->first.c_str(),"Verify_Rx")!=NULL)
			{
				strcat_s(g_MyDAS,BUF_LENGH,it->second.per.c_str());
				strcat_s(g_MyDAS,BUF_LENGH,",");
			}
			else
			{
				//do nothing;
			}
		}

		map<string,Item_Data>::iterator iter = --lk.end();
		strcpy_s(FailItem,BUF_LENGH,iter->first.c_str());
		AfxMessageBox(FailItem);
	}
}


CString CRunInfo::GetMydasData(CString szResult)
{
	CString Data=szResult;
	Data.Replace(" ",",");
	return Data;
}

int CRunInfo::GetSFISData(/*[in]*/char InputBuf[MINBUFSIZE] , /*[out]*/char RtnBuf[MINBUFSIZE])
/*InputBuf: HH(25)+SSN(25)+MAC(12)PNNAME(12)+PASS(4);  */
{
	if(NULL == InputBuf)
	{
		return 0;
	}
	StrTrim(InputBuf," ");

	char seps[MINBUFSIZE]   = " ";
	char seps1[MINBUFSIZE]   = "+";
	char seps2[MINBUFSIZE]   = "(";

	char *pToken, *pNextToken;
	char *pToken1, *pNextToken1;
	//char *pToken2, *pNextToken2;

   // Establish string and get the first token: 
	AfxMessageBox(InputBuf);
	AfxMessageBox(seps1);
	AfxMessageBox(seps2);
    pToken = strtok_s( InputBuf, seps, &pNextToken);
	//AfxMessageBox(pToken);
  
    // While there are tokens in "string1" 
    while (pToken != NULL) 
    {
		AfxMessageBox(pToken);
        // Get next token:
		pToken1 = strtok_s (NULL, seps1, &pNextToken1); 
		AfxMessageBox(pToken1);
    }

	

	return 1;
}
void CRunInfo::GetMac(IN char* source,OUT char* target)
{
	char  *next_token;
	char  sep[] = _T(" \n\t");
	char  Temp[20];
	char  szMACBuf[100];
	char* tok = _tcstok_s(source,sep,&next_token);
	while(NULL!=tok)
	{
		StrTrim(INI.GEN.szDefaultIP , "\r\n");
		if(!_tcscmp(tok,INI.GEN.szDefaultIP))
		{
			tok = _tcstok_s(NULL,sep,&next_token);
			_tcscpy_s(Temp,20,tok);
			int i ,j;
			for(i = 0,j = 0;i < 17;i++)
			{
				if(Temp[i]!='-')
				{
					target[j++]=Temp[i];
				}
			}
			target[j]='\0';
			break;
		}
		tok = _tcstok_s(NULL,sep,&next_token);
	}


	_strupr_s(target,MINBUFSIZE);

	sprintf_s(szMACBuf, MINBUFSIZE, "MAC: %s", target);
	Sleep(1000);
	DisplayRunTimeInfo(szMACBuf);
	DisplayRunTimeInfo("\r\n\r\n");
}


void CRunInfo::DisplayTestInfo(char *Describe , char *parameter )
{

	DisplayRunTimeInfo(Describe);
	DisplayRunTimeInfo(parameter);
	DisplayRunTimeInfo("\r\n");
}

bool  CRunInfo::SendDutCommand(char *pCmd, char *pRtn, int iDelay)
{ 
	int iSubDelay = iDelay/10;
	ResetEvent(m_hEvt);
	g_data="";
	SendEthernetCmd(pCmd);
	
	while(iSubDelay--)
	{
		if(WAIT_OBJECT_0 == WaitForSingleObject(m_hEvt,10))
		{
			if(pRtn==NULL)
			{
				return true;
			}
			else
			{
				if(strstr(g_data,pRtn) != NULL)
				{
					return true;
				}
			}	
			ResetEvent(m_hEvt);
		}		
	}	

	return false;
}
bool CRunInfo::PingSpecifyIP(char* IP, int nSuccessCount)
{
	HANDLE hWritePipe  = NULL;
	HANDLE hReadPipe   = NULL;

	char  szPing[MINBUFSIZE] = "";
	sprintf_s(szPing, MINBUFSIZE, "ping.exe %s -n 30 -w 100", IP);
	char   szReadFromPipeData[MAXBUFSIZE] = "";
	DWORD  byteRead    = 0;

	SECURITY_ATTRIBUTES sa;
	sa.bInheritHandle =true;
	sa.lpSecurityDescriptor = NULL;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);

	if(!CreatePipe(&hReadPipe,&hWritePipe,&sa,0))
	{
		DisplayRunTimeInfo("Create pipe fail\r\n");
		return 0;
	}

	PROCESS_INFORMATION pi;
	STARTUPINFO        si;
	GetStartupInfo(&si);
	si.cb = sizeof(STARTUPINFO);
	si.hStdError  = hWritePipe;
	si.hStdOutput = hWritePipe;
	si.dwFlags=STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow=SW_HIDE;

	if(!CreateProcess(NULL,szPing,NULL,NULL,TRUE,0,NULL,NULL,&si,&pi))
	{
		DisplayRunTimeInfo("Start run ping.exe fail!");
		return 0;
	}

	CloseHandle(pi.hThread);
	CloseHandle(hWritePipe);
	CloseHandle(pi.hProcess);

	DWORD  dwPingFailCount    = 30;
	DWORD  dwPingSuccessCount = nSuccessCount;
	
	while(TRUE)
	{
		memset(szReadFromPipeData,0,MAXBUFSIZE);
		Sleep(100);
		int bResult = ReadFile(hReadPipe,szReadFromPipeData,MAXBUFSIZE,&byteRead,NULL);
		
		if(!bResult)
		{
			DisplayRunTimeInfo("Read ping.exe fail!");
			return 0;
		}

		Sleep(200);
//		DisplayRunTimeInfo(szReadFromPipeData);
		
		if(strstr(szReadFromPipeData,"Reply from") || strstr(szReadFromPipeData,IP))
		{
			dwPingSuccessCount--;
		}
		else
		{
			dwPingFailCount--;
		}
		DisplayRunTimeInfo(szReadFromPipeData);
		if(!dwPingSuccessCount)
		{
			ReadFile(hReadPipe,szReadFromPipeData,MAXBUFSIZE,&byteRead,NULL);
			DisplayRunTimeInfo(szReadFromPipeData);
			return 1;
		}
		if(!dwPingFailCount)
			return 0;
	}

}

bool CRunInfo::Funtion_Downlaod_image()
{
	//1.check test image is here
	if(!PathFileExists(".\\vmlinuz"))
	{
		AfxMessageBox("在測試程序所在目錄沒有發現Test image(vmlinuz),請確認!\r\nDon't find test image(vmlinuz) at program file path,please check!\r\n");
		//SendSfisResultAndShowFail(DL00);
		return false;
	}

	//4.detected FTP download srart
	int iRetryNum = 100;
	while(iRetryNum --)
	{
		if(!PathFileExists(".\\TFTP_Status.ini"))
		{
			DisplayRunTimeInfo("TFTP_Status.ini is not find!\r\n");
			Sleep(200);
			continue;
		}
		if(Read_ini())
		{
			DisplayRunTimeInfo("TFTP_Status.ini read OK!\r\n");
		}		
		else
		{
			DisplayRunTimeInfo("TFTP_Status.ini read failed!\r\n");			
		}
		DisplayTestInfo("INI.DLI.szDownloadDetectedFlag=" , INI.DLI.szDownloadDetectedFlag);

		if(strstr(INI.DLI.szDownloadDetectedFlag , "1") != NULL)
		{
			DisplayRunTimeInfo("TFTP download image start!\r\n");
			
			//change TFTP_Status.ini value
			DisplayTestInfo("INI.DLI.szDownloadDetectedFlag=",INI.DLI.szDownloadDetectedFlag);
			WritePrivateProfileString("STATUS","DETECTED","0",".\\TFTP_Status.ini");
			GetPrivateProfileString("STATUS","DETECTED", "", INI.DLI.szDownloadDetectedFlag, sizeof(INI.DLI.szDownloadDetectedFlag), ".\\TFTP_Status.ini");		
			DisplayTestInfo("INI.DLI.szDownloadDetectedFlag=",INI.DLI.szDownloadDetectedFlag);
			
			break ;
		}
		else
		{
			DisplayRunTimeInfo("TFTP not start download image!\r\n");
			Sleep(200);
		}
	}
	//recode download time
	float iTime1 = float((100-iRetryNum)*200/1000);
	char szBuf1[256] = "";
	sprintf_s(szBuf1 , sizeof(szBuf1) , "Start download time=%f\r\n" , iTime1 );
	DisplayRunTimeInfo(szBuf1);

	if(iRetryNum <= 0)
	{		
		AfxMessageBox("產品沒有download Test image(vmlinuz),請重啟產品!\r\nDUT not download test image,please reboot DUT!\r\n");
		//SendSfisResultAndShowFail(DL00);
		return false;
	}
	//5.wait download finish
	//wait time is 1000*200
	iRetryNum = 1000;
	while(iRetryNum --)
	{
		if(Read_ini())
		{
			DisplayRunTimeInfo("TFTP_Status.ini read OK!\r\n");
		}		
		else
		{
			DisplayRunTimeInfo("TFTP_Status.ini read failed!\r\n");
			Sleep(200);
			continue;
		}
		DisplayTestInfo("INI.DLI.szDownloadFinishFlag=" , INI.DLI.szDownloadFinishFlag);
		if(strstr(INI.DLI.szDownloadFinishFlag , "1") != NULL )
		{
			DisplayRunTimeInfo("Download finish!\r\n");
			
			DisplayTestInfo("INI.DLI.szDownloadFinishFlag=",INI.DLI.szDownloadFinishFlag);
			WritePrivateProfileString("STATUS","SENDOK","0",".\\TFTP_Status.ini");
			GetPrivateProfileString("STATUS","SENDOK", "", INI.DLI.szDownloadFinishFlag, sizeof(INI.DLI.szDownloadFinishFlag), ".\\TFTP_Status.ini");
			DisplayTestInfo("INI.DLI.szDownloadFinishFlag=",INI.DLI.szDownloadFinishFlag);
			
			break;
		}
		else
		{
			DisplayRunTimeInfo("Download not finish!\r\n");
			Sleep(200);
		}

	}
	//recode download time
	float iTime2 = float((1000-iRetryNum)*200/1000);
	char szBuf2[256] = "";
	sprintf_s(szBuf2 , sizeof(szBuf2) , "DownLoad image time=%f\r\n" , iTime2 );
	DisplayRunTimeInfo(szBuf2);

	if(iRetryNum <= 0)
	{		
		AfxMessageBox("產品沒有download Test image(vmlinuz)成功,請重啟產品!\r\nDUT not download test image finish,please reboot DUT!\r\n");
		//SendSfisResultAndShowFail(DL00);
		return false;
	}

	//pRunInfo->DeleteSpecifyExe("IQrun_Console.exe");	

	
	float iTime = iTime1 + iTime2 ;
	char szBuf[256] = "";
	sprintf_s(szBuf , sizeof(szBuf) , "DownLoad test image time=%f\r\n" , iTime );
	DisplayRunTimeInfo(szBuf);

	return true;	
}


int CRunInfo::Read_ini()
{
	//int  ini_status ;
	char iniPath[MAXBUFSIZE];
	GetCurrentDirectory (MAXBUFSIZE, iniPath);
	sprintf_s(iniPath , MAXBUFSIZE , "%s\\%s" , iniPath , "TFTP_Status.ini");

	// if the .ini exists	
	GetPrivateProfileString("STATUS","SENDOK", "", INI.DLI.szDownloadFinishFlag, sizeof(INI.DLI.szDownloadFinishFlag), iniPath);		
	GetPrivateProfileString("STATUS","DETECTED", "", INI.DLI.szDownloadDetectedFlag, sizeof(INI.DLI.szDownloadDetectedFlag), iniPath);		
	
	return 1;
}


int CRunInfo::Run_WiFiTest_IQDemo()
{
	int iTestResult = 0;
	//1.open IQ_demo.exe
	//move to InitDialog

	remove(".\\log\\log_all.txt");
	//2.change LP_StartUp.ini file vaule and start test
	GetPrivateProfileString("TESTER_CONTROL","CTRL_MODE ", "", INI.IDR.szIQDemoRunMarkFlag, sizeof(INI.IDR.szIQDemoRunMarkFlag), ".\\TestStartUp.ini");		
	DisplayTestInfo("INI.IDR.szIQDemoRunMarkFlag=",INI.IDR.szIQDemoRunMarkFlag);
	WritePrivateProfileString("TESTER_CONTROL","CTRL_MODE ","0",".\\TestStartUp.ini");
	GetPrivateProfileString("TESTER_CONTROL","CTRL_MODE ", "", INI.IDR.szIQDemoRunMarkFlag, sizeof(INI.IDR.szIQDemoRunMarkFlag), ".\\TestStartUp.ini");		
	DisplayTestInfo("INI.IDR.szIQDemoRunMarkFlag=",INI.IDR.szIQDemoRunMarkFlag);
	char szBuf[MINBUFSIZE];
	sprintf_s(szBuf , MINBUFSIZE , "%s start running..." , INI.IDR.szIQDemoRunFileName);
	DisplayRunTimeInfo(szBuf);
	Sleep(500);

	//3.Loop read LP_StartUp.ini,if find value != 0/1 ,it means test finish
	//A:Read LP_StartUp.ini just waitting test finish!

	while(1)
	{
		GetPrivateProfileString("TESTER_CONTROL","CTRL_MODE ", "1", INI.IDR.szIQDemoRunMarkFlag, sizeof(INI.IDR.szIQDemoRunMarkFlag), ".\\TestStartUp.ini");		
		DisplayTestInfo("INI.IDR.szIQDemoRunMarkFlag=",INI.IDR.szIQDemoRunMarkFlag);
		if ( atoi(INI.IDR.szIQDemoRunMarkFlag) == 2 ||  atoi(INI.IDR.szIQDemoRunMarkFlag)==3) 
		{
			DisplayRunTimeInfo("Test finish!\r\n");
			break;
		}
		else if(atoi(INI.IDR.szIQDemoRunMarkFlag)>=90 && atoi(INI.IDR.szIQDemoRunMarkFlag)<=99)
		{

			
				return (atoi(INI.IDR.szIQDemoRunMarkFlag)) ;
		}
		else
		{
			DisplayRunTimeInfo("Test not finish,please waitting...\r\n");
			Sleep(1000);
			continue;
		}

	}

	//B:Read test result file find mark information and display result

	FILE *ff;
	char buf[1024] = {0};

	GetPrivateProfileString("General","DebugShow ", "0", INI.GEN.szDebugShow, sizeof(INI.GEN.szDebugShow), ".\\init.ini");		
	DisplayTestInfo("INI.GEN.szDebugShow=",INI.GEN.szDebugShow);
	if( atoi(INI.GEN.szDebugShow) == 1 )
	{
		fopen_s(&ff,".\\Log\\Log_all.txt","r");
		while(!feof(ff))
		{		
			memset(buf,0,1024);
			fgets(buf,1024,ff);
			if( strstr(buf,"Total Test time:") == NULL )  
			{ 
				DisplayRunTimeInfo(buf);
				continue;	
			}
			else
			{			
				DisplayRunTimeInfo("Test finish!\r\n");
				break;
			}
		}
		fclose(ff);	
	}

	//4.parse test result 

	bool bFindFail = false ;
	fopen_s(&ff,".\\log\\Log_all.txt","r");	
	while(!feof(ff))
	{
		memset(buf,0,1024);
		fgets(buf,1024,ff);

		if(strstr(buf,"* P A S S E D *"))  
		{ 
			iTestResult = 1;
		}
		else if(strstr(buf,"* F A I L E D *")) 
		{ 
			bFindFail = true;
		}
		else
		{
			//do nothing
		}
		if(bFindFail)
		{	
			int szPowerLever = 0 ;
			bool HigtPowerLeverRXItesm = false ;
			char szBuf[1024];

			GetPrivateProfileString("TESTER_CONTROL","CTRL_MODE ", "", INI.IDR.szIQDemoRunMarkFlag, sizeof(INI.IDR.szIQDemoRunMarkFlag), ".\\TestStartUp.ini");		
			DisplayTestInfo("INI.IDR.szIQDemoRunMarkFlag=",INI.IDR.szIQDemoRunMarkFlag);
			iTestResult = atoi(INI.IDR.szIQDemoRunMarkFlag);
			if( iTestResult >= 90 && iTestResult < 999)
			{
				return iTestResult;
			}

			if(strstr(buf,"<-- Failed")!=NULL)
			{
				DisplayRunTimeInfo(buf);
				DisplayRunTimeInfo("\r\n");

				if( strstr(buf,"PER" ) != NULL )
				{
					szPowerLever = GetItemPower(buf);						
					sprintf_s(szBuf , 1024 , "szPowerLever = %d\r\n" , szPowerLever);
					DisplayRunTimeInfo(szBuf);
					if(szPowerLever >= -60 && szPowerLever <= 0)
					{
						HigtPowerLeverRXItesm = true ;
					}
				}

				if(strstr(buf,"24")!=NULL)//2.4G test items
				{	
					if( strstr(buf,"EVM") && strstr(buf,"MCS"))       {iTestResult=8;break;}//EV04
					else if( strstr(buf,"EVM") && strstr(buf,"OFDM"))       {iTestResult=9;break;}//EV02
					else if( strstr(buf,"EVM") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=10;break;}//EV01

					else if( strstr(buf,"MASK") && strstr(buf,"MCS"))       {iTestResult=21;break;}//SM04
					else if( strstr(buf,"MASK") && strstr(buf,"OFDM"))       {iTestResult=22;break;}//SM02
					else if( strstr(buf,"MASK") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=23;break;}//SM01
					
					else if( strstr(buf,"POWER") && strstr(buf,"MCS"))       {iTestResult=31;break;}//PW04
					else if( strstr(buf,"POWER") && strstr(buf,"OFDM"))       {iTestResult=32;break;}//PW02
					else if( strstr(buf,"POWER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=33;break;}//PW01

					//else if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=11;break;}//PR04
					//else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=12;break;}//PR02
					//else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=13;break;}//PR01

					else                                         { }

					if( HigtPowerLeverRXItesm )
					{
						if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=14;break;}//PR08
						else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=15;break;}//PR06
						else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=16;break;}//PR05
					}
					else
					{
						if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=11;break;}//PR04
						else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=12;break;}//PR02
						else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=13;break;}//PR01	
					}

				}
				else//5G test items
				{
					if( strstr(buf,"EVM") && strstr(buf,"MCS"))       {iTestResult=41;break;}//EV14
					else if( strstr(buf,"EVM") && strstr(buf,"OFDM"))       {iTestResult=42;break;}//EV12
					else if( strstr(buf,"EVM") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=43;break;}//EV11

					//else if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=44;break;}//PR14
					//else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=45;break;}//PR12
					//else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=46;break;}//PR11

					else if( strstr(buf,"MASK") && strstr(buf,"MCS"))       {iTestResult=51;break;}//SM14
					else if( strstr(buf,"MASK") && strstr(buf,"OFDM"))       {iTestResult=52;break;}//SM12
					else if( strstr(buf,"MASK") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=53;break;}//SM11

					else if( strstr(buf,"POWER") && strstr(buf,"MCS"))       {iTestResult=61;break;}//PW14
					else if( strstr(buf,"POWER") && strstr(buf,"OFDM"))       {iTestResult=62;break;}//PW12
					else if( strstr(buf,"POWER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=63;break;}//PW11

					else                                         { }
					
					if( HigtPowerLeverRXItesm )
					{
						if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=47;break;}//PR18
						else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=48;break;}//PR16
						else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=49;break;}//PR15
					}
					else
					{
						if( strstr(buf,"PER") && strstr(buf,"MCS"))       {iTestResult=44;break;}//PR14
						else if( strstr(buf,"PER") && strstr(buf,"OFDM"))       {iTestResult=45;break;}//PR12
						else if( strstr(buf,"PER") && ((strstr(buf,"CCK")) || (strstr(buf,"DSSS"))))       {iTestResult=46;break;}//PR11	
					}
				}

			}
		}
	}
	fclose(ff);	

	//5.rename test log    For record test log file

	//move to TestPass()/TestFail for recode errorcode at file name



	//6.parse test log get Mydas data

	DisplayTestInfo("INI.MY.szPostMydas=",INI.MY.szPostMydas);
	DisplayTestInfo("INI.MY.szMydasDebug=",INI.MY.szMydasDebug);	
	
	if((OnSFIS && (strstr(pRunInfo->INI.MY.szPostMydas , "1"))) || (strstr(pRunInfo->INI.MY.szMydasDebug , "1")))
	{

			char szResult[4012]="";
			char szbuf[4012]="";

			DisplayTestInfo("Begin Get 5G Data","");
			int iRet = _dxGetData5g(szResult,".\\log\\log_all.txt");
			sprintf_s(szbuf,sizeof(szbuf) ,"%d",iRet);
			DisplayTestInfo("iRet = ",szbuf);
			DisplayTestInfo("5G szResult:",szResult);
			switch (iRet)
			{
			case ERR_OK:
				printf("%s\n",szResult);
				strcat_s(g_MyDAS,BUF_LENGH,szResult);
				break;
			case ERR_NO_CONFIG_FILE:
				printf("No 5gconfig.txt under exe path!\n");
				break;
			case ERR_NO_TEST_ITEM:
				printf("No test items in 5gconfig.txt!\n");
				break;
			default:
				break;
			}
			DisplayTestInfo("Detail test data 5G Bat test collected=" , g_MyDAS);	
	}		
	/*End*/

	return iTestResult;
}

int CRunInfo::GetItemPower( char *TestItems )
{
	char *pFind_1;
	char *pFind_2;
	char szPowerValuse[1024] = "";
	int PowerResult =-999 ;

	if(strstr(TestItems , "#") == NULL)
	{
		PowerResult = -900;
	}

	pFind_1 = strstr(TestItems, "#");
	if(strstr(TestItems , "TX_VERIFY") != NULL)
	{
		pFind_2 = strstr(TestItems, "$");
	}
	else if(strstr(TestItems , "RX_VERIFY") != NULL)
	{
		pFind_2 = strstr(TestItems, "&");
	}
	else
	{
		//do nothing
	}	

	strncpy_s(szPowerValuse, 1024, (pFind_1+1), (pFind_2-pFind_1-1));	
	PowerResult = _tstoi(szPowerValuse);

	return PowerResult;

}

BOOL bGetLastWriteTime(HANDLE hFile, char* lpszString)
{

	 FILETIME ftCreate, ftAccess, ftWrite;
	 SYSTEMTIME stUTC, stLocal;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return FALSE;
	FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	sprintf(lpszString, TEXT("%02d/%02d/%d%02d:%02d%02d"),
		stLocal.wYear, stLocal.wMonth, stLocal.wDay,
		stLocal.wHour, stLocal.wMinute, stLocal.wSecond);	

	return TRUE;
}

BOOL bCompareFileTime()
{



	return TRUE;

}