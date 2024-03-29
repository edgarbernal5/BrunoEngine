#pragma once

namespace Bruno::Gizmo
{
	constexpr float		LINE_LENGTH = 3.0f;
	constexpr float		CONE_HEIGHT = 0.5f;
	constexpr float		CONE_RADIUS = 0.25f * 0.85f;
	constexpr float		STICK_RADIUS = CONE_RADIUS * 0.5f;
	
	constexpr float		LINE_OFFSET = 1.0f;
	constexpr uint32_t	TESSELLATION = 8;
	constexpr uint32_t	RING_TESSELLATION = 32;
	constexpr float		GIZMO_LENGTH = LINE_LENGTH + CONE_HEIGHT;

	constexpr float		GIZMO_SCREEN_SCALE = 0.05f;
	constexpr float		GIZMO_SIZE_CLIP_SPACE = 2.3f;
	constexpr float		CAMERA_GIZMO_SCREEN_SIZE_IN_PIXELS = 60.0f;

	constexpr float		MULTI_AXIS_THICKNESS = 0.05f;
	constexpr float		SINGLE_AXIS_THICKNESS = 0.35f;
}