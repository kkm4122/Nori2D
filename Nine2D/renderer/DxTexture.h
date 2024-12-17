#pragma once
#include "DxShader.h"
#include "IGame.h"
#include "ConstantBuffer.h"

struct Dx2DRenderable;
struct VERTEX;

class DxTexture
{
public:
	DxTexture() : mTextureRV(nullptr), mName(nullptr) {}
	~DxTexture();
    bool isNull() const { return mTextureRV == nullptr; }
    bool isNotNull() const { return mTextureRV != nullptr; }	
	void setNull() { if(mTextureRV) mTextureRV->Release(); mTextureRV=nullptr; }

    DxTexture& operator=(const DxTexture& lp)
	{ 
		setNull();
		mName = lp.mName;
		mTextureRV = lp.mTextureRV;
		mTextureRV->AddRef();

		return *this; 
	}

	void Draw();

	const WCHAR* mName = nullptr;
	ID3D11ShaderResourceView* mTextureRV;
};


class DxTextureMgr
{
public:
	static DxTextureMgr* get();

	DxTextureMgr();
	~DxTextureMgr();


	bool New(DxTexture& tex);
	bool Delete(DxTexture* tex);
	void DestroyAll();
	void GetUV(Dx2DRenderable* rd, VERTEX* Vt);
	ID3D11ShaderResourceView* Find(const WCHAR* name);

	std::map<std::wstring, ID3D11ShaderResourceView*> mTexMap;

};


