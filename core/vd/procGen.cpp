//ew/mesh.h
#include "procGen.h";
#include "..ew/ewMath/ewMath.h"
#include "..ew/ewMath/vec2.h"
#include "..ew/ewMath/vec3.h"

struct Vertex {
	ew::Vec3 pos;
	ew::Vec3 normal;
	ew::Vec2 uv;
}

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

namespace vd{
	ew::MeshData createSphere(float radius, int numSegments)
	{
		/*for (int i = 0; i < numSegments; i++)
		{

		}*/
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		std::vector<Vertex> vertices;
		const float step = 2 * 3.14159265 / numSegments;
		for (int i = 0; i < 2, i++)
		{
			Vertex center;
			center.pos = ew::Vec3(0, height * (i - 0.5), 0);
			center.normal = ew::Vec3(0, -1 + i * 2, 0);
			center.uv = ew::Vec2(0.5, 0.5);
			vertices.pushback(center);
			for (int j = 0; j < numSegments; j++)
			{
				float theta = 0;
				Vertex temp;
				temp.pos.x = cos(theta);
				temp.pos.y = height * (i - 0.5);
				temp.pos.z = sin(theta);
				temp.uv.x = cos(theta);
				temp.uv.y = sin(theta);
				theta += step;
				temp.normal = ew::Vec3(0, -1 + i * 2, 0);
				vertices.pushback(temp);
			}
		}//vertices finished (not tested)
	}

	ew::MeshData createPlane(float size, int subdivisions)
	{
		std::vector<Vertex> vertices;
		for (int i = 0; i <= subdivisions; i++)
		{
			for (int j = 0; j <= subdivisions; j++)
			{
				float width = size / subdivisions * i;
				Vertex temp;
				temp.pos = ew::Vec3(i, i, 0);
				temp.normal = ew::Vec3(0,1,0);
				float u = (float)j / (float)subdivisions, v = (float)i / (float)subdivisions;
				temp.uv = ew::Vec2(u, v);
				vertices.pushback(temp);
			}
		}
		std::vector<unsigned int> indices;
		int numCol = subdivisions + 1;
		for (int i = 0; i <= subdivisions; i++)
		{
			for (int j = 0; j <= subdivisions; j++)
			{
				int start = i * numCol + j;
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + numCol + 1);
				indices.push_back(start + numCol + 1);
				indices.push_back(start + numCol);
				indices.push_back(start);
			}
		}
	}
}
