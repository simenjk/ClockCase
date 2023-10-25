
//------------------------------------------------------------------------------
//
#include <vector>
#include <iostream>
#include <windows.h>
#include <comutil.h>
#include <tchar.h>

typedef const _TCHAR CCAPString;

class CCAPCatalogInfo 
{
public: 
	bool GetNodeText(IXMLDOMNode* pndeNode,
		LPTSTR strTestValue,
		CCAPString& strText);
};


bool CCAPCatalogInfo::GetNodeText(IXMLDOMNode *pndeNode, 
								  LPTSTR strTestValue, 
								  CCAPString &strText)
//------------------------------------------------------------------------------
{
	_ASSERT(pndeNode);

	_bstr_t		    bstrNodeTitle;
	IXMLDOMNode *   pndeTemp = NULL;
	_bstr_t		    desc;
	BOOL			bResult = FALSE;
	BSTR 			bstrVal = NULL;

	strText = _T("");
	try
	{
		
		BSTR strTest = _bstr_t(strTestValue);
		ThrowError(pndeNode->selectSingleNode(_bstr_t(strTestValue), &pndeTemp));
		if (pndeTemp!=NULL)
		{
			bool bResult = false;
			pndeTemp->get_text(&bstrVal);
			std::vector<TCHAR> vecChar;
			vecChar.clear();
			int idx = 0;
			bool bDone = false;

			while (!bDone && idx<30000)
			{
				if (bstrVal[idx]!= 0)
					vecChar.push_back((TCHAR)bstrVal[idx]);
				else
					bstrVal[idx]='\0';

				if (bstrVal[idx]=='\0' || bstrVal[idx] == 0)
					bDone = TRUE;
				idx++;

			}

			for (idx=0; idx<(int)vecChar.size(); idx++)
			{
				strText = strText + vecChar.at(idx);
			}

			strText.Swap("&amp;"/*NT*/,"&"/*NT*/);

			bResult = true;
		}
	}
	catch (_com_error & e)
	{
		//ATLTRACE("CCAPCatalogInfo::GetNodeText::Error 0x%08X\n"/*NT*/, e.Error());
		//elimanate warning
		e = e;
		return false;
	}
	return bResult;
}
