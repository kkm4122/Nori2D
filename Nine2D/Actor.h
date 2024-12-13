#pragma once
#include "Dx2DRenderable.h"
#include "IGame.h"

class Actor :
    public Dx2DRenderable
{
public:
    virtual void Update(float delta);

    XFloat2 mSpeed = {1.f,1.f};

    std::vector<IActorComponent*> mCompList;
};


class MoveComp : public IActorComponent
{
public:
	void Update(Actor* sp, float delta) override;

};