//
//  Mesh.hpp
//  render-mesh
//
//  Created by Andrés Ragot on 8/12/24.
//
#pragma once

#include <glad/glad.h>
#include <glm.hpp>

#include <vector>

namespace udit
{
    using std::vector;

    class Mesh
    {
        
    protected:
        vector < glm::vec3 > coordinates;
        vector < glm::vec3 >     normals;
        vector < GLuint >         indexs;
        vector < glm::vec3 >      colors;
        
        // Índices para indexar el array vbo_ids:
        enum
        {
            COORDINATES_VBO,
            COLOR_VBO,
            INDICES_EBO,
            VBO_COUNT
        };
        
    private:
        GLuint vbo_ids[VBO_COUNT];  // Ids de los VBOs que se usan
        GLuint vao_id;              // Id del VAO de la Mesh
        
    protected:
        glm::vec3 random_color ();
        
    public:
        Mesh() = default;
        ~Mesh()
        {
            glDeleteVertexArrays (1, &vao_id);
            glDeleteBuffers      (VBO_COUNT, vbo_ids);
        };
        
        void build_mesh ();
        
        void render();
    };

    class Plane : public Mesh
    {
        float width;
        float height;
        unsigned cols;
        unsigned rows;
        
    public:
        Plane (float width, float height, unsigned cols, unsigned rows) : Mesh(), width (width), height (height), cols (cols), rows (rows) { build_plane(); }
        
        Plane () = default;
        // ~Plane () = default;
        
    private:
        void build_plane();
    };

    class Cone : public Mesh
    {
        
    };

    class CubeA : public Mesh
    {
        
    };

    class Cylinder : public Mesh
    {
        
    };
}