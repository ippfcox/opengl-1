#include "assimp_loader.hpp"
#include "../../engine/object/material/phong_material.hpp"
#include "../../engine/utils/utils.hpp"

Object *AssimpLoader::Load(const std::string path)
{
    Assimp::Importer importer;
    auto *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        SPDLOG_ERROR("Assimp::Importer ReadFile failed: {}", importer.GetErrorString());
        return nullptr;
    }

    Object *root = new Object();

    ProcessNode(scene->mRootNode, root, scene, path.substr(0, path.find_last_of('/')));

    return root;
}

void AssimpLoader::ProcessNode(aiNode *ai_node, Object *parent, const aiScene *ai_scene, const std::string &root_path)
{
    Object *node = new Object();
    parent->AddChild(node);

    glm::mat4 local_model_matrix = aiMat4x4Toglmmat4(ai_node->mTransformation);
    glm::vec3 position, eular_angle, scale;
    Utils::Decompose(local_model_matrix, position, eular_angle, scale);

    node->SetPosition(position);
    node->SetAngleX(eular_angle.x);
    node->SetAngleY(eular_angle.y);
    node->SetAngleZ(eular_angle.z);
    node->SetScale(scale);

    // so...node(Object) and mesh are all its child? [fixme] please!
    for (int i = 0; i < ai_node->mNumMeshes; ++i)
    {
        node->AddChild(ProcessMesh(ai_scene->mMeshes[ai_node->mMeshes[i]], ai_scene, root_path));
    }

    for (int i = 0; i < ai_node->mNumChildren; ++i)
    {
        ProcessNode(ai_node->mChildren[i], node, ai_scene, root_path);
    }
}

Mesh *AssimpLoader::ProcessMesh(aiMesh *ai_mesh, const aiScene *ai_scene, const std::string &root_path)
{
    std::vector<Vertex> vertices{};
    for (int i = 0; i < ai_mesh->mNumVertices; ++i)
    {
        // [todo] use emplace_back
        // mTextureCoords[0]: color texture uv
        vertices.push_back({
            glm::vec3{ai_mesh->mVertices[i].x, ai_mesh->mVertices[i].y, ai_mesh->mVertices[i].z},
            ai_mesh->mTextureCoords[0] ? glm::vec2{ai_mesh->mTextureCoords[0][i].x, ai_mesh->mTextureCoords[0][i].y} : glm::vec2{0.0f, 0.0f},
            glm::vec3{ai_mesh->mNormals[i].x, ai_mesh->mNormals[i].y, ai_mesh->mNormals[i].z},
        });
    }
    std::vector<unsigned int> indices{};
    for (int i = 0; i < ai_mesh->mNumFaces; ++i)
    {
        for (int j = 0; j < ai_mesh->mFaces[i].mNumIndices; ++j)
        {
            indices.push_back(ai_mesh->mFaces[i].mIndices[j]);
        }
    }

    auto geometry = new Geometry(vertices, indices);
    auto material = new PhongMaterial();
    material->shiness = 32.0f;

    if (ai_mesh->mMaterialIndex >= 0)
    {
        auto ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        aiString ai_rel_path;
        ai_material->GetTexture(aiTextureType_DIFFUSE, 0, &ai_rel_path);

        auto ai_texture = ai_scene->GetEmbeddedTexture(ai_rel_path.C_Str());
        if (ai_texture)
        {
            // compressed image, jpg/png...
            if (ai_texture->mHeight == 0)
            {
                material->diffuse = Texture::CreateByMemoryImage(ai_texture->pcData, ai_texture->mWidth, ai_texture->mFilename.C_Str());
            }
            else // raw rgb [todo] pixel format should be handled
            {
                material->diffuse = Texture::CreateByMemoryRGBA(ai_texture->pcData, ai_texture->mWidth, ai_texture->mHeight, ai_texture->mFilename.C_Str());
            }
        }
        else
        {
            material->diffuse = Texture::CreateByFilename(root_path + "/" + ai_rel_path.C_Str());
        }
    }
    else
    {
        material->diffuse = new Texture();
        material->diffuse->InitByFilename("assets/textures/cloud.jpg"); // [todo] use default texture
    }
    material->diffuse->SetUnit(0);

    return new Mesh(geometry, material);
}

glm::mat4 AssimpLoader::aiMat4x4Toglmmat4(aiMatrix4x4 ai_mat4x4)
{
    return glm::mat4(
        ai_mat4x4.a1, ai_mat4x4.a2, ai_mat4x4.a3, ai_mat4x4.a4,
        ai_mat4x4.b1, ai_mat4x4.b2, ai_mat4x4.b3, ai_mat4x4.b4,
        ai_mat4x4.c1, ai_mat4x4.c2, ai_mat4x4.c3, ai_mat4x4.c4,
        ai_mat4x4.d1, ai_mat4x4.d2, ai_mat4x4.d3, ai_mat4x4.d4);
}
