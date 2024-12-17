#include "pch.h"
#include "DemoGame.h"
#include "renderer\Dx2DRenderer.h"
#include "Actor.h"

MoveAction mv;

bool compare(Actor* a, Actor* b) { return a->mRd.position.y < b->mRd.position.y; }


DemoGame::DemoGame() 
{
}

DemoGame::~DemoGame() 
{
	for(auto a : mActorList) {
		delete a;
	}
}

void DemoGame::Draw(IRenderer* rd)
{

	std::sort(mActorList.begin(), mActorList.end(), compare);

	for(int i=0; i<mActorList.size(); ++i)
	{
		rd->Draw(&mActorList[i]->mRd);
	}
}

void DemoGame::Update(float delta)
{
	for(auto a : mActorList) {
		a->Update(delta);
	}

}

void DemoGame::sample()
{
	for(int i=0; i<1000; ++i) {
		sample1();
	}
}

void DemoGame::sample1()
{
	Actor* aa = new Actor;
	aa->mRd.position.x = g_Dx11.half_width;
	aa->mRd.position.y = g_Dx11.half_height;
	aa->mRd.w = 100;
	aa->mRd.h = 100;
	aa->mRd.ancherX = 0.5f;
	aa->mRd.ancherY = 0.5f;
	aa->mSpeed = {10.f,5.f};
	aa->mRd.color = {1.0f, 1.0f, 1.0f, 1.0f};
	aa->mRd.dir = 0;
	aa->mRd.frameNo = 0;
	aa->mRd.AnimTime = 0;
	aa->mRd.tex.mName = ULTRA_NAME;

	auto mv1 = new MoveAction;
	mv1->RandomTarget(aa);
	aa->mCompList.push_back(mv1);

	mActorList.push_back(aa);
}


