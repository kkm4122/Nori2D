#pragma once
#include "IGame.h"
#include "Dx2DRenderer.h"

class DemoGame : public IGame
{
	Dx2DRenderable mSample;
	std::vector<Dx2DRenderable*>  mRenderableItemList;
	int count = 0;
	IRenderer* mRD = nullptr;

public:
	DemoGame();
	~DemoGame();
	void sample();

	void Update(float delta) override;
	void Draw(IRenderer* rd) override;


};

