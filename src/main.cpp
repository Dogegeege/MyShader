#include <math.h>
#include <filesystem>
#include <iostream>
#include <memory>
#include <vector>

#include "UIrender.h"
#include "_camera.h"
#include "attribute.h"
#include "model.h"
#include "object.h"
#include "shader.h"
#include "shape.h"
#include "skybox.h"
#include "texture.h"
#include "windowrender.h"

int main() {
    //!--------------------------加载资源----------------------------------
    Camera       camera(glm::vec3(0.0f, 0.0f, 3.0f));
    WindowRender windowRender(camera, "LearnOpenGL", SCR_WIDTH, SCR_HEIGHT);
    FrameBuffer* pFrameBuffer = new FrameBuffer(640, 480);
    UIRender     ui(windowRender, camera, pFrameBuffer);
    Grid         grid;

    //!--------------------------着色器----------------------------------

    Shader modelShader("../../assets/shader/model.vsh", "../../assets/shader/model.fsh");
    Shader highLightContour("../../assets/shader/highlightcontour.vsh", "../../assets/shader/highlightcontour.fsh");
    Shader pickShader("../../assets/shader/pick.vsh", "../../assets/shader/pick.fsh");
    Shader skyboxShader("../../assets/shader/skybox.vsh", "../../assets/shader/skybox.fsh");
    Shader gridShader("../../assets/shader/grid.vsh", "../../assets/shader/grid.fsh");
    Shader waterShader("../../assets/shader/water.vsh", "../../assets/shader/water.fsh");

    // 全局Uniform块
    //! 需要封装进Shader.h
    unsigned int uniformBlockIndexModel            = glGetUniformBlockIndex(modelShader.ID, "Matrices");
    unsigned int uniformBlockIndexHighLightContour = glGetUniformBlockIndex(highLightContour.ID, "Matrices");
    unsigned int uniformBlockIndexPick             = glGetUniformBlockIndex(pickShader.ID, "Matrices");
    // unsigned int uniformBlockIndexSkybox           = glGetUniformBlockIndex(skyboxShader.ID, "Matrices");
    unsigned int uniformBlockIndexGrid             = glGetUniformBlockIndex(gridShader.ID, "Matrices");
    unsigned int uniformBlockIndexWater            = glGetUniformBlockIndex(waterShader.ID, "Matrices");

    glUniformBlockBinding(modelShader.ID, uniformBlockIndexModel, 0);
    glUniformBlockBinding(highLightContour.ID, uniformBlockIndexHighLightContour, 0);
    glUniformBlockBinding(pickShader.ID, uniformBlockIndexPick, 0);
    glUniformBlockBinding(gridShader.ID, uniformBlockIndexGrid, 0);
    glUniformBlockBinding(waterShader.ID, uniformBlockIndexWater, 0);

    unsigned int uboMatrices;

    glGenBuffers(1, &uboMatrices);

    glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrices, 0, 2 * sizeof(glm::mat4));

    //!--------------------------------------------------------------------
    auto ourModel = std::make_shared<Model>(1, "../../assets/model/vtuber-neuro-sama-v3/textures/Neuro-v3model-Releaseready4.2.obj");
    auto bunny    = std::make_shared<Model>(2, "../../assets/model/bunny/bunny.obj");
    std::shared_ptr<Object3D>              rectangle = std::make_shared<Rectangle>(3, "Rectangle");
    std::vector<std::shared_ptr<Object3D>> objects;
    objects.push_back(ourModel);
    objects.push_back(bunny);
    // objects.push_back(rectangle);
    Object3D::loadedObject3D.insert({ourModel->GetID(), ourModel});
    Object3D::loadedObject3D.insert({bunny->GetID(), bunny});
    Object3D::loadedObject3D.insert({rectangle->GetID(), rectangle});

    //  创建立方体贴图
    std::vector<std::string> skyboxFaces = {"../../assets/img/skybox/right.jpg", "../../assets/img/skybox/left.jpg",
                                            "../../assets/img/skybox/top.jpg",   "../../assets/img/skybox/bottom.jpg",
                                            "../../assets/img/skybox/front.jpg", "../../assets/img/skybox/back.jpg"};

    Skybox* skybox = new Skybox(skyboxFaces, "Skybox");

    //!-------------------------------------------------------------------

    glEnable(GL_DEPTH_TEST);  // 启用深度测试
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);  // 启用模板测试
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);

    glCullFace(GL_BACK);     // 剔除背面
    glEnable(GL_CULL_FACE);  // 启用面剔除

    while (glfwWindowShouldClose(windowRender.getWindow()) == false) {
        glm::mat4& view       = *ui.view;
        glm::mat4& projection = *ui.projection;

        InputInfo::GetInstance()->TimeUpdate(static_cast<float>(glfwGetTime()));                               // 更新输入时间
        Input::ProcessInputKeyBorard(windowRender.getWindow(), &camera, InputInfo::GetInstance()->deltaTime);  // IO响应

        //!--------------------------Shader--------------------------------

        skyboxShader.use();
        glm::mat4 unTransView = glm::mat4(glm::mat3(camera.GetViewMatrix()));  // remove translation from the view matrix
        skyboxShader.setMat4("view", unTransView);
        skyboxShader.setMat4("projection", projection);

        waterShader.use();
        waterShader.setFloat("iTime", glfwGetTime());
        waterShader.setVec3("iResolution", glm::vec3(pFrameBuffer->GetWidth(), pFrameBuffer->GetHeight(), 0.0f));
        waterShader.setVec4("iMouse", glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
        waterShader.setVec3("iOrig", camera.GetPosition());

        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
        // !---------------------------拾取----------------------------------

        pFrameBuffer->Bind();

        // 渲染ID到拾取附件
        GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT1};
        glDrawBuffers(1, drawBuffers);

        const GLuint clearColor[3] = {0, 0, 0};
        glClearBufferuiv(GL_COLOR, 0, clearColor);  // 清除 drawBuffers 0号索引内容
        glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto object : objects) {
            if (object == nullptr) continue;
            pickShader.use();
            pickShader.setMat4("model", object->modelMatrix);
            pickShader.setUnsignedInt("objectID", object->GetID());

            object->Draw(pickShader);
        }

        pickShader.use();
        pickShader.setMat4("model", rectangle->modelMatrix);
        pickShader.setUnsignedInt("objectID", rectangle->GetID());
        rectangle->Draw(pickShader);

        pFrameBuffer->UnBind();
        //!--------------------------ObjectShade--------------------------------
        pFrameBuffer->Bind();

        GLenum defaultBuffers[1] = {GL_COLOR_ATTACHMENT0};
        glDrawBuffers(1, defaultBuffers);

        glClearColor(63.0f / 255.0f, 63.0f / 255.0f, 63.0f / 255.0f, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        for (auto object : objects) {
            if (object == nullptr) continue;
            modelShader.use();
            modelShader.setMat4("model", object->modelMatrix);
            modelShader.setBool("isZBufferPreview", ui.isZBufferPreview);

            highLightContour.use();
            highLightContour.setMat4("model", object->modelMatrix);

            object->SetHighLight(ui.showHightLight && ui.selectedID == object->GetID());
            object->Draw(modelShader, highLightContour);
        }

        waterShader.use();
        waterShader.setMat4("model", rectangle->modelMatrix);
        highLightContour.use();
        highLightContour.setMat4("model", rectangle->modelMatrix);

        rectangle->SetHighLight(ui.showHightLight && ui.selectedID == rectangle->GetID());
        rectangle->Draw(waterShader, highLightContour);

        pFrameBuffer->UnBind();
        //!-------------------------------SkyBox-----------------------------------------
        pFrameBuffer->Bind();

        if (ui.isSkyboxPreview == true) {  // draw skybox as last
            glDepthFunc(GL_LEQUAL);        // change depth function so depth test passes when values are equal to depth buffer's content
            skybox->Draw(skyboxShader);
            glDepthFunc(GL_LESS);  // set depth function back to default
        }
        pFrameBuffer->UnBind();

        //!-----------------------------------axis---------------------------------
        pFrameBuffer->Bind();

        grid.Draw(gridShader);

#ifdef CUBESHADE

        //!--------------------------Cube--------------------------------

        // cubeShader->use();  // 使用着色器程序

        // // 摄像机属性
        // cubeShader->setMat4("model", model);
        // cubeShader->setMat4("view", view);
        // cubeShader->setMat4("projection", projection);

        // // 设置材质属性
        // cubeShader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
        // cubeShader->setFloat("material.shininess", 32.0f);

        // // 设置光源（对物体）属性
        // cubeShader->setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        // cubeShader->setVec3("light.diffuse", appinfo.lightColor);  // 将光照调暗了一些以搭配场景
        // cubeShader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
        // cubeShader->setVec3("light.position", lightPositions);           // 光源位置
        // cubeShader->setFloat("light.intensity", appinfo.lightIntesity);  // 光源强度

        // cubeShader->setVec3("viewPos", camera.Position);  // 摄像机位置

        // diffuseMap.acitVeTexure();
        // specularMap.acitVeTexure();

        // for (int i = 0; i < 10; i++) {
        //     // glm::vec3 lightColor;
        //     // lightColor.x = sin(glfwGetTime() * 2.0f);
        //     // lightColor.y = sin(glfwGetTime() * 0.7f);
        //     // lightColor.z = sin(glfwGetTime() * 1.3f);

        //     // glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);    // 降低影响
        //     // glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);  // 很低的影响

        //     // cubeShader->setVec3("light.ambient", ambientColor);
        //     // cubeShader->setVec3("light.diffuse", diffuseColor);

        //     cubeShader->setMat4("model", glm::translate(model, cubePositions[i]));

        //     cubeVertex.bindAndDrawElements();
        // }
        // cubeVertex.UnbindVertexArray();

        //!---------------------------Light--------------------------------

        // // 构造光源
        // lightingShader->use();

        // float radius = 2.0f;

        // lightPositions.y = cos(appinfo.lightPose * camera.MovementSpeed) * radius;
        // lightPositions.z = sin(appinfo.lightPose * camera.MovementSpeed) * radius;

        // model = glm::mat4(1.0f);
        // model = glm::translate(model, lightPositions);
        // model = glm::scale(model, glm::vec3(0.2f));  // a smaller cube

        // lightingShader->setMat4("model", glm::translate(model, lightPositions));  // 随便给个位置
        // lightingShader->setMat4("view", view);
        // lightingShader->setMat4("projection", projection);

        // lightingShader->setVec4("lightColor", appinfo.lightColor);

        // ligtingVertex.bindAndDrawElements();
#endif

        pFrameBuffer->UnBind();

        //!-------------------------------imgui-----------------------------------------
        ui.Render();
        //!---------------------------------------------------------------------
        glfwPollEvents();                           // 处理事件
        glfwSwapBuffers(windowRender.getWindow());  // 交换缓冲
    }
    // 显式写出
    ui.~UIRender();
    windowRender.~WindowRender();
    return 0;
}
