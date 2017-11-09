// ServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Server.h"
#include "ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CServerDlg dialog

CServerDlg::CServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CServerDlg::IDD, pParent)
	, m_msgString(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CServerDlg::~CServerDlg()
{
	if (pSock)
		delete[]pSock;
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_BOXCHAT, m_msgString);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_LISTEN, &CServerDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_CANCEL, &CServerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::Split(CString src, CString des[3])
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

char* CServerDlg::ConvertToChar(const CString &s)
{
	int nSize = s.GetLength();
	char *pAnsiString = new char[nSize + 1];
	memset(pAnsiString, 0, nSize + 1);
	wcstombs(pAnsiString, s, nSize + 1);
	return pAnsiString;
}

void CServerDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete sendBuff;
}

int CServerDlg::mRecv(SOCKET sk, CString &Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sk, (char*)&buffLength, sizeof(int), 0);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;

	if (Command.GetLength() == 0)
		return -1;
	return 0;
}

void CServerDlg::OnBnClickedListen()
{
	// TODO: Add your control notification handler code here

	UpdateData();
	sockServer = socket(AF_INET, SOCK_STREAM, 0);	 // tao socket cho server
	serverAdd.sin_family = AF_INET;					// giao thuc cho socket TCP/IP hay UDP
	serverAdd.sin_port = htons(PORT);				// dang ky socket cho server
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);	// dia chi IP
	bind(sockServer, (SOCKADDR*)&serverAdd, sizeof(serverAdd));
	listen(sockServer, 5);
	int err = WSAAsyncSelect(sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");
	GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
	number_Socket = 0;
	pSock = new SockName[5];

	srand((unsigned)time(NULL));
	R = rand() % 10 + 1;
}

void CServerDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
	{
		pSock[number_Socket].sockClient = accept(wParam, NULL, NULL);
		GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
		break;
	}
	case FD_READ:	// co lenh truyen du lieu cho cho server
	{
		int post = -1, dpos = -1;

		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		CString temp;
		if (mRecv(wParam, temp) < 0)
			break;
		Split(temp, strResult);
		int flag = _ttoi(strResult[0]);
		int playflag = _ttoi(strResult[1]);
		char* tem = ConvertToChar(strResult[2]);
		switch (flag)
		{
		case 1://Login, check if 3 client is connected, start game
		{
			char* tem = ConvertToChar(strResult[1]);
			int t = 0;
			if (number_Socket > 0)
			{
				for (int i = 0; i < number_Socket; i++)
				{
					if (strcmp(tem, pSock[i].Name) == 0)//Trung ten user
					{
						t = 1;
						break;
					}
				}
			}

			if (t == 0 && number_Socket < 1)//< 3)
			{
				// Client: "Dang nhap thanh cong"
				// Server: "[Username] login"
				strcpy(pSock[number_Socket].Name, tem);
				Command = _T("1\r\n1\r\n");
				m_msgString += strResult[1] + _T(" login\r\n");
				mSend(wParam, Command);
				number_Socket++;

				// check if number_client == 3 ? startGame : wait for
				if (number_Socket < 1)//3)
				{
					// send to client messenge "Waiting for the other player"
					Command = _T("2\r\n");
					Command += _T("1\r\n");
					mSend(wParam, Command);
				}
				else
				{
					// Client: "Game start"
					// Server: "Game start"
					game = CGame(number_Socket, R, RANDOM);
					game.startGame();
					Command = _T("2\r\n");
					Command += _T("2\r\n");
					Command += CString(game.getStatus(number_Socket - 1).c_str());
					m_msgString += _T("Game start\r\n");
					for (size_t i = 0; i < number_Socket; i++)
						mSend(pSock[i].sockClient, Command);
				}
				
				UpdateData(FALSE);
			}
			else
			{
				// Client: "Dang nhap that bai"
				Command = _T("1\r\n0\r\n");
				mSend(wParam, Command);
			}
			UpdateData(FALSE);
			break;
		}

		case 2:	//Process
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}
			switch (playflag)
			{
			//case 1:	// Waiting for the other player
			//case 2:	// Start game
			case 3:
			{
				if (number_Socket < 1)//< 3)
					Command = "3\r\n0\r\n";
				else
				{
					Command = "3\r\n1\r\n";
					game.giveup(post);
				}
				mSend(wParam, Command);
				break;
			}
			default:
				break;
			}
		
			mSend(wParam, Command);
			UpdateData(FALSE);

			break;
		}

		case 3:	//Moving Disk
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

			bool isOK = game.move(post, playflag, tem);

			if (isOK && !game.isGiveup(post))
			{
				Command = "3\r\n";
				Command += "1\r\n";
				Command += CString(game.getStatus(post).c_str());
			}
			else
			{
				Command = "3\r\n";
				Command += "0\r\n";
				//Command += CString(game.getStatus(post).c_str());
			}
			mSend(wParam, Command);

			break;
		}
		case 4: //Logout
		{
			int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}

			m_msgString += pSock[post].Name;
			m_msgString += " logout\r\n";
			closesocket(wParam);
			for (int j = post; j < number_Socket; j++)
			{
				pSock[post].sockClient = pSock[post + 1].sockClient;
				strcpy(pSock[post].Name, pSock[post + 1].Name);
			}
			number_Socket--;
			UpdateData(FALSE);
			break;
		}
		}
		break;
	}

	case FD_CLOSE:
	{
		UpdateData();
		int post = -1;
		for (int i = 0; i < number_Socket; i++)
		{
			if (pSock[i].sockClient == wParam)
			{
				if (i < number_Socket)
					post = i;
			}
		}

		m_msgString += pSock[post].Name;
		m_msgString += " logout\r\n";
		closesocket(wParam);
		for (int j = post; j < number_Socket; j++)
		{
			pSock[post].sockClient = pSock[post + 1].sockClient;
			strcpy(pSock[post].Name, pSock[post + 1].Name);
		}
		number_Socket--;
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}