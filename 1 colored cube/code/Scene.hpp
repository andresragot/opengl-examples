
// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once

#include "Cube.hpp"
#include <string>

namespace udit
{

    class Scene
    {
    private:

        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;

        GLint  model_view_matrix_id;
        GLint  projection_matrix_id;

        Cube   cube;
        float  angle;

    public:

        Scene(unsigned width, unsigned height);

        void   update ();
        void   render ();
        void   resize (unsigned width, unsigned height);

    private:

        GLuint compile_shaders        ();
        void   show_compilation_error (GLuint  shader_id);
        void   show_linkage_error     (GLuint program_id);

    };

}
