#pragma once

class DxShader
{
public:
	
};

class VsShader
{
public:
	VsShader(const WCHAR* fileName);
	~VsShader();

	ID3DBlob* mBlob = nullptr;
	ID3D11VertexShader* mVertexShader;
};

class PsShader
{
public:
	PsShader(const WCHAR* fileName);
	~PsShader();

	ID3D11PixelShader* mPixelShader;
};










