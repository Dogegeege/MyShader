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

    if (width / static_cast<float>(height) < aspectRatio) {
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
    if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        if (action == GLFW_PRESS)
            camera->middleMouseButtonPressed = true;
        else
            camera->middleMouseButtonPressed = false;
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto camera = WindowRender::windowCameraMap[window];
    if (camera->middleMouseButtonPressed == true) camera->ProcessMouseScroll(static_cast<float>(yoffset));
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

inline void WindowRender::GetScreenSize(int& width, int& height) {
    GLFWmonitor*       primaryMonitor = glfwGetPrimaryMonitor();  // 获取主显示器
    const GLFWvidmode* mode           = glfwGetVideoMode(primaryMonitor);
    width                             = mode->width;
    height                            = mode->height;
}

GLFWwindow* WindowRender::InitWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MY_GLFW_CONTEXT_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MY_GLFW_CONTEXT_VERSION_MINOR);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);  // 启用透明窗口（需系统支持）
    // glfwWindowHint(GLFW_DECORATED, GL_FALSE);  // 无边框
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

    //! 窗口大小改为Imgui内调整
    // glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  // 调整窗口

    glfwSetScrollCallback(window, scroll_callback);             // 鼠标滚轮参数
    glfwSetMouseButtonCallback(window, mouse_button_callback);  // 注册鼠标按键回调
    glfwSetCursorPosCallback(window, mouse_callback);           // 注册鼠标移动回调

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

//-----------------------------------------------------------------------------------

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height) {
    Invaidate();
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_FrameBufferID);
}

void FrameBuffer::Invaidate() {
#if MY_GLFW_CONTEXT_VERSION_MAJOR >= 4 && MY_GLFW_CONTEXT_VERSION_MINOR >= 5

    // 创建缓冲区
    // 帧缓冲对象FBO framebuffer
    // FBO 是一个容器对象，本身不存储数据，而是通过附加其他缓冲区（如纹理或 RBO）实现数据存储
    glCreateFramebuffers(1, &m_FrameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
    // 生成三颜色通道纹理
    glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 生成对应的颜色缓冲区
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
    // 生成深度纹理
    glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Width, m_Height);
    // 生成深度缓冲区
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

#else
    // 生成帧缓冲对象
    glGenFramebuffers(1, &m_FrameBufferID);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);

    glGenTextures(1, &m_ColorAttachment);
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

    glGenTextures(1, &m_DepthAttachment);
    glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Width, m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

#endif
    // 完整性检查（带详细错误报告）
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "[FBO Error] 0x" << std::hex << status << std::dec << " - ";
        switch (status) {
            case GL_FRAMEBUFFER_UNDEFINED: std::cerr << "Default framebuffer does not exist"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: std::cerr << "Attachment incomplete"; break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: std::cerr << "No valid attachments"; break;
            case GL_FRAMEBUFFER_UNSUPPORTED: std::cerr << "Unsupported format combination"; break;
            default: std::cerr << "Unknown error";
        }
        std::cerr << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// 窗口大小改变，重新设置缓冲区
void FrameBuffer::Resize(uint32_t width, uint32_t height) {
    m_Width  = width;
    m_Height = height;

    Invaidate();
}

// 绑定帧缓冲区
void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
}

// 解绑帧缓冲区
void FrameBuffer::UBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}