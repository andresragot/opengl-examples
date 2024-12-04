
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "Cube.hpp"

namespace udit
{

    const GLfloat Cube::coordinates[] =
    {
        -1.f, +1.f, +1.f,
        +1.f, +1.f, +1.f,
        +1.f, -1.f, +1.f,
        -1.f, -1.f, +1.f,
        -1.f, +1.f, -1.f,
        -1.f, +1.f, +1.f,
        -1.f, -1.f, +1.f,
        -1.f, -1.f, -1.f,
        +1.f, +1.f, -1.f,
        -1.f, +1.f, -1.f,
        -1.f, -1.f, -1.f,
        +1.f, -1.f, -1.f,
        +1.f, +1.f, +1.f,
        +1.f, +1.f, -1.f,
        +1.f, -1.f, -1.f,
        +1.f, -1.f, +1.f,
        -1.f, -1.f, +1.f,
        +1.f, -1.f, +1.f,
        +1.f, -1.f, -1.f,
        -1.f, -1.f, -1.f,
        -1.f, +1.f, -1.f,
        +1.f, +1.f, -1.f,
        +1.f, +1.f, +1.f,
        -1.f, +1.f, +1.f,
    };

    const GLfloat Cube::normals[] =
    {
         0.f,  0.f, +1.f,
         0.f,  0.f, +1.f,
         0.f,  0.f, +1.f,
         0.f,  0.f, +1.f,
        -1.f,  0.f,  0.f,
        -1.f,  0.f,  0.f,
        -1.f,  0.f,  0.f,
        -1.f,  0.f,  0.f,
         0.f,  0.f, -1.f,
         0.f,  0.f, -1.f,
         0.f,  0.f, -1.f,
         0.f,  0.f, -1.f,
        +1.f,  0.f,  0.f,
        +1.f,  0.f,  0.f,
        +1.f,  0.f,  0.f,
        +1.f,  0.f,  0.f,
         0.f, -1.f,  0.f,
         0.f, -1.f,  0.f,
         0.f, -1.f,  0.f,
         0.f, -1.f,  0.f,
         0.f, +1.f,  0.f,
         0.f, +1.f,  0.f,
         0.f, +1.f,  0.f,
         0.f, +1.f,  0.f,
    };

    const GLubyte Cube::indices[] =
    {
        1,  0,  3,              // front
        1,  3,  2,
        5,  4,  7,              // left
        5,  7,  6,
        9,  8,  11,             // back
        9,  11, 10,
        13, 12, 15,             // right
        13, 15, 14,
        17, 16, 19,             // bottom
        17, 19, 18,
        20, 23, 22,             // top
        20, 22, 21,
    };

    Cube::Cube()
    {
        // Se generan índices para los VBOs del cubo:

        glGenBuffers (VBO_COUNT, vbo_ids);
        glGenVertexArrays (1, &vao_id);

        // Se activa el VAO del cubo para configurarlo:

        glBindVertexArray (vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de normales y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
        glBufferData (GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un IBO los datos de índices:

        glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glBindVertexArray (0);
    }

    Cube::~Cube()
    {
        // Se liberan los VBOs y el VAO usados:

        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Cube::render ()
    {
        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:

        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);
    }

}
