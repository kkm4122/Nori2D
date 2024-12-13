#include "pch.h"
#include "DemoGame.h"
#include "Dx2DRenderer.h"
#include "Actor.h"

MoveComp mv;

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
	//for(int i=0; i<2000;++i)
	for(int i=0; i<mActorList.size(); ++i)
	{
		rd->Draw(mActorList[i]);
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
	Actor* aa = new Actor;
	aa->x = 0;
	aa->y = 0;
	aa->w = 100;
	aa->h = 100;
	aa->ancherX = 0.5f;
	aa->ancherY = 0.5f;
	aa->mSpeed = {10.f,5.f};
	aa->color = {1.0f, 1.0f, 1.0f, 1.0f};
	aa->texName = L"Carrot.png";
	aa->tex = nullptr;

	aa->mCompList.push_back(&mv);

	mActorList.push_back(aa);
}


