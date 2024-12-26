
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "Terrain.hpp"
#include <glm.hpp>
#include <half.hpp>
#include <vector>

using glm::vec3;
using std::vector;
using half_float::half;

namespace udit
{

    Terrain::Terrain(float width, float depth, unsigned x_slices, unsigned z_slices)
    {
        number_of_vertices = x_slices * z_slices;

        vector< half > coordinates(number_of_vertices * 2);     // Sólo es necesario guardar las coordenadas X y Z
        vector< half > texture_uvs(number_of_vertices * 2);

        float x = -width * .5f;
        float z = -depth * .5f;
        float u =  0.f;
        float v =  0.f;

        float x_step = width / float(x_slices);
        float z_step = depth / float(z_slices);
        float u_step =   1.f / float(x_slices);
        float v_step =   1.f / float(z_slices);

        int   coordinate_index = 0;

        for (unsigned j = 0; j < z_slices; ++j, z += z_step, v += v_step)
        {
            for (unsigned i = 0; i < x_slices; ++i, coordinate_index += 2, x += x_step, u += u_step)
            {
                coordinates[coordinate_index + 0] = half(x);
                coordinates[coordinate_index + 1] = half(z);
                texture_uvs[coordinate_index + 0] = half(u);
                texture_uvs[coordinate_index + 1] = half(v);
            }

            x += x_step = -x_step;                              // Se invierte el sentido para hacer un zigzag
            u += u_step = -u_step;
        }

        // Se crean el VAO y los VBOs:

        glGenVertexArrays (1, &vao_id);
        glGenBuffers (VBO_COUNT, vbo_ids);

        // Se activa el VAO para configurarlo:

        glBindVertexArray (vao_id);

        // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, coordinates.size () * sizeof(half), coordinates.data (), GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glVertexAttribPointer (0, 2, GL_HALF_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de coordenadas de textura y se vinculan al VAO:

        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
        glBufferData (GL_ARRAY_BUFFER, texture_uvs.size () * sizeof(half), texture_uvs.data (), GL_STATIC_DRAW);

        glEnableVertexAttribArray (1);
        glVertexAttribPointer (1, 2, GL_HALF_FLOAT, GL_FALSE, 0, 0);
    }

    Terrain::~Terrain()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Terrain::render ()
    {
        // Se selecciona el VAO que contiene los datos del objeto y se dibujan sus vértices
        // conectándolos con líneas:

        glBindVertexArray (vao_id);
        glDrawArrays (GL_LINE_STRIP, 0, number_of_vertices);
    }

}
