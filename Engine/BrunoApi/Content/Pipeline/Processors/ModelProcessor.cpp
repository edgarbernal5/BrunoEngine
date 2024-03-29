#include "stdafx.h"
#include "ModelProcessor.h"

#include "Content/Pipeline/Graphics/NodeContent.h"
#include "Content/Pipeline/Graphics/MeshContent.h"
#include "Content/Pipeline/Graphics/GeometryContent.h"
#include "Content/Pipeline/Graphics/MaterialContent.h"
#include "Content/Pipeline/Graphics/VertexBufferContent.h"
#include "Content/Pipeline/Graphics/IndexCollection.h"
#include "Content/Pipeline/Processors/ModelContent.h"
#include "Content/Pipeline/Processors/ModelBoneContent.h"
#include "Content/Pipeline/Processors/ModelMeshContent.h"
#include "Content/Pipeline/Processors/ModelMeshPartContent.h"

#include "Content/Pipeline/Graphics/VertexContent.h"
#include "Content/Pipeline/Graphics/VertexChannelCollection.h"

#include "Content/Pipeline/Processors/ContentProcessorContext.h"
#include "Math/MathCollision.h"

namespace BrunoEngine
{
	ModelProcessor::ModelProcessor()
	{
	}

	ModelProcessor::~ModelProcessor()
	{
	}

	ContentItem* ModelProcessor::Process(ContentItem* input, ContentProcessorContext *context)
	{
		NodeContent* inputContent = (NodeContent*)input;

		std::vector<NodeContent*> nodes;
		GetFlattenedNodes(inputContent, nodes);
		std::vector<GeometryContent*> geometries;
		std::vector<MaterialContent*> materials;

		for (auto& node : nodes) {
			auto mesh = dynamic_cast<MeshContent*>(node);
			if (mesh) {
				for (auto& geometry : mesh->GetGeometry()) {
					geometries.push_back(geometry);

					materials.push_back(geometry->GetMaterial());
				}
			}
		}

		for (auto& material : materials) {
			std::vector< GeometryContent*> outcomeGeoms;
			for (auto& geometry : geometries) {
				if (geometry->GetMaterial() == material) {
					outcomeGeoms.push_back(geometry);
				}
			}

			MaterialContent* processedMaterial = ConvertMaterial(material, context);
			ProcessGeometryUsingMaterial(processedMaterial, outcomeGeoms, context);
		}

		std::vector< ModelBoneContent*> boneList;
		std::vector< ModelMeshContent*> meshList;
		ModelBoneContent* rootNode = ProcessNode(inputContent, nullptr, boneList, meshList, context);

		return new ModelContent(rootNode, boneList, meshList);
	}

	MaterialContent* ModelProcessor::ConvertMaterial(MaterialContent* material, ContentProcessorContext* context)
	{
		return (MaterialContent*)context->Convert(material, "MaterialProcessor", nullptr);
	}

	void ModelProcessor::ProcessGeometryUsingMaterial(MaterialContent* material, std::vector< GeometryContent*>& geometryList, ContentProcessorContext* context)
	{
		//Si no tiene material, fallbackear a uno.

		for (auto& geometry : geometryList) {
			geometry->SetMaterial(material);
		}
	}

	void ModelProcessor::GetFlattenedNodes(NodeContent* input, std::vector<NodeContent*>& outputNodes)
	{
		outputNodes.push_back(input);
		for (auto& child : input->GetChildren()) {
			GetFlattenedNodes(child, outputNodes);
		}
	}

	ModelBoneContent* ModelProcessor::ProcessNode(NodeContent* node, ModelBoneContent* parent, std::vector< ModelBoneContent*>& boneList, std::vector< ModelMeshContent*>& meshList, ContentProcessorContext* context)
	{
		ModelBoneContent* boneContent = new ModelBoneContent(node->GetName(), boneList.size(), node->GetTransform(), parent);
		boneList.push_back(boneContent);

		auto nodeAsMesh = dynamic_cast<MeshContent*>(node);
		if (nodeAsMesh) {
			meshList.push_back(ProcessMesh(nodeAsMesh, boneContent, context));
		}

		std::vector<ModelBoneContent*> children;
		for (auto& child : node->GetChildren()) {
			children.push_back(ProcessNode(child, boneContent, boneList, meshList, context));
		}
		boneContent->SetChildren(children);

		return boneContent;
	}

	ModelMeshContent* ModelProcessor::ProcessMesh(MeshContent* mesh, ModelBoneContent* parent, ContentProcessorContext* context)
	{
		VertexBufferContent* vertexBuffer = new VertexBufferContent();
		IndexCollection* indexBuffer = new IndexCollection();
		std::vector<ModelMeshPartContent*> parts;

		int startVertex = 0;
		for (auto& geometry : mesh->GetGeometry()) {
			auto vertices = geometry->GetVertices();
			auto vertexCount = vertices->GetVertexCount();

			ModelMeshPartContent* partContent;
			if (vertexCount == 0) {
				partContent = new ModelMeshPartContent();
			}
			else {
				auto geometryBuffer = vertices->CreateVertexBuffer();
				vertexBuffer->Write(vertexBuffer->GetVertexDataSize(), 1, geometryBuffer->GetVertexData(), geometryBuffer->GetVertexDataSize());

				int startIndex = indexBuffer->size();
				indexBuffer->AddRange(geometry->GetIndices().cbegin(), geometry->GetIndices().cend());

				partContent = new ModelMeshPartContent(vertexBuffer, indexBuffer, startVertex, vertexCount, startIndex, geometry->GetIndices().size() / 3);

				vertexBuffer->SetVertexDeclaration(geometryBuffer->GetVertexDeclaration());

				startVertex += vertexCount;
			}

			partContent->SetMaterial(geometry->GetMaterial());
			parts.push_back(partContent);
		}

		BoundingSphere bSphere;
		BoundingSphere::CreateFromPoints(bSphere, mesh->GetPositions().size(), mesh->GetPositions().data(), sizeof(Vector3));

		BoundingBox bBox;
		BoundingBox::CreateFromPoints(bBox, mesh->GetPositions().size(), mesh->GetPositions().data(), sizeof(Vector3));
				
		return new ModelMeshContent(mesh->GetName(), mesh, parent, bSphere, bBox, parts);
	}
}