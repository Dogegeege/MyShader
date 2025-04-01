#include "trans.h"

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

glm::mat4 Mytransform() {
    // 译注：下面就是矩阵初始化的一个例子，如果使用的是0.9.9及以上版本
    // 下面这行代码就需要改为:
    // glm::mat4 trans = glm::mat4(1.0f)
    // 之后将不再进行提示

    // MVP变换
    glm::mat4 model      = glm::mat4(1.0f);  // make sure to initialize matrix to identity matrix first
    glm::mat4 view       = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model                = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view                 = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection           = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    return projection * model * view;
}