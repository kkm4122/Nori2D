#pragma once
#include "IGame.h"
#include "Actor.h"

class DemoGame : public IGame
{
	std::vector<Actor*>  mActorList;
	int count = 0;
	IRenderer* mRD = nullptr;

public:
	DemoGame();
	~DemoGame();
	void sample();

	void Update(float delta) override;
	void Draw(IRenderer* rd) override;


};

