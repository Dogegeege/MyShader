#ifndef MODEL_H
#define MODEL_H

#include <map>

#include "mesh.h"

class Model {
   public:
    Model(const std::string& path);
    virtual ~Model() { loadedModel.erase(name); }

    void ModelDraw(Shader& shader);

    std::string directory;
    std::string name;

    static std::map<std::string, std::shared_ptr<Model>> loadedModel;

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