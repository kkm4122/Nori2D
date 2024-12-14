#include "pch.h"
#include "Components.h"
#include "Dx2DRenderer.h"
#include "Quad.h"

using namespace DirectX;

ID3D11InputLayout*  Quad::mVertexLayout = nullptr;

// VERTEX gVT[4*1000]; 

void RenderableToVertex(VERTEX* vt, Dx2DRenderable* rd)
{
	float ax = (rd->w * rd->ancherX);
	float ay = (rd->h * rd->ancherY);
	float lx = rd->position.x - ax;
	float ty = rd->position.y + ay;
	float rx = lx + rd->w;
	float by = ty - rd->h;

	float flx = (lx - g_Dx11.half_width) / g_Dx11.half_width;
	float frx = (rx - g_Dx11.half_width) / g_Dx11.half_width;
	float fty = (ty - g_Dx11.half_height) / g_Dx11.half_height;
	float fby = (by - g_Dx11.half_height) / g_Dx11.half_height;


    vt[0] = {flx, fby, 0.0f, 0.f, 1.f};
	vt[1] = {flx, fty, 0.0f, 0.f, 0.f};
	vt[2] = {frx, fby, 0.0f, 1.f, 1.f};
	vt[3] = {frx, fty, 0.0f, 1.f, 0.f};

}



ID3D11Buffer* Update_VB_Renderable2(Dx2DRenderable2* rd)
{
	float ax = (rd->w * rd->ancherX);
	float ay = (rd->h * rd->ancherY);
	float lx = rd->position.x - ax;
	float ty = rd->position.y + ay;
	float rx = lx + rd->w;
	float by = ty - rd->h;

	float flx = (lx - g_Dx11.half_width) / g_Dx11.half_width;
	float frx = (rx - g_Dx11.half_width) / g_Dx11.half_width;
	float fty = (ty - g_Dx11.half_height) / g_Dx11.half_height;
	float fby = (by - g_Dx11.half_height) / g_Dx11.half_height;


    VERTEX OurVertices[4] =
    {
        {flx, fby, 0.0f, 0.f, 1.f},
        {flx, fty, 0.0f, 0.f, 0.f},
        {frx, fby, 0.0f, 1.f, 1.f},
        {frx, fty, 0.0f, 1.f, 0.f}
    };

	ID3D11Buffer *mVertexBuffer = (ID3D11Buffer *) rd->vb;

	if(rd->vb == nullptr) 
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX) * 4;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0; 

		hr = g_Dx11.device->CreateBuffer(&bd, NULL, &mVertexBuffer);       // create the buffer	
		rd->vb = mVertexBuffer;
	}

	g_Dx11.context->UpdateSubresource( mVertexBuffer, 0, nullptr, OurVertices, 0, 0 );

	/*
    D3D11_MAPPED_SUBRESOURCE ms;
    g_Dx11.context->Map(mVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, OurVertices, sizeof(OurVertices) );
    g_Dx11.context->Unmap(mVertexBuffer, NULL);
	*/

	return mVertexBuffer;
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


HRESULT Quad::create()
{
	HRESULT hr;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * mVertexCount;  // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    hr = g_Dx11.device->CreateBuffer(&bd, NULL, &mVertexBuffer);       // create the buffer
	return hr;
}

HRESULT Quad::createInputLayout(ID3DBlob* mBlob)
{
	HRESULT hr;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE(layout);

	hr = g_Dx11.device->CreateInputLayout(layout, 
										  numElements, 
										  mBlob->GetBufferPointer(),
										  mBlob->GetBufferSize(),
										  &mVertexLayout);

	return hr;
}

Quad::~Quad()
{
	SAFE_RELEASE(mVertexBuffer)
	SAFE_RELEASE(mVertexLayout)

}

void Quad::Update(Dx2DRenderable* rd)
{
	float ax = (rd->w * rd->ancherX);
	float ay = (rd->h * rd->ancherY);
	float lx = rd->position.x - ax;
	float ty = rd->position.y + ay;
	float rx = lx + rd->w;
	float by = ty - rd->h;

	float flx = (lx - g_Dx11.half_width) / g_Dx11.half_width;
	float frx = (rx - g_Dx11.half_width) / g_Dx11.half_width;
	float fty = (ty - g_Dx11.half_height) / g_Dx11.half_height;
	float fby = (by - g_Dx11.half_height) / g_Dx11.half_height;


    VERTEX OurVertices[4] =
    {
        {flx, fby, 0.0f, 0.f, 1.f},
        {flx, fty, 0.0f, 0.f, 0.f},
        {frx, fby, 0.0f, 1.f, 1.f},
        {frx, fty, 0.0f, 1.f, 0.f}
    };

	// g_Dx11.context->UpdateSubresource( mVertexBuffer, 0, nullptr, OurVertices, 0, 0 );

	
    D3D11_MAPPED_SUBRESOURCE ms;
    g_Dx11.context->Map(mVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, OurVertices, sizeof(OurVertices) );
    g_Dx11.context->Unmap(mVertexBuffer, NULL);
	
}

void Quad::Draw(Dx2DRenderable* sp)
{
	Update(sp);

	g_Dx11.context->IASetInputLayout(mVertexLayout);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	g_Dx11.context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	g_Dx11.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}

