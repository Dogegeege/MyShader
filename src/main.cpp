#include <math.h>
#include <iostream>
#include <memory>

#include <glad/glad.h>
//
#include <GLFW/glfw3.h>
#include <stb_image/stb_image.h>

#include "camera.h"
#include "shader.h"
#include "trans.h"

#define MY_GLFW_CONTEXT_VERSION_MAJOR 4
#define MY_GLFW_CONTEXT_VERSION_MINOR 3
// 调试回调函数
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = " << type << ", severity = " << severity
              << ", message = " << message << std::endl;
}

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool   firstMouse = true;

float lastX = SCR_WIDTH / 2.0;
float lastY = SCR_HEIGHT / 2.0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse == true) {
        lastX      = xpos;
        lastY      = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX         = xpos;
    lastY         = ypos;

    float sensitivity = 0.1;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void processInput(GLFWwindow* window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);  // 按ESC退出

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MY_GLFW_CONTEXT_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MY_GLFW_CONTEXT_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);  // 创建窗口

    // 启用GLAD加载OpenGL函数,在glfwMakeContextCurrent()之后确保创建上下文
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // 注册回调参数
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 调整窗口
    glfwSetCursorPosCallback(window, mouse_callback);                   // 鼠标移动参数
    glfwSetScrollCallback(window, scroll_callback);                     // 鼠标滚轮参数

    // 启用鼠标监听
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

#ifdef DEBUG
    #if MY_GLFW_CONTEXT_VERSION_MAJOR >= 4 && MY_GLFW_CONTEXT_VERSION_MINOR >= 3
    // 启用调试输出
    std::cerr << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(MessageCallback, nullptr);
    #endif
#endif

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<Shader> myShader       = std::make_shared<Shader>("../src/shader/cube.vs", "../src/shader/cube.fs");
    std::shared_ptr<Shader> lightingShader = std::make_shared<Shader>("../src/shader/light.vs", "../src/shader/light.fs");

    // 立方体的八个顶点(实际每个面都有2个独立的三角形)
    float vertices[] = {-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
                        0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

                        -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
                        0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

                        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
                        0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                        -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
                        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

    unsigned int indices[36] = {
        // 注意索引从0开始!
        // 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
        // 这样可以由下标代表顶点组合成矩形

        0, 1, 2,  // 第一个三角形
        3, 4, 5,  // 第二个三角形
        //....

    };
    for (int i = 0; i < 36; i++) { indices[i] = i; }

    const glm::vec3 cubePositions[] = {glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
                                       glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
                                       glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
                                       glm::vec3(-1.3f, 1.0f, -1.5f)};

    const glm::vec3 lightPositions[] = {
        glm::vec3(5.0f, -2.0f, -5.0f),
    };
    // 共享VBO
    unsigned int VBO;  // 顶点缓冲对象(Vertex Buffer Objects, VBO)调用显存并存储所有顶点数据供显卡使用的缓冲对象。
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 写入缓冲
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

#ifdef DEBUG
    // 打印读取的数据

    float verticesData[36] = {};

    // 从缓冲区中读取数据
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);

    // for (int i = 0; i < sizeof(verticesData) / sizeof(float); i++) { std::cerr << verticesData[i] << std::endl; }

    unsigned int indicesData[36] = {};
    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indicesData), indicesData);
    // for (int i = 0; i < sizeof(indicesData) / sizeof(unsigned int); i++) { std::cerr << indicesData[i] << std::endl; }

#endif

    ////////////////////////////////////////////////
    // 立方体
    unsigned int Cube_VAO;  // 顶点数组对象(Vertex Array Object, VAO)存储顶点的结构定义，用于解析VBO中的属性。
    unsigned int Cube_EBO;  // 索引缓冲对象

    // 首先绑定顶点数组对象，然后绑定和设置顶点缓冲区，然后配置顶点属性。
    // 参数(生成对象个数,返回的ID)

    glGenVertexArrays(1, &Cube_VAO);
    glGenBuffers(1, &Cube_EBO);

    glBindVertexArray(Cube_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Cube_EBO);
    // // 解释缓冲流中的顶点数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);                    // 位置
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // 纹理
    // 启用顶点属性
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //////////////////////////////////////////////////////////
    // 光源  //////////////////////////////////////////////////////

    unsigned int Light_VAO;
    unsigned int Light_EBO;  // 索引缓冲对象

    glGenVertexArrays(1, &Light_VAO);
    glGenBuffers(1, &Light_EBO);

    glBindVertexArray(Light_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Light_EBO);

    // 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);                    // 位置
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));  // 纹理

    glEnableVertexAttribArray(0);
    // glEnableVertexAttribArray(1);
    //  不启用纹理

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 解除绑定
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /////////////////////////////////////////////////////////////////
    // 纹理

    int            width, height, nrChannels;
    unsigned char* data = stbi_load("../texure/container.jpg", &width, &height, &nrChannels, 0);
    unsigned int   textures[2];

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  // 纹理超出边界环绕模式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);                         // 纹理被缩小的时候使用邻近过滤（像素化）
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);  // 纹理被缩小的时候使用MipMap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);                // 被放大时使用线性过滤（模糊）

    if (data != NULL) {
        glGenTextures(1, &textures[0]);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture:\n" << std::endl;
    }
    stbi_image_free(data);

    stbi_set_flip_vertically_on_load(true);  // 载入前翻转这张图片
    data = stbi_load("../texure/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data != NULL) {
        glGenTextures(1, &textures[1]);
        glBindTexture(GL_TEXTURE_2D, textures[1]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture:\n" << std::endl;
    }

    stbi_image_free(data);

    lightingShader->use();

    lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);

    myShader->use();                                                 // 不要忘记在设置uniform变量之前激活着色器程序！
    glUniform1i(glGetUniformLocation(myShader->ID, "texture1"), 0);  // 手动设置
    myShader->setInt("texture2", 1);                                 // 或者使用着色器类设置

    glEnable(GL_DEPTH_TEST);  // 启用深度测试

    // timing
    float deltaTime = 0.0f;  // time between current frame and last frame
    float lastFrame = 0.0f;

    while (glfwWindowShouldClose(window) == false) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime          = currentFrame - lastFrame;
        lastFrame          = currentFrame;

        processInput(window, deltaTime);  // IO响应

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader->use();  // 使用着色器程序

        glm::mat4 model      = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 view       = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glBindVertexArray(Cube_VAO);

        for (int i = 0; i < 10; i++) {
            float timeValue = glfwGetTime();
            float mixValue  = 1.f * static_cast<float>(sin(timeValue) / 2.0f);

            // 混合值
            int mixValueLocation = glGetUniformLocation(myShader->ID, "mixValue");
            if (mixValueLocation == -1) { std::cerr << "Uniform 'mixValue' not found in shader program!" << std::endl; }
            glUniform1f(mixValueLocation, mixValue);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, textures[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, textures[1]);

            myShader->setMat4("model", glm::translate(model, cubePositions[i]));
            myShader->setMat4("view", view);
            myShader->setMat4("projection", projection);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);  // 绘制的模式(三角形),(绘制顶点的个数)
        }
        glBindVertexArray(0);

        // 构造光源
        lightingShader->use();

        lightingShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);  // 物体颜色
        lightingShader->setVec3("lightColor", 1.0f, 1.0f, 1.0f);    // 发光颜色

        model      = glm::mat4(1.0f);
        view       = camera.GetViewMatrix();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        lightingShader->setMat4("model", glm::translate(model, lightPositions[0]));  // 随便给个位置
        lightingShader->setMat4("view", view);
        lightingShader->setMat4("projection", projection);

        glBindVertexArray(Light_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 交换缓冲
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}