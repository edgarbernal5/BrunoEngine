﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace TrioApi.Net.Graphics.Core
{
    public class VertexBuffer : IDisposable
    {
        internal IntPtr m_nativePtr;
        private bool m_disposed;

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "VertexBuffer_Ctor", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_Ctor(IntPtr device, IntPtr vertexDeclaration, int vertexCount, int usage);

        public VertexBuffer(GraphicsDevice device, VertexDeclaration vertexDeclaration, int vertexCount, ResourceUsage usage)
        {
            m_nativePtr = Internal_Ctor(device.m_nativePtr, vertexDeclaration.m_nativePtr, vertexCount, (int)usage);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "VertexBuffer_Ctor2", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_Ctor2(IntPtr device, IntPtr vertexDeclaration, int vertexCount);

        public VertexBuffer(GraphicsDevice device, VertexDeclaration vertexDeclaration, int vertexCount)
        {
            m_nativePtr = Internal_Ctor2(device.m_nativePtr, vertexDeclaration.m_nativePtr, vertexCount);
        }

        internal VertexBuffer(IntPtr nativePtr)
        {
            m_nativePtr = nativePtr;
        }

        public void SetData<T>(T[] data) where T : struct
        {
            SetDataInternal(0, data, 0, data.Length, SetDataOptions.None);
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "VertexBuffer_SetData", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_SetData(IntPtr buffer, IntPtr bytes, uint elementCount, uint sizeArray);

        private void SetDataInternal<T>(int offsetInBytes, T[] data, int startIndex, int elementCount, SetDataOptions options) where T : struct
        {
            int elementSize = Marshal.SizeOf(typeof(T));
            var dataHandle = GCHandle.Alloc(data, GCHandleType.Pinned);

            try
            {
                var dataPtr = (IntPtr)(dataHandle.AddrOfPinnedObject().ToInt64());

                Internal_SetData(m_nativePtr, dataPtr, (uint)(data.Length), (uint)(data.Length * elementSize));
            }
            finally
            {
                dataHandle.Free();
            }
        }

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "VertexBuffer_Dctor", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_Dctor(IntPtr buffer);

        protected virtual void Dispose(bool disposing)
        {
            if (disposing)
            {
                if (!m_disposed)
                {
                    m_disposed = true;
                    Internal_Dctor(m_nativePtr);
                    m_nativePtr = IntPtr.Zero;
                }
            }
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }
    }
}
