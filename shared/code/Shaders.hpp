// author: Andrés Ragot - github.com/andresragot
// Copyright (c)
// 03/12/2024

#pragma once

#include <stdio.h>
#include <string>
#include "glad/glad.h"

using namespace std;

namespace udit
{
    class Shader
    {
    private:
        GLuint  id;
        bool    compilation_succeeded;
        string  error;
        
    protected:
        Shader (const vector< string > & source_code, GLenum type);
        
        GLuint compile_shader();
        void show_compilation_error ();
        
    public:
        Shader () = default;
        // Shader (const Shader &) = delete;
        // Shader (Shader && ) = delete;
        
        // Shader & operator = (const Shader &) = delete;
        // Shader & operator = (Shader &&) = delete;
        
        bool is_ok () const
        {
            return compilation_succeeded;
        }
        
        string * get_error ()
        {
            return error.empty() ? nullptr : &error;
        }
        
        GLuint get_id () const
        {
            return id;
        }
        
        ~Shader()
        {
            glDeleteShader (id);
        }
    };

    class Vertex_Shader : public Shader
    {
    public:
        Vertex_Shader () = default;
        Vertex_Shader (const vector < string > & source_code) : Shader (source_code, GL_VERTEX_SHADER)
        {
        }
    };

    class Fragment_Shader : public Shader
    {
    public:
        Fragment_Shader () = default;
        Fragment_Shader (const vector <string > & source_code) : Shader (source_code, GL_FRAGMENT_SHADER)
        {
        }
    };

    class Shader_Program
    {
    private:
        GLuint program_id;
        Vertex_Shader vertex_shader;
        Fragment_Shader fragment_shader;
    public:
        Shader_Program (const Vertex_Shader & vertex_shader, const Fragment_Shader & fragment_shader)
            { initialize(vertex_shader.get_id(), fragment_shader.get_id()); }
        
        Shader_Program (const vector < string > & source_code_vertex, const vector < string > & source_code_fragment) :
                                          vertex_shader(  source_code_vertex),
                                        fragment_shader(source_code_fragment)
            { initialize(vertex_shader.get_id(), fragment_shader.get_id()); }
        
        void use() const
        {
            glUseProgram (program_id);
        }
        
        GLuint get_id() const
        {
            return program_id;
        }
        
        ~Shader_Program()
        {
            glDeleteProgram (program_id);
        }
        
        GLuint get_uniform_location ( string uniform_name )
        {
            return glGetUniformLocation (program_id, uniform_name.c_str());
        }
        
    private:
        void initialize (GLuint vertex_shader_id, GLuint fragment_shader_id);
        
        void show_linkage_error ();
    };
}
