#include "windowrender.h"

std::map<GLFWwindow*, Camera*> WindowRender::windowCameraMap;

//-----------------------------------------------------------------------------------

// 调试回调函数
void APIENTRY MessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = " << type << ", severity = " << severity
              << ", message = " << message << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    auto camera = WindowRender::windowCameraMap[window];

    // 固定宽高比
    float aspectRatio = camera->aspectRatio;  // 假设宽高比为 16:9
    int   viewportWidth, viewportHeight;

    if (width / (float)height < aspectRatio) {
        viewportWidth  = static_cast<int>(height * aspectRatio);
        viewportHeight = height;
    } else {
        viewportWidth  = width;
        viewportHeight = static_cast<int>(width / aspectRatio);
    }

    int viewportX = (width - viewportWidth) / 2;
    int viewportY = (height - viewportHeight) / 2;

    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    auto camera = WindowRender::windowCameraMap[window];
    if (camera->rightMouseButtonPressed == true) camera->ProcessMouseMovement(xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto camera = WindowRender::windowCameraMap[window];

    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        if (action == GLFW_PRESS) {
            camera->rightMouseButtonPressed = true;  // 鼠标左键按下
        } else if (action == GLFW_RELEASE) {
            camera->rightMouseButtonPressed = false;  // 鼠标左键释放
            camera->firstMouse              = true;   // 重置鼠标状态
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto camera = WindowRender::windowCameraMap[window];
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void WindowRender::processInput(float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);  // 按ESC退出

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera->ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera->ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) camera->ProcessKeyboard(DOWN, deltaTime);
}

WindowRender::WindowRender(Camera& camera, std::string name) : windowName(name), camera(&camera), screenHeight(SCR_HEIGHT), screenWidth(SCR_WIDTH) {
    window = InitWindow();
    windowCameraMap.insert({window, &camera});
}

GLFWwindow* WindowRender::InitWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MY_GLFW_CONTEXT_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MY_GLFW_CONTEXT_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, windowName.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cerr << "Error: Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);  // 创建窗口

    //?是否开启垂直同步?
    //   glfwSwapInterval(1); // Enable vsync

    // 启用GLAD加载OpenGL函数,在glfwMakeContextCurrent()之后确保创建上下文
    if (gladLoadGLLoader((GLADloadproc)glfwGetProcAddress) == false) {
        std::cerr << "Error: Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    //----------------------------注册回调参数---------------------------------------------

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 调整窗口
    glfwSetScrollCallback(window, scroll_callback);                     // 鼠标滚轮参数
    glfwSetMouseButtonCallback(window, mouse_button_callback);          // 注册鼠标按键回调
    glfwSetCursorPosCallback(window, mouse_callback);                   // 注册鼠标移动回调

    // 启用鼠标监听
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // 隐藏鼠标

#if MY_GLFW_CONTEXT_VERSION_MAJOR >= 4 && MY_GLFW_CONTEXT_VERSION_MINOR >= 3
    // 启用调试输出
    std::cerr << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

    glDebugMessageCallback(MessageCallback, this);
#endif

    return window;
}
