#pragma once

#include "TrioApiRequisites.h"

#include <string>
#include <vector>

namespace TrioEngine
{
	class ModelBone;
	class ModelMesh;

	class GraphicsDevice;

	class TRIO_API_EXPORT Model
	{
	public:
		Model(GraphicsDevice* device);
		~Model();

		void Draw();

		inline std::vector<ModelMesh*>& GetModelMeshes() {
			return m_modelMeshes;
		}

		inline std::vector<ModelBone*>& GetModelBones() {
			return m_modelBones;
		}

		friend class ModelReader;
		friend class Scene;
	private:
		ModelBone* m_root;

		std::vector<Matrix> m_bonesMatrices;

		std::vector<ModelMesh*> m_modelMeshes;
		std::vector<ModelBone*> m_modelBones;

		std::map<std::string, Texture*> m_textures;
		GraphicsDevice* m_device;
	};
}