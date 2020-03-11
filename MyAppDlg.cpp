
// MyAppDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "MyApp.h"
#include "MyAppDlg.h"
#include "afxdialogex.h"
#include "MyBusDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyAppDlg 대화 상자



CMyAppDlg::CMyAppDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyAppDlg::IDD, pParent)
	, m_bConnected(FALSE)
	, m_ui32ADCInterval(0)
	, m_ui32FND_Value(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADC_INTERVAL, m_ui32ADCInterval);
	DDX_Text(pDX, IDC_ADC0, m_ui32FND_Value);
	DDX_Control(pDX, IDC_ADC0, m_Editout);
}

BEGIN_MESSAGE_MAP(CMyAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMyAppDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_COMM_CONFIG, &CMyAppDlg::OnBnClickedBtnCommConfig)
	ON_BN_CLICKED(IDC_BTN_COMM_OPEN_CLOSE, &CMyAppDlg::OnBnClickedBtnCommOpenClose)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_ADC_INTERVAL, &CMyAppDlg::OnBnClickedBtnAdcInterval)
	ON_MESSAGE(WM_MYBUS_RECV_FRAME, &CMyAppDlg::OnMyBusRecvFrame)
	ON_BN_CLICKED(IDC_ADC_EVENT_ENABLE, &CMyAppDlg::OnBnClickedAdcEventEnable)
	ON_MESSAGE(WM_MYBUS_ADC_VALUE, &CMyAppDlg::OnMyBusADCValue)
	ON_EN_CHANGE(IDC_ADC0, &CMyAppDlg::OnEnChangeAdc0)
	ON_BN_CLICKED(IDC_SCE_ENABLE, &CMyAppDlg::OnBnClickedSceEnable)
	ON_MESSAGE(WM_MYBUS_SWITCH_UPDATE, &CMyAppDlg::OnMyBusSwitchUpdate)


	ON_BN_CLICKED(IDC_CHECK2, &CMyAppDlg::OnBnClickedCheck2)
END_MESSAGE_MAP()


// CMyAppDlg 메시지 처리기

BOOL CMyAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_bConnected = FALSE;
	m_hComm = NULL;

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMyAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMyAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMyAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyAppDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CMyAppDlg::OnBnClickedBtnCommConfig()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CallCommSetDlg();
}


void CMyAppDlg::OnBnClickedBtnCommOpenClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다
  CString str;			// 통신연결 상태표시용 문자열
  char cParity;			// 패리티 표시용 문자변수
  char* cStopBits;	// Stop bit 표시용 문자열 포인터
  CommConfigData *pCommCfg = new CommConfigData;

// 통신포트 연결여부 확인
  if(!m_bConnected){
    GetCommCfgData(pCommCfg);
    pCommCfg->hOwner = m_hWnd;

    if((m_hComm=CommOpen(*pCommCfg)) != NULL){

       m_bConnected = TRUE;

       if(pCommCfg->Parity == EVENPARITY) cParity = 'E';
       else if(pCommCfg->Parity == ODDPARITY) cParity = 'O';
       else cParity = 'N';

       if(pCommCfg->StopBits == TWOSTOPBITS) cStopBits = "2";
       else if(pCommCfg->StopBits == ONE5STOPBITS) cStopBits = "1.5";
       else cStopBits = "1";

       str.Format(_T("COM%d,%d,%c,%d,%s"), pCommCfg->PortNum,
          pCommCfg->BaudRate,cParity,pCommCfg->ByteSize,cStopBits);

       GetDlgItem(IDC_COMM_STATUS)->SetWindowText(str);
       GetDlgItem(IDC_BTN_COMM_OPEN_CLOSE)->SetWindowText(_T("포트닫기"));
       GetDlgItem(IDC_BTN_COMM_CONFIG)->EnableWindow(false);

	   _MyBusFrame Query;

	   if (m_bConnected == FALSE) return;

	   Query.ui8Func = FC_WOR;                                 //0x06
	   Query.ui8Addr = VC_ADC_EVENT_MASK;    //0x48
	   Query.ui8DataH = 0;


		   //ADC0 ~ ADC2에 대한 이벤트 비활성화
		   Query.ui8DataL = 0;
	 

	   MyBusSendFrame(&Query);



	   MyBusLED_OnOff(0xff);

    } // if
   else{
       str.Format(_T("Open Error COM%d"),pCommCfg->PortNum);
       GetDlgItem(IDC_COMM_STATUS)->SetWindowText(str);
    }
  }// if
  // 통신포트연결해제
 else{
	  CString strText = _T("0");
	  m_Editout.SetWindowTextW(strText);
	 MyBusLED_OnOff(0x00);
	 CommClose();
     m_bConnected = FALSE;
     str.Format(_T(" Closed "));
     GetDlgItem(IDC_COMM_STATUS)->SetWindowText(str);
     GetDlgItem(IDC_BTN_COMM_OPEN_CLOSE)->SetWindowText(_T("포트열기"));
     GetDlgItem(IDC_BTN_COMM_CONFIG)->EnableWindow(true);

  }
  delete pCommCfg;
}


void CMyAppDlg::OnClose()
{
	CString strText = _T("0");
	m_Editout.SetWindowTextW(strText);
	MyBusLED_OnOff(0x00);
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(m_bConnected){
		CommClose();
	}

	CDialogEx::OnClose();
}


void CMyAppDlg::OnBnClickedBtnAdcInterval()
{
	_MyBusFrame	Query;
	if (m_bConnected == FALSE) return;
	UpdateData(TRUE);
	Query.ui8Func = FC_WOR;
	Query.ui8Addr = VC_ADC_INTERVAL;
	Query.ui8DataH = (m_ui32ADCInterval >> 8) & 0xff;  //0x00
	Query.ui8DataL = m_ui32ADCInterval & 0xff;   //0x0a
	MyBusSendFrame(&Query);
}




afx_msg  LRESULT CMyAppDlg::OnMyBusRecvFrame(WPARAM wParam, LPARAM lParam)
{
	CString str;
	UINT8 ui8Func = (lParam >> 24) & 0xff;
	UINT8 ui8Addr = (lParam >> 16) & 0xff;
	UINT16 ui16Value = lParam & 0xffff;
	str.Format(_T("%d"), ui16Value);
	if (ui8Func == FC_RIR) {
		if (ui8Addr == VC_ADC0) {
			GetDlgItem(IDC_ADC0)->SetWindowText(str);
		}
	}
	return afx_msg  LRESULT();
}


void CMyAppDlg::OnBnClickedAdcEventEnable()
{
	_MyBusFrame Query;
	if (m_bConnected == FALSE) return;
	Query.ui8Func = FC_WOR;
	Query.ui8Addr = VC_ADC_EVENT_MASK;
	Query.ui8DataH = 0;
	if (((CButton*)GetDlgItem(IDC_ADC_EVENT_ENABLE))->GetCheck() == 1) {
		Query.ui8DataL = 0x07;
	}
	else {
		Query.ui8DataL = 0;
	}
	MyBusSendFrame(&Query);
}


afx_msg LRESULT CMyAppDlg::OnMyBusADCValue(WPARAM wParam, LPARAM lParam)
{
	CString	str;
	UINT8 ui8Func = (lParam >> 24) & 0xff;
	UINT8 ui8Addr = (lParam >> 16) & 0xff;
	UINT16 ui16Value = lParam & 0xffff;
	str.Format(_T("%d"), ui16Value);
	if (ui8Func == FC_ACE) {
		if (ui8Addr == VC_ADC0) {
			GetDlgItem(IDC_ADC0)->SetWindowText(str);
		}
	}
	return afx_msg LRESULT();
}


void CMyAppDlg::UpdateFNDValue(UINT ui32Value)
{
	_MyBusFrame Query;
	if (m_bConnected == FALSE) return;
	Query.ui8Func = FC_WOR;
	Query.ui8Addr = VC_FND;
	Query.ui8DataH = (ui32Value >> 8) & 0xff;
	Query.ui8DataL = ui32Value & 0xff;
	MyBusSendFrame(&Query);
}


void CMyAppDlg::OnEnChangeAdc0()
{
	if (m_bConnected == TRUE) {
		UpdateData(true);
		UpdateFNDValue(m_ui32FND_Value);
	}
}

//LED 담당하는 부분

void CMyAppDlg::OnBnClickedSceEnable()
{
	_MyBusFrame Query;
	if (m_bConnected == FALSE) return;
	Query.ui8Func = FC_WOR;
	Query.ui8Addr = VC_PIN_EVENT_MASK; 
	Query.ui8DataH = 0;
	if (((CButton*)GetDlgItem(IDC_SCE_ENABLE))->GetCheck() == 1 && ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == 1) {
		Query.ui8DataL = 0x10; 
	}
	else {
		Query.ui8DataL = 0x00;
	}
	MyBusSendFrame(&Query);
}


afx_msg LRESULT CMyAppDlg::OnMyBusSwitchUpdate(WPARAM  wParam, LPARAM  lParam)
{
	_MyBusFrame Response;
	CClientDC dc(this);
	Response.ui8Func = (lParam >> 24) & 0xff;
	Response.ui8Addr = (lParam >> 16) & 0xff;
	Response.ui8DataH = (lParam >> 8) & 0xff;
	Response.ui8DataL = lParam & 0xff;
	LED_Control(Response.ui8DataL);
	return afx_msg LRESULT();
}


void CMyAppDlg::LED_Control(UINT8 ui8SwitchStatus)
{
	UINT8 static ui8OldStatus = 0;	
	UINT8 static ui8LEDStatus = 0;	
	if ((ui8OldStatus & (1 << 7)) && !(ui8SwitchStatus & (1 << 7)))  ui8LEDStatus ^= (0xff);
	ui8OldStatus = ui8SwitchStatus; 
	MyBusLED_OnOff(ui8LEDStatus);

}

//무인 공항 연출
void CMyAppDlg::OnBnClickedCheck2()
{
	if (m_bConnected) {
		if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck() == 1) {
			MyBusLED_OnOff(0x00);
		}
		else {
			MyBusLED_OnOff(0xff);
		}
	}

}
