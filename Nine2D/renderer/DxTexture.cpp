#include "pch.h"
#include "Components.h"
#include "Dx2DRenderer.h"
#include "tk/DDSTextureLoader.h"
#include "tk/WICTextureLoader.h"
#include "AnimationData.h"
#include "Dx2DRenderable.h"

using namespace DirectX;

DxTextureMgr* g_DxTextureMgr = nullptr;

DxTexture::~DxTexture()
{
	if (mTextureRV) {
		ULONG refcount = mTextureRV->Release();
		if (refcount==0) DxTextureMgr::get()->Delete(this);
	}
}

void DxTexture::Draw()
{
	g_Dx11.context->PSSetShaderResources(0, 1, &mTextureRV);

}

DxTextureMgr* DxTextureMgr::get()
{
	if(!g_DxTextureMgr) 
		g_DxTextureMgr=new DxTextureMgr;

	return g_DxTextureMgr;
}

DxTextureMgr::DxTextureMgr()
{
}

DxTextureMgr::~DxTextureMgr()
{
	DestroyAll();
}

bool DxTextureMgr::New(DxTexture& tex)
{
	std::map<std::wstring, ID3D11ShaderResourceView*>::iterator it
		= mTexMap.find(tex.mName);
	if (it != mTexMap.end())
	{
		tex.mTextureRV = it->second;
		tex.mTextureRV->AddRef();
	}
	else 
	{
		ID3D11ShaderResourceView* rv =nullptr;
		HRESULT hr = CreateWICTextureFromFile(g_Dx11.device,
											  tex.mName,
											  nullptr,
											  &rv);
		mTexMap[tex.mName] = rv;
		tex.mTextureRV = rv;
	}

	return true;
}

bool DxTextureMgr::Delete(DxTexture* tex)
{
	mTexMap.erase(tex->mName);

	return true;
}

void DxTextureMgr::DestroyAll()
{
	for (auto tex : mTexMap) {
		tex.second->Release();
	}
}

void DxTextureMgr::GetUV(Dx2DRenderable* rd, VERTEX* Vt)
{
	AnimRectTime animTime = GetActorTime(rd);

	float FrameTime = animTime.totalTime / animTime.totalFrame;

	rd->AnimTime += g_Time.deltaTime;

	if (rd->AnimTime >= FrameTime)
	{
		rd->frameNo++;
		rd->AnimTime -= FrameTime;
		if (rd->frameNo >= animTime.totalFrame)
			rd->frameNo = 0;
	}

	UltraRect uv = GetActorUV(rd);

	Vt[0].U = uv.U1;
	Vt[0].V = uv.V2;
	Vt[1].U = uv.U1;
	Vt[1].V = uv.V1;
	Vt[2].U = uv.U2;
	Vt[2].V = uv.V2;
	Vt[3].U = uv.U2;
	Vt[3].V = uv.V1;
}

ID3D11ShaderResourceView* DxTextureMgr::Find(const WCHAR* name)
{
	std::map<std::wstring, ID3D11ShaderResourceView*>::iterator it
		= mTexMap.find(name);
	if (it != mTexMap.end())
	{
		return it->second;
	}
	else 
	{
		ID3D11ShaderResourceView* rv =nullptr;
		HRESULT hr = CreateWICTextureFromFile(g_Dx11.device,name,nullptr,&rv);
		mTexMap[name] = rv;
		return rv;
	}
}


