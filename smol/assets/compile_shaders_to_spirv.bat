@echo off
setlocal

glslangValidator.exe --target-env opengl --client opengl100 --glsl-version 460 --entry-point main -Os -o tri_v.hpp --variable-name vertex_shader_bin triangle_without_vbo.vert
glslangValidator.exe --target-env opengl --client opengl100 --glsl-version 460 --entry-point main -Os -o tri_f.hpp --variable-name fragment_shader_bin triangle_without_vbo.frag

endlocal