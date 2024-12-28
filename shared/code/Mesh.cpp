// author: Andrés Ragot - github.com/andresragot
// Copyright (c)
// 03/12/2024

#include "Mesh.hpp"

#include <cmath>
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
        
        vec3 vertex = vec3 { - width / 2.f, 0.f, - height / 2.f};
        
        for (unsigned j = 0; j < rows + 1; ++j)
        {
            for (unsigned i = 0; i < cols + 1; ++i)
            {
                coordinates.push_back (vertex);
                    normals.push_back ({0.f, 1.f, 0.f});
                     colors.push_back (random_color());
                
                vertex.x += size_width;
            }
            vertex.z += size_height;
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

    void Cone::build_cone()
    {
        coordinates.reserve (sides + 2);
            normals.reserve (sides + 2);
             colors.reserve (sides + 2);
             indexs.reserve (sides * 3 * 2);
        
        vec3 vertex_top    = vec3 { 0.f, height, 0.f };
        vec3 vertex_bottom = vec3 { 0.f, 0.f, 0.f};
        vec3 normal        = vec3 { 0.f, 0.f, 0.f};
        
        // Indice de abajo
        coordinates.push_back (vertex_bottom);
            normals.push_back ({0.f, -1.f, 0.f});
             colors.push_back (random_color());
        
        // Indice de arriba
        coordinates.push_back (vertex_top);
            normals.push_back ({0.f, 1.f, 0.f}); 
             colors.push_back (random_color());
        
        float angle_step = (360.f / float (sides)) * deg_to_rad;
        
        for (unsigned i = 0; i < sides; ++i)
        {
            float cos_value = cos (angle_step * i);
            float sin_value = sin (angle_step * i);
            
            vertex_bottom.x = radius * cos_value;
            vertex_bottom.z = radius * sin_value;
            
            normal = { cos_value, 0.f, sin_value };
            
            coordinates.push_back (vertex_bottom);
                normals.push_back (glm::normalize(normal));
                 colors.push_back (random_color());
            
            GLuint last_side = i + 3;
            
            if (i + 1 >= sides)
            {
                last_side = 2;
            }
            
            // Cara de lado
            indexs.push_back(i + 2);
            indexs.push_back(1);
            indexs.push_back(last_side);
            
            // Cara del suelo
            indexs.push_back(0);
            indexs.push_back(i + 2);
            indexs.push_back(last_side);
        }
        std::cout << "Debug" << std::endl;
        
        build_mesh();
    }

    void Cylinder::build_cylinder()
    {
        coordinates.reserve ((sides * 2) + 2);
            normals.reserve ((sides * 2) + 2);
             colors.reserve ((sides * 2) + 2);
             indexs.reserve ((sides * 12));
        
        vec3 vertex_top    = vec3 {0.f, height,  0.f};
        vec3 vertex_bottom = vec3 {0.f, 0.f, 0.f};
        vec3 normal        = vec3 {0.f, 0.f, 0.f};
        
        // Indice de abajo
        coordinates.push_back (vertex_bottom);
            normals.push_back ({0.f, -1.f, 0.f});
             colors.push_back (random_color());
        
        // Indice de arriba
        coordinates.push_back (vertex_top);
            normals.push_back ({0.f, 1.f, 0.f});
             colors.push_back (random_color());
        
        float angle_step = (360.f / float (sides)) * deg_to_rad;
        
        // Base de abajo
        for (unsigned i = 0; i < sides; ++i)
        {
            float cos_value = cos (angle_step * i);
            float sin_value = sin (angle_step * i);
            
            normal = { cos_value, 0.f, sin_value };
            
            vertex_bottom.x = bottom_radius * cos_value;
            vertex_bottom.z = bottom_radius * sin_value;
            
            coordinates.push_back (vertex_bottom);
                normals.push_back (glm::normalize(normal));
                 colors.push_back (random_color());
        }
        
        for (unsigned i = 0; i < sides; ++i)
        {
            float cos_value = cos (angle_step * i);
            float sin_value = sin (angle_step * i);
            
            normal = { cos_value, 0.f, sin_value };
            
            vertex_top.x    = top_radius    * cos_value;
            vertex_top.z    = top_radius    * sin_value;
            
            coordinates.push_back (vertex_top);
                normals.push_back (glm::normalize(normal));
                 colors.push_back (random_color());
        }
        
        for (unsigned i = 0; i < sides; ++i)
        {
            GLuint last_side_bottom = i + 3;
            GLuint last_side_top    = i + sides + 3;
            
            if (i + 1 >= sides)
            {
                last_side_bottom = 2;
                last_side_top    = sides + 2;
            }

            // Parte de abajo
            indexs.push_back (0);
            indexs.push_back (i + 2);
            indexs.push_back (last_side_bottom);
            
            // Parte de arriba
            indexs.push_back (i + sides + 2);
            indexs.push_back (1);
            indexs.push_back (last_side_top);
            
            // Lados
            indexs.push_back (last_side_bottom);
            indexs.push_back (i + 2);
            indexs.push_back (i + sides + 2);
            
            indexs.push_back (i + sides + 2);
            indexs.push_back (last_side_top);
            indexs.push_back (last_side_bottom);
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
