/************************************************************************/
/* 제작자 : 태발이														*/
/* E-mail : lovests2@naver.com											*/
/* 블로그 : http://funnism.com											*/
/* 수정내용                                                              */
/* - 2012년 4월 15일 : 미리컴파일된 헤더 사용 및 헤더파일 정리             */
/************************************************************************/

#include "stdafx.h"

FontManager FontManager::singleton;

FontManager::FontManager(void)
{
}

FontManager::~FontManager(void)
{
}

void FontManager::Create(LPDIRECT3DDEVICE9 pd3dDevice)
{
	dwColor = 0xFFFFFFFF;
	pDevice = pd3dDevice;
}

void FontManager::Destroy()
{
	FONT_MAP_Iter iter;

	for (iter = mapFont.begin(); iter != mapFont.end(); ++iter)
	{
		(iter->second)->Release();
		mapFont.erase(iter++);
	}
	mapFont.clear();
}

void FontManager::AddFont(DWORD dwKey, LPCTSTR szFontType, int iWidth, int iHeight)
{
	LPD3DXFONT Font;
	HRESULT hRet;
	D3DXFONT_DESC Desc;
	Desc.CharSet = HANGUL_CHARSET;
	_tcscpy(Desc.FaceName, szFontType);
	Desc.Width = iWidth;
	Desc.Height = iHeight;
	Desc.Quality = DEFAULT_QUALITY;
	Desc.Italic = FALSE;
	Desc.MipLevels = 0;
	Desc.OutputPrecision = 0;
	Desc.PitchAndFamily = 0;
	Desc.Weight = 0;
	hRet = D3DXCreateFontIndirect(pDevice, &Desc, &Font);

	mapFont.insert(FONT_MAP_Pair(dwKey, Font));
}

BOOL FontManager::DescribeFont(int x, int y, LPCTSTR szText, DWORD dwColor, DWORD dwKey)
{
	if (!GetFont(dwKey) == NULL)
	{
		LPD3DXFONT Font = GetFont(dwKey);

		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + SCREEN_WIDTH;
		rect.bottom = rect.top + 100;

		TCHAR buf[256];
		int length = wsprintf(buf, szText);
		Font->DrawText(NULL, buf, length, &rect, DT_TOP, dwColor);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

LPD3DXFONT FontManager::GetFont(DWORD dwKey)
{
	FONT_MAP_Iter iter;
	iter = mapFont.find(dwKey);
	if (iter != mapFont.end())
	{
		return iter->second;
	}
	return NULL;
}

BOOL FontManager::DescribeFontEmotion(int x, int y, LPCTSTR szText, DWORD dwAlphaIncrement, DWORD dwKey)
{
	if (!GetFont(dwKey) == NULL)
	{
		LPD3DXFONT Font = GetFont(dwKey);

		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + SCREEN_WIDTH;
		rect.bottom = rect.top + 100;

		dwColor = (dwAlphaIncrement << 24) | 0XFFFFFFFF;
		if (dwColor>0xFF) dwColor = 0x00;
		TCHAR buf[256];
		int length = wsprintf(buf, szText);
		Font->DrawText(NULL, buf, length, &rect, DT_VCENTER | DT_CENTER, dwColor);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL FontManager::DescribeSyncFont(DWORD dwGregorianTime, int x, int y, LPCTSTR szText, DWORD dwColor, DWORD dwKey)
{
	if (!GetFont(dwKey) == NULL)
	{
		LPD3DXFONT Font = GetFont(dwKey);

		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + SCREEN_WIDTH;
		rect.bottom = rect.top + 100;

		TCHAR buf[256];
		int length = wsprintf(buf, szText);
		Font->DrawText(NULL, buf, length, &rect, DT_VCENTER | DT_CENTER, dwColor);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL FontManager::DeleteFont(DWORD dwKey)
{
	FONT_MAP_Iter iter;
	iter = mapFont.find(dwKey);
	if (iter == mapFont.end())
	{
		return FALSE;
	}
	(iter->second)->Release();
	mapFont.erase(dwKey);
	return TRUE;
}

BOOL FontManager::DescribeFontType(int x, int y, DWORD dwColor, DWORD dwKey)
{
	if (!GetFont(dwKey) == NULL)
	{
		LPD3DXFONT Font = GetFont(dwKey);

		RECT rect;
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + 700;
		rect.bottom = rect.top + 100;

		D3DXFONT_DESC Desc;

		HRESULT hRet = Font->GetDesc(&Desc);
		if (FAILED(hRet))
		{
			return FALSE;
		}


		TCHAR buftype[256];
		int lengthType = wsprintf(buftype, TEXT("타입이름 : %s "), Desc.FaceName);

		Font->DrawText(NULL, buftype, lengthType, &rect, DT_VCENTER, dwColor);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}

BOOL FontManager::DescribeMainMessage(LPCTSTR szText, DWORD dwColor, DWORD dwKey)
{
	if (!GetFont(dwKey) == NULL)
	{
		LPD3DXFONT Font = GetFont(dwKey);

		RECT rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = rect.left + SCREEN_WIDTH;
		rect.bottom = rect.top + SCREEN_HEIGHT;

		TCHAR buf[256];
		int length = wsprintf(buf, szText);
		Font->DrawText(NULL, buf, length, &rect, DT_CENTER | DT_VCENTER, dwColor);
	}
	else
	{
		return FALSE;
	}
	return TRUE;
}