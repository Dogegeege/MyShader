#ifndef VERTEXSHADERLOADER_H
#define VERTEXSHADERLOADER_H

#include <vector>

#include "model.h"
/**
 * @param VAO  顶点数组对象(Vertex Array Object, VAO)存储顶点的结构定义，用于解析VBO中的属性。
 * @param VBO  顶点缓冲对象(Vertex Buffer Objects, VBO)调用显存并存储所有顶点数据供显卡使用的缓冲对象。
 * @param EBO  索引缓冲对象
 */
class VertexShaderLoader {
   public:
    /**
     * @param vertices 顶点类容器
     * @param indicies 索引类容器
     * @param prop 顶点属性类容器，用于VAO配置，区分不同的顶点属性
     */
    VertexShaderLoader(const std::vector<float>&        vertices,
                       const std::vector<unsigned int>& indicies,
                       const std::vector<unsigned int>& prop,
                       unsigned int                     GL_DRAW_TYPE = GL_STATIC_DRAW) {
        // 首先绑定顶点数组对象，然后绑定和设置顶点缓冲区，然后配置顶点属性。

        // 参数(生成对象个数,返回的ID)
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // 绑定VAO
        glBindVertexArray(VAO);

        // 绑定VBO
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DRAW_TYPE);

        // 绑定EBO
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(int), indicies.data(), GL_DRAW_TYPE);

        auto sumOffset = 0;
        auto sumProp   = 0;

        for (auto i : prop) { sumProp += i; }
        for (auto i = 0; i < prop.size(); ++i) {
            setProperty(i, prop[i], sumProp, sumOffset);
            sumOffset += prop[i];
        }

#ifdef DEBUG
        // 打印读取的数据

        float verticesData[216] = {};

        unsigned int indicesData[36] = {};

        // 从缓冲区中读取数据
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);

        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indicesData), indicesData);

#endif
    }

    VertexShaderLoader(const Model& m, unsigned int GL_DRAW_TYPE = GL_STATIC_DRAW)
        : VertexShaderLoader(m.vertices, m.indices, m.prop, GL_DRAW_TYPE) {}  // 注意在CPP-11中使用委托构造函数来确保调用另一个构造函数

    // 设置顶点属性
    inline void setProperty(const unsigned int pID, const unsigned int pSize, const int vSize, const int pOffset = 0) {
        glVertexAttribPointer(pID, pSize, GL_FLOAT, GL_FALSE, vSize * sizeof(float), (void*)(pOffset * sizeof(float)));
        glEnableVertexAttribArray(pID);  // 启用顶点属性
    }

    // 绑定VAO.接口供外部使用
    inline void bindVertexArray() { glBindVertexArray(VAO); }

    // 解除绑定
    static inline void deleteBuffer() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~VertexShaderLoader() { deleteBuffer(); };

   private:
    unsigned int VAO;  // 顶点数组对象(Vertex Array Object, VAO)存储顶点的结构定义，用于解析VBO中的属性。
    unsigned int VBO;  // 顶点缓冲对象(Vertex Buffer Objects, VBO)调用显存并存储所有顶点数据供显卡使用的缓冲对象。
    unsigned int EBO;  // 索引缓冲对象
};

#endif