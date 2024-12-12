#include "pch.h"
#include "DemoGame.h"

DemoGame::DemoGame() 
{
	mRenderer = new Dx2DRenderer;
	sample();

	CBChangesEveryFrame cb;
	cb.vMeshColor = {0.5f, 0.5f, 0.5f, 1.f};
	mRenderer->mCB.SetData(cb);
}

DemoGame::~DemoGame() 
{
	SAFE_DELETE(mRenderer);
}

void DemoGame::Draw()
{
	//for(int i=0; i<2000;++i)
	{
		mRenderer->Draw(&mSample);

	}
}

void DemoGame::Update()
{
	if (++count < 100) return;

	count = 0;

	mSample.x += 1;
	mSample.y += 1;

	if (mSample.x > g_Dx11.width-mSample.w) mSample.x = mSample.w;
	if (mSample.y > g_Dx11.height-mSample.h) mSample.y = mSample.h;
}


void DemoGame::sample()
{
	mSample.x = g_Dx11.half_width;
	mSample.y = g_Dx11.half_height;
	mSample.w = 100;
	mSample.h = 100;
	mSample.ancherX = 0.5f;
	mSample.ancherY = 0.5f;
	mSample.color = {1.0f, 1.0f, 1.0f, 1.0f};
	mSample.tex = mRenderer->mTextureRV;
}


