#pragma once

class DxTexture;

struct Dx2DRenderable 
{
	int x,y;
	int w,h;
	float ancherX, ancherY;
	XFloat4 color;

	DxTexture* tex;
};



