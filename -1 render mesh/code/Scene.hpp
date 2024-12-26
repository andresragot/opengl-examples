// author: Andrés Ragot - github.com/andresragot
// Copyright (c)
// 03/12/2024

#pragma once

#include "Cube.hpp"
#include "Mesh.hpp"
#include "Shaders.hpp"
#include <string>

namespace udit
{
    class Scene
    {
    private:
        static const std::string   vertex_shader_code;
        static const std::string fragment_shader_code;
        
        GLint model_view_matrix_id;
        GLint projection_matrix_id;
        
        // Cube cube;
        Plane plane;
        Cone  cone;
        Cylinder cylinder;

        Shader_Program   shader_program;
        float angle;
    public:
        Scene (unsigned width, unsigned height);
        
        void update ();
        void render ();
        void resize (unsigned width, unsigned height);
    };
}

