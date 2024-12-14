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
        angle (0),
        plane (20, 20, 4, 3),
        cone (4, 6, 10),
        cylinder (2, 2, 2, 10),
        shader_program( { vertex_shader_code }, { fragment_shader_code } )
    {
        // Se establece la configuración básica:
        
        glEnable     (GL_CULL_FACE);
        glDisable    (GL_DEPTH_TEST);
        // glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glClearColor (.2f, .2f, .2f, .1f);
        
        shader_program.use();
        
        model_view_matrix_id = shader_program.get_uniform_location("model_view_matrix");
        projection_matrix_id = shader_program.get_uniform_location("projection_matrix");
        
        resize(width, height);
    }

    void Scene::update()
    {
        angle += 0.001f;
    }

    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Se calcula la posicion y rotación del plano
        glm::mat4 plane_transform(1);
        
        plane_transform = glm::translate (plane_transform, glm::vec3(0.f, -3.f, -10.f));
        plane_transform = glm::rotate    (plane_transform, angle, glm::vec3(0.f, 1.f, 0.f));
        
        // Se dibuja el plano.
        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(plane_transform));
        plane.render();

        // Se calcula la posicion y rotación del cono
        glm::mat4 cone_transform (1);
        
        cone_transform = glm::translate (cone_transform, glm::vec3(-5.f, 0.f, 0.f));
        cone_transform = glm::rotate    (cone_transform, angle * 2, glm::vec3(0.f, 1.f, 0.f));

        cone_transform = plane_transform * cone_transform;
        
        // Se calcula la posicion y rotación del cilindro
        glm::mat4 cylinder_tranform (1);
        
        cylinder_tranform = glm::translate (cylinder_tranform, glm::vec3(5.f, 0.f, 0.f));
        cylinder_tranform = glm::rotate    (cylinder_tranform, angle * 2, glm::vec3(0.f, 1.f, 0.f));
        
        cylinder_tranform = plane_transform * cylinder_tranform;
        
        angle = fmod(angle, glm::radians(360.0f));
        
        if (angle >= 0 && angle < glm::radians(180.f))
        {
            // Se dibuja el cilindro.
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cylinder_tranform));
            cylinder.render();

            
            // Se dibuja el cono.
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone_transform));
            cone.render();
        }
        else
        {
            // Se dibuja el cono.
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cone_transform));
            cone.render();
            
            // Se dibuja el cilindro.
            glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(cylinder_tranform));
            cylinder.render();
        }
        
        
        
        
        
    }

    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 5000.f);
        
        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        
        glViewport (0, 0, width, height);
    }
}
