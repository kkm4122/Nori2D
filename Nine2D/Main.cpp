// Nine0.cpp : Defines the entry point for the application.
//
#include "pch.h"
#include "framework.h"
#include "Nine0.h"
#include "renderer\DxWindow.h"
#include "renderer\Dx2DRenderer.h"
#include "DemoGame.h"
#include "DemoGame1.h"



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    // UNREFERENCED_PARAMETER(hPrevInstance);
    // UNREFERENCED_PARAMETER(lpCmdLine);

    srand(time(nullptr));

    ecs1 = ecs_new(1024, NULL);

    g_Dx11.hInstance = hInstance;
	g_Dx11.titleBarStats = true;
	g_Dx11.titleBarText = "Nori2D Window";
    g_Dx11.dxFeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_1;
    g_Dx11.width = 1280;
    g_Dx11.height = 720;
    g_Dx11.half_width = (1280 / 2);
    g_Dx11.half_height = (720 / 2);

    DxWindow* gDx = new DxWindow;
	Dx2DRenderer* gRender = new Dx2DRenderer;
    DemoGame1* demo = new DemoGame1;
    demo->sample();

    gDx->Run(demo,gRender);

    delete demo;
    delete gRender;
    delete gDx;

    ecs_free(ecs1);

    return 0;
}



