#include "stdafx.h"
#include "ModelReader.h"

#include "Content/GraphicsContentHelper.h"
#include "Content/ContentReader.h"

#include "Graphics/Models/Model.h"
#include "Graphics/Models/ModelBone.h"
#include "Graphics/Models/ModelMesh.h"
#include "Graphics/Models/ModelMeshPart.h"

namespace TrioEngine
{
	ModelReader::ModelReader()
	{
	}


	ModelReader::~ModelReader()
	{
	}

	void* ModelReader::Read(ContentReader* input)
	{
		Model* model = new Model(GraphicsContentHelper::GraphicsDeviceFromContentReader(input));

		ReadBones(model, input);
		ReadMeshes(model, input);
		int rootIndex;
		model->m_root = ReadBoneReference(model, input, rootIndex);

		return model;
	}

	void ModelReader::ReadBones(Model* model, ContentReader* input)
	{
		int boneCount = input->ReadInt32();
		std::vector<ModelBone*> bones = std::vector<ModelBone*>(boneCount, nullptr);
		for (int i = 0; i < boneCount; i++)
		{
			std::string name = input->ReadString();
			Matrix transform = input->ReadMatrix();
			bones[i] = new ModelBone(name, transform, i);
		}
		model->m_modelBones = bones;
		for (int i = 0; i < boneCount; i++)
		{
			int rootIndex;
			ModelBone* newParent = ReadBoneReference(model, input, rootIndex);
			int childCount = input->ReadInt32();
			std::vector<ModelBone*> newChildren(childCount, nullptr);
			for (int j = 0; j < childCount; j++)
			{
				newChildren[j] = ReadBoneReference(model, input, rootIndex);
			}
			model->m_modelBones[i]->m_parent = newParent;
			model->m_modelBones[i]->m_children = newChildren;
		}

		model->m_bonesMatrices.resize(model->m_modelBones.size());
		for (int i = 0; i < model->m_modelBones.size(); i++)
		{
			const ModelBone* bone = model->m_modelBones[i];
			if (bone->m_parent == nullptr)
			{
				model->m_bonesMatrices[i] = bone->m_transform;
			}
			else
			{
				/*
				XMMATRIX M1(&bone->m_Transform.m[0][0]);
				XMMATRIX M2(&model->m_BonesMatrices[bone->m_Parent->m_Index].m[0][0]);

				XMMATRIX mult = M1 * M2;
				XMStoreFloat4x4(&model->m_BonesMatrices[i], mult);
				*/

				model->m_bonesMatrices[i] = bone->m_transform * model->m_bonesMatrices[bone->m_parent->m_index];
			}
		}
	}
	
	void ModelReader::ReadMeshes(Model* model, ContentReader* input)
	{
		int meshCount = input->ReadInt32();
		std::vector<ModelMesh*> meshes(meshCount, nullptr);
		for (int i = 0; i < meshCount; i++)
		{
			std::string name = input->ReadString();
			int rootIndex;
			ModelBone* parentBone = ReadBoneReference(model, input, rootIndex);

			/*BoundingSphere boundingSphere;
			boundingSphere.Center = input->ReadVector3();
			boundingSphere.Radius = input->ReadSingle();

			BoundingBox boundingBox;
			boundingBox.Min = input->ReadVector3();
			boundingBox.Max = input->ReadVector3();*/

			Matrix transform = model->m_bonesMatrices[rootIndex]; //GetAbsoluteTransform(model);
			std::vector<ModelMeshPart*> meshParts = ReadMeshParts(input);
			meshes[i] = new ModelMesh(name, parentBone, transform, /*boundingSphere, boundingBox,*/ meshParts);
		}
		model->m_modelMeshes = meshes;
	}

	//Matrix ModelReader::GetAbsoluteTransform(Model* model) {

	//}
	
	std::vector<ModelMeshPart*> ModelReader::ReadMeshParts(ContentReader* input)
	{
		int meshPartCount = input->ReadInt32();
		std::vector<ModelMeshPart*> meshParts(meshPartCount, nullptr);
		for (int i = 0; i < meshPartCount; i++)
		{
			int vertexOffset = input->ReadInt32();
			int numVertices = input->ReadInt32();
			int startIndex = input->ReadInt32();
			int primitiveCount = input->ReadInt32();

			meshParts[i] = new ModelMeshPart(vertexOffset, numVertices, startIndex, primitiveCount);
			meshParts[i]->m_primitiveType = PrimitiveType::TriangleList;

			int uniqueCopyOfI = i;
			input->ReadSharedResource([=](ContentItem* item)
				{
					meshParts[uniqueCopyOfI]->m_vertexBuffer = (VertexBuffer*)item;
				});

			//input->ReadSharedResource([=](ContentItem* ib)
			input->ReadSharedResource([meshParts, uniqueCopyOfI](ContentItem* item)
				{
					meshParts[uniqueCopyOfI]->m_indexBuffer = (IndexBuffer*)item;
				});

			input->ReadSharedResource([=](ContentItem* item)
				{
					Material* material = (Material*)item;

					meshParts[uniqueCopyOfI]->m_material = material;

					//if (material->GetTextures().size() > 0)
					//{
					//	Texture* diffuseTexture = material->GetTextureByName("Texture");
					//	if (diffuseTexture != nullptr)
					//	{
					//		meshParts[uniqueCopyOfI]->m_diffuseTexture = dynamic_cast<Texture2D*>(diffuseTexture);
					//	}
					//}
				});
		}

		return meshParts;
	}

	ModelBone* ModelReader::ReadBoneReference(Model* model, ContentReader* input, int& boneIndex)
	{
		int totalBones = model->m_modelBones.size() + 1;
		int boneId = 0;
		if (totalBones <= 0xff)
		{
			boneId = input->ReadByte();
		}
		else
		{
			boneId = input->ReadInt32();
		}
		if (boneId != 0)
		{
			boneIndex = boneId - 1;
			return model->m_modelBones[boneId - 1];
		}
		boneIndex = 0;
		return nullptr;
	}
}