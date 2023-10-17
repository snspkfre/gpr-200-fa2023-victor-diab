#pragma once
#include "../ew/ewMath/ewMath.h"
#include "../ew/ewMath/mat4.h"
#include "../ew/ewMath/vec3.h"
#include "../ew/ewMath/transformations.h"

namespace vd {
	struct Camera {
		ew::Vec3 position = ew::Vec3(0, 0, 5); //Camera body position
		ew::Vec3 target = ew::Vec3(0, 0, 0); //Position to look at
		float fov = 60; //Vertical field of view in degrees
		float aspectRatio = 1080.0 / 720.0; //Screen width / Screen height
		float nearPlane = 0.1; //Near plane distance (+Z)
		float farPlane = 100; //Far plane distance (+Z)
		bool orthographic = false; //Perspective or orthographic?
		float orthoSize = 6; //Height of orthographic frustum
		ew::Mat4 ViewMatrix()
		{
			return ew::LookAt(position, target, ew::Vec3(0, 1, 0));
			//World->View
		}
		ew::Mat4 ProjectionMatrix()
		{
			return (orthographic ?
				ew::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane) :
				ew::Perspective(ew::Radians(fov), aspectRatio, nearPlane, farPlane));
			//View->Clip
		}
	};

	struct CameraControls {
		double prevMouseX, prevMouseY; //Mouse position from previous frame
		float yaw = 0, pitch = 0; //Degrees
		float mouseSensitivity = 0.1f; //How fast to turn with mouse
		bool firstMouse = true; //Flag to store initial mouse position
		float moveSpeed = 0.1f; //How fast to move with arrow keys (M/S)
	};
}
