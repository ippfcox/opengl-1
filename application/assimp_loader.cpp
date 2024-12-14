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

    ProcessNode(scene->mRootNode, root, scene);

    return root;
}

void AssimpLoader::ProcessNode(aiNode *ai_node, Object *parent, const aiScene *scene)
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
        node->AddChild(ProcessMesh(scene->mMeshes[ai_node->mMeshes[i]]));
    }

    for (int i = 0; i < ai_node->mNumChildren; ++i)
    {
        ProcessNode(ai_node->mChildren[i], node, scene);
    }
}

Mesh *AssimpLoader::ProcessMesh(aiMesh *ai_mesh)
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
    material->diffuse = new Texture();
    material->diffuse->InitByFilename("assets/textures/box.png");
    material->shiness = 32.0f;

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
