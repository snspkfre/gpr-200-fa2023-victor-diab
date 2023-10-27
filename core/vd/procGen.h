//procGen.h
#pragma once
#include "..ew/mesh.h"
#include <vector>
namespace vd {
	ew::MeshData createSphere(float radius, int numSegments);
	ew::MeshData createCylinder(float height, float radius, int numSegments);
	ew::MeshData createPlane(float size, int subdivisions);
}