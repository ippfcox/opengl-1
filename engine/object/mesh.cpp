#include "mesh.hpp"

Mesh::Mesh(Geometry *geometry, Material *material)
    : geometry(geometry), material(material)
{
    type_ = ObjectType::Mesh;
}

Mesh::~Mesh()
{
}