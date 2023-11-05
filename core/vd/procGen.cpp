//ew/mesh.h
#include "procGen.h";
#include "../ew/mesh.h"

namespace vd{
	ew::MeshData createMobius(float radius, float width, int numSegments)
	{
		ew::MeshData mobiusMesh;

		float step1 = 2 * ew::PI / numSegments;
		float step2 = step1 / 2;
		float halfpi = ew::PI / 2;
		for (int i = 0; i <= numSegments * 4; i++)
		{
			float theta1 = i * step1;
			float theta2 = i * step2;

			ew::Vertex temp;
			ew::Vec3 outPos = ew::Vec3(cos(theta1), 0, sin(theta1)) * radius;
			ew::Vec3 inPos = ew::Vec3(cos(theta1) * cos(theta2), sin(theta2), sin(theta1) * cos(theta2)) * width;
			temp.pos = outPos + inPos;
			temp.normal = ew::Normalize(ew::Vec3(cos(theta1) * cos(theta2 - halfpi), sin(theta2 - halfpi), sin(theta1) * cos(theta2 - halfpi)));
			temp.uv.x = (float)i / numSegments;
			temp.uv.y = 0;
			mobiusMesh.vertices.push_back(temp);
			temp.pos = outPos - inPos;
			temp.uv.y = 1;
			mobiusMesh.vertices.push_back(temp);
		}

		for (int i = 0; i < numSegments * 4; i += 2)
		{

			mobiusMesh.indices.push_back(i + 1);
			mobiusMesh.indices.push_back(i);
			mobiusMesh.indices.push_back(i + 2);

			mobiusMesh.indices.push_back(i + 1);
			mobiusMesh.indices.push_back(i + 2);
			mobiusMesh.indices.push_back(i + 3);
		}
		return mobiusMesh;
	}

	ew::MeshData createTorus(float radius, float thickness, int numSegmentsOut, int numSegmentsIn)
	{
		ew::MeshData torusMesh;

		float outStep = 2 * ew::PI / numSegmentsOut;
		float inStep = 2 * ew::PI / numSegmentsIn;

		for (int i = 0; i <= numSegmentsOut; i++)
		{
			float thetaOut = i * outStep;
			ew::Vec3 outPos = ew::Vec3(cos(thetaOut), sin(thetaOut), 0) * radius;
			for (int j = 0; j <= numSegmentsIn; j++)
			{
				float thetaIn = j * inStep;
				ew::Vec3 inPos = ew::Vec3(cos(thetaIn) * cos(thetaOut), cos(thetaIn) * sin(thetaOut), sin(thetaIn)) * thickness;
				ew::Vertex temp;
				temp.pos = inPos + outPos;
				temp.normal = ew::Normalize(inPos);
				temp.uv.x = (float)j / (float)numSegmentsIn;
				temp.uv.y = (float)i / (float)numSegmentsOut;
				torusMesh.vertices.push_back(temp);
			}
		}


		int columns = numSegmentsIn + 1;
		
		for (int i = 0; i < numSegmentsOut; i++)
		{
			for (int j = 0; j < numSegmentsIn; j++)
			{
				int start = i * columns + j;
				
				torusMesh.indices.push_back(start + 1);
				torusMesh.indices.push_back(start);
				torusMesh.indices.push_back(start + columns + 1);
				
				torusMesh.indices.push_back(start);
				torusMesh.indices.push_back(start + columns);
				torusMesh.indices.push_back(start + columns + 1);
			}
		}

		return torusMesh;
	}

	ew::MeshData createSphere(float radius, int numSegments)
	{
		ew::MeshData sphereMesh;

		float thetaStep = 2 * ew::PI / numSegments;
		float phiStep = ew::PI / numSegments;

		for (int i = 0; i <= numSegments; i++)
		{
			float phi = i * phiStep;
			for (int j = 0; j <= numSegments; j++)
			{
				float theta = j * thetaStep;
				ew::Vertex temp;
				temp.pos.x = radius * cos(theta) * sin(phi);
				temp.pos.y = radius * cos(phi);
				temp.pos.z = radius * sin(theta) * sin(phi);
				temp.normal = ew::Normalize(temp.pos);
				temp.uv.x = (float)j / (float)numSegments;
				temp.uv.y = (float)i / (float)numSegments;
				sphereMesh.vertices.push_back(temp);
			}
		}

		int poleStart = 0;
		int sideStart = numSegments + 1;

		for (int i = 0; i < numSegments; i++)
		{
			sphereMesh.indices.push_back(sideStart + i);
			sphereMesh.indices.push_back(poleStart + i);
			sphereMesh.indices.push_back(sideStart + i + 1);
		}

		int columns = numSegments + 1;
		for (int i = 1; i < numSegments - 1; i++)
		{
			for (int j = 0; j < numSegments; j++)
			{
				int start = i * columns + j;
				
				sphereMesh.indices.push_back(start);
				sphereMesh.indices.push_back(start + 1);
				sphereMesh.indices.push_back(start + columns + 1);
				
				sphereMesh.indices.push_back(start + columns);
				sphereMesh.indices.push_back(start);
				sphereMesh.indices.push_back(start + columns + 1);
			}
		}
		
		poleStart = (numSegments + 1) * numSegments;
		sideStart = poleStart - numSegments - 1;

		for (int i = 0; i < numSegments; i++)
		{

			sphereMesh.indices.push_back(poleStart + i);
			sphereMesh.indices.push_back(sideStart + i);
			sphereMesh.indices.push_back(sideStart + i + 1);
		}

		return sphereMesh;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments)
	{
		ew::MeshData cylinderMesh;
		
		const float step = 2 * ew::PI / (float)numSegments;
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
			for (int j = 0; j <= numSegments; j++)
			{
				ew::Vertex temp;
				temp.pos.x = sin(theta) * radius;
				temp.pos.y = height * (i - 0.5) * -1;
				temp.pos.z = cos(theta) * radius;
				temp.uv.x = temp.pos.x / radius / 2 + 0.5;
				temp.uv.y = temp.pos.z / radius / 2 + 0.5;
				theta += step;
				temp.normal = ew::Vec3(0, 1 - i * 2, 0);
				cylinderMesh.vertices.push_back(temp);
			}
		}
		for (int i = 0; i < 2; i++)
		{
			float theta = 0;
			for (int j = 0; j <= numSegments; j++)
			{
				ew::Vertex temp;
				temp.pos.x = sin(theta) * radius;
				temp.pos.y = height * (i - 0.5) * -1;
				temp.pos.z = cos(theta) * radius;
				temp.uv.x = (float)j / (float)numSegments;
				temp.uv.y = 1-i;
				temp.normal = ew::Vec3(sin(theta), 0, cos(theta));
				cylinderMesh.vertices.push_back(temp);
				theta += step;
			}
		}
		
		int start = 2;
		int center = 0;

		for (int i = 0; i < numSegments; i++)
		{
			cylinderMesh.indices.push_back(center);
			cylinderMesh.indices.push_back(start + i);
			cylinderMesh.indices.push_back(start + i + 1);
		}
		
		start += numSegments + 1;
		center++;
		
		for (int i = 0; i < numSegments; i++)
		{
			cylinderMesh.indices.push_back(start + i);
			cylinderMesh.indices.push_back(center);
			cylinderMesh.indices.push_back(start + i + 1);
		}
		
		
		int sideStart = start + numSegments + 1;
		int numCol = numSegments + 1;
		for (int i = 0; i < numCol - 1; i++)
		{
			int start = i + sideStart;
		
			cylinderMesh.indices.push_back(start + 1);
			cylinderMesh.indices.push_back(start);
			cylinderMesh.indices.push_back(start + numCol + 1);
		
		
			cylinderMesh.indices.push_back(start + numCol);
			cylinderMesh.indices.push_back(start + numCol + 1);
			cylinderMesh.indices.push_back(start);
		}
		
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
