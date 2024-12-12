// Nine0.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "framework.h"
#include "Nine0.h"
#include "DxWindow.h"
#include "Dx2DRenderer.h"
#include "DemoGame.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    g_Dx11.hInstance = hInstance;
	g_Dx11.titleBarStats = true;
	g_Dx11.titleBarText = "DirectX Window";
    g_Dx11.dxFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;
    g_Dx11.width = 1280;
    g_Dx11.height = 720;
    g_Dx11.half_width = 1280 / 2;
    g_Dx11.half_height = 720 / 2;

    DxWindow* gDx = new DxWindow;
    DemoGame* demo = new DemoGame;
    gDx->mDrawObj = demo;
    gDx->Run();

    delete demo;
    delete gDx;

    return 0;
}



