
// Este código es de dominio público
// angel.rodriguez@udit.es

#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER

    #include <vector>
    #include <memory>
    #include "Camera.hpp"
    #include "Texture_Cube.hpp"

    namespace udit
    {

        class Skybox
        {
        private:

            static const GLfloat              coordinates[];
            static const std::string   vertex_shader_code;
            static const std::string fragment_shader_code;

            GLuint       vbo_id;                                // Id del VBO de las coordenadas
            GLuint       vao_id;                                // Id del VAO del cubo

            GLuint       shader_program_id;

            GLint        model_view_matrix_id;
            GLint        projection_matrix_id;

            Texture_Cube texture_cube;

        public:

            Skybox(const std::string & texture_path);
           ~Skybox();

        public:

            void render (const Camera & camera);

        private:

            GLuint compile_shaders        ();
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

        };

    }

#endif
