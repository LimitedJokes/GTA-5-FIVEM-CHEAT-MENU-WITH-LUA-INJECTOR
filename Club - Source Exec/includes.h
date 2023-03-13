#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <dxgi.h>
#include "kiero/kiero.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

// Define a function pointer type named "Present"
// The function takes a pointer to an IDXGISwapChain object, and two unsigned integers as parameters
// The function returns an HRESULT value
typedef HRESULT (__stdcall *Present)(IDXGISwapChain* pSwapChain, UINT syncInterval, UINT flags);

// Define a function pointer type named "WNDPROC"
// The function takes four parameters: a HWND (window handle), an unsigned integer (message), a WPARAM (word-sized parameter), and an LPARAM (long-sized parameter)
// The function returns an LRESULT value
typedef LRESULT (CALLBACK *WNDPROC)(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Define an integer type named "PTR" that is an alias of the uintptr_t type
typedef uintptr_t PTR;
