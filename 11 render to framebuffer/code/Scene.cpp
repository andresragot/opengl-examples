
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
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec3 vertex_coordinates;"
        "layout (location = 1) in vec2 vertex_texture_uv;"
        ""
        "out vec2 texture_uv;"
        ""
        "void main()"
        "{"
        "   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
        "   texture_uv  = vertex_texture_uv;"
        "}";

    const string Scene::fragment_shader_code =

        "#version 330\n"
        ""
        "uniform sampler2D sampler2d;"
        ""
        "in  vec2 texture_uv;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(texture (sampler2d, texture_uv.st).rgb, 1.0);"
        "}";

    Scene::Scene(int width, int height)
    :
        angle(0)
    {
        // Se crea la textura y se dibuja algo en ella:

        build_framebuffer ();

        glBindFramebuffer (GL_FRAMEBUFFER, framebuffer_id);     // Se activa el framebuffer de la textura

        render_to_texture ();

        glBindFramebuffer (GL_FRAMEBUFFER, 0);                  // Se activa el framebuffer de la ventana

        // Se compilan y se activan los shaders:

        GLuint program_id = compile_shaders ();

        glUseProgram (program_id);

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");

        // Se establece la configuración básica:

        glEnable     (GL_CULL_FACE);
        glClearColor (0.f, 0.f, 0.f, 1.f);

        resize (width, height);
    }

    void Scene::update ()
    {
        angle += 0.01f;
    }

    void Scene::render ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Se rota el cubo y se empuja hacia el fondo a la izquierda:

        glm::mat4 model_view_matrix(1);
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -4.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(1.f, 2.f, 1.f));

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se activa la textura renderizada:

        glBindTexture (GL_TEXTURE_2D, out_texture_id);

        // Se renderiza el cubo con ella:

        cube.render ();
    }

    void Scene::resize (int width, int height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 50.f);

        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glViewport (0, 0, width, height);
    }

    void Scene::build_framebuffer ()
    {
        // Se crea un framebuffer en el que poder renderizar:
        {
            glGenFramebuffers (1, &framebuffer_id);
            glBindFramebuffer (GL_FRAMEBUFFER, framebuffer_id);
        }

        // Se crea una textura que será el búfer de color vinculado al framebuffer:
        {
            glGenTextures   (1, &out_texture_id);
            glBindTexture   (GL_TEXTURE_2D, out_texture_id);

            // El búfer de color tendrá formato RGB:

            glTexImage2D    (GL_TEXTURE_2D, 0, GL_RGB, framebuffer_width, framebuffer_height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        }

        // Se crea un Z-Buffer para usarlo en combinación con el framebuffer:
        {
            glGenRenderbuffers        (1, &depthbuffer_id);
            glBindRenderbuffer        (GL_RENDERBUFFER, depthbuffer_id);
            glRenderbufferStorage     (GL_RENDERBUFFER, GL_DEPTH_COMPONENT,  framebuffer_width, framebuffer_height);
            glFramebufferRenderbuffer (GL_FRAMEBUFFER,  GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER,   depthbuffer_id);
        }

        // Se configura el framebuffer:
        {
            glFramebufferTexture (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, out_texture_id, 0);

            const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;

            glDrawBuffers (1, &draw_buffer);
        }

        // Se comprueba que el framebuffer está listo:

        assert(glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        // Se desvincula el framebuffer:

        glBindFramebuffer (GL_FRAMEBUFFER, 0);
    }

    void Scene::render_to_texture ()
    {
        // Este código se ha intentado mantener simple y compacto.
        // Se asumen ciertas cosas que permiten evitar cierto código de inicialización,
        // como que el core profile está desactivado para no necesitar shaders aquí.
        // La idea general es que se puede renderizar cualquier cosa con normalidad.

        static const GLfloat triangle[] =
        {
            -0.9f, -0.9f, 0.0f,
            +0.9f, -0.5f, 0.0f,
             0.0f, +0.9f, 0.0f
        };

        GLuint triangle_vbo;

        glViewport   (0, 0, framebuffer_width, framebuffer_height);

        glClearColor (.0f, .5f, .5f, 1.0f);
        glClear      (GL_COLOR_BUFFER_BIT);

        glGenBuffers (1, &triangle_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, triangle_vbo);
        glBufferData (GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);

        glEnableVertexAttribArray (0);
        glBindBuffer (GL_ARRAY_BUFFER, triangle_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glDrawArrays (GL_TRIANGLES, 0, 3);
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

        glDeleteShader  (  vertex_shader_id);
        glDeleteShader  (fragment_shader_id);

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
