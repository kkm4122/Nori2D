#pragma once

class DxTexture;

struct Dx2DRenderable 
{
	float x,y;
	float w,h;
	float ancherX, ancherY;
	XFloat4 color;

	DxTexture* tex = nullptr;
	const WCHAR* texName = nullptr;
};


class IRenderer
{
public:
	virtual void Draw(Dx2DRenderable* sprite) = 0;
};

