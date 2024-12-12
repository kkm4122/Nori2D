#pragma once

#include "pico\pico_ecs.h"
#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include <string>
#include <map> 
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>




#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")


typedef DirectX::XMFLOAT2 XFloat2;
typedef DirectX::XMFLOAT3 XFloat3;
typedef DirectX::XMFLOAT4 XFloat4; 

extern ecs_t* ecs_;


