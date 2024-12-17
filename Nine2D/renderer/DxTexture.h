#pragma once
#include "DxShader.h"
#include "IGame.h"
#include "ConstantBuffer.h"

struct Dx2DRenderable;
struct VERTEX;

struct STexture
{
	const WCHAR* mName = nullptr;
	ID3D11ShaderResourceView* mTextureRV = nullptr;

    bool isNull() const { return mTextureRV == nullptr; }
    bool isNotNull() const { return mTextureRV != nullptr; }	
	void Draw();
};

class DxTexture : public STexture
{
public:
	~DxTexture();
	void setNull() { if(mTextureRV) mTextureRV->Release(); mTextureRV=nullptr; }

    DxTexture& operator=(const DxTexture& lp)
	{ 
		setNull();
		mName = lp.mName;
		mTextureRV = lp.mTextureRV;
		mTextureRV->AddRef();

		return *this; 
	}
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


