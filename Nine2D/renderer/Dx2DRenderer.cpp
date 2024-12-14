#include "pch.h"
#include "Components.h"
#include "Dx2DRenderer.h"
#include "tk/DDSTextureLoader.h"
#include "tk/WICTextureLoader.h"

using namespace DirectX;

Dx2DRenderer* Dx2DRenderer::g = nullptr;





HRESULT Dx2DRenderer::create()
{
	HRESULT hr;

	mVS = new VsShader(L"VS.cso");
	mPS = new PsShader(L"PS.cso");
	mQuad = new Quad;

	if (!mQuad->mVertexLayout) { 
		mQuad->createInputLayout(mVS->mBlob);
	}

	hr = mCB.Create();
	hr = createSampler();
	hr = createBS();
	
	//creaVB();

	CBChangesEveryFrame cb;
	cb.vMeshColor = {0.5f, 0.5f, 0.5f, 1.f};
	mCB.SetData(cb);


	return hr;
}

ID3D11Buffer* Dx2DRenderer::creaVB()
{
	HRESULT hr;
	ID3D11Buffer* vb;

    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(VERTEX) * 100;  // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    hr = g_Dx11.device->CreateBuffer(&bd, NULL, &vb);       // create the buffer
	if (FAILED(hr))
		return nullptr;

	return vb;
}

HRESULT Dx2DRenderer::createBS()
{
	HRESULT hr;

	CD3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = g_Dx11.device->CreateBlendState(&desc, &mBlendState);
	return hr;
}

HRESULT Dx2DRenderer::createSampler()
{
	HRESULT hr;
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
	//SAFE_RELEASE(mVertexBuffer);
	SAFE_DELETE(mVS);
	SAFE_DELETE(mPS);
	SAFE_DELETE(mQuad);
}




void Dx2DRenderer::Draw(Dx2DRenderable* sp)
{
	if(sp->tex.isNull()) {
		DxTextureMgr::get()->New(sp->tex);
	}

	mQuad->Draw(sp);

	CBChangesEveryFrame cb;
	cb.vMeshColor = sp->color;
	mCB.SetData(cb);

	mVS->Draw();
	mPS->Draw();

	g_Dx11.context->PSSetConstantBuffers(0, 1, &mCB.mConstantBuffer);
	g_Dx11.context->PSSetSamplers(0, 1, &mSamplerLinear);
	sp->tex.Draw();

	g_Dx11.context->OMSetBlendState(mBlendState, 0, 0xFFFFFFFF);
	g_Dx11.context->Draw(mQuad->mVertexCount, 0);
}




void Dx2DRenderer::Draw2(Dx2DRenderable2* sp)
{
	if(sp->tex == nullptr) {
		sp->tex = DxTextureMgr::get()->Find(sp->texName);
	}

	ID3D11Buffer* vb = mQuad->mVertexBuffer;

	mQuad->Draw( (Dx2DRenderable*)sp);

	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	g_Dx11.context->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	g_Dx11.context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	mVS->Draw();
	mPS->Draw();

	g_Dx11.context->PSSetConstantBuffers(0, 1, &mCB.mConstantBuffer);
	g_Dx11.context->PSSetSamplers(0, 1, &mSamplerLinear);
	g_Dx11.context->PSSetShaderResources(0, 1, &sp->tex);

	g_Dx11.context->OMSetBlendState(mBlendState, 0, 0xFFFFFFFF);
	g_Dx11.context->Draw(mQuad->mVertexCount, 0);
}





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
