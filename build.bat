@echo off

call cmd /C "g++ -IV/dependencies/json -IV/dependencies/glm -IV/dependencies/stb/include -IV/dependencies/glad/include -IV/dependencies/glfw/include -LV/dependencies/glfw/lib-mingw-w64 -IV/include ./V/dependencies/glad/src/glad.c ./V/renderer/shaders/shaderClass.cc ./V/renderer/vertex_buffer/vertexBuffer.cc ./V/renderer/EBO/EBO.cc ./V/renderer/vertex_array/vertexArray.cc ./V/renderer/Texture/Texture.cc ./V/renderer/Camera/Camera.cc ./V/renderer/Mesh/Mesh.cc ./V/renderer/Model/Model.cc ./V/Util/Util.cc main.cc -lpthread -lglfw3 -lopengl32 -lgdi32 -o main"

if errorlevel 0 ( echo Successful ) else ( echo error )