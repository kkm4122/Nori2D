#pragma once
#include "IGame.h"
#include "Actor1.h"

class DemoGame1 : public IGame
{
	std::vector<Actor1*>  mActorList;
	int count = 0;
	IRenderer* mRD = nullptr;

public:
	DemoGame1();
	~DemoGame1();
	void sample();

	void Update(float delta) override;
	void Draw(IRenderer* rd) override;


};

