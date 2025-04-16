#include "model.h"

/**
 * @brief 对每一个 `mesh` 进行绘制
 */
void Model::Draw(Shader& shader) {
    for (unsigned int i = 0; i < meshes.size(); i++) meshes[i].Draw(shader);
}

void Model::loadModel(const std::string& path) {
    Assimp::Importer import;

    /**
     *@brief 这个函数需要一个文件路径，它的第二个参数是一些后期处理(Post-processing)的选项。
    除了加载文件之外，Assimp允许我们设定一些选项来强制它对导入的数据做一些额外的计算或操作。
    通过设定 @param aiProcess_Triangulate 我们告诉Assimp，如果模型不是（全部）由三角形组成，
    它需要将模型所有的图元形状变换为三角形。
    @param aiProcess_FlipUVs 将在处理的时候翻转y轴的纹理坐标（你可能还记得我们在纹理教程中说过，
    在OpenGL中大部分的图像的y轴都是反的，所以这个后期处理选项将会修复这个）。
    其它一些比较有用的选项有：

    @param aiProcess_GenNormals ：如果模型不包含法向量的话，就为每个顶点创建法线。
    @param aiProcess_SplitLargeMeshes ：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
    @param aiProcess_OptimizeMeshes ：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。

    Assimp提供了很多有用的后期处理指令，你可以在`这里`找到全部的指令。实际上使用Assimp加载模型是非常容易的（你也可以看到）。
    困难的是之后使用返回的场景对象将加载的数据转换到一个Mesh对象的数组。
    */
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || scene->mRootNode == nullptr) {
        std::cerr << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

/**
 * @brief 每个节点包含了一系列的网格索引，每个索引指向场景对象中的那个特定网格，处理每个网格，接着对每个节点的子节点重复这一过程。
 * ?此处忽略了父子网格，直接搜索遍历出所有的网格，该方法并不保证可以对子网格操作，比如想位移一个汽车的网格时，
 * ?不能保证它的所有子网格（比如引擎网格、方向盘网格、轮胎网格）都会随着一起位移。这样的系统能够用父子关系就很容易地创建出来。
 */
void Model::processNode(aiNode* node, const aiScene* scene) {
    // 处理节点所有的网格（如果有的话）
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    // 接下来对它的子节点重复这一过程
    for (unsigned int i = 0; i < node->mNumChildren; i++) { processNode(node->mChildren[i], scene); }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex>  vertices;
    std::vector<GLuint>  indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;

        // 处理顶点位置、法线和纹理坐标
        //! 注意 这里默认有纹理，没有纹理会导致崩溃

        glm::vec3 vector;
        vector.x        = mesh->mVertices[i].x;
        vector.y        = mesh->mVertices[i].y;
        vector.z        = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals()) {
            vector.x      = mesh->mNormals[i].x;
            vector.y      = mesh->mNormals[i].y;
            vector.z      = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        //! Assimp允许一个模型在一个顶点上有最多8个不同的纹理坐标，我们不会用到那么多，我们只关心第一组纹理坐标。
        //! 我们同样也想检查网格是否真的包含了纹理坐标（可能并不会一直如此）
        if (mesh->mTextureCoords[0] != nullptr) {
            glm::vec2 vec;

            vec.x            = mesh->mTextureCoords[0][i].x;
            vec.y            = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        } else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        if (mesh->HasTangentsAndBitangents()) {
            // tangent
            vector.x       = mesh->mTangents[i].x;
            vector.y       = mesh->mTangents[i].y;
            vector.z       = mesh->mTangents[i].z;
            vertex.Tangent = vector;

            // bitangent
            vector.x         = mesh->mBitangents[i].x;
            vector.y         = mesh->mBitangents[i].y;
            vector.z         = mesh->mBitangents[i].z;
            vertex.Bitangent = vector;
        } else {
            vertex.Tangent   = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.Bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // 处理索引
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) indices.push_back(face.mIndices[j]);
    }

    // 加载纹理(材质)贴图
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    // std::cout << "mesh->mMaterialIndex  " << mesh->mMaterialIndex << " scene->mMaterials[mesh->mMaterialIndex] "
    //           << scene->mMaterials[mesh->mMaterialIndex] << std::endl;

    // 漫反射材质纹理
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 镜面反射材质纹理
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 法线纹理
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 高度纹理
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;  // 纹理路径
        mat->GetTexture(type, i, &str);

        bool skip = false;

        //! 优化建议,开 set
        for (unsigned int j = 0; j < textures_loaded.size(); j++) {
            // 如果在已有材质中找到存在的材质，则退出
            if (std::strcmp(textures_loaded[j].path.data, str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (skip == false) {  // 如果纹理还没有被加载，则加载它
            Texture texture;

            texture.id   = TextureFromFile(str.C_Str(), directory, false);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // 添加到已加载的纹理中
        }
    }
    return textures;
}

GLuint TextureFromFile(const char* path, const std::string& directory, bool gamma) {
    std::string filename = std::string(path);
    filename             = directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    // stbi_set_flip_vertically_on_load(true);

    // std::cout << "textureID :" << textureID << " Path  ->  " << filename << std::endl;

    int            width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data != nullptr) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}