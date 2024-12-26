
// Este código es de dominio público
// angel.rodriguez@udit.es

#ifndef CUBE_HEADER
#define CUBE_HEADER

    #include <memory>
    #include <glad/glad.h>

    namespace udit
    {

        class Cube
        {
        private:

            // Índices para indexar el array vbo_ids:

            enum
            {
                COORDINATES_VBO,
                TEXTURE_UVS_VBO,
                INDICES_IBO,
                VBO_COUNT
            };

            // Arrays de datos del cubo base:

            static const GLfloat coordinates[];
            static const GLfloat texture_uvs[];
            static const GLubyte indices    [];

        private:

            GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
            GLuint vao_id;                  // Id del VAO del cubo

        public:

            Cube();
           ~Cube();

        public:

            void render ();

        };

    }

#endif
