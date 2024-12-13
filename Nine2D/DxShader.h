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

	void Draw() {
		g_Dx11.context->VSSetShader(mVertexShader, 0, 0);
	}

	ID3DBlob* mBlob = nullptr;
	ID3D11VertexShader* mVertexShader;
};

class PsShader
{
public:
	PsShader(const WCHAR* fileName);
	~PsShader();

	void Draw() {
		g_Dx11.context->PSSetShader(mPixelShader, 0, 0);
	}
	ID3D11PixelShader* mPixelShader;
};










