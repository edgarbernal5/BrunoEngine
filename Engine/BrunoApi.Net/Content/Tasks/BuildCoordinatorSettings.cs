
using System.Runtime.InteropServices;

namespace BrunoApi.Net.Content.Tasks
{
    [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Ansi)]
    public struct BuildCoordinatorSettings
    {
        [MarshalAs(UnmanagedType.LPStr)]
        public string IntermediateDirectory;

        [MarshalAs(UnmanagedType.LPStr)]
        public string OutputDirectory;

        [MarshalAs(UnmanagedType.LPStr)]
        public string RootDirectory;
    }
}
