#include "StdAfx.h"
#include "ClockWnd.h"

#define SETTING_XML_FILE	_T("setting.xml")

#define VERIFY(x)



CClockWnd::CClockWnd( void )
: m_pDialImage(NULL)
, m_pHourNeedle(NULL)
, m_pMinuteNeedle(NULL)
, m_pSecondNeedle(NULL)
, m_hWnd(NULL)
, m_nWidth(0)
, m_nHeight(0)
{
}

CClockWnd::~CClockWnd(void)
{
	Release();
}

VOID CClockWnd::Release(void)
{
	SAFE_DELETE(m_pDialImage);
	SAFE_DELETE(m_pHourNeedle);
	SAFE_DELETE(m_pMinuteNeedle);
	SAFE_DELETE(m_pSecondNeedle);
}

void CClockWnd::Attach(HWND hWnd)
{
	VERIFY( IsWindow(hWnd ) );
	m_hWnd = hWnd;
}

//-------------------------------------------------------------------------
// Function Name    :LoadTheme
// Parameter(s)     :LPCTSTR lpszThemeName	The skin name
// Return           :
// Create			:2008-8-18 19:55
// Memo             :load the skin
//-------------------------------------------------------------------------
BOOL CClockWnd::LoadTheme(LPCTSTR lpszThemeName)
{
	VERIFY( IsWindow(m_hWnd) );

	// combine path
	TCHAR tszBuf[MAX_PATH] = {'\0'};
	::GetModuleFileName( NULL, tszBuf, MAX_PATH);
	CString strTemp(tszBuf);
	CString strDirectory;
	strDirectory.Format( _T("%sTheme\\%s\\")
		, strTemp.Mid( 0, strTemp.ReverseFind('\\') + 1)
		, lpszThemeName
		);

	//////////////////////////////////////////////////////////////////////////
	// read the configuration file and verify the file existing
	if( !CXml::IsFileExist( strDirectory + SETTING_XML_FILE ) )
		return FALSE;

	CXml xml;	
	if( !xml.Open(strDirectory + SETTING_XML_FILE) )
		return FALSE;

	CXmlNode node = xml.SelectSingleNode( _T("/skin/dial/filename") );
	if( node.IsNull() )
		return FALSE;
	CString strBgFile = strDirectory + node.GetValue();
	if( !CXml::IsFileExist( strBgFile ) )
		return FALSE;

	node = xml.SelectSingleNode( _T("/skin/hourNeedle/filename") );
	if( node.IsNull() )
		return FALSE;
	CString strHourNeedleFile = strDirectory + node.GetValue();
	if( !CXml::IsFileExist( strHourNeedleFile ) )
		return FALSE;

	node = xml.SelectSingleNode( _T("/skin/minuteNeedle/filename") );
	if( node.IsNull() )
		return FALSE;
	CString strMinuteIndicatorFile = strDirectory + node.GetValue();
	if( !CXml::IsFileExist( strMinuteIndicatorFile ) )
		return FALSE;

	node = xml.SelectSingleNode( _T("/skin/secondNeedle/filename") );
	if( node.IsNull() )
		return FALSE;
	CString strSecondIndicatorFile = strDirectory + node.GetValue();
	if( !CXml::IsFileExist( strSecondIndicatorFile ) )
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	// Start to load the image file
	Release();

	// Dial
	m_pDialImage = this->LoadImage(strBgFile);
	m_nWidth = m_pDialImage->GetWidth();
	m_nHeight = m_pDialImage->GetHeight();
	m_nDialCenterX = xml.SelectSingleNode(_T("/skin/dial/centerX"))->GetValue(0);
	m_nDialCenterY = xml.SelectSingleNode(_T("/skin/dial/centerY"))->GetValue(0);
	::SetWindowPos( m_hWnd
		, NULL
		, 0
		, 0
		, m_nWidth
		, m_nHeight
		, SWP_NOZORDER | SWP_NOMOVE
		);

	// Hour needle
	m_pHourNeedle = this->LoadImage(strHourNeedleFile);
	m_nHourNeedleCenterX = xml.SelectSingleNode(_T("/skin/hourNeedle/centerX"))->GetValue(0);
	m_nHourNeedleCenterY = xml.SelectSingleNode(_T("/skin/hourNeedle/centerY"))->GetValue(0);


	// Minute Needle
	m_pMinuteNeedle = this->LoadImage(strMinuteIndicatorFile);
	m_nMinuteNeedleCenterX = xml.SelectSingleNode(_T("/skin/minuteNeedle/centerX"))->GetValue(0);
	m_nMinuteNeedleCenterY = xml.SelectSingleNode(_T("/skin/minuteNeedle/centerY"))->GetValue(0);

	// Second Needle
	m_pSecondNeedle = this->LoadImage(strSecondIndicatorFile);
	m_nSecondNeedleCenterX = xml.SelectSingleNode(_T("/skin/secondNeedle/centerX"))->GetValue(0);
	m_nSecondNeedleCenterY = xml.SelectSingleNode(_T("/skin/secondNeedle/centerY"))->GetValue(0);

	Render();

	return TRUE;
}


// Load the PNG file
Image * CClockWnd::LoadImage(LPCTSTR lpszFile)
{
	HANDLE hFile = ::CreateFile( lpszFile
		, GENERIC_READ | GENERIC_WRITE
		, FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE
		, NULL
		, OPEN_EXISTING
		, NULL
		, NULL
		);
	VERIFY(hFile);

	DWORD dwSize = ::GetFileSize( hFile, NULL);

	HGLOBAL hMem = ::GlobalAlloc(GMEM_FIXED, dwSize);
	LPBYTE pMem = (LPBYTE)::GlobalLock(hMem);
	DWORD dwRead = 0;
	::ReadFile( hFile, pMem, dwSize, &dwRead, NULL);
	::CloseHandle(hFile);
	VERIFY( dwSize != 0 );
	VERIFY( dwRead == dwSize );
	
	IStream * pStream = NULL;
	::CreateStreamOnHGlobal( hMem, FALSE, &pStream);

	Image * pImage = Image::FromStream(pStream);

	::GlobalUnlock(hMem);
	pStream->Release();


	return pImage;
}


//-------------------------------------------------------------------------
// Function Name    :Render
// Parameter(s)     :
// Return           :
// Create			:2008-8-18 20:21
// Memo             :Draw with GDI+
//-------------------------------------------------------------------------
void CClockWnd::Render(void)
{
	if( !IsWindow(m_hWnd) ||
		m_pDialImage == NULL ||
		m_pHourNeedle == NULL )
		return;

	RECT rc;
	::GetWindowRect( m_hWnd, &rc);
	POINT ptSrc = { 0, 0};
	POINT ptWinPos = { rc.left, rc.top};
	SIZE szWin = { m_nWidth, m_nHeight };
	BLENDFUNCTION stBlend = { AC_SRC_OVER, 0, 0xFF, AC_SRC_ALPHA };


	HDC hDC = ::GetDC(m_hWnd);
	HDC hdcMemory = ::CreateCompatibleDC(hDC);

	BITMAPINFOHEADER bmih = { 0 };   
	int nBytesPerLine = ((m_nWidth * 32 + 31) & (~31)) >> 3;
	bmih.biSize = sizeof(BITMAPINFOHEADER);   
	bmih.biWidth = m_nWidth;   
	bmih.biHeight = m_nHeight;   
	bmih.biPlanes = 1;   
	bmih.biBitCount = 32;   
	bmih.biCompression = BI_RGB;   
	bmih.biClrUsed = 0;   
	bmih.biSizeImage = nBytesPerLine * m_nHeight;   

	PVOID pvBits = NULL;   
	HBITMAP hbmpMem = ::CreateDIBSection(NULL, (PBITMAPINFO)&bmih, DIB_RGB_COLORS, &pvBits, NULL, 0);
	ASSERT(hbmpMem != NULL);
	memset( pvBits, 0, m_nWidth * 4 * m_nHeight);
	if(hbmpMem)   
	{   
		HGDIOBJ hbmpOld = ::SelectObject( hdcMemory, hbmpMem); 
		Graphics graph(hdcMemory);

		graph.SetPageScale(1.0);
		graph.SetPageUnit(UnitPixel);
		graph.SetSmoothingMode(SmoothingModeNone);

		// Draw the dial
		graph.DrawImage( m_pDialImage, 0, 0, m_nWidth, m_nHeight);

		COleDateTime dtNow = COleDateTime::GetCurrentTime();
		REAL rAngel = (REAL)(((dtNow.GetHour() % 12) * 60 + dtNow.GetMinute()) / 60.0 * 30.0);
		this->DrawNeedle( graph
			, m_pHourNeedle
			, m_nHourNeedleCenterX
			, m_nHourNeedleCenterY
			, rAngel
			);

		rAngel = (REAL)((dtNow.GetMinute() * 60 + dtNow.GetSecond()) / 60.0 * 6.0);
		this->DrawNeedle( graph
			, m_pMinuteNeedle
			, m_nMinuteNeedleCenterX
			, m_nMinuteNeedleCenterY
			, rAngel
			);

		rAngel = (REAL)(dtNow.GetSecond() * 6.0);
		this->DrawNeedle( graph
			, m_pSecondNeedle
			, m_nSecondNeedleCenterX
			, m_nSecondNeedleCenterY
			, rAngel
			);

		::UpdateLayeredWindow( m_hWnd
			, hDC
			, &ptWinPos
			, &szWin
			, hdcMemory
			, &ptSrc
			, 0
			, &stBlend
			, ULW_ALPHA
			);

		graph.ReleaseHDC(hdcMemory);
		::SelectObject( hdcMemory, hbmpOld);   
		::DeleteObject(hbmpMem); 
	}


	::DeleteDC(hdcMemory);
	::DeleteDC(hDC);

}


//  draw needles 
void CClockWnd::DrawNeedle( Graphics & graph, Image *& pNeedle, INT nNeedleCenterX, INT nNeedleCenterY, REAL rAngle)
{
	int nX = m_nDialCenterX - nNeedleCenterX;
	int nY = m_nDialCenterY - nNeedleCenterY;
	
	
	graph.RotateTransform( rAngle, MatrixOrderAppend);
	graph.TranslateTransform( m_nDialCenterX, m_nDialCenterY, MatrixOrderAppend);

	graph.DrawImage( pNeedle
		, -nNeedleCenterX
		, -nNeedleCenterY
		, pNeedle->GetWidth()
		, pNeedle->GetHeight()
		);

	graph.ResetTransform();
		
}
