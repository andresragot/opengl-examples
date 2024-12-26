
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "Scene.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <vector>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace std;
using namespace glm;

namespace udit
{

    const string Scene::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   front_color = vertex_color;"
        "}";

    const string Scene::fragment_shader_code =

        "#version 330\n"
        ""
        "in  vec3    front_color;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(front_color, 1.0);"
        "}";

    Scene::Scene(int width, int height)
    :
        angle(0)
    {
        // Se establece la configuración básica:

        glEnable     (GL_CULL_FACE);
        glEnable     (GL_DEPTH_TEST);
        glClearColor (.1f, .1f, .1f, 1.f);

        // Se compilan y se activan los shaders:

        GLuint program_id = compile_shaders ();

        glUseProgram (program_id);

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");

        resize (width, height);

        load_mesh ("../../../../../../../../shared/assets/stanford-bunny.obj");
    }

    Scene::~Scene()
    {
        glDeleteVertexArrays (1, &vao_id);
        glDeleteBuffers      (VBO_COUNT, vbo_ids);
    }

    void Scene::update ()
    {
        angle += 0.01f;
    }

    void Scene::render ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Se rota el malla y se empuja hacia el fondo:

        glm::mat4 model_view_matrix(1);

        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -2.75f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se dibuja la malla:

        glBindVertexArray (vao_id);
        glDrawElements    (GL_TRIANGLES, number_of_indices, GL_UNSIGNED_SHORT, 0);
    }

    void Scene::resize (int width, int height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 5000.f);

        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glViewport (0, 0, width, height);
    }

    GLuint Scene::compile_shaders ()
    {
        GLint succeeded = GL_FALSE;

        // Se crean objetos para los shaders:

        GLuint   vertex_shader_id = glCreateShader (GL_VERTEX_SHADER  );
        GLuint fragment_shader_id = glCreateShader (GL_FRAGMENT_SHADER);

        // Se carga el código de los shaders:

        const char *   vertex_shaders_code[] = {          vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = {        fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = { (GLint)  vertex_shader_code.size  () };
        const GLint  fragment_shaders_size[] = { (GLint)fragment_shader_code.size  () };

        glShaderSource  (  vertex_shader_id, 1,   vertex_shaders_code,   vertex_shaders_size);
        glShaderSource  (fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);

        // Se compilan los shaders:

        glCompileShader (  vertex_shader_id);
        glCompileShader (fragment_shader_id);

        // Se comprueba que si la compilación ha tenido éxito:

        glGetShaderiv   (  vertex_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (  vertex_shader_id);

        glGetShaderiv   (fragment_shader_id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error (fragment_shader_id);

        // Se crea un objeto para un programa:

        GLuint program_id = glCreateProgram ();

        // Se cargan los shaders compilados en el programa:

        glAttachShader  (program_id,   vertex_shader_id);
        glAttachShader  (program_id, fragment_shader_id);

        // Se linkan los shaders:

        glLinkProgram   (program_id);

        // Se comprueba si el linkage ha tenido éxito:

        glGetProgramiv  (program_id, GL_LINK_STATUS, &succeeded);
        if (!succeeded) show_linkage_error (program_id);

        // Se liberan los shaders compilados una vez se han linkado:

        glDeleteShader (  vertex_shader_id);
        glDeleteShader (fragment_shader_id);

        return (program_id);
    }

    void Scene::show_compilation_error (GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetShaderInfoLog (shader_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        #ifdef _MSC_VER
            //OutputDebugStringA (info_log.c_str ());
        #endif

        assert(false);
    }

    void Scene::show_linkage_error (GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;

        glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);

        info_log.resize (info_log_length);

        glGetProgramInfoLog (program_id, info_log_length, NULL, &info_log.front ());

        cerr << info_log.c_str () << endl;

        #ifdef _MSC_VER
            //OutputDebugStringA (info_log.c_str ());
        #endif

        assert(false);
    }

    void Scene::load_mesh (const std::string & mesh_file_path)
    {
        Assimp::Importer importer;

        auto scene = importer.ReadFile
        (
            mesh_file_path,
            aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
        );

        // Si scene es un puntero nulo significa que el archivo no se pudo cargar con éxito:

        if (scene && scene->mNumMeshes > 0)
        {
            // Para este ejemplo se coge la primera malla solamente:

            auto mesh = scene->mMeshes[0];

            size_t number_of_vertices = mesh->mNumVertices;

            // Se generan índices para los VBOs del cubo:

            glGenBuffers (VBO_COUNT, vbo_ids);
            glGenVertexArrays (1, &vao_id);

            // Se activa el VAO del cubo para configurarlo:

            glBindVertexArray (vao_id);

            // Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

            static_assert(sizeof(aiVector3D) == sizeof(fvec3), "aiVector3D should composed of three floats");

            glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
            glBufferData (GL_ARRAY_BUFFER, number_of_vertices * sizeof(aiVector3D), mesh->mVertices, GL_STATIC_DRAW);

            glEnableVertexAttribArray (0);
            glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

            // El archivo del modelo 3D de ejemplo no guarda un color por cada vértice, por lo que se va
            // a crear un array de colores aleatorios (tantos como vértices):

            vector< vec3 > vertex_colors(number_of_vertices);

            for (auto & color : vertex_colors)
            {
                color = random_color ();
            }

            // Se suben a un VBO los datos de color y se vinculan al VAO:

            glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COLORS_VBO]);
            glBufferData (GL_ARRAY_BUFFER, vertex_colors.size () * sizeof(vec3), vertex_colors.data (), GL_STATIC_DRAW);

            glEnableVertexAttribArray (1);
            glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

            // Los índices en ASSIMP están repartidos en "faces", pero OpenGL necesita un array de enteros
            // por lo que vamos a mover los índices de las "faces" a un array de enteros:
            
            // Se asume que todas las "faces" son triángulos (revisar el flag aiProcess_Triangulate arriba).

            number_of_indices = mesh->mNumFaces * 3;

            vector< GLshort > indices(number_of_indices);

            auto vertex_index = indices.begin ();

            for (unsigned i = 0; i < mesh->mNumFaces; ++i)
            {
                auto & face = mesh->mFaces[i];

                assert(face.mNumIndices == 3);

                *vertex_index++ = face.mIndices[0];
                *vertex_index++ = face.mIndices[1];
                *vertex_index++ = face.mIndices[2];
            }

            // Se suben a un EBO los datos de índices:

            glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_EBO]);
            glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size () * sizeof(GLshort), indices.data (), GL_STATIC_DRAW);
        }
    }

    vec3 Scene::random_color ()
    {
        return vec3
        (
            float(rand ()) / float(RAND_MAX),
            float(rand ()) / float(RAND_MAX),
            float(rand ()) / float(RAND_MAX)
        );
    }

}
