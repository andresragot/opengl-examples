//
//  Shaders.cpp
//  OpenGL Example-ColoredCube
//
//  Created by Andrés Ragot on 25/11/24.
//
#include <cassert>
#include "Shaders.hpp"
#include <iostream>
#include <vector>

using namespace std;

Shader :: Shader (const vector < string > & source_code, GLenum type)
{
    GLint succeeded = GL_FALSE;
    
    id = glCreateShader (type);
    
    if (source_code.size() > 1)
    {
        // Preguntar a profesor qué se hace en este caso.
        const char * shader_code [source_code.size()];
        GLint        shader_size [source_code.size()];
        // auto begin = source_code.begin();
        // auto end   = source_code.end();
        // for (; begin != end; ++begin)
        for (size_t i = 0; i < source_code.size(); ++i)
        {
            shader_code[i] = source_code[i].c_str();
            shader_size[i] = (GLint) source_code[i].size();
        }
    }
    else
    {
        const char * shader_code[] = { source_code.front().c_str() };
        const GLint  shader_size [] = { (GLint) source_code.front().size()};
        
        glShaderSource  (id, 1, shader_code, shader_size);
        
        glCompileShader (id);
        
        glGetShaderiv (id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error ();
        
        
        // Este shader luego habrá que attach a algún programa.
    }
}


void Shader :: show_compilation_error()
{
    GLint info_log_length;
    
    glGetShaderiv (id, GL_INFO_LOG_LENGTH, &info_log_length);
    
    error.resize (info_log_length);
    
    glGetShaderInfoLog (id, info_log_length, NULL, &error.front());
    
    cerr << error.c_str() << endl;
    
    assert(false);
}
