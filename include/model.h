#ifndef MODEL_H
#define MODEL_H

#include <map>

#include "mesh.h"

class Model {
   public:
    Model(const std::string& path);

    void ModelDraw(Shader& shader);

   private:
    void                                    loadModel(const std::string& path);
    void                                    processNode(aiNode* node, const aiScene* scene);
    std::shared_ptr<Mesh>                   processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<std::shared_ptr<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    /*  模型数据  */
    std::vector<std::shared_ptr<Mesh>>      meshes;
    std::string                             directory;
    std::vector<std::shared_ptr<Texture2D>> textures_loaded;  // 存储所有已知纹理，保证只加载一次
    bool                                    gammaCorrection;
};

static std::map<std::string, std::shared_ptr<Model>> LoadedModel;

#endif