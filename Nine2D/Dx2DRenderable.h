#pragma once



struct Dx2DRenderable 
{
	int x,y;
	int w,h;
	float ancherX, ancherY;
	XFloat4 color;

	ID3D11ShaderResourceView* tex;
};



