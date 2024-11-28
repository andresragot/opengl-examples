
// Este código es de dominio público
// angel.rodriguez@udit.es

#ifndef SCENE_HEADER
#define SCENE_HEADER

    #include "Cube.hpp"
    #include <glad/glad.h>
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
            GLint      normal_matrix_id;

            Cube   cube;
            float  angle;

        public:

            Scene(int width, int height);

            void   update ();
            void   render ();
            void   resize (int width, int height);

        private:

            GLuint compile_shaders        ();
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

            void   configure_material     (GLuint program_id);
            void   configure_light        (GLuint program_id);

        };

    }

#endif
