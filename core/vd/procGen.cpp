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
			center.pos = ew::Vec3(0, height * (i - 0.5) * -1, 0);
			center.normal = ew::Vec3(0, 1 - i * 2, 0);
			center.uv = ew::Vec2(0.5, 0.5);
			cylinderMesh.vertices.push_back(center);
		}
		for (int i = 0; i < 2; i++)
		{
			float theta = 0;
			for (int j = 0; j < numSegments; j++)
			{
				ew::Vertex temp;
				temp.pos.x = sin(theta) * radius;
				temp.pos.y = height * (i - 0.5) * -1;
				temp.pos.z = cos(theta) * radius;
				temp.uv.x = temp.pos.x / radius;
				temp.uv.y = temp.pos.z / radius;
				theta += step;
				temp.normal = ew::Vec3(0, 1 - i * 2, 0);
				cylinderMesh.vertices.push_back(temp);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			float theta = -0;
			for (int j = 0; j < numSegments; j++)
			{
				ew::Vertex temp;
				temp.pos.x = sin(theta) * radius;
				temp.pos.y = height * (i - 0.5) * -1;
				temp.pos.z = cos(theta) * radius;
				temp.uv.x = (float)j / (float)numSegments;
				temp.uv.y = i;
				theta += step;
				temp.normal = ew::Vec3(cos(theta), 0, sin(theta));
				cylinderMesh.vertices.push_back(temp);
			}
		}
		
		int start = 2;
		int center = 0;

		for (int i = 0; i < numSegments - 1; i++)
		{
			cylinderMesh.indices.push_back(center);
			cylinderMesh.indices.push_back(start + i);
			cylinderMesh.indices.push_back(start + i + 1);
		}
		cylinderMesh.indices.push_back(center); 
		cylinderMesh.indices.push_back(start + numSegments - 1);
		cylinderMesh.indices.push_back(start);
		
		start += numSegments;
		center++;
		
		for (int i = 0; i < numSegments - 1; i++)
		{
			cylinderMesh.indices.push_back(start + i);
			cylinderMesh.indices.push_back(center);
			cylinderMesh.indices.push_back(start + i + 1);
		}
		cylinderMesh.indices.push_back(start + numSegments - 1);
		cylinderMesh.indices.push_back(center);
		cylinderMesh.indices.push_back(start);
		
		
		int sideStart = start + numSegments;
		int numCol = numSegments + 1;
		for (int i = 0; i < numCol - 2; i++)
		{
			int start = i + sideStart;

			cylinderMesh.indices.push_back(start + 1);
			cylinderMesh.indices.push_back(start);
			cylinderMesh.indices.push_back(start + numCol);
		

			cylinderMesh.indices.push_back(start + numCol - 1);
			cylinderMesh.indices.push_back(start + numCol);
			cylinderMesh.indices.push_back(start);
		}


		cylinderMesh.indices.push_back(sideStart);
		cylinderMesh.indices.push_back(sideStart + numCol - 2);
		cylinderMesh.indices.push_back(sideStart + numCol - 1);
		

		cylinderMesh.indices.push_back(sideStart + numCol - 2 + numCol - 1);
		cylinderMesh.indices.push_back(sideStart + numCol - 1);
		cylinderMesh.indices.push_back(sideStart + numCol - 2);
		
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
