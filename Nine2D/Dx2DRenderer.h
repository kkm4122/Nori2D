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
	Dx2DRenderer() { create(); }
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



class Demo : public IDraw
{
	Dx2DRenderer* mRenderer;
	Dx2DRenderable  mSample;
	int count = 0;
public:
	Demo() {
		mRenderer = new Dx2DRenderer;
		sample();

		CBChangesEveryFrame cb;
		cb.vMeshColor = { 0.5f, 0.5f, 0.5f, 1.f };
		mRenderer->mCB.SetData(cb);
	}

	void sample()
	{
		mSample.x = g_Dx11.half_width;
		mSample.y = g_Dx11.half_height;
		mSample.w = 100;
		mSample.h = 100;
		mSample.ancherX = 0.5f;
		mSample.ancherY = 0.5f;
		mSample.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		mSample.tex = mRenderer->mTextureRV;
	}

	~Demo() {
		SAFE_DELETE(mRenderer);
	}

	void Draw() override
	{
		//for(int i=0; i<2000;++i)
		{
			Update();
			mRenderer->Draw(&mSample);	
		
		}
	}

	void Update()
	{
		if(++count < 100) return;
		
		count = 0;

		mSample.x += 1;
		mSample.y += 1;

		if(mSample.x > g_Dx11.width-mSample.w) mSample.x = mSample.w;
		if(mSample.y > g_Dx11.height-mSample.h) mSample.y = mSample.h;
	}
};

