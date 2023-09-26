#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <vd/shader.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

struct Vertex {
	float x, y, z;
	float u, v;
};
Vertex vertices[4] = {
	 //x    y    z    u    v
	{ -1.0,-1.0, 0.0, 0.0, 0.0 }, //Bottom left
	{  1.0,-1.0, 0.0, 1.0, 0.0 }, //Bottom right
	{  1.0, 1.0, 0.0, 1.0, 1.0 },  //Top right
	{ -1.0, 1.0, 0.0, 0.0, 1.0 }  //Top left
};

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices);

unsigned int indices[6] = {
	0, 1, 2, //Triangle 1
	2, 3, 0  //Triangle 2
};

float dayColor[3] = { 0.5f, 0.25f, 0.0f };
float nightColor[3] = { 0.0f, 0.0f, 0.1f };
float mountainColor[3] = { 0.471f, 0.259f, 0.031f };
float sunRadius = 0.3f;
float sunSpeed = 1.0f;
float sunColor[3] = { 1.0f, 0.8f, 0.2f };

float triangleColor[3] = { 1.0f, 0.5f, 0.0f };
float triangleBrightness = 1.0f;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
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

	vd::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	unsigned int vao = createVAO(vertices, 4, indices, 6);

	shader.use();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set uniforms		
		shader.setFloat("iTime", (float)glfwGetTime());
		shader.setVec2("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setVec3("sunColor", sunColor[0], sunColor[1], sunColor[2]);
		shader.setVec3("dayColor", dayColor[0], dayColor[1], dayColor[2]);
		shader.setVec3("nightColor", nightColor[0], nightColor[1], nightColor[2]);
		shader.setVec3("mountainColor", mountainColor[0], mountainColor[1], mountainColor[2]);
		shader.setFloat("sunRadius", sunRadius);
		shader.setFloat("sunSpeed", sunSpeed);


		//Wireframe
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//Shaded
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);
			ImGui::ColorEdit3("Sun Color", sunColor);
			ImGui::ColorEdit3("Day Color", dayColor);
			ImGui::ColorEdit3("Night Color", nightColor);
			ImGui::ColorEdit3("Mountain Color", mountainColor);
			ImGui::SliderFloat("Sun Radius", &sunRadius, 0.0f, 2.0f);
			ImGui::SliderFloat("Sun Speed", &sunSpeed, 0.0f, 10.0f);

			
			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

unsigned int createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices) {
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);

	//Define a new buffer id
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	//Allocate space for + send vertex data to GPU.
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 5, vertexData, GL_STATIC_DRAW);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
	glEnableVertexAttribArray(0);

	//UV
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, u)));
	glEnableVertexAttribArray(1);
	return vao;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

