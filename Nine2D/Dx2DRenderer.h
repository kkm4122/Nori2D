#pragma once
#include "DxShader.h"
#include "IDraw.h"
#include "ConstantBuffer.h"
#include "Dx2DRenderable.h"


struct VS_CONSTANT_BUFFER
{
	float color_R;
	float color_G;
	float color_B;

	float pos_X;
	float pos_Y;
};


struct VERTEX
{
      FLOAT X, Y, Z;
	  FLOAT U, V;
};


struct CBChangesEveryFrame
{
	XFloat4 vMeshColor;
};



class Quad
{
	HRESULT create();
public:
	Quad() { create(); }
	~Quad();
	HRESULT createInputLayout(ID3DBlob* mBlob);

	void Update(Dx2DRenderable* rd);

	int mVertexCount = 0;
	ID3D11Buffer *mVertexBuffer = nullptr;  
	ID3D11InputLayout*  mVertexLayout = nullptr;

};



class Dx2DRenderer
{
	HRESULT create();

	HRESULT createBS();
	HRESULT createCB();
	HRESULT createTex();
public:
	static Dx2DRenderer* g;

	Dx2DRenderer() { g=this; create(); }
	~Dx2DRenderer();
	void Draw(Dx2DRenderable* sprite);

public:
	VsShader*	mVS = nullptr;
	PsShader*	mPS = nullptr;
	Quad*	mQuad = nullptr;
	ConstantBuffer<CBChangesEveryFrame> mCB;
	ID3D11ShaderResourceView* mTextureRV = nullptr;
	ID3D11SamplerState* mSamplerLinear = nullptr;
	ID3D11BlendState* mBlendState = nullptr;
};



