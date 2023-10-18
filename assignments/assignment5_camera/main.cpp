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
void moveCamera(GLFWwindow* window, vd::Camera* camera, vd::CameraControls* controls, float deltaTime);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
vd::Transform cubeTransforms[NUM_CUBES];
vd::Camera cam;
vd::CameraControls camControls;

bool orbitting = false;
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
	float prevTime = 0; //Timestamp of previous frame
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		float time = (float)glfwGetTime(); //Timestamp of current frame
		float deltaTime = time - prevTime;
		prevTime = time;

		moveCamera(window, &cam, &camControls, deltaTime);

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
					else
						cam.position.y = 0;
				}
				else
				{
					funny2 = false;
				}
			}
			ImGui::DragFloat3("Position", &cam.position.x, 0.05f);
			ImGui::DragFloat3("Target", &cam.target.x, 0.05f);
			ImGui::Checkbox("Orthographic", &cam.orthographic);
			if (cam.orthographic)
				ImGui::DragFloat("Ortho Height", &cam.orthoSize, 0.05f);
			else
				ImGui::SliderFloat("fov", &cam.fov, 0, 180);
			ImGui::DragFloat("Near Plane", &cam.nearPlane, 0.05f);
			ImGui::DragFloat("Far Plane", &cam.farPlane, 0.05f);
			if (ImGui::Button("Reset"))
			{
				camControls.yaw = 0, camControls.pitch = 0; //Degrees
				cam.position = ew::Vec3(0, 0, 5); //Camera body position
				cam.target = ew::Vec3(0, 0, 0); //Position to look at
				cam.fov = 60; //Vertical field of view in degrees
				cam.aspectRatio = 1080.0 / 720.0; //Screen width / Screen height
				cam.nearPlane = 0.1; //Near plane distance (+Z)
				cam.farPlane = 100; //Far plane distance (+Z)
				cam.orthographic = false; //Perspective or orthographic?
				cam.orthoSize = 6; //Height of orthographic frustum
			}
			
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

void moveCamera(GLFWwindow* window, vd::Camera* camera, vd::CameraControls* controls, float deltaTime)
{
	//If right mouse is not held, release cursor and return early.
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move our mouse.
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just started right clicking, set prevMouse values to current position.
	//This prevents a bug where the camera moves as soon as we click.
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//TODO: Get mouse position delta for this frame
	ew::Vec2 mousePosDelta = ew::Vec2(mouseX - controls->prevMouseX, mouseY - controls->prevMouseY);
	//TODO: Add to yaw and pitch
	controls->yaw += mousePosDelta.x;
	controls->pitch -= mousePosDelta.y;
	//TODO: Clamp pitch between -89 and 89 degrees
	if (controls->pitch >= 89)
		controls->pitch = 89;
	else if(controls->pitch <= -89)
		controls->pitch = -89;
	
	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	ew::Vec3 forward = ew::Vec3(cosf(ew::Radians(controls->yaw - 90))*cosf(ew::Radians(controls->pitch)), sinf(ew::Radians(controls->pitch)), sinf(ew::Radians(controls->yaw - 90)) * cosf(ew::Radians(controls->pitch)));
		//By setting target to a point in front of the camera along its forward direction, our LookAt will be updated accordingly when rendering.
	//camera->target = camera->position + forward;

	//TODO: Using camera forward and world up (0,1,0), construct camera right and up vectors. Graham-schmidt process!
	ew::Vec3 right = ew::Normalize(ew::Cross(ew::Vec3(0,1,0), forward));
	ew::Vec3 up = ew::Normalize(ew::Cross(forward, right));
	//TODO: Keyboard controls for moving along forward, back, right, left, up, and down. See Requirements for key mappings.
	if (glfwGetKey(window, GLFW_KEY_D) && !glfwGetKey(window, GLFW_KEY_A)) {
		camera->position -= right * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_A) && !glfwGetKey(window, GLFW_KEY_D)) {
		camera->position += right * controls->moveSpeed * deltaTime;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) && !glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		camera->position += up * controls->moveSpeed * deltaTime;
	}
	if (!glfwGetKey(window, GLFW_KEY_SPACE) && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
		camera->position -= up * controls->moveSpeed * deltaTime;
	}
	//EXAMPLE: Moving along forward axis if W is held.
	//Note that this is framerate dependent, and will be very fast until you scale by deltaTime. See the next section.
	if (glfwGetKey(window, GLFW_KEY_W) && !glfwGetKey(window, GLFW_KEY_S)) {
		camera->position += forward * controls->moveSpeed * deltaTime;
	}
	if (glfwGetKey(window, GLFW_KEY_S) && !glfwGetKey(window, GLFW_KEY_W)) {
		camera->position -= forward * controls->moveSpeed * deltaTime;
	}
	//Setting camera.target should be done after changing position. Otherwise, it will use camera.position from the previous frame and lag behind
	camera->target = camera->position + forward;

}