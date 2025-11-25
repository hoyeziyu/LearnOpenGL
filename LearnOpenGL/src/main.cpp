#include <glad/glad.h>      // 请务必在引入 GLFW 之前引入 GLAD。GLAD 的头文件包含了后台所需的 OpenGL 头文件
#include <GLFW/glfw3.h>
#include <iostream>


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

    // glfw window creation
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

        /*
            双缓冲层(单个缓冲区进行绘制时，生成的图像可能会出现闪烁问题,
            因为输出图像并非瞬间绘制完成，而是逐像素绘制，通常是从左到右、从上到下)
            所有渲染命令都绘制到后缓冲区。一旦所有渲染命令完成，我们就将后缓冲区的内容切换到前缓冲区
        */
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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
    */
    glViewport(0, 0, width, height);
}