#include "pch.h"
#include "DxWindow.h"
#include "Components.h"
#include <WindowsX.h>

TimeData g_Time;
Dx11Vars g_Dx11;

DxWindow* DxWindow::g = nullptr;
ID3D11Debug* m_d3dDebug = 0; // only used in debug mode
POINT prevMousePos;


DxWindow::DxWindow()
{
	g = this;
	InitWindow();
	InitDirectX();
}

DxWindow::~DxWindow()
{
	SAFE_RELEASE(g_Dx11.context)
	SAFE_RELEASE(g_Dx11.depthStencilView)
	SAFE_RELEASE(g_Dx11.backBufferRTV)
	SAFE_RELEASE(g_Dx11.swapChain)
	SAFE_RELEASE(g_Dx11.device)
}

LRESULT DxWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g->onMessage(hWnd, uMsg, wParam, lParam);
}


HRESULT DxWindow::InitWindow()
{
	WNDCLASS wndClass = {};
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = DxWindow::WindowProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = g_Dx11.hInstance;
	wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszMenuName = NULL;
	wndClass.lpszClassName = "Direct3DWindowClass";	

	if (!RegisterClass(&wndClass))
	{
		// Get the most recent error
		DWORD error = GetLastError();

		// If the class exists, that's actually fine.  Otherwise,
		// we can't proceed with the next step.
		if (error != ERROR_CLASS_ALREADY_EXISTS)
			return HRESULT_FROM_WIN32(error);
	}

	RECT clientRect;
	SetRect(&clientRect, 0, 0, g_Dx11.width, g_Dx11.height);
	AdjustWindowRect(&clientRect,WS_OVERLAPPEDWINDOW,false);

	RECT desktopRect;
	GetClientRect(GetDesktopWindow(), &desktopRect);
	int centeredX = (desktopRect.right / 2) - (clientRect.right / 2);
	int centeredY = (desktopRect.bottom / 2) - (clientRect.bottom / 2);

	g_Dx11.hWnd = CreateWindow(
						wndClass.lpszClassName,
						g_Dx11.titleBarText.c_str(),
						WS_OVERLAPPEDWINDOW,
						centeredX,
						centeredY,
						clientRect.right - clientRect.left,	// Calculated width
						clientRect.bottom - clientRect.top,	// Calculated height
						0,			// No parent window
						0,			// No menu
						g_Dx11.hInstance,	// The app's handle
						0);			// No other windows in our application

	// Ensure the window was created properly
	if (g_Dx11.hWnd == NULL)
	{
		DWORD error = GetLastError();
		return HRESULT_FROM_WIN32(error);
	}

	// The window exists but is not visible yet
	// We need to tell Windows to show it, and how to show it
	ShowWindow(g_Dx11.hWnd, SW_SHOW);

	return S_OK;
}

HRESULT DxWindow::InitDirectX()
{
	unsigned int deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// If we're in debug mode in visual studio, we also
	// want to make a "Debug DirectX Device" to see some
	// errors and warnings in Visual Studio's output window
	// when things go wrong!
	deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 1;
	swapDesc.BufferDesc.Width = g_Dx11.width;
	swapDesc.BufferDesc.Height = g_Dx11.height;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.Flags = 0;
	swapDesc.OutputWindow = g_Dx11.hWnd;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapDesc.Windowed = true;

	// Result variable for below function calls
	HRESULT hr = S_OK;

	// 1. DirectX Device 积己
	hr = D3D11CreateDeviceAndSwapChain(
				0,							// Video adapter (physical GPU) to use, or null for default
				D3D_DRIVER_TYPE_HARDWARE,	// We want to use the hardware (GPU)
				0,							// Used when doing software rendering
				deviceFlags,				// Any special options
				0,							// Optional array of possible verisons we want as fallbacks
				0,							// The number of fallbacks in the above param
				D3D11_SDK_VERSION,			// Current version of the SDK
				&swapDesc,					// Address of swap chain options
				&(g_Dx11.swapChain),					// Pointer to our Swap Chain pointer
				&(g_Dx11.device),					// Pointer to our Device pointer
				&(g_Dx11.dxFeatureLevel),			// This will hold the actual feature level the app will use
				&(g_Dx11.context));					// Pointer to our Device Context pointer
	if (FAILED(hr)) return hr;
	
	g_Dx11.device->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&m_d3dDebug));
		
	// 2. Backbuffer俊 措茄 View 积己
	ID3D11Texture2D* backBufferTexture;
	g_Dx11.swapChain->GetBuffer(0,__uuidof(ID3D11Texture2D),(void**)&backBufferTexture);
	g_Dx11.device->CreateRenderTargetView(backBufferTexture, 0, &g_Dx11.backBufferRTV);
	backBufferTexture->Release();

	// 3. Depth Buffer view 积己,
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = g_Dx11.width;
	depthStencilDesc.Height = g_Dx11.height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	ID3D11Texture2D* depthBufferTexture;
	g_Dx11.device->CreateTexture2D(&depthStencilDesc, 0, &depthBufferTexture);
	g_Dx11.device->CreateDepthStencilView(depthBufferTexture, 0, &g_Dx11.depthStencilView);
	depthBufferTexture->Release();

	// 4. Rendering pipeline俊 利侩.
	// Bind the views to the pipeline, so rendering properly 
	// uses their underlying textures
	g_Dx11.context->OMSetRenderTargets(1, &g_Dx11.backBufferRTV, g_Dx11.depthStencilView);

	// 5. Viewport 汲沥.
	D3D11_VIEWPORT viewport = {};
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)g_Dx11.width;
	viewport.Height = (float)g_Dx11.height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	g_Dx11.context->RSSetViewports(1, &viewport);

	return S_OK;
}


HRESULT DxWindow::Run()
{
 	// Grab the start time now that
	// the game loop is running
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	g_Time.startTime = now;
	g_Time.currentTime = now;
	g_Time.previousTime = now;

	// Query performance counter for accurate timing information
	__int64 perfFreq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&perfFreq);
	g_Time.perfCounterSeconds = 1.0 / (double)perfFreq;

	MessageLoop();

	return E_NOTIMPL;
}

HRESULT DxWindow::MessageLoop()
{

	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			UpdateTimer();

			UpdateTitleBarStats();

			// Texture toggle
			if (GetAsyncKeyState('O') & 0x8000)
			{
				//Mesh.CleanUpTexture(registry);
			}
			else if (GetAsyncKeyState('I') & 0x8000)
			{
				//Mesh.InitTexture(registry);
			}

			if (GetAsyncKeyState('M') & 0x8000)
			{
				//SystemsPlan::Plan->CreateModelGeometry(registry);
			}			
			if(mDrawObj) mDrawObj->Update();
			
			Draw();
			// Draw(&mycompRender, obj_shaderClass, &cameraComponents, &mycompshaderstruct, &mycompPixel, &mycompvsStruct, &mycompPSStruct, obj_time, obj_SkyShader, obj_BoneDatMesh);

		}
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//     E  N  D
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	// Uncomment below line for debug purpose
	/*
	ID3D11Debug::ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	m_d3dDebug->Release();
	*/

	return E_NOTIMPL;
}




LRESULT DxWindow::onMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{	
	// This is the message that signifies the window closing
	case WM_DESTROY:
		PostQuitMessage(0);	// Send a quit message to our own program
		return 0;

	// Prevent beeping when we "alt-enter" into fullscreen
	case WM_MENUCHAR: 
		return MAKELRESULT(0, MNC_CLOSE);

	// Prevent the overall window from becoming too small
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 200;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 200;
		return 0;

	// Sent when the window size changes
	case WM_SIZE:
		// Don't adjust anything when minimizing,
		// since we end up with a width/height of zero
		// and that doesn't play well with DirectX
		if (wParam == SIZE_MINIMIZED)
			return 0;

		//dxCoreWinRendererComps->width = LOWORD(lParam);
		//dxCoreWinRendererComps->height = HIWORD(lParam);
		////// If DX is initialized, resize 
		////// our required buffers
		//if (dxCoreWinRendererComps->device)
		//	OnResize();

		return 0;

	// Mouse button being pressed (while the cursor is currently over our window)
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		OnMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	// Mouse button being released (while the cursor is currently over our window)
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	// Cursor moves over the window (or outside, while we're currently capturing it)
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	// Mouse wheel is scrolled
	case WM_MOUSEWHEEL:
		OnMouseWheel(GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	//case WM_KEYDOWN:
	//	SpawnMeshes();
	//	return 0;
	}

	// Let Windows handle any messages we're not touching
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


// Save the previous mouse position, so we have it for the future
void DxWindow::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// prevMousePos.x = x;
	// prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	//SetCapture(dxCoreWinRendererComps->hWnd);
}

void DxWindow::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	
	// ReleaseCapture();
}

void DxWindow::OnMouseMove(WPARAM buttonState, int x, int y)
{
	/*
	//// Add any custom code here...
	//// Check left mouse button
	if (buttonState & 0x0001)
	{
		float xDiff = (x - prevMousePos.x) * 0.005f;
		float yDiff = (y - prevMousePos.y) * 0.005f;
		camera.Rotate(yDiff, xDiff, *registry2); // need to implement
	}
	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
	*/

}

void DxWindow::OnMouseWheel(float wheelDelta, int x, int y)
{

}

void DxWindow::Draw()
{
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	int count = 0;

	g_Dx11.context->ClearRenderTargetView(g_Dx11.backBufferRTV,
												 color);
	g_Dx11.context->ClearDepthStencilView(g_Dx11.depthStencilView,
												 D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
												 1.0f,
												 0);

	if(mDrawObj) mDrawObj->Draw();


	g_Dx11.swapChain->Present(0, 0);
	
	g_Dx11.context->OMSetRenderTargets(1, &g_Dx11.backBufferRTV, g_Dx11.depthStencilView);

}





void DxWindow::UpdateTimer()
{
	// Grab the current time
	__int64 now;
	QueryPerformanceCounter((LARGE_INTEGER*)&now);
	g_Time.currentTime = now;

	double d = (g_Time.currentTime - g_Time.previousTime) * g_Time.perfCounterSeconds;
	g_Time.deltaTime = max( (float)d, 0.0f);

	// Calculate the total time from start to now
	g_Time.totalTime = (float)((g_Time.currentTime - g_Time.startTime) * g_Time.perfCounterSeconds);

	// Save current time for next frame
	g_Time.previousTime = g_Time.currentTime;
}



//
// --------------------------------------------------------
// Updates the window's title bar with several stats once
// per second, including:
//  - The window's width & height
//  - The current FPS and ms/frame
//  - The version of DirectX actually being used (usually 11)
// --------------------------------------------------------
void DxWindow::UpdateTitleBarStats()
{
	g_Time.fpsFrameCount++;

	// Only calc FPS and update title bar once per second
	float timeDiff = g_Time.totalTime - g_Time.fpsTimeElapsed;
	if (timeDiff < 1.0f)
		return;

	// How long did each frame take?  (Approx)
	float mspf = 1000.0f / (float)g_Time.fpsFrameCount;

	// Quick and dirty title bar text (mostly for debugging)
	std::ostringstream output;
	output.precision(6);
	output << g_Dx11.titleBarText <<
		"    Width: "		<< g_Dx11.width <<
		"    Height: "		<< g_Dx11.height <<
		"    FPS: "			<< g_Time.fpsFrameCount <<
		"    Frame Time: "	<< mspf << "ms";

	// Append the version of DirectX the app is using
	switch (g_Dx11.dxFeatureLevel)
	{
	case D3D_FEATURE_LEVEL_11_1: output << "    DX 11.1"; break;
	case D3D_FEATURE_LEVEL_11_0: output << "    DX 11.0"; break;
	case D3D_FEATURE_LEVEL_10_1: output << "    DX 10.1"; break;
	case D3D_FEATURE_LEVEL_10_0: output << "    DX 10.0"; break;
	case D3D_FEATURE_LEVEL_9_3:  output << "    DX 9.3";  break;
	case D3D_FEATURE_LEVEL_9_2:  output << "    DX 9.2";  break;
	case D3D_FEATURE_LEVEL_9_1:  output << "    DX 9.1";  break;
	default:                     output << "    DX ???";  break;
	}

	SetWindowText(g_Dx11.hWnd, output.str().c_str());

	g_Time.fpsFrameCount = 0;
	g_Time.fpsTimeElapsed += 1.0f;
}


