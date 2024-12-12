#pragma once


template<typename T>
class ConstantBuffer
{
public:
	ConstantBuffer() {}
	~ConstantBuffer() { 
		if (mConstantBuffer) { mConstantBuffer->Release(); } 
		mConstantBuffer = nullptr; 
	}

	HRESULT Create()
	{
		D3D11_BUFFER_DESC bd = {};
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(T);
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		// D3D11_SUBRESOURCE_DATA initData = {};
		// initData.pSysMem = nullptr;
		return g_Dx11.device->CreateBuffer(&bd, nullptr, &mConstantBuffer);
	}

	void SetData(T const& value) noexcept
	{
		D3D11_MAPPED_SUBRESOURCE mappedResource;
		if (SUCCEEDED(g_Dx11.context->Map(mConstantBuffer,
										  0,
										  D3D11_MAP_WRITE_DISCARD,
										  0,
										  &mappedResource)))
		{
			*static_cast<T*>(mappedResource.pData) = value;

			g_Dx11.context->Unmap(mConstantBuffer, 0);
		}
	}

public:
	ID3D11Buffer* mConstantBuffer = nullptr;
};





