// include the basic windows header files and the Direct3D header file
#include "stdafx.h"

// define the screen resolution and keyboard macros
// global declarations
LPDIRECT3D9 d3d;    // the pointer to our Direct3D interface
LPDIRECT3DDEVICE9 d3ddev;    // the pointer to the device class
LPD3DXSPRITE d3dspt;    // the pointer to our Direct3D Sprite interface

// sprite declarations
LPDIRECT3DTEXTURE9 sprite;    // the pointer to the sprite (Background)
LPDIRECT3DTEXTURE9 sprite_hero;    // the pointer to the sprite (hero, animation frame 01)
LPDIRECT3DTEXTURE9 sprite_hero_1;    // the pointer to the sprite (hero, animation frame 02)
LPDIRECT3DTEXTURE9 sprite_hero_2;    // the pointer to the sprite (hero, animation frame 03)
LPDIRECT3DTEXTURE9 sprite_hero_3;    // the pointer to the sprite (hero, animation frame 04)
LPDIRECT3DTEXTURE9 sprite_HP;
LPDIRECT3DTEXTURE9 sprite_enemy;    // the pointer to the sprite (enemy)
LPDIRECT3DTEXTURE9 sprite_bullet;    // the pointer to the sprite (bullet)
LPDIRECT3DTEXTURE9 sprite_superBullet;    // the pointer to the sprite (superBullet)
LPDIRECT3DTEXTURE9 sprite_boss;    // the pointer to the sprite (boss)

int gamemode;

// for scolling declarations
RECT rct; // this will control the size of the image
int offsetx = 0;
int offsety = 0;
D3DXVECTOR3 Pos(0, 0, 0); // Background Pos for scrolling

// for get score declarations
static int score = 0;
wstring	strScoreMessage;
LPCTSTR GetScore();

void initD3D(HWND hWnd);    // sets up and initializes Direct3D
void render_frame(void);    // renders a single frame
void cleanD3D(void);		// closes Direct3D and releases memory

void draw_background();

void init_game(void);
void do_game_logic(void);
bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1);

// the WindowProc function prototype
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

enum { MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT };

//기본 클래스 
class entity {

public:
	float x_pos;
	float y_pos;
	int status;
	int HP;
};

bool sphere_collision_check(float x0, float y0, float size0, float x1, float y1, float size1)
{
	if (x0 < x1 + size1 && y0 < y1 + size1 && x0 + size0 > x1 && y0 + size0>y1)
		return true;
	else
		return false;
}

//주인공 클래스 
class Hero :public entity {

public:
	void move(int i);
	void init(float x, float y);
	bool check_collision(float x, float y);
};

void Hero::init(float x, float y)
{
	HP = 3;
	x_pos = x;
	y_pos = y;
}

void Hero::move(int i)
{
	switch (i)
	{
	case MOVE_UP:
		y_pos -= 5;
		break;

	case MOVE_DOWN:
		y_pos += 5;
		break;

	case MOVE_LEFT:
		x_pos -= 5;
		break;

	case MOVE_RIGHT:
		x_pos += 5;
		break;
	}
}

bool Hero::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 32, x, y, 32) == true)
	{
		return true;
	}
	else {
		return false;
	}
}

//보스 클래스 
class Boss :public entity {

public:
	void move();
	void init(float x, float y);
};

void Boss::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
	HP = 5;
}

void Boss::move()
{
	x_pos -= 2;
}

// 적 클래스 
class Enemy :public entity {

public:
	void init(float x, float y);
	void move();
};

void Enemy::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

void Enemy::move()
{
	x_pos -= 2;
}

// 총알 클래스 
class Bullet :public entity {

public:
	bool bShow;

	void init(float x, float y);
	void move();
	bool show();
	void hide();
	void active();
	bool check_collision(float x, float y);
	bool super_check_collision(float x, float y);
};

bool Bullet::check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 20, x, y, 32) == true)
	{
		bShow = false;
		score++;
		return true;
	}
	else {
		return false;
	}
}

bool Bullet::super_check_collision(float x, float y)
{
	//충돌 처리 시 
	if (sphere_collision_check(x_pos, y_pos, 100, x, y, 100) == true)
	{
		score++;
		return true;
	}
	else {
		return false;
	}
}

void Bullet::init(float x, float y)
{
	x_pos = x;
	y_pos = y;
}

bool Bullet::show()
{
	return bShow;
}

void Bullet::active()
{
	bShow = true;
}

void Bullet::move()
{
	x_pos += 8;
}

void Bullet::hide()
{
	bShow = false;
}

LPCTSTR GetScore()
{
	TCHAR buf[256] = { 0 };
	wsprintf(buf, TEXT("%d"), score);
	strScoreMessage.assign(buf);
	return strScoreMessage.c_str();
}

//객체 생성 
Hero hero;
Enemy enemy[ENEMY_NUM];
Boss boss;
Bullet bullet[BULLET_NUM];
Bullet superbullet;

// the entry point for any Windows program
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL, L"WindowClass", L"Our Direct3D Program",
		WS_EX_TOPMOST | WS_POPUP, 200, 200, SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);

	// set up and initialize Direct3D
	initD3D(hWnd);

	//게임 오브젝트 초기화 
	init_game();
	SOUNDMANAGER()->InitSound();

	// enter the main loop:

	MSG msg;

	while (TRUE)
	{
		DWORD starting_point = GetTickCount();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (gamemode == INTRO && GetAsyncKeyState(VK_RETURN))
		{
			SOUNDMANAGER()->Play(BEEP_IN);
			Sleep(300);
			gamemode++;
		}

		if (gamemode == PLAY)
		{
			SOUNDMANAGER()->PlaySound("BG.mp3");
			do_game_logic();
			if (hero.HP == 0)
			{
				SOUNDMANAGER()->StopSound();
				SOUNDMANAGER()->Play(BEEP_OUT);
				gamemode++;
			}
		}

		render_frame();

		// check the 'escape' key
		if (KEY_DOWN(VK_ESCAPE))
			PostMessage(hWnd, WM_DESTROY, 0, 0);

		while ((GetTickCount() - starting_point) < 25);
	}

	// clean up DirectX and COM
	cleanD3D();

	return msg.wParam;
}

// this is the main message handler for the program
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

// this function initializes and prepares Direct3D for use
void initD3D(HWND hWnd)
{
	d3d = Direct3DCreate9(D3D_SDK_VERSION);

	D3DPRESENT_PARAMETERS d3dpp;

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.Windowed = TRUE;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;

	// create a device class using this information and the info from the d3dpp stuct
	d3d->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	D3DXCreateSprite(d3ddev, &d3dspt);    // create the Direct3D Sprite object

	FONTMANAGER()->Create(d3ddev);
	FONTMANAGER()->AddFont(0, TEXT("Fixedsys"), 0, 35);

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"Background.png",    // the file name
		SCREEN_WIDTH,    // default width
		SCREEN_HEIGHT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"kitten_0.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"kitten_1.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero_1);    // load to sprite	

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"kitten_2.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero_2);    // load to sprite	

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"kitten_3.png",    // the file name
		D3DX_DEFAULT,    // default width
		D3DX_DEFAULT,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_hero_3);    // load to sprite	

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"HP.png",    // the file name
		34,    // default width
		34,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_HP);    // load to sprite	

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"enemy.png",    // the file name
		100,    // default width
		52,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_enemy);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"bullet.png",    // the file name
		20,    // default width
		32,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_bullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"superbullet.png",    // the file name
		100,    // default width
		100,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_superBullet);    // load to sprite

	D3DXCreateTextureFromFileEx(d3ddev,    // the device pointer
		L"boss.png",    // the file name
		150,    // default width
		78,    // default height
		D3DX_DEFAULT,    // no mip mapping
		NULL,    // regular usage
		D3DFMT_A8R8G8B8,    // 32-bit pixels with alpha
		D3DPOOL_MANAGED,    // typical memory handling
		D3DX_DEFAULT,    // no filtering
		D3DX_DEFAULT,    // no mip filtering
		D3DCOLOR_XRGB(255, 0, 255),    // the hot-pink color key
		NULL,    // no image info struct
		NULL,    // not using 256 colors
		&sprite_boss);    // load to sprite

	return;
}

void init_game(void)
{
	gamemode = INTRO;

	//객체 초기화
	hero.init(SCREEN_WIDTH / 7, SCREEN_HEIGHT / 2);

	//적들 초기화 
	for (int i = 0; i<ENEMY_NUM; ++i)
	{
		enemy[i].init((float)(rand() % SCREEN_WIDTH + SCREEN_WIDTH), rand() % SCREEN_HEIGHT);
	}

	boss.init(rand() % SCREEN_WIDTH + SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
}

void do_game_logic(void)
{
	// hero
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		hero.move(MOVE_UP);

	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		hero.move(MOVE_DOWN);

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		hero.move(MOVE_LEFT);

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		hero.move(MOVE_RIGHT);

	// enemy
	for (int i = 0; i<ENEMY_NUM; i++)
	{
		if (enemy[i].x_pos < 0)
			enemy[i].init((float)(SCREEN_WIDTH), rand() % SCREEN_HEIGHT);
		else
			enemy[i].move();
	}

	if (boss.x_pos < -200)
		boss.init(750, rand() % 100);
	else
		boss.move();

	// bullet fire
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].show() == false)
		{
			if (GetAsyncKeyState(VK_SPACE))
			{
				bullet[i].active();
				bullet[i].init(hero.x_pos + 64, hero.y_pos + 20);
				break;
			}
		}
	}

	// superbullet fire
	if (GetAsyncKeyState(0x46) && 0x8000)
	{
		if (superbullet.show() == false)
		{
			superbullet.bShow = true;
			SOUNDMANAGER()->Play(SUPERBULLET);
			superbullet.init(hero.x_pos + 64, hero.y_pos + 10);
		}
	}

	// bullet check collision
	for (int i = 0; i < BULLET_NUM; i++)
	{
		if (bullet[i].show() == true)
		{
			if (bullet[i].x_pos > SCREEN_WIDTH)
				bullet[i].hide();
			else
				bullet[i].move();

			for (int j = 0; j < ENEMY_NUM; j++)
			{
				if (bullet[i].check_collision(enemy[j].x_pos, enemy[j].y_pos) == true)
				{
					SOUNDMANAGER()->Play(DAMAGE);
					enemy[j].init((float)(SCREEN_WIDTH), rand() % SCREEN_HEIGHT);
				}
			}

			if (bullet[i].check_collision(boss.x_pos, boss.y_pos) == true)
			{
				boss.HP--;

				if (boss.HP < 0)
					boss.init(rand() % SCREEN_WIDTH + SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
			}
		}
	}

	// superbullet check collision
	if (superbullet.show() == true)
	{
		if (superbullet.x_pos > SCREEN_WIDTH)
			superbullet.hide();
		else
			superbullet.move();

		for (int j = 0; j < ENEMY_NUM; j++)
		{
			if (superbullet.super_check_collision(enemy[j].x_pos, enemy[j].y_pos) == true)
			{
				SOUNDMANAGER()->Play(DAMAGE);
				enemy[j].init((float)(SCREEN_WIDTH), rand() % SCREEN_HEIGHT);
			}
		}

		if (superbullet.check_collision(boss.x_pos, boss.y_pos) == true)
		{
			boss.HP -= 7;

			if (boss.HP < 0)
				boss.init(rand() % SCREEN_WIDTH + SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		}
	}

	for (int i = 0; i < ENEMY_NUM; i++)
	{
		if (hero.check_collision(enemy[i].x_pos, enemy[i].y_pos) == true)
		{
			enemy[i].init((float)(SCREEN_WIDTH), rand() % SCREEN_HEIGHT);
			hero.HP--;
		}
	}

	if (hero.check_collision(boss.x_pos, boss.y_pos) == true)
	{
		boss.init(rand() % SCREEN_WIDTH + SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
		hero.HP = 0;
	}
}

void draw_background(void)
{
	Pos.x = 0;//reset VECTOR3.x to 0
	Pos.y = 0;//reset VECTOR3.y to 0

			  //top-right
	rct.left = SCREEN_WIDTH - offsetx;
	rct.right = SCREEN_WIDTH;
	rct.top = SCREEN_HEIGHT - offsety;
	rct.bottom = SCREEN_HEIGHT;
	d3dspt->Draw(sprite, &rct, NULL, &Pos, 0xFFFFFFFF);

	//bottom-right
	rct.left = SCREEN_WIDTH - offsetx;
	rct.right = SCREEN_WIDTH;
	rct.top = 0;
	rct.bottom = SCREEN_HEIGHT - offsety;
	Pos.x = 0;
	Pos.y = offsety;
	d3dspt->Draw(sprite, &rct, NULL, &Pos, 0xFFFFFFFF);

	//top-left
	rct.left = 0;
	rct.right = SCREEN_WIDTH;
	rct.top = SCREEN_HEIGHT - offsety;
	rct.bottom = SCREEN_HEIGHT;
	Pos.x = offsetx;
	Pos.y = 0;
	d3dspt->Draw(sprite, &rct, NULL, &Pos, 0xFFFFFFFF);

	//bottom-left
	rct.left = 0;
	rct.right = SCREEN_WIDTH - offsetx;
	rct.top = 0;
	rct.bottom = SCREEN_HEIGHT - offsety;
	Pos.x = offsetx;
	Pos.y = offsety;
	d3dspt->Draw(sprite, &rct, NULL, &Pos, 0xFFFFFFFF);

	// once the offsets reach there limits they are reset
	if (offsety > SCREEN_HEIGHT) offsety = 0;

	if (offsety < 0) offsety = SCREEN_HEIGHT;

	if (offsetx > SCREEN_WIDTH) offsetx = 0;

	if (offsetx < 0) offsetx = SCREEN_WIDTH;

	// with keyboard input
	// if (KEY_DOWN(VK_UP))
	// offsety++;

	// if (KEY_DOWN(VK_DOWN))
	// offsety--;

	if (KEY_DOWN(VK_LEFT))
	offsetx++;

	if (KEY_DOWN(VK_RIGHT))
	offsetx--;
}

// this is the function used to render a single frame
void render_frame(void)
{
	static int counter = 0;

	// clear the window to a deep blue
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	d3ddev->BeginScene();    // begins the 3D scene

	d3dspt->Begin(D3DXSPRITE_ALPHABLEND);    // // begin sprite drawing with transparency

											 //UI 창 렌더링 


											 /*
											 static int frame = 21;    // start the program on the final frame
											 if(KEY_DOWN(VK_SPACE)) frame=0;     // when the space key is pressed, start at frame 0
											 if(frame < 21) frame++;     // if we aren't on the last frame, go to the next frame

											 // calculate the x-position
											 int xpos = frame * 182 + 1;

											 RECT part;
											 SetRect(&part, xpos, 0, xpos + 181, 128);
											 D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
											 D3DXVECTOR3 position(150.0f, 50.0f, 0.0f);    // position at 50, 50 with no depth
											 d3dspt->Draw(sprite, &part, &center, &position, D3DCOLOR_ARGB(127, 255, 255, 255));
											 */

	switch (gamemode)
	{
	case INTRO:
		offsetx -= 2;
		draw_background();
		break;

	case PLAY:
	{
		counter += 1;
		offsetx -= 2;
		draw_background();

		// Hero
		RECT part;
		SetRect(&part, 0, 0, 64, 64);
		D3DXVECTOR3 center(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position(hero.x_pos, hero.y_pos, 0.0f);    // position at 50, 50 with no depth
		switch (counter % 16) {
		case 0:
		case 1:
		case 2:
		case 3:
			d3dspt->Draw(sprite_hero, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 4:
		case 5:
		case 6:
		case 7:
			d3dspt->Draw(sprite_hero_1, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			d3dspt->Draw(sprite_hero_2, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 12:
		case 13:
		case 14:
		case 15:
			d3dspt->Draw(sprite_hero_3, &part, &center, &position, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}

		// Bullet
		for (int i = 0; i < BULLET_NUM; i++)
		{
			if (bullet[i].bShow == true)
			{
				RECT part1;
				SetRect(&part1, 0, 0, 20, 32);
				D3DXVECTOR3 center1(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
				D3DXVECTOR3 position1(bullet[i].x_pos, bullet[i].y_pos, 0.0f);    // position at 50, 50 with no depth
				d3dspt->Draw(sprite_bullet, &part1, &center1, &position1, D3DCOLOR_ARGB(255, 255, 255, 255));
			}
		}

		if (superbullet.bShow == true)
		{
			RECT part3;
			SetRect(&part3, 0, 0, 100, 100);
			D3DXVECTOR3 center3(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
			D3DXVECTOR3 position3(superbullet.x_pos, superbullet.y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_superBullet, &part3, &center3, &position3, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// Enemy
		RECT part2;
		SetRect(&part2, 0, 0, 100, 52);
		D3DXVECTOR3 center2(0.0f, 0.0f, 0.0f);    // center at the upper-left corner

		for (int i = 0; i < ENEMY_NUM; i++)
		{
			D3DXVECTOR3 position2(enemy[i].x_pos, enemy[i].y_pos, 0.0f);    // position at 50, 50 with no depth
			d3dspt->Draw(sprite_enemy, &part2, &center2, &position2, D3DCOLOR_ARGB(255, 255, 255, 255));
		}

		// Boss
		RECT part7;
		SetRect(&part7, 0, 0, 150, 78);
		D3DXVECTOR3 center7(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position7(boss.x_pos, boss.y_pos, 0.0f);    // position at 50, 50 with no depth
		d3dspt->Draw(sprite_boss, &part7, &center7, &position7, D3DCOLOR_ARGB(30 * boss.HP + 105, 255, 255, 255));

		// HP
		RECT part4;
		SetRect(&part4, 0, 0, 34, 34);
		D3DXVECTOR3 center4(0.0f, 0.0f, 0.0f);    // center at the upper-left corner
		D3DXVECTOR3 position4(910, 25, 0.0f);    // position at 50, 50 with no depth
		D3DXVECTOR3 position5(940, 25, 0.0f);    // position at 50, 50 with no depth
		D3DXVECTOR3 position6(970, 25, 0.0f);    // position at 50, 50 with no depth

		switch (hero.HP)
		{
		case 3:
			d3dspt->Draw(sprite_HP, &part4, &center4, &position4, D3DCOLOR_ARGB(255, 255, 255, 255));
			d3dspt->Draw(sprite_HP, &part4, &center4, &position5, D3DCOLOR_ARGB(255, 255, 255, 255));
			d3dspt->Draw(sprite_HP, &part4, &center4, &position6, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 2:
			d3dspt->Draw(sprite_HP, &part4, &center4, &position5, D3DCOLOR_ARGB(255, 255, 255, 255));
			d3dspt->Draw(sprite_HP, &part4, &center4, &position6, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		case 1:
			d3dspt->Draw(sprite_HP, &part4, &center4, &position6, D3DCOLOR_ARGB(255, 255, 255, 255));
			break;
		}
	}
	break;

	case ENDING:
		break;
	}

	d3dspt->End();    // end sprite drawing

	switch (gamemode)
	{
	case INTRO:
		FONTMANAGER()->DescribeMainMessage(TEXT("press enter to continue"), 0xffffffff, 0);
		break;
	case PLAY:
		FONTMANAGER()->DescribeFont(25, 25, GetScore(), 0xffffffff, 0);
		break;
	case ENDING:
	{
		TCHAR buf[256];
		StringCchPrintf(buf, 256, TEXT("GAME OVER\nYour Score:%d\n\npress ESC to exit"), score);
		FONTMANAGER()->DescribeMainMessage(buf, 0xffffffff, 0);
	}
	break;
	}

	d3ddev->EndScene();    // ends the 3D scene

	d3ddev->Present(NULL, NULL, NULL, NULL);

	return;
}

// this is the function that cleans up Direct3D and COM
void cleanD3D(void)
{
	sprite->Release();
	d3ddev->Release();
	d3d->Release();

	//객체 해제 
	sprite_hero->Release();
	sprite_hero_1->Release();
	sprite_hero_2->Release();
	sprite_hero_3->Release();
	sprite_HP->Release();
	sprite_enemy->Release();
	sprite_bullet->Release();
	sprite_superBullet->Release();
	sprite_boss->Release();

	return;
}