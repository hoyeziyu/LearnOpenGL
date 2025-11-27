#include <glad/glad.h>      // 请务必在引入 GLFW 之前引入 GLAD。GLAD 的头文件包含了后台所需的 OpenGL 头文件
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

/*
	图形管线graphics pipeline可以分为两大部分：
		第一部分将三维坐标转换为二维坐标，(所有三维坐标转换为适合屏幕的二维像素)
		第二部分将二维坐标转换为实际的彩色像素

	图形管线graphics pipeline以一组三维坐标作为input，并将其转换为屏幕上的彩色二维像素
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation 屏幕或窗口是一个二维像素数组
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers（GLAD 管理 OpenGL 的函数指针）
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// build and compile our shader program
	Shader ourShader("assets/shaders/basicvs.vertexshader", "assets/shaders/basicfs.fragmentshader");

	// 作为graphics pipeline的输入(顶点集合)
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	/*
		VBO是用于存储顶点数据的对象，VAO是存储顶点属性配置和要使用的VBO
	*/
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);   // 使用这些缓冲区对象的优势在于，可以一次性向显卡发送大量数据
	/*
		a.bind the Vertex Array Object first,
		b.then bind and set vertex buffer(s),
		c.and then configure vertex attributes(s).
	*/
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// render loop 渲染循环的一次迭代通常被称为一帧
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// render
		/*
			在帧开始时清空屏幕(否则会看到上一帧的结果)
		*/
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // 指定了清屏颜色
		glClear(GL_COLOR_BUFFER_BIT);          // 清空屏幕的颜色缓冲区,整个颜色缓冲区都会填充 glClearColor 配置的颜色

		// draw our first triangle
		ourShader.useProgram();
		glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawArrays(GL_TRIANGLES, 0, 3);
		// glBindVertexArray(0); // no need to unbind it every time 

		/*
			双缓冲层(单个缓冲区进行绘制时，生成的图像可能会出现闪烁问题,
			因为输出图像并非瞬间绘制完成，而是逐像素绘制，通常是从左到右、从上到下)
			所有渲染命令都绘制到后缓冲区。一旦所有渲染命令完成，我们就将后缓冲区的内容切换到前缓冲区
		*/
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	ourShader.deleteProgram();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes 注册回调
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	/*
	* make sure the viewport matches the new window dimensions; note that width and
	* height will be significantly larger than specified on retina displays.
	渲染窗口的大小(Viewport的大小),可以将视口尺寸Viewport设置为小于 GLFW 尺寸的值 
	NDC 坐标将通过视口变换viewport transform, 通过 glViewport提供的数据(width, height)转换为屏幕空间坐标。
	生成的屏幕空间坐标随后将转换为片段
	*/
	glViewport(0, 0, width, height);
}