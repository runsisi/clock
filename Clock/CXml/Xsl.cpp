#include "StdAfx.h"
#include "./Xsl.h"

namespace Generic
{
	CXsl::CXsl(void)
	{
	}

	CXsl::~CXsl(void)
	{
		Close();
	}

	//-------------------------------------------------------------------------
	// Function Name    :Open
	// Parameter(s)     :LPCTSTR lpszXslFilePath	XSL file path
	// Return           :BOOL
	// Create			:2008-1-16 16:09 Jerry.Wang
	// Memo             :Open the xsl file
	//-------------------------------------------------------------------------
	BOOL CXsl::Open(LPCTSTR lpszXslFilePath)
	{
		Close();

		HRESULT hr = S_OK;

		try
		{
			hr = m_pIXSLTemplate.CreateInstance( __uuidof(MSXML2::XSLTemplate) );
			if( FAILED(hr) ) { ASSERT(FALSE); return FALSE; }

			hr = m_pStyleSheet.CreateInstance( __uuidof(MSXML2::FreeThreadedDOMDocument) );
			if( FAILED(hr) ) { ASSERT(FALSE); return FALSE; }

			VARIANT_BOOL vbSuccessful = VARIANT_TRUE;
			vbSuccessful = m_pStyleSheet->load( _variant_t(lpszXslFilePath) );
			if( vbSuccessful == VARIANT_FALSE ) { ASSERT(FALSE); return FALSE; }

			hr = m_pIXSLTemplate->putref_stylesheet(m_pStyleSheet);
			if( FAILED(hr) ) { return FALSE; }

			m_pIXSLProcessor = m_pIXSLTemplate->createProcessor();
			if( m_pIXSLProcessor == NULL ) { ASSERT(FALSE); return FALSE; }

			return TRUE;
		}
		catch( _com_error e)
		{
			TRACE( _T("CXsl::Open(%s) failed:%s\n"), lpszXslFilePath, e.ErrorMessage());
			ASSERT( FALSE );
			return FALSE;
		}
	}

	//-------------------------------------------------------------------------
	// Function Name    :Close
	// Parameter(s)     :void
	// Return           :void
	// Create			:2008-1-16 16:10 Jerry.Wang
	// Memo             :Close the xsl file
	//-------------------------------------------------------------------------
	void CXsl::Close(void)
	{
		RELEASE_PTR(m_pIXSLTemplate);
		RELEASE_PTR(m_pStyleSheet);
		RELEASE_PTR(m_pIXSLProcessor);	
	}

	//-------------------------------------------------------------------------
	// Function Name    :TransformToFile
	// Parameter(s)     :CXml & objXml			the CXml instance
	//					:LPCTSTR lpszFilePath	The destination file
	// Return           :BOOL
	// Create			:2008-1-16 16:38 Jerry.Wang
	// Memo             :Transform XML + XSL To File
	//-------------------------------------------------------------------------
	BOOL CXsl::TransformToFile( CXml & objXml, LPCTSTR lpszFilePath)
	{
		HRESULT hr					= S_FALSE;
		IStream * pOutStream		= NULL;
		LPVOID pOutput				= NULL;
		VARIANT_BOOL vbSuccessful	= VARIANT_TRUE;

		try
		{
			::CreateStreamOnHGlobal( NULL, TRUE, &pOutStream);
			m_pIXSLProcessor->put_output(_variant_t(pOutStream));

			hr = m_pIXSLProcessor->put_input( _variant_t((IUnknown*)objXml.m_pDoc) );
			if( FAILED(hr) ) { ASSERT(FALSE); return FALSE; }

			vbSuccessful = m_pIXSLProcessor->transform();
			if( vbSuccessful == VARIANT_FALSE ) { return FALSE; }

			HGLOBAL hg = NULL;
			hr = pOutStream->Write( (void const*)_T("\0"), 1, 0);
			GetHGlobalFromStream( pOutStream, &hg);
			GlobalUnlock(hg);

			LARGE_INTEGER start = {0, 0};
			ULARGE_INTEGER cb = {0, 0};
			pOutStream->Seek( start, STREAM_SEEK_CUR, &cb);

			pOutput = GlobalLock(hg);

			// save to file
			HANDLE hFile = ::CreateFile( lpszFilePath
				, GENERIC_WRITE
				, FILE_SHARE_WRITE | FILE_SHARE_READ
				, NULL
				, CREATE_ALWAYS
				, 0
				, NULL
				);
			if( hFile == INVALID_HANDLE_VALUE )
			{
				ASSERT(FALSE);
				return FALSE;
			}

			DWORD dwWritten = 0;
			::WriteFile( hFile, pOutput, (UINT)cb.QuadPart, &dwWritten, NULL);
			::FlushFileBuffers(hFile);
			::CloseHandle(hFile);

			GlobalUnlock(hg);

			return TRUE;
		}
		catch( _com_error e)
		{
			TRACE( _T("CXsl::Transform failed:%s\n"), e.ErrorMessage());
			ASSERT( FALSE );
			return FALSE;
		}
	}
}
