#include "pch.h"
#include "Components.h"
#include "Dx2DRenderer.h"
#include "tk/DDSTextureLoader.h"
#include "tk/WICTextureLoader.h"

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


