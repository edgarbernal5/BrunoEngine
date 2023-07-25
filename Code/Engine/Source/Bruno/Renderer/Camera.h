#pragma once

#include <Bruno/Math/Math.h>

namespace Bruno
{
	class Camera
	{
	public:
		Camera();

		const Math::Matrix& GetView() const { return m_view; }
		const Math::Matrix& GetViewProjection() const { return m_viewProjection; }

		void SetViewport(const Math::Viewport& viewport);

		void LookAt(const Math::Vector3& position, const Math::Vector3& target, const Math::Vector3& up);
		void SetLens(float fovY, const Math::Viewport& viewport, float nearPlane, float farPlane);

		void HandTool(const Math::Int2& mousePosition, const Math::Int2& previousPosition);
		void Rotate(const Math::Int2& mousePosition, const Math::Int2& previousPosition);
		void PitchYaw(const Math::Int2& mousePosition, const Math::Int2& previousPosition);
		void Zoom(float delta);
		void Strafe(float delta);
		void Walk(float delta);

		void UpdateMatrices();

	private:
		Math::Vector3 m_position;
		Math::Vector3 m_target;
		Math::Vector3 m_up;

		float m_nearPlane;
		float m_farPlane;
		float m_fovY;
		Math::Viewport m_viewport;

		Math::Matrix m_view;
		Math::Matrix m_inverseView;
		Math::Matrix m_projection;
		Math::Matrix m_viewProjection;

		struct States
		{
			bool ViewDirty : 1;
			bool ProjectionDirty : 1;
			bool ViewProjectionDirty : 1;
		};
		States m_states{};
	};
}