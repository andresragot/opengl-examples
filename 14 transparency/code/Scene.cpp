
// Este código es de dominio público
// angel.rodriguez@udit.es

#include <iostream>
#include <cassert>
#include <SOIL2.h>

#include <glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

#include "Scene.hpp"

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
        "uniform sampler2D sampler;"
        ""
        "in  vec2 texture_uv;"
        "out vec4 fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(texture (sampler, texture_uv).rgb, 0.5);"
        "}";

    const string Scene::texture_path = "../../../shared/assets/uv-checker.png";

    Scene::Scene(int width, int height)
    :
        angle(0.0f)
    {
        // Se compilan y se activan los shaders:

        program_id = compile_shaders ();

        glUseProgram (program_id);

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");

        // Se carga la textura y se envía a la GPU:

        texture_id = create_texture_2d (texture_path);

        there_is_texture = texture_id > 0;

        // Se establece la configuración básica:

        glEnable     (GL_CULL_FACE );
        glEnable     (GL_DEPTH_TEST);
        glClearColor (0.4f, 0.4f, 0.4f, 1.f);

        resize (width, height);
    }

    Scene::~Scene()
    {
        glDeleteProgram (program_id);

        if (there_is_texture)
        {
            glDeleteTextures (1, &texture_id);
        }
    }

    void Scene::update ()
    {
        angle += 0.01f;
    }

    void Scene::render ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Se selecciona la textura si está disponible:

        if (there_is_texture)
        {
            glBindTexture (GL_TEXTURE_2D, texture_id);
        }

        // PRIMERA ETAPA (RENDER DE LOS OBJETOS OPACOS):

        // Se rota un cubo y se empuja hacia el fondo:

        glm::mat4 model_view_matrix;

        model_view_matrix = glm::mat4(1);
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -5.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -2.f));

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se renderiza un cubo:

        cube.render ();

        // SEGUNDA ETAPA (RENDER DE LOS OBJETOS TRANSPARENTES):

        // Se habilita la mezcla con el color de fondo usando el canal alpha y se deshabilita la escritura en el Z-Buffer:

        glDepthMask (GL_FALSE);
        glEnable    (GL_BLEND);
        glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Se rota otro cubo y se empuja hacia el fondo:

        model_view_matrix = glm::mat4(1);
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -5.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, +2.f));

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se renderiza un cubo:

        cube.render ();

        // Se deshabilita la mezcla con el fondo y se restaura escritura en el Z-Buffer:

        glDisable   (GL_BLEND);
        glDepthMask (GL_TRUE );
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

        return program_id;
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

    GLuint Scene::create_texture_2d (const std::string & texture_path)
    {
        auto image = load_image (texture_path);

        if (image)
        {
            // Se habilitan las texturas, se genera un id para un búfer de textura,
            // se selecciona el búfer de textura creado y se configuran algunos de
            // sus parámetros:

            GLuint texture_id;

            glEnable      (GL_TEXTURE_2D );
            glGenTextures (1, &texture_id);
            glBindTexture (GL_TEXTURE_2D, texture_id);

            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexImage2D
            (
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                image->get_width  (),
                image->get_height (),
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                image->colors ()
            );

            glGenerateMipmap (GL_TEXTURE_2D);

            return texture_id;
        }

        return -1;
    }

    unique_ptr< Scene::Color_Buffer > Scene::load_image (const string & image_path)
    {
        // Se carga la imagen del archivo usando SOIL2:

        int image_width    = 0;
        int image_height   = 0;
        int image_channels = 0;

        uint8_t * loaded_pixels = SOIL_load_image
        (
            image_path.c_str (),
           &image_width, 
           &image_height, 
           &image_channels,
            SOIL_LOAD_RGBA              // Indica que nos devuelva los pixels en formato RGB32
        );                              // al margen del formato usado en el archivo

        // Si loaded_pixels no es nullptr, la imagen se ha podido cargar correctamente:

        if (loaded_pixels)
        {
            auto image = make_unique< Color_Buffer > (image_width, image_height);
            
            // Se copian los bytes de un buffer a otro directamente:

            std::copy_n
            (
                loaded_pixels, 
                size_t(image_width) * size_t(image_height) * sizeof(Color_Buffer::Color),
                reinterpret_cast< uint8_t * >(image->colors ())
            );

            // Se libera la memoria que reservó SOIL2 para cargar la imagen:

            SOIL_free_image_data (loaded_pixels);

            return image;
        }

        return nullptr;
    }

}
