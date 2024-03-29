﻿
using Bruno.Interop;
using System;
using System.Runtime.InteropServices;
using BrunoApi.Net.Graphics;
using BrunoApi.Net.Maths;
using static BrunoApi.Net.Renderer.Renderer;
using BrunoApi.Net.Game.Components;

namespace BrunoApi.Net.Game
{
    public class Scene : CppObject
    {
        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_GetActiveScene", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_GetActiveScene();

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_SetActiveScene", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_SetActiveScene(IntPtr scene);

        public static Scene ActiveScene
        {
            get
            {
                return new Scene(Internal_GetActiveScene());
            }
            set
            {
                if (value == null)
                {
                    Internal_SetActiveScene(IntPtr.Zero);
                    return;
                }
                Internal_SetActiveScene(value.NativePointer);
            }
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_Ctor", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_ctor();

        public Scene()
            : base()
        {
            m_nativePointer = Internal_ctor();
        }

        internal Scene(IntPtr nativePtr)
            : base(nativePtr)
        {
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_Dtor", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_Dtor(IntPtr scene);

        protected override void OnDisposing(bool disposing)
        {
            if (disposing)
            {
            }
            Internal_Dtor(NativePointer);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_AddEmptyObject", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern long Internal_AddEmptyObject(IntPtr scene, [MarshalAs(UnmanagedType.LPStr)] string name);

        public long AddEmptyObject(string name)
        {
            return Internal_AddEmptyObject(m_nativePointer, name);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_GetCamera", CallingConvention = CallingConvention.StdCall)]
        private static extern Camera Internal_GetCamera();

        public static Camera Camera
        {
            get
            {
                return Internal_GetCamera();
            }
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_Update", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_Update(IntPtr scene);

        public void Update()
        {
            Internal_Update(m_nativePointer);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_UpdateCamera", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_Update(Camera camera);

        public static void UpdateCamera(Camera camera)
        {
            Internal_Update(camera);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_LoadFromModel", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_LoadFromModel(IntPtr scene, IntPtr model);

        public void LoadFromModel(Model model)
        {
            Internal_LoadFromModel(NativePointer, model.NativePointer);
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct HierarchyComponentBridge
        {
            public long id;
            public long parentId;
            public uint componentsMask;

            [MarshalAs(UnmanagedType.LPStr)]
            public string name;

        };


        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_GetHierarchyForEntity", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_GetHierarchyForEntity(IntPtr scene, long entity, ref HierarchyComponentBridge hierarchyComponentBridge);

        public HierarchyComponentBridge GetHierarchyForEntity(long entity)
        {
            HierarchyComponentBridge hierarchyComponentBridge = default(HierarchyComponentBridge);
            Internal_GetHierarchyForEntity(NativePointer, entity, ref hierarchyComponentBridge);

            return hierarchyComponentBridge;
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_GetHierarchies", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_GetHierarchies(IntPtr scene, ref int size, ref IntPtr collection);

        public HierarchyComponentBridge[] GetHierarchies()
        {
            int size = 0;
            var unmanagedArray = IntPtr.Zero;
            Internal_GetHierarchies(NativePointer, ref size, ref unmanagedArray);

            if (size > 0)
            {
                var outcome = new HierarchyComponentBridge[size];
                for (int i = 0; i < size; i++)
                {
                    var unmanagedItemPtr = unmanagedArray + Marshal.SizeOf<HierarchyComponentBridge>() * i;
                    outcome[i] = Marshal.PtrToStructure<HierarchyComponentBridge>(unmanagedItemPtr);

                    var unmanagedNameFieldPtr = unmanagedItemPtr + Marshal.OffsetOf<HierarchyComponentBridge>("name").ToInt32();
                    var namePtr = Marshal.ReadIntPtr(unmanagedNameFieldPtr);
                    Marshal.FreeCoTaskMem(namePtr);
                }

                Marshal.FreeCoTaskMem(unmanagedArray);

                return outcome;
            }
            return new HierarchyComponentBridge[0];
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_GetTransformMatrixForEntity", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_GetTransformMatrixForEntity(IntPtr scene, long entity, ref Matrix worldMatrix, ref Vector3 localPosition, ref Vector3 localScale, ref Quaternion localRotation);

        public SceneTransform GetSceneTransformFor(long entity)
        {
            var sceneTransform = new SceneTransform();

            Internal_GetTransformMatrixForEntity(m_nativePointer, entity, ref sceneTransform.WorldMatrix, ref sceneTransform.LocalPosition, ref sceneTransform.LocalScale, ref sceneTransform.LocalRotation);
            return sceneTransform;
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_GetBoundingBoxForEntity", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_GetBoundingBoxForEntity(IntPtr scene, long entity, ref Vector3 center, ref Vector3 extents);

        public SceneBoundingBox GetBoundingBoxForEntity(long entity)
        {
            Vector3 center = Vector3.Zero;
            Vector3 extents = Vector3.Zero;
            Internal_GetBoundingBoxForEntity(m_nativePointer, entity, ref center, ref extents);
            return new SceneBoundingBox(center, extents);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformTranslate", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformTranslate(IntPtr scene, long entity, ref Vector3 delta);

        public void TransformTranslate(long entity, Vector3 delta)
        {
            Internal_TransformTranslate(m_nativePointer, entity, ref delta);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformScale", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformScale(IntPtr scene, long entity, ref Vector3 delta);

        public void TransformScale(long entity, Vector3 delta)
        {
            Internal_TransformScale(m_nativePointer, entity, ref delta);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformScaleUniform", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformScaleUniform(IntPtr scene, long entity, float scale);

        public void TransformScaleUniform(long entity, float scaleFactor)
        {
            Internal_TransformScaleUniform(m_nativePointer, entity, scaleFactor);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformRotate", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformRotate(IntPtr scene, long entity, ref Quaternion rotate);

        public void TransformRotate(long entity, Quaternion deltaRotation)
        {
            Internal_TransformRotate(m_nativePointer, entity, ref deltaRotation);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformRotatePitchYawRoll", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformRotatePitchYawRoll(IntPtr scene, long entity, ref Vector3 pitchYawRoll);

        public void TransformRotatePitchYawRoll(long entity, Vector3 pitchYawRoll)
        {
            Internal_TransformRotatePitchYawRoll(m_nativePointer, entity, ref pitchYawRoll);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformSetLocalRotation", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformSetLocalRotation(IntPtr scene, long entity, ref Quaternion rotate);

        public void TransformSetLocalRotation(long entity, Quaternion rotate)
        {
            Internal_TransformSetLocalRotation(m_nativePointer, entity, ref rotate);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformSetLocalScale", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformSetLocalScale(IntPtr scene, long entity, ref Vector3 scale);

        public void TransformSetLocalScale(long entity, Vector3 scale)
        {
            Internal_TransformSetLocalScale(m_nativePointer, entity, ref scale);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_SetLocalPositionForEntity", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_SetLocalPositionForEntity(IntPtr scene, long entity, ref Vector3 localPosition);

        public void TransformSetLocalPosition(long entity, Vector3 localPosition)
        {
            Internal_SetLocalPositionForEntity(m_nativePointer, entity, ref localPosition);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_TransformUpdate", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_TransformUpdate(IntPtr scene, long entity);

        public void TransformUpdate(long entity)
        {
            Internal_TransformUpdate(m_nativePointer, entity);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_SetNameForEntity", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern void Internal_SetNameForEntity(IntPtr scene, long entity, [MarshalAs(UnmanagedType.LPStr)] string name);

        public void NameForEntity(long entity, string name)
        {
            Internal_SetNameForEntity(m_nativePointer, entity, name);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "Scene_RemoveEntity", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_RemoveEntity(IntPtr scene, long entity);

        public void RemoveEntity(long entity)
        {
            Internal_RemoveEntity(m_nativePointer, entity);
        }
    }
}
