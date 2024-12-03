//
//  Cube.hpp
//  render-mesh
//
//  Created by Andrés Ragot on 3/12/24.
//

#pragma once

#include <glad/glad.h>

namespace udit
{
    class Cube
    {
    private:
        
        // Índices para indexar el array vbo_ids:
        enum
        {
            COORDINATES_VBO,
            COLOR_VBO,
            INDICES_EBO,
            VBO_COUNT
        };
        
        // Arrays de datos del cubo base:
        static const GLfloat coordinates [];
        static const GLfloat colors      [];
        static const GLubyte indices     [];
        
    private:
        GLuint vbo_ids[VBO_COUNT];  // Ids de los VBOs que se usan
        GLuint vao_id;              // Id del VAO del cubo
        
    public:
        
         Cube();
        ~Cube();
        
        void render ();
        
    };
}
