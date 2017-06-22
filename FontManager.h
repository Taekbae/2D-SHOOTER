/************************************************************************/
/* 제작자 : 태발이														*/
/* E-mail : lovests2@naver.com											*/
/* 블로그 : http://funnism.com											*/
/* 수정내용                                                              */
/* - 2012년 4월 15일 : 미리컴파일된 헤더 사용 및 헤더파일 정리             */
/************************************************************************/

#pragma once

typedef		hash_map<DWORD, LPD3DXFONT>		FONT_MAP;
typedef		FONT_MAP::iterator				FONT_MAP_Iter;
typedef		pair<DWORD, LPD3DXFONT>			FONT_MAP_Pair;

class FontManager
{
private:
	static FontManager singleton;
	FontManager(void);

public:
	static enum TextColor { White };
	static FontManager* Instance() { return &singleton; }
	~FontManager(void);

protected:
	LPDIRECT3DDEVICE9		pDevice;
	FONT_MAP				mapFont;
	DWORD					dwColor;

public:
	void					Create(LPDIRECT3DDEVICE9 pd3dDevice);
	void					Destroy();

	LPD3DXFONT 				GetFont(DWORD dwKey);

	void					AddFont(DWORD dwKey, LPCTSTR szFontType, int iWidth, int iHeight);
	BOOL					DescribeFont(int x, int y, LPCTSTR szText, DWORD dwColor, DWORD dwKey);
	BOOL					DescribeMainMessage(LPCTSTR szText, DWORD dwColor, DWORD dwKey);
	BOOL					DescribeFontEmotion(int x, int y, LPCTSTR szText, DWORD dwAlphaIncrement, DWORD dwKey);
	BOOL					DescribeSyncFont(DWORD dwGregorianTime, int x, int y, LPCTSTR szText, DWORD dwColor, DWORD dwKey);
	BOOL					DescribeFontType(int x, int y, DWORD dwColor, DWORD dwKey);
	BOOL					DeleteFont(DWORD dwKey);
};

#define FONTMANAGER()	FontManager::Instance()

/************************************************************************/
/* How to use                                                                      */
/************************************************************************/
// Font() -> Create ( LPDIRECT3DDEVICE ) 생성
// Font() -> AddFont( DWORD dwKey, LPCTSTR szFontType, int iWidth, int iHeight, int iQuality ); 폰트타입추가 / 키 = 타입
// Font() -> DescribeFont( int x, int y, LPCTSTR szText, DWORD dwKey ); x y 좌표에 폰트 그리기 
// Font() -> Destroy( void ) 파괴자