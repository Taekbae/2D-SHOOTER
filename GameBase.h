/************************************************************************/
/* ������ : �¹���														*/
/* E-mail : lovests2@naver.com											*/
/* ��α� : http://funnism.com											*/
/* ��������                                                              */
/* - 2012�� 4�� 15�� : �̸������ϵ� ��� ��� �� ������� ����             */
/************************************************************************/

#pragma once

#pragma comment(lib, "winmm.lib")
#pragma message("Automatically linking : Windows Library -> winmm.lib")

#pragma comment(lib, "d3d9.lib")
#pragma message("Automatically linking : DirectX Library -> d3d9.lib")

#ifdef _DEBUG
#pragma comment(lib, "d3dx9d.lib")
#pragma message("Automatically linking : DirectX Library -> d3dx9d.lib")
#pragma comment(lib, "fmodexL_vc.lib")
#pragma message("Automatically linking : FMOD Sound System Library -> fmodexL_vc.lib")
#else
#pragma comment(lib, "d3dx9.lib")
#pragma message("Automatically linking : DirectX Library -> d3dx9.lib")
#pragma comment(lib, "fmodex_vc.lib")
#pragma message("Automatically linking : FMOD Sound System Library -> fmodex_vc.lib")
#endif

#pragma comment(lib, "dxguid.lib")
#pragma message("Automatically linking : DirectX Library -> dxguid.lib")
#pragma comment(lib, "dinput8.lib")
#pragma message("Automatically linking : DirectX Library -> dinput8.lib")

#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1

#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <vector>
#include <hash_map>
#include <windows.h>
#include <map>
#include <string>
#include <time.h>
#include <iostream>

#include "directX(June2010)/Include/d3d9.h"
#include "directX(June2010)/Include/d3dx9.h"
#include "directX(June2010)/Include/dinput.h"
#include "fmod/inc/fmod.hpp"
#include "fmod/inc/fmod_errors.h"

using namespace std;
using namespace stdext;
using namespace FMOD;

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 512

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define ENEMY_NUM 20
#define BULLET_NUM 100

#include "FontManager.h"
#include "SoundManager.h"