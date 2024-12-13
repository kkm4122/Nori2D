#include "pch.h"
#include "Actor.h"

void Actor::Update(float delta)
{
    for (auto comp:mCompList) {
        comp->Update(this, delta);
    }
}

MoveAction::MoveAction()
{
    
}

void MoveAction::RandomTarget(Actor* aa)
{
    int x = rand() % g_Dx11.width;
    int y = rand() % g_Dx11.height;
    float mTargetX = ((float)x);
    float mTargetY = ((float)y);

    aa->mSpeed.x = mTargetX- aa->mRd.x; 
    aa->mSpeed.y = mTargetY- aa->mRd.y;
    aa->mSpeed.normalize();
    aa->mSpeed.x *= 100.f;
    aa->mSpeed.y *= 100.f;
}

void MoveAction::Update(Actor* aa, float delta)
{
    aa->mRd.x += (aa->mSpeed.x*delta);
    aa->mRd.y += (aa->mSpeed.y*delta);

    if (aa->mRd.x > (float)g_Dx11.width ||
        aa->mRd.x < 100.f ||
        aa->mRd.y > (float)g_Dx11.height ||
        aa->mRd.y < 100.f ) 
            { RandomTarget(aa);}
	
}
