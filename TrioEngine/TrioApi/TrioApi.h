#pragma once

#include "TrioApiRequisites.h"

#include "Content/Pipeline/Tasks/BuildCoordinatorSettings.h"
#include "Renderer/Camera.h"

using namespace TrioEngine;

namespace TrioEngine {

	class Game;
	class Effect;
	class EffectTechnique;
	class EffectParameter;
	class EffectPass;
	class Scene;
	class Transform;
	class GameObject;
	class Component;
	class Object;
	class Texture2D;
	class RenderTarget2D;
	class Model;
	class ModelMesh;
	class ModelMeshPart;
	class Material;

	class ContentManager;
	//class ContentReader;

	class BuildCoordinator;
	class TextureContent;

	class RenderPathForward;
}

/*
DepthStencilState
*/
extern "C" TRIO_API_EXPORT DepthStencilState* __stdcall DepthStencilState_Ctor();
extern "C" TRIO_API_EXPORT DepthStencilState* __stdcall DepthStencilState_Default();
extern "C" TRIO_API_EXPORT DepthStencilState* __stdcall DepthStencilState_DepthRead();
extern "C" TRIO_API_EXPORT DepthStencilState* __stdcall DepthStencilState_None();

/*
BlendState
*/
extern "C" TRIO_API_EXPORT BlendState* __stdcall BlendState_Ctor();
extern "C" TRIO_API_EXPORT BlendState* __stdcall BlendState_Additive();
extern "C" TRIO_API_EXPORT BlendState* __stdcall BlendState_AlphaBlend();
extern "C" TRIO_API_EXPORT BlendState* __stdcall BlendState_NonPremultiplied();
extern "C" TRIO_API_EXPORT BlendState* __stdcall BlendState_Opaque();

/*
BuildCoordinator
*/
struct BuildCoordinatorSettingsBridge {
	char* IntermediateDirectory;
	char* OutputDirectory;
	char* RootDirectory;

};

extern "C" TRIO_API_EXPORT BuildCoordinator* __stdcall BuildCoordinator_Ctor(const char* intermediateDirectory, const char* outputDirectory, const char* rootDirectory);
extern "C" TRIO_API_EXPORT BuildCoordinator* __stdcall BuildCoordinator_Ctor2(BuildCoordinatorSettingsBridge settings);
extern "C" TRIO_API_EXPORT void __stdcall BuildCoordinator_Dtor(BuildCoordinator* buildCoordinator);
extern "C" TRIO_API_EXPORT void __stdcall BuildCoordinator_GetSettings(BuildCoordinator* coordinator, char* const intermediateDir, char* const outputDir, char* const rootDir);
extern "C" TRIO_API_EXPORT void __stdcall BuildCoordinator_GetSettings2(BuildCoordinator* coordinator, BuildCoordinatorSettingsBridge* managedSettings);
extern "C" TRIO_API_EXPORT void __stdcall BuildCoordinator_RequestBuildWithoutOpaqueData(BuildCoordinator* coordinator, const char* sourceFilename, const char* assetName, const char* importerName, const char* processorName);
extern "C" TRIO_API_EXPORT void __stdcall BuildCoordinator_RequestBuild(BuildCoordinator * coordinator, const char* sourceFilename, const char* assetName, const char* importerName, const char* processorName, int opaqueDataSize, const char** opaqueDataKeys, const char** opaqueDataValues);
extern "C" TRIO_API_EXPORT void __stdcall BuildCoordinator_RunTheBuild(BuildCoordinator* coordinator);

/*
Component
*/
extern "C" TRIO_API_EXPORT bool __stdcall Component_GetEnabled(Component* component);

/*
ContentManager
*/
extern "C" TRIO_API_EXPORT ContentManager* __stdcall ContentManager_Ctor(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT ContentManager* __stdcall ContentManager_Ctor2(GraphicsDevice* device, const char* rootDirectory);
extern "C" TRIO_API_EXPORT uint8_t * __stdcall ContentManager_Load(ContentManager* contentManager, const char* assetName);

/*
Effect
*/
extern "C" TRIO_API_EXPORT Effect* __stdcall Effect_Ctor(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT void __stdcall Effect_CompileEffectFromFile(Effect* effect, const char* filename);
extern "C" TRIO_API_EXPORT void __stdcall Effect_GetParameters(Effect* effect, EffectParameter*** parameters, int *size);
extern "C" TRIO_API_EXPORT void __stdcall Effect_GetTechniques(Effect* effect, EffectTechnique*** techniques, int *size);

/*
EffectParameter
*/
extern "C" TRIO_API_EXPORT char* __stdcall EffectParameter_GetName(EffectParameter* parameter);
extern "C" TRIO_API_EXPORT void __stdcall EffectParameter_SetValueScalar(EffectParameter* parameter, float scalar);
extern "C" TRIO_API_EXPORT void __stdcall EffectParameter_SetValueMatrix(EffectParameter* parameter, Matrix* matrix);
extern "C" TRIO_API_EXPORT void __stdcall EffectParameter_SetValueVector2(EffectParameter* parameter, Vector2* vector);
extern "C" TRIO_API_EXPORT void __stdcall EffectParameter_SetValueVector3(EffectParameter* parameter, Vector3* vector);
extern "C" TRIO_API_EXPORT void __stdcall EffectParameter_SetValueVector4(EffectParameter* parameter, Vector4* vector);
extern "C" TRIO_API_EXPORT void __stdcall EffectParameter_SetValueColor(EffectParameter* parameter, uint32_t packedColor);

/*
EffectPass
*/
extern "C" TRIO_API_EXPORT char* __stdcall EffectPass_GetName(EffectPass* pass);
extern "C" TRIO_API_EXPORT void __stdcall EffectPass_Apply(EffectPass * pass);

/*
EffectTechnique
*/
extern "C" TRIO_API_EXPORT void __stdcall EffectTechnique_GetPasses(EffectTechnique* technique, EffectPass*** passes, int *size);
extern "C" TRIO_API_EXPORT char* __stdcall EffectTechnique_GetName(EffectTechnique* technique);

/*
Game
*/
extern "C" TRIO_API_EXPORT Game* __stdcall Game_Ctor();
extern "C" TRIO_API_EXPORT void __stdcall Game_Run(Game* game);
extern "C" TRIO_API_EXPORT void __stdcall Game_Tick(Game* game);

/*
GameObject
*/
//extern "C" TRIO_API_EXPORT GameObject* __stdcall GameObject_Create(const char* name);
//extern "C" TRIO_API_EXPORT void __stdcall GameObject_Destroy(GameObject *gameObject);
//extern "C" TRIO_API_EXPORT Transform* __stdcall GameObject_GetTransform(GameObject* gameObject);
//extern "C" TRIO_API_EXPORT bool __stdcall GameObject_IsActiveInTree(GameObject* gameObject);
//extern "C" TRIO_API_EXPORT bool __stdcall GameObject_IsActiveSelf(GameObject* gameObject);
//extern "C" TRIO_API_EXPORT void __stdcall GameObject_SetActiveSelf(GameObject* gameObject, bool active);

/*
GraphicsAdapter
*/
extern "C" TRIO_API_EXPORT void __stdcall GraphicsAdapter_GetAdapters(GraphicsAdapter*** adapters, int *size);

/*
GraphicsDevice
*/
extern "C" TRIO_API_EXPORT GraphicsDevice* __stdcall GraphicsDevice_Ctor(GraphicsAdapter* adapter, PresentationParameters parameters);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_Dtor(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_Clear(GraphicsDevice * device, float* colorAsFloat);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_ClearAsRGBA8(GraphicsDevice * device, uint32_t packedColor);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_DrawIndexedPrimitives(GraphicsDevice * device, PrimitiveType primitiveType, uint32_t baseVertex, uint32_t startIndex, uint32_t primitiveCount);
extern "C" TRIO_API_EXPORT BlendState* __stdcall GraphicsDevice_GetBlendState(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT DepthStencilState* __stdcall GraphicsDevice_GetDepthStencilState(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT RasterizerState* __stdcall GraphicsDevice_GetRasterizerState(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT Viewport __stdcall GraphicsDevice_GetViewport(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_Present(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_Reset(GraphicsDevice* device, PresentationParameters parameters);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetBlendState(GraphicsDevice* device, BlendState* state);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetDepthStencilState(GraphicsDevice* device, DepthStencilState* state);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetRasterizerState(GraphicsDevice* device, RasterizerState* state);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetIndexBuffer(GraphicsDevice * device, IndexBuffer *indexBuffer);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetVertexBuffer(GraphicsDevice * device, VertexBuffer * vertexBuffer);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetViewport(GraphicsDevice* device, Viewport viewport);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetRenderTarget(GraphicsDevice* device, RenderTarget2D* renderTarget);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_SetRenderTargets(GraphicsDevice * device, RenderTarget2D** renderTarget, int size);
extern "C" TRIO_API_EXPORT void __stdcall GraphicsDevice_Flush(GraphicsDevice * device);

/*
IndexBuffer
*/
extern "C" TRIO_API_EXPORT IndexBuffer* __stdcall IndexBuffer_Ctor(GraphicsDevice* device, int elementSize, int indexCount, int usage);
extern "C" TRIO_API_EXPORT IndexBuffer * __stdcall IndexBuffer_Ctor2(GraphicsDevice * device, int elementSize, int indexCount);
extern "C" TRIO_API_EXPORT void __stdcall IndexBuffer_Dctor(IndexBuffer* indexBuffer);
extern "C" TRIO_API_EXPORT void __stdcall IndexBuffer_SetData(IndexBuffer* buffer, uint8_t* data, uint32_t elementCount, uint32_t sizeArrayBytes);

/*
Matrix
*/
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateLookAt(Matrix *pMatrix1, Vector3* eye, Vector3* target, Vector3* up);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreatePerspectiveFieldOfView(Matrix *pMatrix1, float fieldOfView, float aspectRatio, float nearPlaneDistance, float farPlaneDistance);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateRotationX(Matrix *pMatrix1, float radians);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateRotationY(Matrix *pMatrix1, float radians);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateRotationZ(Matrix *pMatrix1, float radians);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateScale(Matrix *pMatrix1, Vector3* scale);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateTranslation(Matrix *pMatrix1, Vector3* translation);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_Division(Matrix *pMatrix1, Matrix *pMatrix2);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_Invert(Matrix *pMatrix);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_Multiply(Matrix *pMatrix1, Matrix *pMatrix2);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_MultiplyScalar(Matrix *pMatrix1, float scalar);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_Sub(Matrix *pMatrix1, Matrix *pMatrix2);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_Sum(Matrix *pMatrix1, Matrix *pMatrix2);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_Transpose(Matrix *pMatrix);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateTRS(Matrix *pMatrix1, Vector3* position, Quaternion* rotation, Vector3* scale);
extern "C" TRIO_API_EXPORT void __stdcall Matrix_CreateWorld(Matrix *pMatrix1, Vector3* position, Vector3* forward, Vector3* up);

/*
Model
*/
extern "C" TRIO_API_EXPORT Model * __stdcall Model_Ctor(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT void __stdcall Model_Dtor(Model * model);
extern "C" TRIO_API_EXPORT void __stdcall Model_GetModelMeshes(Model * model, ModelMesh * **modelMeshes, int* size);
extern "C" TRIO_API_EXPORT void __stdcall Model_Draw(Model * model);

/*
ModelMesh
*/
//extern "C" TRIO_API_EXPORT Model * __stdcall ModelMesh_Ctor(GraphicsDevice * device);
extern "C" TRIO_API_EXPORT void __stdcall ModelMesh_Dtor(ModelMesh * modelMesh);
extern "C" TRIO_API_EXPORT char* __stdcall ModelMesh_GetName(ModelMesh * modelMesh);
extern "C" TRIO_API_EXPORT void __stdcall ModelMesh_GetModelMeshParts(ModelMesh * modelMesh, ModelMeshPart * **modelMeshParts, int* size);

/*
ModelMeshPart
*/
//extern "C" TRIO_API_EXPORT Model * __stdcall ModelMesh_Ctor(GraphicsDevice * device);
extern "C" TRIO_API_EXPORT void __stdcall ModelMeshPart_Dtor(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT VertexBuffer* __stdcall ModelMeshPart_GetVertexBuffer(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT IndexBuffer * __stdcall ModelMeshPart_GetIndexBuffer(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT Material * __stdcall ModelMeshPart_GetMaterial(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT uint32_t __stdcall ModelMeshPart_GetVertexCount(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT uint32_t __stdcall ModelMeshPart_GetPrimitiveCount(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT uint32_t __stdcall ModelMeshPart_GetVertexOffset(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT uint32_t __stdcall ModelMeshPart_GetStartIndex(ModelMeshPart * modelMeshPart);
extern "C" TRIO_API_EXPORT PrimitiveType __stdcall ModelMeshPart_GetPrimitiveType(ModelMeshPart * modelMeshPart);

/*
Object
*/
//extern "C" TRIO_API_EXPORT int __stdcall Object_GetId(Object* object);
//extern "C" TRIO_API_EXPORT char* __stdcall Object_GetName(Object* object);
//extern "C" TRIO_API_EXPORT void __stdcall Object_SetName(Object* object, const char* name);

/*
Renderer
*/
extern "C" TRIO_API_EXPORT void __stdcall Renderer_DrawScene(Camera camera);
extern "C" TRIO_API_EXPORT void __stdcall Renderer_Initialize(GraphicsDevice* device);
extern "C" TRIO_API_EXPORT void __stdcall UpdatePerFrameData();

/*
RenderPathForward
*/
extern "C" TRIO_API_EXPORT RenderPathForward * __stdcall RenderPathForward_Ctor();

/*
RenderTarget2D
*/
extern "C" TRIO_API_EXPORT RenderTarget2D* __stdcall RenderTarget2D_Ctor(GraphicsDevice* device, int width, int height, SurfaceFormat format);
extern "C" TRIO_API_EXPORT RenderTarget2D * __stdcall RenderTarget2D_Ctor2(GraphicsDevice* device, int width, int height, uint32_t mipmapLevels, SurfaceFormat preferredFormat, DepthFormat preferredDepthFormat, RenderTargetUsage usage);
extern "C" TRIO_API_EXPORT RenderTarget2D * __stdcall RenderTarget2D_Ctor3(GraphicsDevice* device, int width, int height, uint32_t mipmapLevels, SurfaceFormat preferredFormat, DepthFormat preferredDepthFormat);
extern "C" TRIO_API_EXPORT void __stdcall RenderTarget2D_Dctor(RenderTarget2D* renderTarget);

/*
RasterizerState
*/
extern "C" TRIO_API_EXPORT RasterizerState* __stdcall RasterizerState_Ctor();
extern "C" TRIO_API_EXPORT RasterizerState* __stdcall RasterizerState_CullClockwise();
extern "C" TRIO_API_EXPORT RasterizerState* __stdcall RasterizerState_CullCounterClockwise();
extern "C" TRIO_API_EXPORT RasterizerState* __stdcall RasterizerState_CullNone();

/*
Scene
*/
extern "C" TRIO_API_EXPORT Scene* __stdcall Scene_Ctor();
extern "C" TRIO_API_EXPORT void __stdcall Scene_Dtor(Scene* scene);
extern "C" TRIO_API_EXPORT Scene* __stdcall Scene_GetActiveScene();
extern "C" TRIO_API_EXPORT void __stdcall Scene_Update(Scene* scene);

/*
Transform
*/
//extern "C" TRIO_API_EXPORT Transform* __stdcall Transform_GetParent(Transform* transform);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_GetPosition(Transform* transform, Vector3 *position);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_GetRotation(Transform* transform, Quaternion *rotation);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_GetScale(Transform* transform, Vector3 *scale);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_GetLocalPosition(Transform* transform, Vector3 *position);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_GetLocalRotation(Transform* transform, Quaternion *rotation);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_GetLocalScale(Transform* transform, Vector3 *scale);
//
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetPosition(Transform* transform, Vector3 *position);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetRotation(Transform* transform, Quaternion *rotation);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetScale(Transform* transform, Vector3 *scale);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetLocalPosition(Transform* transform, Vector3 *position);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetLocalRotation(Transform* transform, Quaternion *rotation);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetLocalScale(Transform* transform, Vector3 *scale);
//extern "C" TRIO_API_EXPORT void __stdcall Transform_SetParent(Transform* transform, Transform* parent);


/*
Texture
*/
extern "C" TRIO_API_EXPORT SurfaceFormat __stdcall Texture_GetFormat(Texture* texture);

/*
Texture2D
*/
extern "C" TRIO_API_EXPORT Texture2D* __stdcall Texture2D_Ctor(GraphicsDevice* device, int width, int height);
extern "C" TRIO_API_EXPORT Texture2D* __stdcall Texture2D_Ctor2(GraphicsDevice* device, int width, int height, SurfaceFormat format);
extern "C" TRIO_API_EXPORT void __stdcall Texture2D_Dctor(Texture2D * texture);
extern "C" TRIO_API_EXPORT void __stdcall Texture2D_GetData(Texture2D* texture, uint8_t* data, uint32_t elementCount);
extern "C" TRIO_API_EXPORT void __stdcall Texture2D_TestLoadFromFile(Texture2D* texture);
extern "C" TRIO_API_EXPORT int __stdcall Texture2D_GetWidth(Texture2D* texture);
extern "C" TRIO_API_EXPORT int __stdcall Texture2D_GetHeight(Texture2D* texture);

/*
Vector3
*/
extern "C" TRIO_API_EXPORT void __stdcall Vector3_Clamp(Vector3 *pVector1, Vector3 *pMin, Vector3 *pMax);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_Cross(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT float __stdcall Vector3_Distance(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT float __stdcall Vector3_DistanceSquared(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT float __stdcall Vector3_Dot(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_MultiplyDivision(Vector3 *pVector, float scalar);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_MultiplyTwoVectors(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_MultiplyScalar(Vector3 *pVector, float scalar);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_Normalize(Vector3 *pVector);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_SubTwoVectors(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_SumTwoVectors(Vector3 *pVector1, Vector3 *pVector2);
extern "C" TRIO_API_EXPORT void __stdcall Vector3_UnaryNegation(Vector3 *pVector);

/*
VertexBuffer
*/
extern "C" TRIO_API_EXPORT VertexBuffer* __stdcall VertexBuffer_Ctor(GraphicsDevice* device, VertexDeclaration* vertexDeclaration, int vertexCount, int usage);
extern "C" TRIO_API_EXPORT VertexBuffer* __stdcall VertexBuffer_Ctor2(GraphicsDevice* device, VertexDeclaration* vertexDeclaration, int vertexCount);
extern "C" TRIO_API_EXPORT void __stdcall VertexBuffer_Dctor(VertexBuffer* vertexBuffer);
extern "C" TRIO_API_EXPORT void __stdcall VertexBuffer_SetData(VertexBuffer* buffer, uint8_t* data, uint32_t elementCount, uint32_t sizeArrayBytes);

/*
VertexDeclaration
*/
extern "C" TRIO_API_EXPORT VertexDeclaration* __stdcall VertexDeclaration_Ctor(int vertexStride, VertexElement* elements, int sizeElements);
extern "C" TRIO_API_EXPORT VertexDeclaration* __stdcall VertexDeclaration_Ctor2(VertexElement* elements, int sizeElements);
extern "C" TRIO_API_EXPORT VertexDeclaration* __stdcall VertexDeclaration_GetP();
extern "C" TRIO_API_EXPORT VertexDeclaration* __stdcall VertexDeclaration_GetPC();
extern "C" TRIO_API_EXPORT VertexDeclaration* __stdcall VertexDeclaration_GetPNT();