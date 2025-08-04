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
    Input::ProcessInputMouseMovement(window, camera, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    Input::ProcessInputMouseButton(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto camera = WindowRender::windowCameraMap[window];
    Input::ProcessInputMouseWheel(window, camera, yoffset);
}

WindowRender::WindowRender(Camera& camera, const std::string& name, int wihdth, int height)
    : windowName(name), camera(&camera), screenWidth(wihdth), screenHeight(height) {
    window = InitWindow();
    windowCameraMap.insert({window, &camera});
}

void WindowRender::GetScreenSize(int& width, int& height) {
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

void WindowRender::TerminateWindow() {
    windowCameraMap.erase(window);
    glfwDestroyWindow(window);
    glfwTerminate();
}

//-----------------------------------------------------------------------------------

FrameBuffer::FrameBuffer(const uint32_t width, const uint32_t height) : m_Width(width), m_Height(height) {
#if MY_GLFW_CONTEXT_VERSION_MAJOR >= 4 && MY_GLFW_CONTEXT_VERSION_MINOR >= 5
    //! 未修改
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

    glGenFramebuffers(1, &m_FrameBufferID);     // 生成帧缓冲id
    glGenTextures(1, &m_ColorAttachment);       // 生成颜色缓冲纹理
    glGenTextures(1, &p_FBO);                   // 拾取ID缓冲纹理
    glGenRenderbuffers(1, &m_DepthAttachment);  // 生成深度渲染缓冲

    // 设置m_renderTexture纹理参数
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, p_FBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, width, height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    this->Bind();
    // 设置主渲染颜色附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);
    // 设置拾取ID颜色附件
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, p_FBO, 0);

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

    this->UnBind();
    Resize(width, height);  // 窗口大小改变，重新设置缓冲区
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &m_FrameBufferID);
}

// 窗口大小改变，重新设置缓冲区
void FrameBuffer::Resize(const uint32_t width, const uint32_t height) {
    m_Width  = width;
    m_Height = height;

    // 设置纹理的大小
    glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, p_FBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32UI, m_Width, m_Height, 0, GL_RGB_INTEGER, GL_UNSIGNED_INT, nullptr);
    glBindTexture(GL_TEXTURE_2D, 0);

    /* Setup depth-stencil buffer (24 + 8 bits) */
    glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachment);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, m_Width, m_Height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    Bind();
    // 附加深度缓冲与模板缓冲
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachment);
    UnBind();
}

// 绑定帧缓冲区
void FrameBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferID);
}

// 解绑帧缓冲区
void FrameBuffer::UnBind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::vec3 FrameBuffer::ReadPixel(const unsigned int x, const unsigned int y) {
    this->Bind();
    glReadBuffer(GL_COLOR_ATTACHMENT1);  // 0为颜色附件，1为拾取ID附件
    glm::uvec3 pixel = {0, 0, 0};
    glReadPixels(x, y, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_INT, &pixel);
    glReadBuffer(GL_NONE);
    this->UnBind();

    // std::cout << x << " " << y << " " << pixel.x << " " << pixel.y << " " << pixel.z << std::endl;

    return pixel;
}