#pragma once


class RefObject
{
protected:
	unsigned int _referenceCount;

public:
	RefObject() : _referenceCount(0) {} 
	virtual ~RefObject() {}
	unsigned int getReferenceCount() const { return _referenceCount; }
	void AddRef() 
	{
		++_referenceCount;
	}

	unsigned int Release()
	{
		--_referenceCount;

		unsigned int r = _referenceCount;

		if (_referenceCount == 0)
		{
			delete this;
		}
		return r;
	}
};


template<typename T>
class Ptr 
{
	T* mPtr;
public:
    Ptr() : mPtr(nullptr) {}
    Ptr(T* p) : mPtr(p) {}
    Ptr(const Ptr<T>& lp){
        mPtr = lp.mPtr;
        if (mPtr) mPtr->AddRef();
    }
    ~Ptr() {
		if(mPtr) mPtr->Release();
        mPtr = nullptr;
	}
    bool isNull() const { return mPtr == nullptr; }
    bool isNotNull() const { return mPtr != nullptr; }
	void setNull() { if(mPtr) mPtr->Release(); mPtr=nullptr; }
    void set(T* lp) {
        setNull();
        mPtr = lp;
        if (mPtr) mPtr->AddRef(); 
    }

    T* get() const { return mPtr; }
	T* const* GetAddressOf() const {return &mPtr;}
	T** GetAddressOf()  {return &mPtr;}

    T* operator->() const{ return mPtr; }
	T& operator*() { return *mPtr; }

    T* operator=(T* lp) { set(lp); return mPtr; }
    T* operator=(const Ptr<T>& lp) { set(lp.mPtr); return mPtr; }
	
};
