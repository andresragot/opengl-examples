
// Este código es de dominio público
// angel.rodriguez@udit.es

#ifndef VIEW_HEADER
#define VIEW_HEADER

    #include <string>
    #include <memory>
    #include <vector>
    #include "Cube.hpp"

    namespace udit
    {

        class Scene
        {
        private:

            typedef std::shared_ptr< Cube > Cube_Ptr;
            typedef std::vector< Cube_Ptr > Cube_List;

            static const std::string   vertex_shader_code;
            static const std::string fragment_shader_code;

            GLint  model_view_matrix_id;
            GLint  projection_matrix_id;

            float  angle;

            Cube_List cubes;

        public:

            Scene(int width, int height);

            void   update ();
            void   render ();
            void   resize (int width, int height);

        private:

            GLuint compile_shaders        ();
            void   show_compilation_error (GLuint  shader_id);
            void   show_linkage_error     (GLuint program_id);

        };

    }

#endif
