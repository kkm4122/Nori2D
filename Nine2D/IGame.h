#pragma once

class IRenderer;
class Actor;

class IGame
{
public:
	virtual void Update(float delta) = 0;
	virtual void Draw(IRenderer* rd) = 0;
	virtual void getMouse(int x, int y) = 0;
};


class ITickObj
{
public:
	virtual void Tick() = 0;
};


class IActorComponent
{
public:
	virtual void Update(Actor* sp, float delta) = 0;
};


