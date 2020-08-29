﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using TrioWpfFramework.Net;
using TrioWpfFramework.Net.Graphics;

namespace TrioApi.Net.Graphics.Core
{
    public class RenderTarget2D : Texture2D
    {
        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "RenderTarget2D_Ctor", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_Ctor(IntPtr device, int width, int height, int preferredFormat);

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "RenderTarget2D_Ctor2", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_Ctor2(IntPtr device, int width, int height, uint mipmap, int preferredFormat, int preferredDepthFormat, int renderTargetUsage);

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "RenderTarget2D_Ctor3", CallingConvention = CallingConvention.StdCall)]
        private static extern IntPtr Internal_Ctor3(IntPtr device, int width, int height, uint mipmap, int preferredFormat, int preferredDepthFormat);

        [DllImport(ImportConfiguration.DllImportFilename, EntryPoint = "RenderTarget2D_Dctor", CallingConvention = CallingConvention.StdCall)]
        private static extern void Internal_Dctor(IntPtr renderTarget);

        public RenderTarget2D(GraphicsDevice device, int width, int height)
        {
            m_nativePtr = Internal_Ctor(device.m_nativePtr, width, height, (int)SurfaceFormat.Color);
        }

        public RenderTarget2D(GraphicsDevice device, int width, int height, SurfaceFormat preferredFormat)
        {
            m_nativePtr = Internal_Ctor(device.m_nativePtr, width, height, (int)preferredFormat);
        }

        public RenderTarget2D(GraphicsDevice device, int width, int height, uint mipmap, SurfaceFormat preferredFormat, DepthFormat preferredDepthFormat, RenderTargetUsage usage)
        {
            m_nativePtr = Internal_Ctor2(device.m_nativePtr, width, height, mipmap, (int)preferredFormat, (int)preferredDepthFormat, (int)usage);
        }

        public RenderTarget2D(GraphicsDevice device, int width, int height, uint mipmap, SurfaceFormat preferredFormat, DepthFormat preferredDepthFormat)
        {
            m_nativePtr = Internal_Ctor3(device.m_nativePtr, width, height, mipmap, (int)preferredFormat, (int)preferredDepthFormat);
        }

        protected override void Dispose(bool disposing)
        {
            if (m_disposed)
            {
                return;
            }

            if (disposing)
            {
                // TODO: dispose managed state (managed objects).
            }

            // TODO: free unmanaged resources (unmanaged objects) and override a finalizer below.
            // TODO: set large fields to null.
            Internal_Dctor(m_nativePtr);
            m_nativePtr = IntPtr.Zero;
            m_disposed = true;

            // Call the base class implementation.
            base.Dispose(disposing);
        }

    }
}