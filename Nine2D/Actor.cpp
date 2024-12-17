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
    
    Vec2 target((float)x, (float)y);
    aa->mSpeed = target - aa->mRd.position;
    aa->mSpeed.normalize();
    aa->mSpeed *= 100.f;

    float angle = aa->mSpeed.getAngle();
    angle = MATH_RAD_TO_DEG(angle);
    char buf[128];
    sprintf_s(buf, 128, "%.1f\n", angle);
    OutputDebugStringA(buf);

    if (angle > 0)
    {

    }
    else

    //aa->mRd.dir
}

void MoveAction::Update(Actor* aa, float delta)
{
    aa->mRd.position += (aa->mSpeed*delta);

    if (aa->mRd.position.x > (float)g_Dx11.width ||
        aa->mRd.position.x < 100.f ||
        aa->mRd.position.y > (float)g_Dx11.height ||
        aa->mRd.position.y < 100.f ) 
    { 
        RandomTarget(aa);
    }
	
}
