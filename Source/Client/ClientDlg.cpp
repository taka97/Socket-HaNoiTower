// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClientDlg dialog

CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientDlg::IDD, pParent)
	, m_msgString(_T(""))
	, IP(_T(""))
	, m_userName(_T(""))
	, disk(_T(""))
	, toCol(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MSGBOX, m_msgString);
	DDX_Text(pDX, IDC_IP, IP);
	DDX_Text(pDX, IDC_USER, m_userName);
	DDX_Text(pDX, IDC_DISK, disk);
	DDX_Text(pDX, IDC_TOCOL, toCol);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LOGIN, &CClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_LOGOUT, &CClientDlg::OnBnClickedLogout)
	ON_BN_CLICKED(IDC_GIVEUP, &CClientDlg::OnBnClickedGiveup)
	ON_BN_CLICKED(IDC_MOVE, &CClientDlg::OnBnClickedMove)
END_MESSAGE_MAP()


// CClientDlg message handlers

BOOL CClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	IP = "127.0.0.1";
	GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
	//GetDlgItem(IDC_GIVEUP)->EnableWindow(FALSE);
	//GetDlgItem(IDC_MOVE)->EnableWindow(FALSE);
	UpdateData(FALSE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientDlg::Split(CString src, CString des[5])
{
	int p1, p2, p3, p4, p5;

	// type of command
	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	// agrv 1
	p2 = src.Find(_T("\r\n"), p1 + 1);
	des[1] = src.Mid(p1 + 2, p2 - (p1 + 2));

	// agrv[2]
	p3 = src.Find(_T("\r\n"), p2 + 1);
	des[2] = src.Mid(p2 + 2, p3 - (p2 + 2));

	// agrv[3]
	p4 = src.Find(_T("\r\n"), p3 + 1);
	des[3] = src.Mid(p3 + 2, p4 - (p3 + 2));

	// agrv[4]
	p5 = src.Find(_T("\r\n"), p4 + 1);
	des[4] = src.Mid(p4 + 2, p5 - (p4 + 2));
}

char* CClientDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CClientDlg::mSend(CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sClient, (char*)&Len, sizeof(Len), 0);
	send(sClient, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CClientDlg::mRecv(CString &Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sClient, (char*)&buffLength, sizeof(int), 0);
	recv(sClient, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

LRESULT CClientDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{

	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_READ:
	{
		CString temp;
		if (mRecv(temp) < 0)
			break;

		Split(temp, strResult);
		int flag1 = _ttoi(strResult[0]);
		int flag2 = _ttoi(strResult[1]);
		int flag3 = _ttoi(strResult[2]);
		char *tmp = NULL;

		switch (flag1)
		{
		case 1:
		{
			if (flag2 == 1)
			{
				m_msgString += _T("Dang nhap thanh cong\r\n");
				GetDlgItem(IDC_LOGOUT)->EnableWindow(TRUE);
				GetDlgItem(IDC_LOGIN)->EnableWindow(FALSE);
			}
			else
				m_msgString += _T("Dang nhap that bai\r\n");

			UpdateData(FALSE);
			break;
		}

		case 2:
		{
			switch (flag2)
			{
			case 1:
				m_msgString += "Dang cho nguoi choi khac\r\n";
				break;
			case 2:
				m_msgString = "---- HA NOI TOWER GAME ----\r\n";
				m_msgString += strResult[2] + _T("\r\n");
				m_msgString += strResult[3] + _T("\r\n");
				m_msgString += strResult[4] + _T("\r\n");
				break;
			case 3:
				if (flag3 == 1)
					m_msgString += "Ban da bo cuoc\r\n";
				else
					m_msgString += "Ban khong the bo cuoc\r\n";
				break;
			}
			UpdateData(FALSE);
			break;
		}
		case 3:
		{
			if (flag2 == 0)
				m_msgString += "Di chuyen that bai\r\n";
			else // flag2 == 1
			{
				m_msgString = "---- HA NOI TOWER GAME ----\r\n";
				m_msgString += strResult[2] + _T("\r\n");
				m_msgString += strResult[3] + _T("\r\n");
				m_msgString += strResult[4] + _T("\r\n");
			}
			UpdateData(FALSE);
			break;
		}
		case 4:
		{
			std::string winner = ConvertToChar(strResult[2].Mid(0, strResult[2].Find(_T("\t"), 0)));
			m_msgString += strResult[2] + _T("\r\n");
			m_msgString += strResult[3] + _T("\r\n");
			m_msgString += strResult[4] + _T("\r\n");
			m_msgString += "Winner: ";
			m_msgString += winner.c_str();
			m_msgString += "\r\n";

			UpdateData(FALSE);
			break;
		}
		}
		break;
	}
	case FD_CLOSE:
	{
		closesocket(sClient);
		m_msgString += _T("Server da dong ket noi\r\n");
		GetDlgItem(IDC_LOGOUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_LOGIN)->EnableWindow(TRUE);
		UpdateData(FALSE);
		break;
	}
	}
	return 0;
}

void CClientDlg::OnBnClickedLogin()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if (IP == "")
	{
		MessageBox(_T("Vui long nhap IP may chu"));
		return;
	}

	if (m_userName == "")
	{
		MessageBox(_T("Vui long nhap ten user"));
		return;
	}

	sClient = socket(AF_INET, SOCK_STREAM, 0);
	hostent* host = NULL;
	if (sClient == INVALID_SOCKET)
	{
		MessageBox(_T("socket() failed"), _T("ERROR"), 0);
		return;
	}

	servAdd.sin_family = AF_INET;
	servAdd.sin_port = htons(PORT);

	char* cIP = ConvertToChar(IP);

	servAdd.sin_addr.s_addr = inet_addr(cIP);

	CStringA cpy_IP(IP);

	if (servAdd.sin_addr.s_addr == INADDR_NONE)
	{
		host = (gethostbyname(cpy_IP));
		if (host == NULL)
		{
			MessageBox(_T("Khong the ket noi den server."), _T("ERROR"), 0);
		}
		CopyMemory(&servAdd.sin_addr, host->h_addr_list[0],
			host->h_length);
		return;
	}

	int err = connect(sClient, (struct sockaddr*)&servAdd, sizeof(servAdd));
	if (err == SOCKET_ERROR) {
		MessageBox(_T("Ket noi that bai"), _T("ERROR"), 0);
		return;
	}

	Command = _T("1\r\n");
	Command += m_userName + _T("\r\n");

	mSend(Command);

	WSAAsyncSelect(sClient, m_hWnd, WM_SOCKET, FD_READ | FD_CLOSE);
	UpdateData(FALSE);
}

void CClientDlg::OnBnClickedLogout()
{
	// TODO: Add your control notification handler code here
	INT_PTR i = MessageBox(_T("Ban muon logout?"), _T("Confirm"), MB_OKCANCEL);
	if (i == IDCANCEL)
		return;
	Command = _T("4\r\n");
	mSend(Command);
}

void CClientDlg::OnBnClickedGiveup()
{
	// TODO: Add your control notification handler code here
	Command = "2\r\n3\r\n";
	mSend(Command);
}

void CClientDlg::OnBnClickedMove()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (disk == "")
	{
		MessageBox(_T("Vui long nhap so dia"));
		return;
	}

	if (toCol == "")
	{
		MessageBox(_T("Vui long nhap vi tri cot ban muon chuyen den"));
		return;
	}

	if (!checkCol(toCol))
	{
		MessageBox(_T("Vi tri cot khong hop le!!!\r\nVi tri cot chi co the la A B C"));
		return;
	}

	Command = "3\r\n";
	Command += disk;
	Command += "\r\n";
	Command += toCol;
	Command += "\r\n";
	mSend(Command);

	disk = "";
	toCol = "";
	GotoDlgCtrl(GetDlgItem(IDC_DISK));
	UpdateData(FALSE);
}

bool CClientDlg::checkCol(CString &toCol)
{
	std::string str(ConvertToChar(toCol));
	if (str == "A" || str == "B" || str == "C")
		return true;
	if (str == "a" || str == "b" || str == "c")
	{
		str[0] -= 'A';
		toCol = str.c_str();
		return true;
	}
	
	return false;
}
