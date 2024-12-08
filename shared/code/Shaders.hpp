//
//  Shaders.hpp
//  OpenGL Example-ColoredCube
//
//  Created by Andrés Ragot on 25/11/24.
//

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
        Shader (const Shader &) = delete;
        Shader (Shader && ) = delete;
        
        Shader & operator = (const Shader &) = delete;
        Shader & operator = (Shader &&) = delete;
        
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
        Vertex_Shader (const vector < string > & source_code) : Shader (source_code, GL_VERTEX_SHADER)
        {
        }
    };

    class Fragment_Shader : public Shader
    {
    public:
        Fragment_Shader (const vector <string > & source_code) : Shader (source_code, GL_FRAGMENT_SHADER)
        {
        }
    };

    class Shader_Program
    {
    private:
        GLuint program_id;
    public:
        Shader_Program (const Vertex_Shader & vertex_shader, const Fragment_Shader & fragment_shader);
        
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
        void show_linkage_error ();
    };
}
