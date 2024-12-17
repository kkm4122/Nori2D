#pragma once
#include "base/Vec2.h"

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(_ptr) { if(_ptr){_ptr->Release();} _ptr = nullptr; }
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(_ptr) { if(_ptr){ delete (_ptr);} _ptr = nullptr; }
#endif


struct TimeData
{
	// Timing related data
	double perfCounterSeconds;
	float totalTime;
	float deltaTime;
	__int64 startTime; 
	__int64 currentTime;
	__int64 previousTime;

	// FPS calculation
	int fpsFrameCount;
	float fpsTimeElapsed;
};


struct Dx11Vars
{
	int width;
	int height;

	float half_width;
	float half_height;

	HINSTANCE	hInstance;	
	std::string titleBarText;
	bool		titleBarStats;
	HWND hWnd;

	//D3D_FEATURE_LEVEL		dxFeatureLevel;
	IDXGISwapChain* swapChain = 0;
	ID3D11Device* device = 0;
	ID3D11DeviceContext* context = 0;

	ID3D11RenderTargetView* backBufferRTV = 0;
	ID3D11DepthStencilView* depthStencilView = 0;

	D3D_FEATURE_LEVEL		dxFeatureLevel;
};


struct CameraComponents
{
	// Camera matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;

	// Transformations
	DirectX::XMFLOAT3 startPosition;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT4 rotation;
	float xRotation;
	float yRotation;
};

extern Dx11Vars g_Dx11;
extern TimeData g_Time;


struct Vec2_t
{
    float x, y;
};

struct Pos_t
{
    float x, y;
	float angle;
};

struct Velocity_t
{
    float vx, vy;
};

struct Img_t
{
	float w,h;
	float ancherX, ancherY;
	int dir;
	int frameNo;
	float AnimTime;

	const WCHAR* texName; 
    void* tex;
} ;


struct Rect_t
{
    int x, y, w, h;
} ;



extern ecs_t* ecs1;

void register_components();
void register_systems();
void Update_system_all(float dt);
void Render_system_all(float dt);
void create_entity1();
void create_entity2();


