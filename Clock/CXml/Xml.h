#pragma once
#include "./XmlNode.h"
#include "./Xsl.h"
#include <map>


//-------------------------------------------------------------------------
// Classes Name	    :CXml CXmlNode CXmlNodes
// Author			:Jerry.Wang QQ32775973 jerry.wang@clochase.com MSN:sky-sea-earth@21cn.com
// CreateTime       :2005-10-23
// Version          :v2.0 2007-07-16
//					:v2.1	Improve: Add LoadXml method
//					:		Improve: Add GetVersion method
//					:		Improve: Add const for GetXXX methods
//					:		Improve: define ASSERT as ATLASSERT for ATL 
//					:		Improve: define TRACE as ATLTRACE for ATL 
//					:V2.2	Improve: Add the parameter lpszRootName for the CXml::Open
//					:		Improve: Remove the CXml::GetLastError
//					:		Improve: Add the CXml::AddNamespace
//					:		Improve: Add two new overrides for the CXml::CreateNode with namespace support
//					:V3.0	Improve: Add another copy constructor for CXmlNode and CXmlNodes
//					:		Improve: Add const modifier for some variables
//					:		Improve: Add CXmlNode::GetInnerXml
//					:		Improve: Add CXmlNode::GetOuterXml
//					:		Improve: Add the CXml::Create
//					:		Improve: Change the MSXML version for create to 6.0 -> 3.0 -> 5.0 -> 4.0
//					:		Improve: Add namespace support for attribute
//					:		Improve: Add a new class named CXsl
//-------------------------------------------------------------------------





namespace Generic
{
	typedef enum
	{
		MSXML_UNKNOWN	= 00,
		MSXML26 		= 26,
		MSXML30 		= 30,
		MSXML40 		= 40,
		MSXML50 		= 50,
		MSXML60 		= 60,
	} MSXML_VERSION;

	class CXml
	{
		friend class CXsl;

	public:
		CXml(void);
		~CXml(void);

	protected:
		MSXML2::IXMLDOMDocument2Ptr m_pDoc; 
		CString						m_strFilePath;
		MSXML_VERSION				m_emVersion;
		std::map< CString, CString>	m_mpNamespace;

		BOOL CreateInstance(void);

	public:
		// Open xml file
		BOOL Open(LPCTSTR lpszXmlFilePath);

		// Create a new xml file
		BOOL Create( LPCTSTR lpszRootName = _T("xmlRoot")
			, LPCTSTR lpszEncoding = _T("UTF-8")
			, LPCTSTR lpszPrefix = _T("")
			, LPCTSTR lpszNamespaceURI = _T("")
			);

		// Load Xml string
		BOOL LoadXml(LPCTSTR lpszXmlContent);

		// save xml file
		BOOL Save(LPCTSTR lpszFilePath = NULL);

		// close xml file
		void Close(void);

		CString GetXmlFile(void) const;

		// Encode the binary data into string
		CString Base64Encode( LPBYTE pBuf, ULONG ulSize);

		// Decode the string into binary data
		BOOL Base64Decode( CString strIn, LPBYTE pBuf, LONG & lSize);
		

		// namespace
		void AddSelectionNamespace( LPCTSTR lpszPrefix, LPCTSTR lpszURI);

		// get the root element of 
		CXmlNodePtr GetRoot(void);

		// get single node by XPath
		CXmlNodePtr SelectSingleNode(LPCTSTR lpszPath);

		// get nodes by XPath
		CXmlNodesPtr SelectNodes(LPCTSTR lpszPath);

		// create node
		CXmlNodePtr CreateNode(LPCTSTR lpszName
			, LPCTSTR lpszPrefix = _T("")
			, LPCTSTR lpszNamespaceURI = _T("")
			);

		// get the current version of MSXML
		MSXML_VERSION GetVersion(void) const;

		// whether file exist
		static BOOL IsFileExist(CString strFilePath);
	};
}