
// Este código es de dominio público
// angel.rodriguez@udit.es

#ifndef SCENE_HEADER
#define SCENE_HEADER

    #include <glad/glad.h>
    #include <glm.hpp>
    #include <string>

    namespace udit
    {

        using glm::vec3;

        class Scene
        {
        private:

            enum
            {
                COORDINATES_VBO,
                COLORS_VBO,
                INDICES_EBO,
                VBO_COUNT
            };

            static const std::string   vertex_shader_code;
            static const std::string fragment_shader_code;

            GLuint  vbo_ids[VBO_COUNT];
            GLuint  vao_id;

            GLsizei number_of_indices;

            GLint   model_view_matrix_id;
            GLint   projection_matrix_id;

            float   angle;

        public:

            Scene(int width, int height);
           ~Scene();

            void   update ();
            void   render ();
            void   resize (int width, int height);

        private:

            GLuint compile_shaders        ();
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);
            void   load_mesh              (const std::string & mesh_file_path);
            vec3   random_color           ();

        };

    }

#endif
