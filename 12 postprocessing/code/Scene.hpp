
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

            static const GLsizei framebuffer_width  = 256;
            static const GLsizei framebuffer_height = 256;

            static const std::string     cube_vertex_shader_code;
            static const std::string   cube_fragment_shader_code;
            static const std::string   effect_vertex_shader_code;
            static const std::string effect_fragment_shader_code;

            GLint  model_view_matrix_id;
            GLint  projection_matrix_id;

            GLuint framebuffer_id;
            GLuint depthbuffer_id;
            GLuint out_texture_id;

            Cube   cube;

            GLuint   cube_program_id;
            GLuint effect_program_id;

            GLuint framebuffer_quad_vao;
            GLuint framebuffer_quad_vbos[2];

            float  angle;

            int    window_width;
            int    window_height;

        public:

            Scene(int width, int height);
           ~Scene();

            void   update ();
            void   render ();
            void   resize (int  width, int height);

        private:

            void   build_framebuffer      ();
            void   render_framebuffer     ();

            GLuint compile_shaders        (const std::string & vertex_shader_code, const std::string & fragment_shader_code);
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

        };

    }

#endif
