#pragma once
#include "Components.h"
#include <functional>
#include "IDraw.h"

class DxWindow
{
public:
	DxWindow();
	~DxWindow();

	IDraw* mDrawObj = nullptr;

	static DxWindow* g;

	static LRESULT CALLBACK WindowProc(
		HWND hWnd,		// Window handle
		UINT uMsg,		// Message
		WPARAM wParam,	// Message's first parameter
		LPARAM lParam	// Message's second parameter
		);

	HRESULT InitWindow();
	HRESULT InitDirectX();	
	HRESULT Run();
	void Draw();
	HRESULT MessageLoop();

	LRESULT onMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// Convenience methods for handling mouse input, since we
	// can easily grab mouse input from OS-level messages
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);

	void UpdateTimer();

	void UpdateTitleBarStats();

};

