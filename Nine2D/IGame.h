#pragma once

class IRenderer;

class IGame
{
public:
	virtual void Update(float delta) = 0;
	virtual void Draw(IRenderer* rd) = 0;
};


class ITickObj
{
public:
	virtual void Tick() = 0;
};


