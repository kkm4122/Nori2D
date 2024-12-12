#include "pch.h"
#include "Components.h"
#include "DxShader.h"

extern Dx11Vars g_Dx11;

VsShader::VsShader(const WCHAR* fileName)
{
	HRESULT result;

	result = D3DReadFileToBlob(fileName, &mBlob);
	result = g_Dx11.device->CreateVertexShader(mBlob->GetBufferPointer(),
											   mBlob->GetBufferSize(),
											   0,
											   &mVertexShader );

}

VsShader::~VsShader()
{
	SAFE_RELEASE(mBlob)
	SAFE_RELEASE(mVertexShader)
}


PsShader::PsShader(const WCHAR* fileName)
{
	HRESULT result;
	ID3DBlob* mBlob = nullptr;

	result = D3DReadFileToBlob(fileName, &mBlob);
	result = g_Dx11.device->CreatePixelShader(mBlob->GetBufferPointer(),
											   mBlob->GetBufferSize(),
											   0,
											   &mPixelShader );
	mBlob->Release();
}

PsShader::~PsShader()
{
	SAFE_RELEASE(mPixelShader)
}




