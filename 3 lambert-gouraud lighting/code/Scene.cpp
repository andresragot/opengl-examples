
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
        "struct Light"
        "{"
        "    vec4 position;"
        "    vec3 color;"
        "};"
        ""
        "uniform Light light;"
        "uniform float ambient_intensity;"
        "uniform float diffuse_intensity;"
        ""
        "uniform vec3 material_color;"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        "uniform mat4     normal_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec3 vertex_normal;"
        ""
        "out vec3 front_color;"
        ""
        "void main()"
        "{"
        "    vec4  normal   = normal_matrix * vec4(vertex_normal, 0.0);"
        "    vec4  position = model_view_matrix * vec4(vertex_coordinates, 1.0);"
        ""
        "    vec4  light_direction = light.position - position;"
        "    float light_intensity = diffuse_intensity * max (dot (normalize (normal.xyz), normalize (light_direction.xyz)), 0.0);"
        ""
        "    front_color = ambient_intensity * material_color + diffuse_intensity * light_intensity * light.color * material_color;"
        "    gl_Position = projection_matrix * position;"
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

        glEnable (GL_CULL_FACE );
        glEnable (GL_DEPTH_TEST);

        glClearColor (.2f, .2f, .2f, 1.f);

        // Se compilan y se activan los shaders:

        GLuint program_id = compile_shaders ();

        glUseProgram (program_id);

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");
            normal_matrix_id = glGetUniformLocation (program_id,     "normal_matrix");

        configure_material (program_id);
        configure_light    (program_id);

        resize (width, height);
    }

    void Scene::update ()
    {
        angle += 0.01f;
    }

    void Scene::render ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Se rota el cubo y se empuja hacia el fondo:

        glm::mat4 model_view_matrix(1);

        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -4.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(1.f, 2.f, 1.f));

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        glm::mat4 normal_matrix = glm::transpose (glm::inverse (model_view_matrix));

        glUniformMatrix4fv (normal_matrix_id, 1, GL_FALSE, glm::value_ptr(normal_matrix));

        // Se dibuja el cubo:

        cube.render ();
    }

    void Scene::resize (int width, int height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 500.f);

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

    void Scene::configure_material (GLuint program_id)
    {
        GLint material_color = glGetUniformLocation (program_id, "material_color");

        glUniform3f (material_color, 0.f, 1.f, 0.f);
    }

    void Scene::configure_light (GLuint program_id)
    {
        GLint light_position    = glGetUniformLocation (program_id, "light.position");
        GLint light_color       = glGetUniformLocation (program_id, "light.color"   );
        GLint ambient_intensity = glGetUniformLocation (program_id, "ambient_intensity");
        GLint diffuse_intensity = glGetUniformLocation (program_id, "diffuse_intensity");

        glUniform4f (light_position,   10.0f, 10.f, 10.f, 1.f);
        glUniform3f (light_color,       1.0f,  1.f,  1.f);
        glUniform1f (ambient_intensity, 0.2f);
        glUniform1f (diffuse_intensity, 0.8f);
    }

}
