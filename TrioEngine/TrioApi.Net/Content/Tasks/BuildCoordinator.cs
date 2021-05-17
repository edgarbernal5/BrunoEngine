﻿
using Estero.Interop;
using System;
using System.Linq;
using System.Runtime.InteropServices;

namespace TrioApi.Net.Content.Tasks
{
    public class BuildCoordinator : CppObject
    {
        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_Ctor", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern IntPtr Internal_ctor(
            [MarshalAs(UnmanagedType.LPStr)] string intermediateDirectory,
            [MarshalAs(UnmanagedType.LPStr)] string outputDirectory,
            [MarshalAs(UnmanagedType.LPStr)] string rootDirectory);

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_Ctor2", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern IntPtr Internal_ctor2(BuildCoordinatorSettings settings);

        public BuildCoordinator(BuildCoordinatorSettings settings)
        {
            //m_nativePointer = Internal_ctor(settings.IntermediateDirectory, settings.OutputDirectory, settings.RootDirectory);
            m_nativePointer = Internal_ctor2(settings);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_Dtor", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_dtor(IntPtr buildCoordinator);

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_GetSettings", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern void Internal_GetSettings(IntPtr buildCoordinator, IntPtr intermediateDir, IntPtr outputDir, IntPtr rootDir);

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_GetSettings2", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern void Internal_GetSettings2(IntPtr buildCoordinator, ref BuildCoordinatorSettings settings);

        public BuildCoordinatorSettings Settings
        {
            get
            {
                BuildCoordinatorSettings outcome = new BuildCoordinatorSettings();
                Internal_GetSettings2(m_nativePointer, ref outcome);

                return outcome;
            }
        }

        protected override void OnDisposing(bool disposing)
        {
            if (disposing)
            {
            }
            Internal_dtor(m_nativePointer);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_RunTheBuild", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_RunTheBuild(IntPtr buildCoordinator);

        public void RunTheBuild()
        {
            Internal_RunTheBuild(m_nativePointer);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_RequestBuildWithoutOpaqueData", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern void Internal_RequestBuildWithoutOpaqueData(IntPtr buildCoordinator,
            [MarshalAs(UnmanagedType.LPStr)] string sourceFilename,
            [MarshalAs(UnmanagedType.LPStr)] string assetName,
            [MarshalAs(UnmanagedType.LPStr)] string importerName,
            [MarshalAs(UnmanagedType.LPStr)] string processorName);

        public void RequestBuild(string sourceFilename, string assetName, string importerName, string processorName)
        {
            Internal_RequestBuildWithoutOpaqueData(m_nativePointer, sourceFilename, assetName, importerName, processorName);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_GetRelativePath", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        [return: MarshalAs(UnmanagedType.LPStr)]
        private static extern string Internal_GetRelativePath(IntPtr coordinator, [MarshalAs(UnmanagedType.LPStr)] string path);

        public string GetRelativePath(string path)
        {
            return Internal_GetRelativePath(m_nativePointer, path);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "BuildCoordinator_GetOutputFiles", CallingConvention = CallingConvention.StdCall, CharSet = CharSet.Ansi)]
        private static extern void Internal_GetOutputFiles(IntPtr coordinator,
            ref IntPtr outputFilenames,
            ref int size);

        public string[] GetOutputFiles()
        {
            int total = 0;
            IntPtr unmanagedArray = IntPtr.Zero;
            Internal_GetOutputFiles(m_nativePointer, ref unmanagedArray, ref total);

            if (total > 0)
            {
                string[] managedArray = new string[total];

                IntPtr[] intPtrArray = new IntPtr[total];
                Marshal.Copy(unmanagedArray, intPtrArray, 0, total);

                for (int i = 0; i < total; i++)
                {
                    managedArray[i] = Marshal.PtrToStringAnsi(intPtrArray[i]);
                    Marshal.FreeCoTaskMem(intPtrArray[i]);
                }

                Marshal.FreeCoTaskMem(unmanagedArray);
                return managedArray;
            }

            return new string[0];
        }
    }
}
