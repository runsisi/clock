#pragma once

class CClockWnd
{
public:
	CClockWnd(void);
	~CClockWnd(void);

	BOOL LoadTheme(LPCTSTR lpszThemeName);
	Image * LoadImage(LPCTSTR lpszFile);
	VOID Attach(HWND hWnd);
	VOID Render(void);
	VOID Release(void);

public:
	Image	*	m_pDialImage;
	Image	*	m_pHourNeedle;
	Image	*	m_pMinuteNeedle;
	Image	*	m_pSecondNeedle;

	HWND		m_hWnd;
	INT			m_nWidth;
	INT			m_nHeight;

	INT			m_nDialCenterX;
	INT			m_nDialCenterY;
	INT			m_nHourNeedleCenterX;
	INT			m_nHourNeedleCenterY;
	INT			m_nMinuteNeedleCenterX;
	INT			m_nMinuteNeedleCenterY;
	INT			m_nSecondNeedleCenterX;
	INT			m_nSecondNeedleCenterY;


	void DrawNeedle( Graphics & graph, Image *& pNeedle, INT nNeedleCenterX, INT nNeedleCenterY, REAL rAngle);
};
