#pragma once
#include "DxTexture.h"

struct VERTEX;

struct Dx2DRenderable 
{
	Vec2 position;
	float angle;

	float w,h;
	float ancherX, ancherY;
	XFloat4 color;

	DxTexture tex;
};

struct Dx2DRenderable2
{
	Vec2 position;
	float angle;

	float w,h;
	float ancherX, ancherY;
	XFloat4 color;

	const WCHAR* texName = nullptr;
	ID3D11ShaderResourceView* tex = nullptr;
	ID3D11Buffer *vb = nullptr;
};


class IRenderer
{
public:
	virtual void Draw(Dx2DRenderable* sprite) = 0;
	virtual void Draw2(Dx2DRenderable2* sprite) = 0;
};

