#include "pch.h"
#include "Components.h"
#include "Dx2DRenderer.h"
#include "tk/DDSTextureLoader.h"
#include "tk/WICTextureLoader.h"

using namespace DirectX;



HRESULT Dx2DRenderer::create()
{
	HRESULT hr;

	mVS = new VsShader(L"VS.cso");
	mPS = new PsShader(L"PS.cso");
	mQuad = new Quad;

	createCB();
	createTex();
	hr = createBS();

	return hr;
}

HRESULT Dx2DRenderer::createBS()
{
	HRESULT hr;

	CD3D11_BLEND_DESC desc;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	hr = g_Dx11.device->CreateBlendState(&desc, &mBlendState);
	return hr;
}

HRESULT Dx2DRenderer::createCB()
{
	return mCB.Create(); 
}

HRESULT Dx2DRenderer::createTex()
{
	HRESULT hr;

	hr = CreateWICTextureFromFile(g_Dx11.device, L"Carrot.png", nullptr, &mTextureRV);
	if (FAILED(hr))
		return hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = g_Dx11.device->CreateSamplerState(&sampDesc, &mSamplerLinear);
	if (FAILED(hr))
		return hr;

	return hr;
}


Dx2DRenderer::~Dx2DRenderer()
{
	SAFE_DELETE(mVS);
	SAFE_DELETE(mPS);
	SAFE_DELETE(mQuad);
}

void Dx2DRenderer::Draw(Dx2DRenderable* sp)
{
	mQuad->Update(sp);

	CBChangesEveryFrame cb;
	cb.vMeshColor = sp->color;
	mCB.SetData(cb);

	if( !mQuad->mVertexLayout ) mQuad->createInputLayout(mVS->mBlob);
	g_Dx11.context->IASetInputLayout(mQuad->mVertexLayout);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	g_Dx11.context->IASetVertexBuffers(0, 1, &mQuad->mVertexBuffer, &stride, &offset);
	g_Dx11.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	g_Dx11.context->VSSetShader(mVS->mVertexShader, 0, 0);

	g_Dx11.context->PSSetConstantBuffers(0, 1, &mCB.mConstantBuffer);
	g_Dx11.context->PSSetShaderResources(0, 1, &sp->tex);
	g_Dx11.context->PSSetSamplers(0, 1, &mSamplerLinear);
    g_Dx11.context->PSSetShader(mPS->mPixelShader, 0, 0);

	g_Dx11.context->OMSetBlendState(mBlendState, 0, 0xFFFFFFFF);
	g_Dx11.context->Draw(mQuad->mVertexCount, 0);
}



HRESULT Quad::create()
{
	mVertexCount = 4;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * mVertexCount;  // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    g_Dx11.device->CreateBuffer(&bd, NULL, &mVertexBuffer);       // create the buffer


	return E_NOTIMPL;
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

	return E_NOTIMPL;
}

Quad::~Quad()
{
	SAFE_RELEASE(mVertexBuffer)
}

void Quad::Update(Dx2DRenderable* rd)
{
	int ax = (rd->w * rd->ancherX);
	int ay = (rd->h * rd->ancherY);
	int lx = rd->x - ax;
	int ty = rd->y + ay;
	int rx = lx + rd->w;
	int by = ty - rd->h;

	float flx =  (float)(lx - g_Dx11.half_width) / (float)g_Dx11.half_width;
	float frx = (float)(rx - g_Dx11.half_width) / (float)g_Dx11.half_width;
	float fty = (float)(ty - g_Dx11.half_height) / (float)g_Dx11.half_height;
	float fby = (float)(by - g_Dx11.half_height) / (float)g_Dx11.half_height;


    VERTEX OurVertices[4] =
    {
        {flx, fby, 0.0f, 0.f, 1.f},
        {flx, fty, 0.0f, 0.f, 0.f},
        {frx, fby, 0.0f, 1.f, 1.f},
        {frx, fty, 0.0f, 1.f, 0.f}
    };

    D3D11_MAPPED_SUBRESOURCE ms;
    g_Dx11.context->Map(mVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
    memcpy(ms.pData, OurVertices, sizeof(OurVertices) );
    g_Dx11.context->Unmap(mVertexBuffer, NULL);

}


