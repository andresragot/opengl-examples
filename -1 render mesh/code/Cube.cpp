//
//  Cube.cpp
//  render-mesh
//
//  Created by Andrés Ragot on 3/12/24.
//

#include "Cube.hpp"

namespace udit
{
    const GLfloat Cube::coordinates[] =
    {
        -1,-1,+1,           // 0
        +1,-1,+1,           // 1
        +1,+1,+1,           // 2
        -1,+1,+1,           // 3
        -1,-1,-1,           // 4
        +1,-1,-1,           // 5
        +1,+1,-1,           // 6
        -1,+1,-1,           // 7
    };

    const GLfloat Cube::colors[] =
    {
        0,0,1,              // 0
        1,0,1,              // 1
        1,1,1,              // 2
        0,1,1,              // 3
        0,0,0,              // 4
        1,0,0,              // 5
        1,1,0,              // 6
        0,1,0,              // 7
    };

    const GLubyte Cube::indices[] =
    {
        0, 1, 2,            // front
        0, 2, 3,
        4, 0, 3,            // left
        4, 3, 7,
        7, 5, 4,            // back
        7, 6, 5,
        1, 5, 6,            // right
        1, 6, 2,
        3, 2, 6,            // top
        3, 6, 7,
        5, 0, 4,            // bottom
        5, 1, 0,
    };

    Cube::Cube()
    {
        // Se generan Índices para los VBOs del cubo:
        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);
        
        // Se activa el VAO del cubo para configurarlo:
        
        glBindVertexArray (vao_id);
        
        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        
        // Se suben a un VBO los datos de color y se vinculan al VAO:
        
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLOR_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        
    }
}
