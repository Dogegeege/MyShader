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
                       const int                        fragSize     = 3,
                       unsigned int                     GL_DRAW_TYPE = GL_STATIC_DRAW)
        : fragSize(fragSize), verticesSize(vertices.size()), indicesSize(indicies.size()) {
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
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), indicies.data(), GL_DRAW_TYPE);

        auto sumOffset = 0;
        auto sumProp   = 0;
        auto pID       = 0;

        for (auto i : prop) { sumProp += i; }
        for (auto i = 0; i < prop.size(); ++i) {
            if (prop[i] == 0) continue;  // OpenGL禁止0属性顶点
            setProperty(pID++, prop[i], sumProp, sumOffset);
            sumOffset += prop[i];
        }

#ifdef DEBUG
        // 打印读取的数据

        float verticesData[216] = {};

        unsigned int indicesData[36] = {};

        // 从缓冲区中读取数据
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticesData), verticesData);

        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indicesData), indicesData);

        int x = 1;
#endif
    }

    VertexShaderLoader(const Model& m, unsigned int GL_DRAW_TYPE = GL_STATIC_DRAW)
        : VertexShaderLoader(m.vertices, m.indices, m.prop, m.fragSize, GL_DRAW_TYPE) {}  // 注意在CPP-11中使用委托构造函数来确保调用另一个构造函数

    // 设置顶点属性
    inline void setProperty(const unsigned int pID, const unsigned int pSize, const int vSize, const int pOffset = 0) {
        glVertexAttribPointer(pID, pSize, GL_FLOAT, GL_FALSE, vSize * sizeof(float), (void*)(pOffset * sizeof(float)));
        glEnableVertexAttribArray(pID);  // 启用顶点属性
    }

    // 绑定VAO.接口供外部使用
    inline void bindVertexArray() { glBindVertexArray(VAO); }
    inline void UnbindVertexArray() { glBindVertexArray(0); }

    inline void bindAndDrawElements() {
        bindVertexArray();

        if (fragSize == 1)
            glDrawElements(GL_POINTS, this->indicesSize, GL_UNSIGNED_INT, 0);
        else if (fragSize == 2)
            glDrawElements(GL_LINES, this->indicesSize, GL_UNSIGNED_INT, 0);
        else if (fragSize == 3)
            glDrawElements(GL_TRIANGLES, this->indicesSize, GL_UNSIGNED_INT, 0);
        else if (fragSize == 4)
            glDrawElements(GL_TRIANGLE_STRIP, this->indicesSize, GL_UNSIGNED_INT, 0);

        // else if (fragSize == 5)
        //     glDrawElements(GL_LINE_STRIP, this->indicesSize, GL_UNSIGNED_INT, 0);
        // else if (fragSize == 6)
        //     glDrawElements(GL_LINE_LOOP, this->indicesSize, GL_UNSIGNED_INT, 0);
    }

    // 解除绑定
    static inline void unBindBuffer() {
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~VertexShaderLoader() { unBindBuffer(); };

   private:
    unsigned int VAO;  // 顶点数组对象(Vertex Array Object, VAO)存储顶点的结构定义，用于解析VBO中的属性。
    unsigned int VBO;  // 顶点缓冲对象(Vertex Buffer Objects, VBO)调用显存并存储所有顶点数据供显卡使用的缓冲对象。
    unsigned int EBO;  // 索引缓冲对象

    int fragSize;      // 每个面的顶点数
    int verticesSize;  // 顶点数据大小
    int indicesSize;   // 索引数据大小
};

#endif