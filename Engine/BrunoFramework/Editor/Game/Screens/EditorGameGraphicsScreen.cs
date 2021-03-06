
using BrunoFramework.Editor.Game.Gizmos;
using BrunoFramework.Editor.Units;
using BrunoFramework.Graphics;
using BrunoFramework.Graphics.Data;
using BrunoFramework.Graphics.Editor;
using System;
using BrunoApi.Net.Graphics.Core;
using BrunoApi.Net.Maths;
using BrunoApi.Net.Renderer;

namespace BrunoFramework.Editor.Graphics
{
    public class EditorGameGraphicsScreen : GameGraphicsScreen
    {
        public GridMesh GridMesh
        {
            get => m_gridMesh;
            set => m_gridMesh = value;
        }
        private GridMesh m_gridMesh;

        public IGizmoService GizmoService
        {
            get => m_gizmoService;
            set => m_gizmoService = value;
        }
        private IGizmoService m_gizmoService;

        public RenderPath RenderPath
        {
            get => m_renderPath;
            set => m_renderPath = value;
        }
        private RenderPath m_renderPath;

        public EditorGameGraphicsScreen() 
            : base()
        {
            
        }

        public override void Update(TimeSpan deltaTime)
        {
            //Scene.Update();
        }

        public override void Render(RenderContext renderContext)
        {
            renderContext.Screen = this;
            DoOnRender(renderContext);
            renderContext.Screen = null;
        }

        private void DoOnRender(RenderContext renderContext)
        {
            BrunoApi.Net.Game.Scene.ActiveScene = Scene;
            Scene.Update();

            renderContext.GraphicsDevice.Clear(ColorRGBA8.CornflowerBlue);

            //
            renderContext.Camera = Camera;

            //float time = (float)GameUnit.m_gameStepTimer.TotalTime.TotalSeconds;
            //Camera.Rotation = Quaternion.CreateFromYawPitchRoll(time * 0.1f, -0.3f, 0);

            //Camera.Recalculate();

            BrunoApi.Net.Game.Scene.UpdateCamera(ConvertToCamera(Camera));
            m_renderPath.Render();

            m_gridMesh.Render(renderContext);

            m_gizmoService.Render(renderContext);
        }

        private Renderer.Camera ConvertToCamera(Camera camera)
        {
            return new Renderer.Camera()
            {
                m_position = camera.Position,
                m_target = camera.Target,
                m_up = camera.Up,

                m_nearPlane = camera.NearPlane,
                m_farPlane = camera.FarPlane,
                m_fieldOfView = camera.FieldOfView,
                m_aspectRatio = camera.AspectRatio
            };
        }
    }
}
