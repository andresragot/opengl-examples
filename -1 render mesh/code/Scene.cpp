//
//  Scene.cpp
//  render-mesh
//
//  Created by Andrés Ragot on 3/12/24.
//


#pragma once

#include "Scene.hpp"

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
        angle(0)
    {
        // Se establece la configuración básica:
        
        glEnable     (GL_CULL_FACE);
        glDisable    (GL_DEPTH_TEST);
        glClearColor (.2f, .2f, .2f, .1f);
        
        // Se compilan y se activan los shaders:
        
        GLuint program_id = compile_shader();
        
        glUseProgram(program_id);
        
        model_view_matrix_id = glGetUniformLocation (program_id, "model_view_matrix");
        projection_matrix_id = glGetUniformLocation (program_id, "projection_matrix");
        
        resize(width, height);
    }

    void Scene::update()
    {
        angle += 0.01f;
    }

    void Scene::render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Se rota el cubo y se empija hacia el fondo:
        
        glm::mat4 model_view_matrix(1);
        
        model_view_matrix = glm::translate (model_view_matrix, glm::vec3(0.f, 0.f, -4.f));
        model_view_matrix = glm::rotate    (model_view_matrix, angle, glm::vec3(1.f, 2.f, 1.f));
        
        glUniformMatrix4fv (model_view_matrix_id, 1, GL_FALSE, glm::value_ptr(model_view_matrix));
        
        // Se dibuja el cubo:
        cube.render();
    }

    void Scene::resize(unsigned width, unsigned height)
    {
        glm::mat4 projection_matrix = glm::perspective (20.f, GLfloat(width) / height, 1.f, 5000.f);
        
        glUniformMatrix4fv (projection_matrix_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));
        
        glViewport (0, 0, width, height);
    }

    GLuint Scene::compile_shader()
    {
        GLint succeded = GL_FALSE;
        
        // Se crean objetos para los shaders:
        
        GLuint     vertex_shader_id = glCreateShader (GL_VERTEX_SHADER);
        GLuint fragment_shader_id = glCreateShader (GL_FRAGMENT_SHADER);
        
        // Se carga el código de los shaders;
        
        const char *   vertex_shaders_code[] = {          vertex_shader_code.c_str () };
        const char * fragment_shaders_code[] = {        fragment_shader_code.c_str () };
        const GLint    vertex_shaders_size[] = { (GLint)   vertex_shader_code.size () };
        const GLint  fragment_shaders_size[] = { (GLint) fragment_shader_code.size () };
        
        glShaderSource (  vertex_shader_id, 1,   vertex_shaders_code,   vertex_shaders_size);
        glShaderSource (fragment_shader_id, 1, fragment_shaders_code, fragment_shaders_size);
        
        // Se compilan los shaders:
        
        glCompileShader (  vertex_shader_id);
        glCompileShader (fragment_shader_id);
        
        // Se comprueba que si la compilación ha tenido Éxito:
        glGetShaderiv (  vertex_shader_id, GL_COMPILE_STATUS, &succeded);
        if (!succeded) show_compilation_error (  vertex_shader_id);
        
        glGetShaderiv (fragment_shader_id, GL_COMPILE_STATUS, &succeded);
        if (!succeded) show_compilation_error (fragment_shader_id);
        
        // Se crea un objeto para un programa:
        
        GLuint program_id = glCreateProgram();
        
        // se cargan los shaders compilados en el programa:
        
        glAttachShader  (program_id,   vertex_shader_id);
        glAttachShader  (program_id, fragment_shader_id);
        
        // Se linkan los shaders:
        
        glLinkProgram   (program_id);
        
        // Se comprueba si el linkage ha tneido Éxito:
        
        glGetProgramiv  (program_id, GL_LINK_STATUS, &succeded);
        if (!succeded) show_linkage_error (program_id);
        
        // Se liberan los shaders compilados una vez se han linkado:
        
        glDeleteShader (  vertex_shader_id);
        glDeleteShader (fragment_shader_id);
        
        return (program_id);
    }

    void Scene::show_compilation_error(GLuint shader_id)
    {
        string info_log;
        GLint  info_log_length;
        
        glGetShaderiv (shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
        
        info_log.resize (info_log_length);
        
        glGetShaderInfoLog (shader_id, info_log_length, NULL, &info_log.front ());
        
        cerr << info_log.c_str() << endl;
        
        assert(false);
    }
        
    void Scene::show_linkage_error(GLuint program_id)
    {
        string info_log;
        GLint  info_log_length;
        
        glGetProgramiv (program_id, GL_INFO_LOG_LENGTH, &info_log_length);
        
        info_log.resize (info_log_length);
        
        glGetProgramInfoLog (program_id, info_log_length, NULL, &info_log.front ());
        
        cerr << info_log.c_str() << endl;
        
        assert(false);
    }
}
