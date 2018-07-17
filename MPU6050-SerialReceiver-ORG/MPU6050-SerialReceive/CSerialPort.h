/*
**	FILENAME			CSerialPort.h
**
**	PURPOSE				This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	12-11-1997
**
**	AUTHOR				Remon Spekreijse
**
**
************************************************************************************/

#ifndef __CSERIALPORT_H__
#define __CSERIALPORT_H__

#include "stdio.h"
#include "tchar.h"

#include "Windows.h"

#include <string>
#include <list>

struct serialPortBuffer
{
	int len;
	PBYTE buffer;
};

#ifndef WM_COMM_MSG_BASE 
#define WM_COMM_MSG_BASE		    WM_USER + 109		//!< ��Ϣ��ŵĻ���  
#endif

#define WM_COMM_BREAK_DETECTED		WM_COMM_MSG_BASE + 1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_COMM_MSG_BASE + 2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_COMM_MSG_BASE + 3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_COMM_MSG_BASE + 4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_COMM_MSG_BASE + 5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_COMM_MSG_BASE + 6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_COMM_MSG_BASE + 7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_COMM_MSG_BASE + 8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_COMM_MSG_BASE + 9	// The last character in the output buffer was sent.  
#define WM_COMM_RXSTR               WM_COMM_MSG_BASE + 10   // Receive string

#define MaxSerialPortNum 200    ///��Ч�Ĵ����ܸ��������Ǵ��ڵĺ� //add by itas109 2014-01-09
#define IsReceiveString  1      //���ú��ַ�ʽ���գ�ReceiveString 1���ַ������գ���Ӧ��Ӧ����ΪWm_SerialPort_RXSTR����ReceiveString 0һ���ַ�һ���ַ����գ���Ӧ��Ӧ����ΪWm_SerialPort_RXCHAR��
#define _SEND_DATA_WITH_SIGSLOT //ʹ��sigslot������յ�����

#ifdef _SEND_DATA_WITH_SIGSLOT
	#include "sigslot.h"
#else
	//will remove on V4.0
	struct serialPortInfo
	{
		UINT portNr;//���ں�
		DWORD bytesRead;//��ȡ���ֽ���
	};
#endif

	class CSerialPort
	{
	public:
		// contruction and destruction
		CSerialPort();
		virtual ~CSerialPort();

		// port initialisation		
		// UINT stopsbits = ONESTOPBIT   stop is index 0 = 1 1=1.5 2=2 
		// �мǣ�stopsbits = 1������ֹͣλΪ1��
		// by itas109 20160506
		BOOL		InitPort(HWND pPortOwner, UINT portnr = 1, UINT baud = 9600,
			TCHAR parity = _T('N'), UINT databits = 8, UINT stopsbits = ONESTOPBIT,
			DWORD dwCommEvents = EV_RXCHAR | EV_CTS, UINT nBufferSize = 512,

			DWORD ReadIntervalTimeout = 1000,
			DWORD ReadTotalTimeoutMultiplier = 1000,
			DWORD ReadTotalTimeoutConstant = 1000,
			DWORD WriteTotalTimeoutMultiplier = 1000,
			DWORD WriteTotalTimeoutConstant = 1000);

		// start/stop comm watching
		///���ƴ��ڼ����߳�
		BOOL		 StartMonitoring();//��ʼ����
		BOOL		 ResumeMonitoring();//�ָ�����
		BOOL		 SuspendMonitoring();//�������
		BOOL         IsThreadSuspend(HANDLE hThread);//�ж��߳��Ƿ���� //add by itas109 2016-06-29

		DWORD		 GetWriteBufferSize();///��ȡд�����С
		DWORD		 GetCommEvents();///��ȡ�¼�
		DCB			 GetDCB();///��ȡDCB

		///д���ݵ�����
		void		WriteToPort(char* string, size_t n); // add by mrlong 2007-12-25
		void		WriteToPort(PBYTE Buffer, size_t n);// add by mrlong
		void		ClosePort();					 // add by mrlong 2007-12-2  
		BOOL		IsOpened();

#ifdef _SEND_DATA_WITH_SIGSLOT
		//sigslot
		static signal3<unsigned char*, int, int> sendMessageSignal;
#endif
		
		std::string GetVersion();

	protected:
		// protected memberfunctions
		void		ProcessErrorMessage(TCHAR* ErrorText);///������
		static DWORD WINAPI CommThread(LPVOID pParam);///�̺߳���
		static void	ReceiveChar(CSerialPort* port);
		static void ReceiveStr(CSerialPort* port); //add by itas109 2016-06-22
		static void	WriteChar(CSerialPort* port);

	private:
		// thread
		HANDLE			    m_Thread;
		BOOL                m_bIsSuspened;///thread�����߳��Ƿ����

		// synchronisation objects
		CRITICAL_SECTION	m_csCommunicationSync;///�ٽ���Դ
		BOOL				m_bThreadAlive;///�����߳����б�־

		// handles
		HANDLE				m_hShutdownEvent;  //stop�������¼�
		HANDLE				m_hComm;		   // ���ھ�� 
		HANDLE				m_hWriteEvent;	 // write

		// Event array. 
		// One element is used for each event. There are two event handles for each port.
		// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
		// There is a general shutdown when the port is closed. 
		///�¼����飬����һ��д�¼��������¼����ر��¼�
		///һ��Ԫ������һ���¼����������¼��̴߳����˿ڡ�
		///д�¼��ͽ����ַ��¼�λ��overlapped�ṹ�壨m_ov.hEvent����
		///���˿ڹر�ʱ����һ��ͨ�õĹرա�
		HANDLE				m_hEventArray[3];

		// structures
		OVERLAPPED			m_ov;///�첽I/O
		COMMTIMEOUTS		m_SerialPortTimeouts;///��ʱ����
		DCB					m_dcb;///�豸���ƿ�

		// owner window
		HWND				m_pOwner;

		// misc
		UINT				m_nPortNr;		///���ں�
		PBYTE				m_szWriteBuffer;///д������
		std::list<serialPortBuffer> m_bufferList;
		serialPortBuffer m_bufferStruct;
		DWORD				m_dwCommEvents;
		DWORD				m_nWriteBufferSize;///д�����С

		size_t				m_nWriteSize;//д���ֽ��� //add by mrlong 2007-12-25
	};

	class CSerialPortInfo
	{
	public:
		CSerialPortInfo();
		~CSerialPortInfo();

		static std::list<std::string> availablePorts();
	};

#endif __CSERIALPORT_H__