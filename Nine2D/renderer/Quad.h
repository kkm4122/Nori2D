#pragma once



class Quad
{
	HRESULT create();
public:
	Quad() { create(); }
	~Quad();
	HRESULT createInputLayout(ID3DBlob* mBlob);

	void Update(Dx2DRenderable* rd);
	void Draw(Dx2DRenderable* sp);

	int mVertexCount = 4;
	ID3D11Buffer *mVertexBuffer = nullptr;  
	static ID3D11InputLayout*  mVertexLayout;

};




