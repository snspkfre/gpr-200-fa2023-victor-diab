#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/texture.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <ew/camera.h>
#include <ew/cameraController.h>
#include <vd/procGen.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void resetCamera(ew::Camera& camera, ew::CameraController& cameraController);

int SCREEN_WIDTH = 1080;
int SCREEN_HEIGHT = 720;

float prevTime;

struct AppSettings {
	const char* shadingModeNames[6] = { "Solid Color","Normals","UVs","Texture","Lit","Texture Lit"};
	int shadingModeIndex;

	ew::Vec3 bgColor = ew::Vec3(0.1f);
	ew::Vec3 shapeColor = ew::Vec3(1.0f);
	bool wireframe = true;
	bool drawAsPoints = false;
	bool backFaceCulling = true;

	//Euler angles (degrees)
	ew::Vec3 lightRotation = ew::Vec3(0, 0, 0);
}appSettings;

ew::Camera camera;
ew::CameraController cameraController;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);
	glPointSize(3.0f);
	glPolygonMode(GL_FRONT_AND_BACK, appSettings.wireframe ? GL_LINE : GL_FILL);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int brickTexture = ew::loadTexture("assets/brick_color.jpg",GL_REPEAT,GL_LINEAR);

	int sphereSegments = 10;
	int cylinderSegments = 10;
	int planeSegments = 5;

	float wPlane = 0.5, hPlane = 0.5;
	float hCylinder = 0.5, cRad = 0.25;
	float sRad = 0.5;

	float tRad = 0.5, tThick = 0.3;
	int tSegmentsOut = 10, tSegmentsIn = 8;
	float mRad = 0.5, mWidth = 0.2;
	int mSegments = 20;


	//Create cube
	ew::MeshData cubeMeshData = ew::createCube(0.5f);
	ew::Mesh cubeMesh(cubeMeshData);
	ew::MeshData planeMeshData = vd::createPlane(wPlane, hPlane, planeSegments);
	ew::Mesh planeMesh(planeMeshData);
	ew::MeshData cylinderMeshData = vd::createCylinder(hCylinder, cRad, cylinderSegments);
	ew::Mesh cylinderMesh(cylinderMeshData);
	ew::MeshData sphereMeshData = vd::createSphere(sRad, sphereSegments);
	ew::Mesh sphereMesh(sphereMeshData);
	ew::MeshData torusMeshData = vd::createTorus(tRad, tThick, tSegmentsOut, tSegmentsIn);
	ew::MeshData mobiusMeshData = vd::createMobius(mRad, mWidth, mSegments);

	//Initialize transforms
	ew::Transform cubeTransform;
	cubeTransform.position.x -= 1.5;
	ew::Transform planeTransform;
	planeTransform.position.x -= 0.75;
	planeTransform.position.y -= 0.2;
	planeTransform.position.z -= 0.25;
	ew::Transform cylinderTransform;
	cylinderTransform.position.x += 0.5;
	ew::Transform sphereTransform;
	sphereTransform.position.x += 1.5;
	ew::Transform torusTransform;
	torusTransform.position.x += 3;
	ew::Transform mobiusTransform;
	mobiusTransform.position.x -= 3;

	resetCamera(camera,cameraController);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;

		float time = (float)glfwGetTime();
		float deltaTime = time - prevTime;
		prevTime = time;

		cameraController.Move(window, &camera, deltaTime);

		//Render
		glClearColor(appSettings.bgColor.x, appSettings.bgColor.y, appSettings.bgColor.z,1.0f);

		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		

		shader.use();
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		shader.setInt("_Texture", 0);
		shader.setInt("_Mode", appSettings.shadingModeIndex);
		shader.setVec3("_Color", appSettings.shapeColor);
		shader.setMat4("_ViewProjection", camera.ProjectionMatrix() * camera.ViewMatrix());

		//Euler angels to forward vector
		ew::Vec3 lightRot = appSettings.lightRotation * ew::DEG2RAD;
		ew::Vec3 lightF = ew::Vec3(sinf(lightRot.y) * cosf(lightRot.x), sinf(lightRot.x), -cosf(lightRot.y) * cosf(lightRot.x));
		shader.setVec3("_LightDir", lightF);

		//Draw cube
		shader.setMat4("_Model", cubeTransform.getModelMatrix());
		cubeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		/*shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		*/

		planeMeshData = vd::createPlane(wPlane, hPlane, planeSegments);
		cylinderMeshData = vd::createCylinder(hCylinder, cRad, cylinderSegments);
		sphereMeshData = vd::createSphere(sRad, sphereSegments);
		torusMeshData = vd::createTorus(tRad, tThick, tSegmentsOut, tSegmentsIn);
		mobiusMeshData = vd::createMobius(mRad, mWidth, mSegments);
		
		
		ew::Mesh planeMesh(planeMeshData);
		ew::Mesh cylinderMesh(cylinderMeshData);
		ew::Mesh sphereMesh(sphereMeshData);
		ew::Mesh torusMesh(torusMeshData);
		ew::Mesh mobiusMesh(mobiusMeshData);
		
		shader.setMat4("_Model", planeTransform.getModelMatrix());
		planeMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", cylinderTransform.getModelMatrix());
		cylinderMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", sphereTransform.getModelMatrix());
		sphereMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", torusTransform.getModelMatrix());
		torusMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		shader.setMat4("_Model", mobiusTransform.getModelMatrix());
		mobiusMesh.draw((ew::DrawMode)appSettings.drawAsPoints);
		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			if (ImGui::CollapsingHeader("Camera")) {
				ImGui::DragFloat3("Position", &camera.position.x, 0.1f);
				ImGui::DragFloat3("Target", &camera.target.x, 0.1f);
				ImGui::Checkbox("Orthographic", &camera.orthographic);
				if (camera.orthographic) {
					ImGui::DragFloat("Ortho Height", &camera.orthoHeight, 0.1f);
				}
				else {
					ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
				}
				ImGui::DragFloat("Near Plane", &camera.nearPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Far Plane", &camera.farPlane, 0.1f, 0.0f);
				ImGui::DragFloat("Move Speed", &cameraController.moveSpeed, 0.1f);
				ImGui::DragFloat("Sprint Speed", &cameraController.sprintMoveSpeed, 0.1f);
				if (ImGui::Button("Reset")) {
					resetCamera(camera, cameraController);
				}
			}

			ImGui::ColorEdit3("BG color", &appSettings.bgColor.x);
			ImGui::ColorEdit3("Shape color", &appSettings.shapeColor.x);
			ImGui::Combo("Shading mode", &appSettings.shadingModeIndex, appSettings.shadingModeNames, IM_ARRAYSIZE(appSettings.shadingModeNames));
			if (appSettings.shadingModeIndex > 3) {
				ImGui::DragFloat3("Light Rotation", &appSettings.lightRotation.x, 1.0f);
			}
			ImGui::Checkbox("Draw as points", &appSettings.drawAsPoints);
			if (ImGui::Checkbox("Wireframe", &appSettings.wireframe)) {
				glPolygonMode(GL_FRONT_AND_BACK, appSettings.wireframe ? GL_LINE : GL_FILL);
			}
			if (ImGui::Checkbox("Back-face culling", &appSettings.backFaceCulling)) {
				if (appSettings.backFaceCulling)
					glEnable(GL_CULL_FACE);
				else
					glDisable(GL_CULL_FACE);
			}
			if (ImGui::CollapsingHeader("Bonus - Dynamic")) 
			{
				ImGui::DragFloat("Sphere Radius", &sRad, 0.1f, 0.1f, 100000);
				ImGui::DragInt("Sphere Segments", &sphereSegments, 1.0f, 3, 100000);
				ImGui::DragFloat("Cylinder Height", &hCylinder, 0.1f, 0.1f, 100000);
				ImGui::DragFloat("Cylinder Radius", &cRad, 0.1f, 0.1f, 100000);
				ImGui::DragInt("Cylinder Segments", &cylinderSegments, 1.0f, 3, 10000);
				ImGui::DragFloat("Plane Width", &wPlane, 0.1f, 0.1f, 100000);
				ImGui::DragFloat("Plane Height", &hPlane, 0.1f, 0.1f, 100000);
				ImGui::DragInt("Plane Segments", &planeSegments, 1.0f, 1, 100000);
				ImGui::DragFloat("Torus Radius", &tRad, 0.1f, 0.1f, 100000);
				ImGui::DragFloat("Torus Thickness", &tThick, 0.1f, 0.1f, 100000);
				ImGui::DragInt("Torus Outer Segments", &tSegmentsOut, 1.0f, 3, 100000);
				ImGui::DragInt("Torus Inner Segments", &tSegmentsIn, 1.0f, 3, 100000);
				ImGui::DragFloat("Mobius Radius", &mRad, 0.1f, 0.1f, 10000000);
				ImGui::DragFloat("Mobius Width", &mWidth, 0.1f, 0.1f, 10000000);
				ImGui::DragInt("Mobius Segments", &mSegments, 1.0f, 4, 100000);
			}

			ImGui::End();
			
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	SCREEN_WIDTH = width;
	SCREEN_HEIGHT = height;
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
}

void resetCamera(ew::Camera& camera, ew::CameraController& cameraController) {
	camera.position = ew::Vec3(0, 0, 3);
	camera.target = ew::Vec3(0);
	camera.aspectRatio = (float)SCREEN_WIDTH / SCREEN_HEIGHT;
	camera.fov = 60.0f;
	camera.orthoHeight = 6.0f;
	camera.nearPlane = 0.1f;
	camera.farPlane = 100.0f;
	camera.orthographic = false;

	cameraController.yaw = 0.0f;
	cameraController.pitch = 0.0f;
}


