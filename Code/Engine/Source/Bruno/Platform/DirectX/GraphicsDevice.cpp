#include "brpch.h"
#include "GraphicsDevice.h"

#include "GraphicsAdapter.h"
#include "CommandQueue.h"
#include "UploadContext.h"
#include "GPUBuffer.h"
#include "Surface.h"

#include "D3D12MemAlloc.h"

#include <numeric>

namespace Bruno
{
	GraphicsDevice::GraphicsDevice(std::shared_ptr<GraphicsAdapter> adapter) :
		m_adapter(adapter)
	{
        if (!m_adapter)
        {
            m_adapter = GraphicsAdapter::Create();
        }

#if BR_DEBUG
        Microsoft::WRL::ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
        if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
        {
            m_dxgiFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;

            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
            dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

            DXGI_INFO_QUEUE_MESSAGE_ID hide[] =
            {
                80 /* IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides. */,
            };
            DXGI_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<uint32_t>(std::size(hide));
            filter.DenyList.pIDList = hide;
            dxgiInfoQueue->AddStorageFilterEntries(DXGI_DEBUG_DXGI, &filter);
        }
        
#endif
        ThrowIfFailed(CreateDXGIFactory2(m_dxgiFactoryFlags, IID_PPV_ARGS(m_dxgiFactory.ReleaseAndGetAddressOf())));

        // Determines whether tearing support is available for fullscreen borderless windows.
        //if (m_options & c_AllowTearing)
        //{
        //}

        // Create the DX12 API device object.
        HRESULT hr = D3D12CreateDevice(
            m_adapter->GetHandle(),
            m_d3dMinFeatureLevel,
            IID_PPV_ARGS(m_d3dDevice.ReleaseAndGetAddressOf())
        );
        ThrowIfFailed(hr);

        m_d3dDevice->SetName(L"GraphicsDevice");

#ifndef NDEBUG
        // Configure debug device (if active).
        Microsoft::WRL::ComPtr<ID3D12InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(m_d3dDevice.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D12_MESSAGE_ID hide[] =
            {
                D3D12_MESSAGE_ID_MAP_INVALID_NULLRANGE,
                D3D12_MESSAGE_ID_UNMAP_INVALID_NULLRANGE,
                // Workarounds for debug layer issues on hybrid-graphics systems
                D3D12_MESSAGE_ID_EXECUTECOMMANDLISTS_WRONGSWAPCHAINBUFFERREFERENCE,
                D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE,
            };
            D3D12_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<uint32_t>(std::size(hide));
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
#endif

        // Determine maximum supported feature level for this device
        static const D3D_FEATURE_LEVEL s_featureLevels[] =
        {
    #if defined(NTDDI_WIN10_FE) || defined(USING_D3D12_AGILITY_SDK)
            D3D_FEATURE_LEVEL_12_2,
    #endif
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
        };

        D3D12_FEATURE_DATA_FEATURE_LEVELS featLevels =
        {
            static_cast<uint32_t>(std::size(s_featureLevels)), s_featureLevels, D3D_FEATURE_LEVEL_11_0
        };

        hr = m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featLevels, sizeof(featLevels));
        if (SUCCEEDED(hr))
        {
            m_d3dFeatureLevel = featLevels.MaxSupportedFeatureLevel;
        }
        else
        {
            m_d3dFeatureLevel = m_d3dMinFeatureLevel;
        }

        m_graphicsQueue = std::make_unique<CommandQueue>(this, D3D12_COMMAND_LIST_TYPE_DIRECT);
        m_computeQueue = std::make_unique<CommandQueue>(this, D3D12_COMMAND_LIST_TYPE_COMPUTE);
        m_copyQueue = std::make_unique<CommandQueue>(this, D3D12_COMMAND_LIST_TYPE_COPY);

        m_rtvDescriptorHeap = std::make_unique<StagingDescriptorHeap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, Graphics::Core::RTV_STAGING_DESCRIPTORS_COUNT);
        m_dsvDescriptorHeap = std::make_unique<StagingDescriptorHeap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, Graphics::Core::DSV_STAGING_DESCRIPTORS_COUNT);
        m_srvDescriptorHeap = std::make_unique<StagingDescriptorHeap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, Graphics::Core::SRV_STAGING_DESCRIPTORS_COUNT);
        m_samplerRenderPassDescriptorHeap = std::make_unique<RenderPassDescriptorHeap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 0, Graphics::Core::SAMPLER_DESCRIPTORS_COUNT);

        for (uint32_t frameIndex = 0; frameIndex < Graphics::Core::FRAMES_IN_FLIGHT_COUNT; frameIndex++)
        {
            m_srvRenderPassDescriptorHeaps[frameIndex] = std::make_unique<RenderPassDescriptorHeap>(*this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, Graphics::Core::RESERVED_SRV_DESCRIPTORS_COUNT, Graphics::Core::SRV_RENDER_PASS_USER_DESCRIPTORS_COUNT);
        }

        D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};

        // This is the highest version the sample supports. If
        // CheckFeatureSupport succeeds, the HighestVersion returned will not be
        // greater than this.
        featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

        if (FAILED(m_d3dDevice->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE,
            &featureData,
            sizeof(featureData))))
        {
            featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
        }
        m_highestRootSignatureVersion = featureData.HighestVersion;

        D3D12MA::ALLOCATOR_DESC desc = {};
        desc.Flags = D3D12MA::ALLOCATOR_FLAG_NONE;
        desc.pDevice = m_d3dDevice.Get();
        desc.pAdapter = m_adapter.get()->GetHandle();

        D3D12MA::CreateAllocator(&desc, &m_allocator);

        BufferCreationDesc uploadBufferDesc;
        uploadBufferDesc.Size = AlignU32(10 * 1024 * 1024, 256);
        uploadBufferDesc.AccessFlags = BufferAccessFlags::HostWritable;

        BufferCreationDesc uploadTextureDesc;
        uploadTextureDesc.Size = AlignU32(80 * 1024 * 1024, 256);
        uploadTextureDesc.AccessFlags = BufferAccessFlags::HostWritable;

        for (uint32_t frameIndex = 0; frameIndex < Graphics::Core::FRAMES_IN_FLIGHT_COUNT; frameIndex++)
        {
            m_uploadContexts[frameIndex] = std::make_unique<UploadContext>(*this, std::make_unique<GPUBuffer>(*this, uploadBufferDesc), std::make_unique<GPUBuffer>(*this, uploadTextureDesc));
        }

        //D3D12_SAMPLER_DESC samplerDescs[Graphics::Core::SAMPLER_DESCRIPTORS_COUNT]{};
        //samplerDescs[0].Filter = D3D12_FILTER_ANISOTROPIC;
        //samplerDescs[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        //samplerDescs[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        //samplerDescs[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        //samplerDescs[0].BorderColor[0] = samplerDescs[0].BorderColor[1] = samplerDescs[0].BorderColor[2] = samplerDescs[0].BorderColor[3] = 0.0f;
        //samplerDescs[0].MipLODBias = 0.0f;
        //samplerDescs[0].MaxAnisotropy = 16;
        //samplerDescs[0].ComparisonFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        //samplerDescs[0].MinLOD = 0;
        //samplerDescs[0].MaxLOD = D3D12_FLOAT32_MAX;

        //DescriptorHandle samplerDescriptorBlock = m_samplerRenderPassDescriptorHeap->Allocate(Graphics::Core::SAMPLER_DESCRIPTORS_COUNT);
        //D3D12_CPU_DESCRIPTOR_HANDLE currentSamplerDescriptor = samplerDescriptorBlock.Cpu;
        //for (uint32_t samplerIndex = 0; samplerIndex < Graphics::Core::SAMPLER_DESCRIPTORS_COUNT; samplerIndex++)
        //{
        //    m_d3dDevice->CreateSampler(&samplerDescs[samplerIndex], currentSamplerDescriptor);
        //    //currentSamplerDescriptor.ptr += m_samplerRenderPassDescriptorHeap->GetDescriptorSize();
        //}

        //CD3DX12_STATIC_SAMPLER_DESC linearRepeatSampler(0, D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR);

        m_freeReservedDescriptorIndices.resize(Graphics::Core::RESERVED_SRV_DESCRIPTORS_COUNT);
        for (size_t i = 0; i < m_freeReservedDescriptorIndices.size(); i++)
        {
            m_freeReservedDescriptorIndices[i] = i;
        }
	}

    GraphicsDevice::~GraphicsDevice()
    {
        WaitForIdle();

        for (uint32_t frameIndex = 0; frameIndex < Graphics::Core::FRAMES_IN_FLIGHT_COUNT; frameIndex++)
        {
            m_srvRenderPassDescriptorHeaps[frameIndex] = nullptr;
            m_uploadContexts[frameIndex] = nullptr;
        }

        SafeRelease(m_allocator);
    }

    void GraphicsDevice::BeginFrame()
    {
        m_frameId = (m_frameId + 1) % Graphics::Core::FRAMES_IN_FLIGHT_COUNT;

        //Wait on fences from 2 frames ago.
        m_graphicsQueue->WaitForFenceCPUBlocking(m_endOfFrameFences[m_frameId].GraphicsQueueFence);
        m_computeQueue->WaitForFenceCPUBlocking(m_endOfFrameFences[m_frameId].ComputeQueueFence);
        m_copyQueue->WaitForFenceCPUBlocking(m_endOfFrameFences[m_frameId].CopyQueueFence);

        ProcessDestructions(m_frameId);

        m_uploadContexts[m_frameId]->ResolveProcessedUploads();
        m_uploadContexts[m_frameId]->Reset();

        m_contextSubmissions[m_frameId].clear();
    }

    void GraphicsDevice::EndFrame()
    {
        m_uploadContexts[m_frameId]->ProcessUploads();
        SubmitContextWork(*m_uploadContexts[m_frameId]);

        m_endOfFrameFences[m_frameId].ComputeQueueFence = m_computeQueue->SignalFence();
        m_endOfFrameFences[m_frameId].CopyQueueFence = m_copyQueue->SignalFence();
    }

    void GraphicsDevice::Present(Surface* surface)
    {
        surface->Present();
        m_endOfFrameFences[m_frameId].GraphicsQueueFence = m_graphicsQueue->SignalFence();
    }

    D3D12MA::Allocator* GraphicsDevice::GetAllocator() const
    {
        return m_allocator;
    }

    uint32_t GraphicsDevice::GetFreeReservedDescriptorIndex()
    {
        uint32_t index = m_freeReservedDescriptorIndices.back();
        m_freeReservedDescriptorIndices.pop_back();
        return index;
    }

    ContextSubmissionResult GraphicsDevice::SubmitContextWork(Context& context)
    {
        uint64_t fenceResult = 0;

        switch (context.GetCommandType())
        {
        case D3D12_COMMAND_LIST_TYPE_DIRECT:
            fenceResult = m_graphicsQueue->ExecuteCommandList(context.GetCommandList());
            break;
        case D3D12_COMMAND_LIST_TYPE_COMPUTE:
            fenceResult = m_computeQueue->ExecuteCommandList(context.GetCommandList());
            break;
        case D3D12_COMMAND_LIST_TYPE_COPY:
            fenceResult = m_copyQueue->ExecuteCommandList(context.GetCommandList());
            break;
        default:
            BR_ASSERT_ERROR("Unsupported submission type.");
            break;
        }

        ContextSubmissionResult submissionResult;
        submissionResult.mFrameId = m_frameId;
        submissionResult.mSubmissionIndex = static_cast<uint32_t>(m_contextSubmissions[m_frameId].size());

        m_contextSubmissions[m_frameId].push_back(std::make_pair(fenceResult, context.GetCommandType()));

        return submissionResult;
    }

    void GraphicsDevice::WaitForIdle()
    {
        m_graphicsQueue->WaitForIdle();
        m_computeQueue->WaitForIdle();
        m_copyQueue->WaitForIdle();
    }

    std::shared_ptr<GraphicsDevice> GraphicsDevice::Create(std::shared_ptr<GraphicsAdapter> adapter)
    {
        return std::make_shared<GraphicsDevice>(adapter);
    }

    void GraphicsDevice::CopyDescriptorsSimple(uint32_t numDescriptors, D3D12_CPU_DESCRIPTOR_HANDLE destDescriptorRangeStart, D3D12_CPU_DESCRIPTOR_HANDLE srcDescriptorRangeStart, D3D12_DESCRIPTOR_HEAP_TYPE descriptorType)
    {
        m_d3dDevice->CopyDescriptorsSimple(numDescriptors, destDescriptorRangeStart, srcDescriptorRangeStart, descriptorType);
    }

    void GraphicsDevice::ProcessDestructions(uint32_t frameIndex)
    {
    }

    void GraphicsDevice::CopyDescriptors(uint32_t numDestDescriptorRanges, const D3D12_CPU_DESCRIPTOR_HANDLE* destDescriptorRangeStarts, const uint32_t* destDescriptorRangeSizes, uint32_t numSrcDescriptorRanges, const D3D12_CPU_DESCRIPTOR_HANDLE* srcDescriptorRangeStarts, const uint32_t* srcDescriptorRangeSizes, D3D12_DESCRIPTOR_HEAP_TYPE descriptorType)
    {
        m_d3dDevice->CopyDescriptors(numDestDescriptorRanges, destDescriptorRangeStarts, destDescriptorRangeSizes, numSrcDescriptorRanges, srcDescriptorRangeStarts, srcDescriptorRangeSizes, descriptorType);
    }

    void GraphicsDevice::CopySRVHandleToReservedTable(DescriptorHandle srvHandle, uint32_t index)
    {
        for (uint32_t frameIndex = 0; frameIndex < Graphics::Core::FRAMES_IN_FLIGHT_COUNT; frameIndex++)
        {
            DescriptorHandle targetDescriptor = m_srvRenderPassDescriptorHeaps[frameIndex]->GetReservedDescriptor(index);

            CopyDescriptorsSimple(1, targetDescriptor.Cpu, srvHandle.Cpu, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
        }
    }

    IDXGIFactory4* GraphicsDevice::GetFactory()
    {
        return m_dxgiFactory.Get();
    }

    ID3D12Device5* GraphicsDevice::GetD3DDevice()
    {
        return m_d3dDevice.Get();
    }

    StagingDescriptorHeap& GraphicsDevice::GetRtvDescriptionHeap()
    {
        return *m_rtvDescriptorHeap;
    }

    StagingDescriptorHeap& GraphicsDevice::GetDsvDescriptionHeap()
    {
        return *m_dsvDescriptorHeap;
    }

    StagingDescriptorHeap& GraphicsDevice::GetSrvDescriptionHeap()
    {
        return *m_srvDescriptorHeap;
    }
}