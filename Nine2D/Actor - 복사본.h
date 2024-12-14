#pragma once
#include "Dx2DRenderable.h"
#include "IGame.h"

class Actor 
{
public:
    virtual void Update(float delta);

    Dx2DRenderable mRd;
    Vec2 mSpeed = {1.f,1.f};

    std::vector<IActorComponent*> mCompList;
};


class MoveAction : public IActorComponent
{
public:
    MoveAction();
    void RandomTarget(Actor* aa);
	void Update(Actor* sp, float delta) override;

};


