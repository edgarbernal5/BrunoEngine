#include "brpch.h"
#include "ModelReader.h"

#include "Bruno/Content/ContentReader.h"
#include "Bruno/Platform/DirectX/IndexBuffer.h"
#include "Bruno/Platform/DirectX/VertexBuffer.h"

namespace Bruno
{
	BR_RTTI_DEFINITIONS(ModelReader);

	ModelReader::ModelReader() :
		ContentTypeReader(Model::GetTypeIdClass())
	{
	}

	std::shared_ptr<Model> ModelReader::ReadInternal(ContentReader& input)
	{
		size_t materialCount;
		input.ReadUInt64(materialCount);

		for (size_t i = 0; i < materialCount; i++)
		{
			std::string name;
			input.ReadString(name);

			size_t texturexCount;
			input.ReadUInt64(texturexCount);
			for (size_t j = 0; j < texturexCount; j++)
			{
				std::string textureName;
				input.ReadString(textureName);

				auto texture = input.ReadExternalReference();
			}
			
		}

		size_t meshesCount;
		input.ReadUInt64(meshesCount);

		std::vector<Mesh> meshes;
		meshes.reserve(meshesCount);

		for (size_t i = 0; i < meshesCount; i++)
		{
			std::string name;
			input.ReadString(name);
			
			uint32_t materialIndex;
			input.ReadUInt32(materialIndex);

			size_t count;
			input.ReadUInt64(count);
			std::vector<Math::Vector3> vertices;
			vertices.reserve(count);

			for (size_t j = 0; j < count; j++)
			{
				Math::Vector3 vertex;
				input.ReadVector3(vertex);

				vertices.push_back(vertex);
			}

			input.ReadUInt64(count);
			std::vector<Math::Vector3> normals;
			normals.reserve(count);

			for (size_t j = 0; j < count; j++)
			{
				Math::Vector3 normal;
				input.ReadVector3(normal);

				normals.push_back(normal);
			}

			input.ReadUInt64(count);
			std::vector<Math::Vector3> tangents;
			tangents.reserve(count);

			for (size_t j = 0; j < count; j++)
			{
				Math::Vector3 tangent;
				input.ReadVector3(tangent);

				tangents.push_back(tangent);
			}

			input.ReadUInt64(count);
			std::vector<Math::Vector3> binormals;
			binormals.reserve(count);

			for (size_t j = 0; j < count; j++)
			{
				Math::Vector3 binormal;
				input.ReadVector3(binormal);

				binormals.push_back(binormal);
			}

			std::vector < std::vector<Math::Vector3>> texCoords;
			input.ReadUInt64(count);
			for (size_t j = 0; j < count; j++)
			{
				size_t textureCount;
				input.ReadUInt64(textureCount);
				std::vector<Math::Vector3> texCoordsChannel;
				texCoordsChannel.reserve(textureCount);

				for (size_t k = 0; k < textureCount; k++)
				{
					Math::Vector3 texCoord;
					input.ReadVector3(texCoord);

					texCoordsChannel.push_back(texCoord);
				}

				texCoords.emplace_back(std::move(texCoordsChannel));
			}
			uint32_t faceCount;
			input.ReadUInt32(faceCount);

			input.ReadUInt64(count);
			std::vector< uint32_t> indices;
			indices.reserve(count);

			for (size_t j = 0; j < count; j++)
			{
				uint32_t index;
				input.ReadUInt32(index);

				indices.push_back(index);
			}

			auto indexBuffer = std::make_shared<IndexBuffer>(static_cast<uint32_t>(count), indices.data(), sizeof(uint32_t));
			auto vertexBuffer = std::make_shared<VertexBuffer>(static_cast<uint32_t>(vertices.size()), vertices.data(), sizeof(uint32_t));

			//auto mesh = std::make_shared<Mesh>(vertices, normals, tangents, binormals, texCoords);
			auto mesh = std::make_shared<Mesh>(std::move(vertices), std::move(normals), std::move(texCoords));

			//meshes.emplace_back(std::move(mesh));
		}
		auto model = std::make_shared<Model>(/*std::move(meshes)*/);
		return model;
	}
}