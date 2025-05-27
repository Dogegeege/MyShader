#ifndef MODEL_H
#define MODEL_H

#include <map>

#include "mesh.h"
#include "object.h"

std::map<std::string, std::shared_ptr<Object3D>> Object3D::loadedObject3D;

class Model : public Object3D {
   public:
    Model(const std::string& path);

    void Draw(Shader& shader) override;

    std::string directory;

   private:
    void                                    LoadModel(const std::string& path);
    void                                    ProcessNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh>                   ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    /*  模型数据  */
    std::vector<std::shared_ptr<Mesh>>      meshes;
    std::vector<std::shared_ptr<Texture2D>> textures_loaded;  // 存储所有已知纹理，保证只加载一次
    bool                                    gammaCorrection;
};

#endif