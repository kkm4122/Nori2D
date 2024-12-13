#include "pch.h"
#include "Actor.h"

void Actor::Update(float delta)
{
    for (auto comp:mCompList) {
        comp->Update(this, delta);
    }
}

void MoveComp::Update(Actor* aa, float delta)
{
    aa->x += (aa->mSpeed.x*delta);
    aa->y += (aa->mSpeed.y*delta);

	if (aa->x > g_Dx11.width - aa->w)  aa->x = aa->w;
	if (aa->y > g_Dx11.height - aa->h) aa->y = aa->h;
}
