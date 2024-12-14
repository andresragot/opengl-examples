
// Este código es de dominio público
// angel.rodriguez@udit.es

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <string>
    #include <memory>
    #include "Cube.hpp"

    namespace udit
    {

        class Scene
        {
        private:

            static const GLsizei framebuffer_width  = 64;
            static const GLsizei framebuffer_height = 64;

            static const std::string   vertex_shader_code;
            static const std::string fragment_shader_code;

            GLint  model_view_matrix_id;
            GLint  projection_matrix_id;

            GLuint framebuffer_id;
            GLuint depthbuffer_id;
            GLuint out_texture_id;

            Cube   cube;

            float  angle;

        public:

            Scene(int width, int height);

            void   update ();
            void   render ();
            void   resize (int  width, int height);

        private:

            void   build_framebuffer      ();
            void   render_to_texture      ();

            GLuint compile_shaders        ();
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

        };

    }

#endif
