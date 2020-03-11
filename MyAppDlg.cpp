
// MyAppDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "MyApp.h"
#include "MyAppDlg.h"
#include "afxdialogex.h"
#include "MyBusDll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
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


// CMyAppDlg ��ȭ ����



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


// CMyAppDlg �޽��� ó����

BOOL CMyAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_bConnected = FALSE;
	m_hComm = NULL;

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
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

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CMyAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CMyAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMyAppDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void CMyAppDlg::OnBnClickedBtnCommConfig()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CallCommSetDlg();
}


void CMyAppDlg::OnBnClickedBtnCommOpenClose()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
  // TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�
  CString str;			// ��ſ��� ����ǥ�ÿ� ���ڿ�
  char cParity;			// �и�Ƽ ǥ�ÿ� ���ں���
  char* cStopBits;	// Stop bit ǥ�ÿ� ���ڿ� ������
  CommConfigData *pCommCfg = new CommConfigData;

// �����Ʈ ���Ῡ�� Ȯ��
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
       GetDlgItem(IDC_BTN_COMM_OPEN_CLOSE)->SetWindowText(_T("��Ʈ�ݱ�"));
       GetDlgItem(IDC_BTN_COMM_CONFIG)->EnableWindow(false);

	   _MyBusFrame Query;

	   if (m_bConnected == FALSE) return;

	   Query.ui8Func = FC_WOR;                                 //0x06
	   Query.ui8Addr = VC_ADC_EVENT_MASK;    //0x48
	   Query.ui8DataH = 0;


		   //ADC0 ~ ADC2�� ���� �̺�Ʈ ��Ȱ��ȭ
		   Query.ui8DataL = 0;
	 

	   MyBusSendFrame(&Query);



	   MyBusLED_OnOff(0xff);

    } // if
   else{
       str.Format(_T("Open Error COM%d"),pCommCfg->PortNum);
       GetDlgItem(IDC_COMM_STATUS)->SetWindowText(str);
    }
  }// if
  // �����Ʈ��������
 else{
	  CString strText = _T("0");
	  m_Editout.SetWindowTextW(strText);
	 MyBusLED_OnOff(0x00);
	 CommClose();
     m_bConnected = FALSE;
     str.Format(_T(" Closed "));
     GetDlgItem(IDC_COMM_STATUS)->SetWindowText(str);
     GetDlgItem(IDC_BTN_COMM_OPEN_CLOSE)->SetWindowText(_T("��Ʈ����"));
     GetDlgItem(IDC_BTN_COMM_CONFIG)->EnableWindow(true);

  }
  delete pCommCfg;
}


void CMyAppDlg::OnClose()
{
	CString strText = _T("0");
	m_Editout.SetWindowTextW(strText);
	MyBusLED_OnOff(0x00);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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

//LED ����ϴ� �κ�

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

//���� ���� ����
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
