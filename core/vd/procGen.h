//procGen.h
#pragma once
#include "../ew/mesh.h"
#include <vector>
namespace vd {
	ew::MeshData createAsteroid(float radius, float thickness, int numSegmentsOut, int numSegmentsIn);
	ew::MeshData createMobius(float radius, float width, int segments);
	ew::MeshData createTorus(float radius, float thickness, int numSegmentsOut, int numSegmentsIn);
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float width, float height, int subdivisions);
}