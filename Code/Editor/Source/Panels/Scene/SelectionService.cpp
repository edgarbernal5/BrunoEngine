#include "SelectionService.h"

#include "Bruno/Scene/Scene.h"
#include "Content/EditorAssetManager.h"
#include "Bruno/Renderer/Model.h"
#include "Bruno/Content/AssetManager.h"

namespace Bruno
{
	SelectionService::SelectionService(std::shared_ptr<Scene> scene, AbstractAssetManager* assetManager) :
		m_scene(scene),
		m_assetManager(assetManager)
	{
	}

	void SelectionService::Select(UUID selection)
	{
		m_selections.clear();
		m_selections.push_back(selection);
	}

	UUID SelectionService::FindEntityWithRay(Math::Ray ray, float maxDistance)
	{
		auto entities = m_scene->GetAllEntitiesWith<IdComponent, ModelComponent>();
		for (auto& ent : entities)
		{
			auto [idComponent, modelComponent] = entities.get<IdComponent, ModelComponent>(ent);
			auto model = m_assetManager->GetAsset<Model>(modelComponent.ModelHandle);

			uint32_t meshIndex = modelComponent.MeshIndex;
			auto& meshes = model->GetMeshes();
			auto& mesh = meshes[meshIndex];
			Math::BoundingBox& bbox = mesh->GetBoundingBox();

			float distance;
			if (ray.Intersects(bbox, distance)) {
				return idComponent.Id;
			}
		}
		return UUID(0);
	}

	Math::Matrix SelectionService::GetSelectionTransform()
	{
		return m_scene->GetWorldSpaceMatrix(m_scene->GetEntityWithUUID(m_selections[0]));
	}
}