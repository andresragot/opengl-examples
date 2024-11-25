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

class Shader
{
    GLuint  id;
    bool    compilation_succeeded;
    string  error;
    
protected:
    Shader (const vector< string > & source_code, GLenum type);
    
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

