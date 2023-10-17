#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include <vd/camera.h>
#include <vd/transformations.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
vd::Transform cubeTransforms[NUM_CUBES];
vd::Camera cam;

bool orbitting = true;
bool funny = false;
bool funny2 = false;

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

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();
		shader.setMat4("_View", cam.ViewMatrix());
		shader.setMat4("_Projection", cam.ProjectionMatrix());
		//TODO: Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//Construct model matrix
			shader.setMat4("_Model", cubeTransforms[i].getModelMatrix());
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Camera");
			ImGui::Checkbox("Orbit", &orbitting);
			if (orbitting)
			{
				cam.position.x = cosf((float) glfwGetTime()) * 5.0;
				cam.position.z = sinf((float) glfwGetTime()) * 5.0;
				funny = false;
			}
			else
			{
				ImGui::Checkbox("Funny", &funny);
				if (funny)
				{
					cam.position.x = cosf((float)glfwGetTime()) * 5.0 * sinf((float)glfwGetTime() * 5.0);
					cam.position.z = sinf((float)glfwGetTime()) * 5.0 * sinf((float)glfwGetTime() * 5.0);
					ImGui::Checkbox("More funny", &funny2);
					if (funny2)
						cam.position.y = sinf((float)glfwGetTime() * 25) * 5.0;
				}
				else
					funny2 = false;
			}
			ImGui::DragFloat3("Position", &cam.position.x, 0.05f);
			ImGui::DragFloat3("Target", &cam.target.x, 0.05f);
			ImGui::Checkbox("Orthographic", &cam.orthographic);
			if (cam.orthographic)
				ImGui::DragFloat("Ortho Height", &cam.orthoSize, 0.05f);
			else
				ImGui::SliderFloat("fov", &cam.fov, 0, 180);
			ImGui::DragFloat("Near Plane", &cam.nearPlane, 0.0f);
			ImGui::DragFloat("Far Plane", &cam.farPlane, 0.0f);
			
			/*ImGui::Text("Cubes");
			for (size_t i = 0; i < NUM_CUBES; i++)
			{
				ImGui::PushID(i);
				if (ImGui::CollapsingHeader("Transform")) {
					ImGui::DragFloat3("Position", &cubeTransforms[i].position.x, 0.05f);
					ImGui::DragFloat3("Rotation", &cubeTransforms[i].rotation.x, 1.0f);
					ImGui::DragFloat3("Scale", &cubeTransforms[i].scale.x, 0.05f);
				}
				ImGui::PopID();
			}*/

			
			
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
	cam.aspectRatio = (float) width / (float) height;
}

