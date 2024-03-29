#include "brpch.h"
#include "D3DCore.h"

#include <d3d12.h>
#include <wrl/client.h>

namespace Bruno::Device::Core
{
    void EnableDebugLayer()
    {
        Microsoft::WRL::ComPtr<ID3D12Debug> debugInterface;
        if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface))))
        {
            debugInterface->EnableDebugLayer();
        }
        else
        {
            BR_CORE_WARN << "D3D12 Debug interface is not available." << std::endl;
        }
    }
}