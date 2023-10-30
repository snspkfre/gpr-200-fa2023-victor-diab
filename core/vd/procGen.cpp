//ew/mesh.h
#include "procGen.h";
#include "../ew/mesh.h"

namespace vd{
	/*ew::MeshData createSphere(float radius, int numSegments)
	{
		for (int i = 0; i < numSegments; i++)
		{

		}
	}*/

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData cylinderMesh;
		
		const float step = 2 * 3.14159265 / (float)numSegments;
		for (int i = 0; i < 2; i++)
		{
			ew::Vertex center;
			center.pos = ew::Vec3(0, height * (i - 0.5), 0);
			center.normal = ew::Vec3(0, -1 + i * 2, 0);
			center.uv = ew::Vec2(0.5, 0.5);
			cylinderMesh.vertices.push_back(center);
			float theta = 0;
			for (int j = 0; j < numSegments; j++)
			{
				ew::Vertex temp;
				temp.pos.x = cos(theta) * radius;
				temp.pos.y = height * (i - 0.5);
				temp.pos.z = sin(theta) * radius;
				temp.uv.x = cos(theta);
				temp.uv.y = sin(theta);
				theta += step;
				temp.normal = ew::Vec3(0, -1 + i * 2, 0);
				cylinderMesh.vertices.push_back(temp);
			}
			for (int j = 0; j < numSegments; j++)
			{
				ew::Vertex temp;
				temp.pos.x = cos(theta) * radius;
				temp.pos.y = height * (i - 0.5);
				temp.pos.z = sin(theta) * radius;
				temp.uv.x = cos(theta);
				temp.uv.y = sin(theta);
				theta += step;
				temp.normal = ew::Vec3(sin(theta), 0, cos(theta));
				cylinderMesh.vertices.push_back(temp);
			}
		}
		
		int start = 1;
		int center = 0;

		for (int i = 0; i < numSegments - 1; i++)
		{
			cylinderMesh.indices.push_back(center);
			cylinderMesh.indices.push_back(start + i);
			cylinderMesh.indices.push_back(start + i + 1);
		}
		cylinderMesh.indices.push_back(center);
		cylinderMesh.indices.push_back(numSegments);
		cylinderMesh.indices.push_back(start);
		
		start = 2 * numSegments + 2;
		center = 2 * numSegments + 1;
		for (int i = 0; i < numSegments - 1; i++)
		{
			cylinderMesh.indices.push_back(start + i);
			cylinderMesh.indices.push_back(center);
			cylinderMesh.indices.push_back(start + i + 1);
		}
		cylinderMesh.indices.push_back(start + numSegments - 1);
		cylinderMesh.indices.push_back(center);
		cylinderMesh.indices.push_back(start);
		

		
		int numCol = numSegments + 1;
		for (int i = 0; i < numCol - 2; i++)
		{
			start = i + 1 + numSegments;
			cylinderMesh.indices.push_back(start + 1);
			cylinderMesh.indices.push_back(start);
			cylinderMesh.indices.push_back(start + numSegments + numCol + 1);
		
			cylinderMesh.indices.push_back(start + numSegments + numCol);
			cylinderMesh.indices.push_back(start + numSegments + numCol + 1);
			cylinderMesh.indices.push_back(start);
		}
		
		cylinderMesh.indices.push_back(1 + numSegments);
		cylinderMesh.indices.push_back(numCol + numSegments - 1);
		cylinderMesh.indices.push_back(numCol + numSegments + 1);
		
		cylinderMesh.indices.push_back(numCol - 1 + numCol + numSegments);
		cylinderMesh.indices.push_back(numCol + numSegments + 1);
		cylinderMesh.indices.push_back(numCol - 1);
		
		return cylinderMesh;
	}	
		
	ew::MeshData createPlane(float width, float height, int subdivisions)
	{	
		ew::MeshData planeMesh;
		
		//std::vector<Vertex> vertices;
		for (int i = 0; i <= subdivisions; i++)
		{
			for (int j = 0; j <= subdivisions; j++)
			{
				ew::Vertex temp;
				float w = width / subdivisions * i;
				float h = height / subdivisions * j;
				temp.pos = ew::Vec3(w, 0, h);
				temp.normal = ew::Vec3(0,1,0);
				float u = (float)j / (float)subdivisions, v = (float)i / (float)subdivisions;
				temp.uv = ew::Vec2(u, v);
				planeMesh.vertices.push_back(temp);
			}
		}
		
		int numCol = subdivisions + 1;
		for (int i = 0; i < subdivisions; i++)
		{
			for (int j = 0; j < subdivisions; j++)
			{
				int start = i * numCol + j;
				planeMesh.indices.push_back(start);
				planeMesh.indices.push_back(start + 1);
				planeMesh.indices.push_back(start + numCol + 1);
				
				planeMesh.indices.push_back(start + numCol + 1);
				planeMesh.indices.push_back(start + numCol);
				planeMesh.indices.push_back(start);
			}
		}
		return planeMesh;
	}
}
