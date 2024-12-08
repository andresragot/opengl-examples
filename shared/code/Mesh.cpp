//
//  Mesh.cpp
//  render-mesh
//
//  Created by Andrés Ragot on 8/12/24.
//

#include "Mesh.hpp"
#include <iostream>

namespace udit {
    using glm::vec3;

    void CheckGLError()
    {
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            
            switch (err) {
                case GL_INVALID_OPERATION:
                    error = "INVALID_OPERATION";
                    break;
                case GL_INVALID_ENUM:
                    error = "INVALID_ENUM";
                    break;
                case GL_INVALID_VALUE:
                    error = "INVALID_VALUE";
                    break;
                    
                case GL_INVALID_INDEX:
                    error = "INVALID_INDEX";
                    break;
                
                case GL_OUT_OF_MEMORY:
                    error = "OUT_OF_MEMORY";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    error = "INVALID_FRAMEBUFFER_OPERATION";
                    break;
            }
            
            std::cerr << "OpenGL Error: " << error << std::endl;
            
            assert(false);
        }
    }

    void Plane::build_plane()
    {
        coordinates.reserve ((cols + 1) * (rows + 1));
            normals.reserve ((cols + 1) * (rows + 1));
             colors.reserve ((cols + 1) * (rows + 1));
             indexs.reserve (cols * rows * 2 * 3);
        
        float size_width  = width  / float (cols);
        float size_height = height / float (rows);
        
        vec3 vertex = vec3 { -width / 2.f, height / 2.f, -10.f};
        
        for (unsigned j = 0; j < rows + 1; ++j)
        {
            for (unsigned i = 0; i < cols + 1; ++i)
            {
                coordinates.push_back (vertex);
                    normals.push_back ({0.f, 0.f, 1.f});
                     colors.push_back (random_color());
                
                vertex.x += size_width;
            }
            vertex.y -= size_height;
            vertex.x = - width / 2.f;
        }
        
        for (unsigned j = 0; j < rows; ++j)
        {
            for (unsigned i = 0; i < cols; ++i)
            {
                // First Triangle
                indexs.push_back ((j * cols) + j + i);
                indexs.push_back ((j * cols) + j + i + cols + 1);
                indexs.push_back ((j * cols) + j + i + cols + 2);
                
                // Second Triangle
                indexs.push_back ((j * cols) + j + i);
                indexs.push_back ((j * cols) + j + i + cols + 2);
                indexs.push_back ((j * cols) + j + i + 1);
                
                // indexs.push_back ({j * cols + j + i, j * cols + j + i + cols + 1, j * cols + j + i + cols + 2});
                // indexs.push_back ({j * cols + j + i, j * cols + j + i + cols + 2, j * cols + j + i + 1});
            }
        }
        
        build_mesh();
    }

    void Mesh::build_mesh()
    {
        // Se generan los índices para los VBOs del cubo:
        glGenBuffers (VBO_COUNT, vbo_ids);
        CheckGLError();
        
        glGenVertexArrays (1, &vao_id);
        CheckGLError();
        
        // Se activa el VAO del cubo para configurarlo;
        
        glBindVertexArray (vao_id);
        CheckGLError();
        
        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        CheckGLError();
        glBufferData (GL_ARRAY_BUFFER, coordinates.size() * sizeof (vec3), coordinates.data(), GL_STATIC_DRAW);
        CheckGLError();
        
        glEnableVertexAttribArray (0);
        CheckGLError();
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CheckGLError();
        
        // Se suben a un VBO los datos de color y se vinculan al VAO:
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLOR_VBO]);
        CheckGLError();
        glBufferData (GL_ARRAY_BUFFER, colors.size() * sizeof (vec3), colors.data(), GL_STATIC_DRAW);
        CheckGLError();
        
        glEnableVertexAttribArray (1);
        CheckGLError();
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        CheckGLError();
        
        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
        CheckGLError();
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, indexs.size() * sizeof (GLuint), indexs.data(), GL_STATIC_DRAW);
        CheckGLError();
        
        glBindVertexArray (0);
        CheckGLError();
    }


    void Mesh::render()
    {
        glBindVertexArray (vao_id);
        CheckGLError();
        glDrawElements    (GL_TRIANGLES, (GLsizei) indexs.size() * sizeof (GLuint), GL_UNSIGNED_INT, nullptr);
        CheckGLError();
        glBindVertexArray (0);
        CheckGLError();
    }

    vec3 Mesh::random_color()
    {
        return vec3
            (
                float(rand ()) / float(RAND_MAX),
                float(rand ()) / float(RAND_MAX),
                float(rand ()) / float(RAND_MAX)
            );
    }

    
}
