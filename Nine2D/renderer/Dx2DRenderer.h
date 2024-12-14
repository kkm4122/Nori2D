#pragma once
#include "DxShader.h"
#include "IGame.h"
#include "ConstantBuffer.h"
#include "Dx2DRenderable.h"
#include "DxTexture.h"

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
	void Draw(Dx2DRenderable* sp);

	int mVertexCount = 4;
	ID3D11Buffer *mVertexBuffer = nullptr;  
	static ID3D11InputLayout*  mVertexLayout;

};


class Dx2DRenderer : public IRenderer
{
	HRESULT create();
	ID3D11Buffer* creaVB();

	HRESULT createBS();
	HRESULT createSampler();
public:
	static Dx2DRenderer* g;

	Dx2DRenderer() { g=this; create(); }
	~Dx2DRenderer();
	void Draw(Dx2DRenderable* sprite) override;
	void Draw2(Dx2DRenderable2* sprite) override;

public:
	VsShader*	mVS = nullptr;
	PsShader*	mPS = nullptr;
	Quad*		mQuad = nullptr;
	ConstantBuffer<CBChangesEveryFrame> mCB;

	ID3D11SamplerState* mSamplerLinear = nullptr;
	ID3D11BlendState*	mBlendState = nullptr;
};



