//
//  Scene.cpp
//  render-mesh
//
//  Created by Andrés Ragot on 3/12/24.
//

#include "Scene.hpp"
#include "Mesh.hpp"

#include <iostream>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

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
        "   fragment_color = vec4(front_color, 1.0);"
        "}";
    
    Scene::Scene(unsigned width, unsigned height)
    :
        angle(0),
        plane (10, 8, 4, 3),
        cone (4, 6, 4),
        shader_program( { vertex_shader_code }, { fragment_shader_code } )
    {
        // Se establece la configuración básica:
        
        glEnable     (GL_CULL_FACE);
        glDisable    (GL_DEPTH_TEST);
        // glFrontFace(GL_CW); // Cambia la orientación frontal a sentido horario
        glClearColor (.2f, .2f, .2f, .1f);
        
        shader_program.use();
        
        model_view_matrix_id = shader_program.get_uniform_location("model_view_matrix");
        projection_matrix_id = shader_program.get_uniform_location("projection_matrix");
        
        resize(width, height);
    }

    void Scene::update()
    {
        angle += 0.01f;
    }

    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Se rota el cubo y se empuja hacia el fondo:
        
        glm::mat4 model_view_matrix(1);
        
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -10.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
        
        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        
        // Se dibuja el cubo:
        plane.render();
         cone.render();
    }

    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 5000.f);
        
        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        
        glViewport (0, 0, width, height);
    }
}
