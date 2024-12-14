#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "../../engine/common.hpp"
#include "../../engine/object/object.hpp"
#include "../../engine/object/mesh.hpp"

class AssimpLoader
{
public:
    static Object *Load(const std::string path);

private:
    static void ProcessNode(aiNode *ai_node, Object *parent, const aiScene *ai_scene, const std::string &root_path);
    static Mesh *ProcessMesh(aiMesh *ai_mesh, const aiScene *ai_scene, const std::string &root_path);
    static glm::mat4 aiMat4x4Toglmmat4(aiMatrix4x4 ai_mat4x4);
};