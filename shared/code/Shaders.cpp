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

namespace udit
{
    Shader :: Shader (const vector < string > & source_code, GLenum type)
    {
        GLint succeeded = GL_FALSE;
        
        id = glCreateShader (type);
        
        // const char * shader_code[] = { source_code.front().c_str() };
        // const GLint  shader_size [] = { (GLint) source_code.front().size()};
        
        vector < const char * > shader_code;
        vector < GLint > shader_size;
        
        shader_code.reserve (source_code.size());
        shader_size.reserve (source_code.size());
        
        for (const auto &str : source_code)
        {
            shader_code.push_back(str.c_str());
            shader_size.push_back((GLint) str.size());
        }
        
        for (size_t i = 0; i < shader_code.size(); ++i)
        {
            std::cout << "Shader source[" << i << "]: " << shader_code[i] << std::endl;
            std::cout << "Shader size[" << i << "]: " << shader_size[i] << std::endl;
        }
        
        glShaderSource  (id, (GLsizei) shader_code.size(), shader_code.data(), shader_size.data());
        
        glCompileShader (id);
        
        glGetShaderiv (id, GL_COMPILE_STATUS, &succeeded);
        if (!succeeded) show_compilation_error ();
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

    Shader_Program::Shader_Program (const Vertex_Shader & vertex_shader, const Fragment_Shader & fragment_shader)
    {
        GLint succeded = GL_FALSE;
        
        program_id = glCreateProgram ();
        
        glAttachShader (program_id, vertex_shader.get_id());
        glAttachShader (program_id, fragment_shader.get_id());
        glLinkProgram  (program_id);
        
        glGetProgramiv (program_id, GL_LINK_STATUS, &succeded);
        if (!succeded)  show_linkage_error();
    }

    void Shader_Program::show_linkage_error()
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
