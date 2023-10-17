/*
	Author: Eric Winebrenner
*/

#pragma once
#include "mat4.h"
#include "vec3.h"
#include "ewMath.h"

namespace ew {
	//Identity matrix
	inline ew::Mat4 Identity() {
		return ew::Mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	};
	//Scale on x,y,z axes
	inline ew::Mat4 Scale(const ew::Vec3& s) {
		return ew::Mat4(
			s.x, 0, 0, 0,
			0, s.y, 0, 0,
			0, 0, s.z, 0,
			0, 0, 0, 1
		);
	};
	//Rotation around X axis (pitch) in radians
	inline ew::Mat4 RotateX(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return Mat4(
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, cosA, -sinA, 0.0f,
			0.0f, sinA, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Rotation around Y axis (yaw) in radians
	inline ew::Mat4 RotateY(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return Mat4(
			cosA, 0.0f, sinA, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			-sinA, 0.0f, cosA, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Rotation around Z axis (roll) in radians
	inline ew::Mat4 RotateZ(float rad) {
		const float cosA = cosf(rad);
		const float sinA = sinf(rad);
		return Mat4(
			cosA, -sinA, 0.0f, 0.0f,
			sinA, cosA, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Translate x,y,z
	inline ew::Mat4 Translate(const ew::Vec3& t) {
		return Mat4(
			1.0f, 0.0f, 0.0f, t.x,
			0.0f, 1.0f, 0.0f, t.y,
			0.0f, 0.0f, 1.0f, t.z,
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};

	inline ew::Mat4 LookAt(ew::Vec3 eye, ew::Vec3 target, ew::Vec3 up) {
		ew::Vec3 f = ew::Normalize(eye - target);
		ew::Vec3 r = ew::Normalize(ew::Cross(up, f));
		ew::Vec3 u = ew::Normalize(ew::Cross(f, r));
		return Mat4(
			r.x, r.y, r.z, -ew::Dot(r, eye),
			u.x, u.y, u.z, -ew::Dot(u, eye),
			f.x, f.y, f.z, -ew::Dot(f, eye),
			0.0f, 0.0f, 0.0f, 1.0f
		);
			//use ew::Cross for cross product!
	};
	//Orthographic projection
	inline ew::Mat4 Orthographic(float height, float aspect, float near, float far) {
		float r = (height * aspect) / 2.0f;
		float t = height / 2.0f;
		float l = r * -1;
		float b = t * -1;

		return Mat4(
			2/(r-l), 0.0f, 0.0f, -(r+l)/(r-l),
			0.0f, 2/(t-b), 0.0f, -(t + b) /( t - b),
			0.0f, 0.0f, -2/(far-near), -(far + near)/(far - near),
			0.0f, 0.0f, 0.0f, 1.0f
		);
	};
	//Perspective projection
	//fov = vertical aspect ratio (radians)
	inline ew::Mat4 Perspective(float fov, float aspect, float near, float far) {
		return ew::Mat4(
			1/(tanf(fov/2)*aspect), 0, 0, 0,
			0, 1/(tanf(fov/2)), 0, 0,
			0, 0, (near+far)/(near-far), (2*far*near)/(near-far),
			0, 0, -1, 0
		);
	};
}
