#pragma once
#include "IDraw.h"
#include "Dx2DRenderer.h"

class DemoGame :
    public IDraw
{
	Dx2DRenderable mSample;
	std::vector<Dx2DRenderable*>  mRenderableItemList;

	int count = 0;
public:
	DemoGame();
	~DemoGame();

	void sample();
	void Draw() override;
	void Update() override;

};

