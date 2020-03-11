
// MyAppDlg.h : ��� ����
//

#pragma once


// CMyAppDlg ��ȭ ����
class CMyAppDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CMyAppDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MYAPP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	BOOL m_bConnected;
	HANDLE m_hComm;
	afx_msg void OnBnClickedBtnCommConfig();
	afx_msg void OnBnClickedBtnCommOpenClose();
	afx_msg void OnClose();
	UINT m_ui32ADCInterval;
	afx_msg void OnBnClickedBtnAdcInterval();
	afx_msg void OnBnClickedBtnGetAdc0();
	afx_msg  LRESULT OnMyBusRecvFrame(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedAdcEventEnable();
	afx_msg LRESULT OnMyBusADCValue(WPARAM wParam, LPARAM lParam);
	void UpdateFNDValue(UINT ui32Value);
	afx_msg void OnEnChangeAdc0();
	UINT m_ui32FND_Value;
	afx_msg void OnBnClickedSceEnable();
	afx_msg LRESULT OnMyBusSwitchUpdate(WPARAM  wParam, LPARAM  lParam);
	void LED_Control(UINT8 ui8SwitchStatus);
	afx_msg void OnBnClickedCheck2();
	CEdit m_Editout;
};
