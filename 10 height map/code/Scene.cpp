
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "Scene.hpp"

#include <glm.hpp>                          // vec3, vec4, mat4, etc.
#include <gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <gtc/type_ptr.hpp>                 // value_ptr

#include <opengl-recipes.hpp>

namespace udit
{

    using namespace std;

    const string Scene::vertex_shader_code =

        "#version 330\n"
        ""
        "uniform mat4 model_view_matrix;"
        "uniform mat4 projection_matrix;"
        ""
        "layout (location = 0) in vec2 vertex_xz;"
        "layout (location = 1) in vec2 vertex_uv;"
        ""
        "uniform sampler2D sampler;"
        "uniform float     max_height;"
        ""
        "out float intensity;"
        ""
        "void main()"
        "{"
        "   float sample = texture (sampler, vertex_uv).r;"
        "   intensity    = sample * 0.75 + 0.25;"
        "   float height = sample * max_height;"
        "   vec4  xyzw   = vec4(vertex_xz.x, height, vertex_xz.y, 1.0);"
        "   gl_Position  = projection_matrix * model_view_matrix * xyzw;"
        "}";

    const string Scene::fragment_shader_code =

        "#version 330\n"
        ""
        "in  float intensity;"
        "out vec4  fragment_color;"
        ""
        "void main()"
        "{"
        "    fragment_color = vec4(intensity, intensity, intensity, 1.0);"
        "}";

    const string Scene::texture_path = "../../../shared/assets/height-map.png";

    Scene::Scene(int width, int height)
    :
        terrain(10.f, 10.f, 50, 50),
        angle  (0.f)
    {
        // Se compilan y se activan los shaders:

        program_id = compile_shaders (vertex_shader_code, fragment_shader_code);

        glUseProgram (program_id);

        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");

        // Se establece la altura máxima del height map en el vertex shader:

        glUniform1f (glGetUniformLocation (program_id, "max_height"), 5.f);

        // Se carga la textura y se envía a la GPU:

        texture_id = create_texture_2d< Monochrome8 > (texture_path);

        there_is_texture = texture_id > 0;

        // Se establece la configuración básica:

        glEnable     (GL_CULL_FACE );
        glEnable     (GL_DEPTH_TEST);
        glClearColor (0.1f, 0.1f, 0.1f, 1.f);

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
        angle += .005f;
    }

    void Scene::render ()
    {
        glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Se rota el cubo y se empuja hacia el fondo:

        glm::mat4 model_view_matrix(1);

        model_view_matrix = glm::translate (model_view_matrix,        glm::vec3(0.f, 0.f, -11.f));
        model_view_matrix = glm::rotate    (model_view_matrix,   .4f, glm::vec3(1.f, 0.f,   0.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(0.f, 1.f,   0.f));

        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));

        // Se selecciona la textura si está disponible:

        if (there_is_texture)
        {
            glBindTexture (GL_TEXTURE_2D, texture_id);
        }

        // Se renderiza el cubo:

        terrain.render ();
    }

    void Scene::resize (int width, int height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 500.f);

        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));

        glViewport (0, 0, width, height);
    }

}
