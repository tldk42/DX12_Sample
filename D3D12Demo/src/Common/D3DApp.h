#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3dUtil.h"
#include "GameTimer.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

class D3DApp
{
protected:
	D3DApp(HINSTANCE hInstance);
	D3DApp(const D3DApp& rhs) = delete;
	D3DApp& operator=(const D3DApp& rhs) = delete;
	virtual ~D3DApp();

public:
	static D3DApp* GetApp();

	HINSTANCE AppInst() const;
	HWND MainWnd() const;
	float AspectRatio() const;

	bool Get4xMsaaState() const;
	void Set4xMsaaState(bool value);

	int Run();

	virtual bool Initialize();
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

protected:
	virtual void CreateRtvAndDsvDescriptorHeaps();
	virtual void OnResize();
	virtual void Update(const GameTimer& gt) = 0;
	virtual void Draw(const GameTimer& gt) = 0;

	virtual void OnMouseDown(WPARAM btnState, int x, int y) {};
	virtual void OnMouseUp(WPARAM btnState, int x, int y) {};
	virtual void OnMouseMove(WPARAM btnState, int x, int y) {};

protected:
	bool InitMainWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();

	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer() const
	{
		return mSwapChainBuffer[mCurrBackBuffer].Get();
	}

	/**
	 * \brief �־��� �����¿� �ش��ϴ� �ĸ� ���� RTV�� �ڵ��� ��ȯ
	 */
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const
	{
		return CD3DX12_CPU_DESCRIPTOR_HANDLE(
			mRtvHeap->GetCPUDescriptorHandleForHeapStart(),
			mCurrBackBuffer,
			mRtvDescriptorSize
		);
	}

	/**
	 * \brief �־��� �����¿� �ش��ϴ� �ĸ� ���� DSV�� �ڵ��� ��ȯ
	 */
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const
	{
		return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	}

	void CalculateFrameStats() const;

	void LogAdapters();
	void LogAdapterOutputs(IDXGIAdapter* adapter);
	void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);


protected:
	static D3DApp* mApp;

	HINSTANCE mhAppInstance = nullptr;
	HWND mhMainWnd = nullptr;
	bool bAppPaused = false;
	bool bMinimized = false;
	bool bMaximized = false;
	bool bResizing = false;
	bool bFullScreenState = false;

	bool b4xMsaaState = false;
	UINT m4xMsaaQuality = 0;

	GameTimer mTimer;

	Microsoft::WRL::ComPtr<IDXGIFactory4> mdxgiFactory;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Device> md3dDevice;

	Microsoft::WRL::ComPtr<ID3D12Fence> mFence; // GPU�� ��� ��⿭�� ��ɵ� �� Ư�� �������� ó�� �Ϸ�ɶ� ���� CPU�� ��ٸ��� ��Ÿ���� ģ��.
	UINT64 mCurrentFence = 0; // ��Ÿ�� ��ü�� UINT64��(�ð����� Ư�� ��Ÿ�� ������ �ĺ�)�� �ϳ� �����Ѵ�.

	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandQueue; // GPU�� ��� ��⿭ D3D12_COMMAND_QUEUE_DESC ����ü�� ä�� �� CreateCommandQueue�� ȣ���ؾ��Ѵ�.
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc; //
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> mCommandList; //

	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mDsvHeap;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvDescriptorSize = 0;

	std::wstring mMainWndCaption = L"Sample RenderApp";
	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	int mClientWidth = 1600;
	int mClientHeight = 900;
};
