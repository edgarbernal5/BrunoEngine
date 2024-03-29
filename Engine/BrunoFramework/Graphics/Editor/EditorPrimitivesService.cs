﻿using System;
using BrunoApi.Net.Graphics.Core;

namespace BrunoFramework.Graphics.Editor
{
    public class EditorPrimitivesService : IEditorPrimitivesService
    {
        private readonly IGraphicsService m_graphicsService;

        public GridMesh GridMesh { get; set; }
        public Effect LineEffect { get; set; }

        public EditorPrimitivesService(IGraphicsService graphicsService)
        {
            m_graphicsService = graphicsService;
        }

        public void InitializeMeshes()
        {
            var device = m_graphicsService.GraphicsDevice;
            GridMesh = new GridMesh(device, 64);

            LineEffect = new Effect(device, @"D:\Edgar\Documentos\Proyectos\CG\BrunoEngineGit\Shaders\LineEffect.fx");
        }
    }
}
