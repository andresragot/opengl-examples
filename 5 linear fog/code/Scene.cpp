
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "Scene.hpp"
#include <iostream>
#include <cassert>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

namespace udit
{

    using namespace std;

    const string Scene::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4  model_view_matrix;"
        "uniform mat4  projection_matrix;"
        ""
        "uniform float fog_near;"
        "uniform float fog_far;"
        "uniform vec3  fog_color;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_color;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "   vec4  model_view_vertex = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        ""
        "   float fog_intensity = 1.0 - clamp ((fog_far + model_view_vertex.z) / (fog_far - fog_near), 0.0, 1.0);"
        ""
        "   gl_Position = projection_matrix * model_view_vertex;"
        "   front_color = mix (vertex_color, fog_color, fog_intensity);"
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

        glEnable     (GL_CULL_FACE );
        glEnable     (GL_DEPTH_TEST);               // Necesario para el cubo transparente
        glClearColor (1.f, 1.f, 1.f, 1.f);

        // Se compilan y se activan los shaders:

        GLuint program_id = compile_shaders ();

        glUseProgram (program_id);

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");

        // Se configura la niebla:

        GLint fog_near  = glGetUniformLocation (program_id, "fog_near" );
        GLint fog_far   = glGetUniformLocation (program_id, "fog_far"  );
        GLint fog_color = glGetUniformLocation (program_id, "fog_color");

        glUniform1f (fog_near,  5.f);
        glUniform1f (fog_far,  35.f);
        glUniform3f (fog_color, 1.f, 1.f, 1.f);

        // Se establece la matriz de proyección:

        resize (width, height);

        // Se crea la lista de cubos:

        for (size_t index = 0; index < 10; index++)
        {
            cubes.push_back (Cube_Ptr(new Cube));
        }
    }

    void Scene::update ()
    {
        angle += 0.01f;
    }

    void Scene::render ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Se rotan los cubos y se empujan hacia el fondo:

        glm::mat4 model_view_matrix;

        for (size_t index = 0, number_of_cubes = cubes.size (); index < number_of_cubes; index++)
        {
            model_view_matrix = glm::mat4(1);
            model_view_matrix = glm::translate (model_view_matrix, glm::vec3(2.f - 4 * index, 0.f, -5.f - 5.f * index));
            model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(1.f, 2.f, 1.f));

            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

            cubes[index]->render ();
        }
    }

    void Scene::resize (int width, int height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 0.1f, 1000.f);

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

        const char *   vertex_shaders_code[] = {   vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = { fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = { GLint(  vertex_shader_code.size ()) };
        const GLint  fragment_shaders_size[] = { GLint(fragment_shader_code.size ()) };

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

}
