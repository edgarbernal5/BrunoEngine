#include "ContentProcessorContext.h"

#include "GameContentBuilder.h"
#include "Pipeline/ProcessorManager.h"

namespace Bruno
{
	ContentProcessorContext::ContentProcessorContext(GameContentBuilder* builder, BuildItem* buildItem) :
		m_gameContentBuilder(builder),
		m_currentBuildItem(buildItem)
	{
	}

	ExternalReferenceContentItem ContentProcessorContext::BuildAsset(ExternalReferenceContentItem sourceAsset, const std::string& processorName, const std::wstring& assetName)
	{
		auto buildItem = m_gameContentBuilder->RequestBuild(sourceAsset.Filename, assetName, processorName);

		return ExternalReferenceContentItem(m_gameContentBuilder->GetAbsolutePath(buildItem->OutputFilename));
	}
}