$deps = "-Idependencies/imgui -Idependencies/json -Idependencies/glm -Idependencies/stb/include -Idependencies/glad/include -Idependencies/glfw/include -Ldependencies/glfw/lib-mingw-w64 -Iinclude"

$depfiles = "./dependencies/glad/src/glad.c ./dependencies/imgui/imgui.cpp ./dependencies/imgui/imgui_demo.cpp ./dependencies/imgui/imgui_draw.cpp ./dependencies/imgui/imgui_impl_glfw.cpp ./dependencies/imgui/imgui_impl_opengl3.cpp ./dependencies/imgui/imgui_tables.cpp ./dependencies/imgui/imgui_widgets.cpp"

$enginefiles = "./renderer/core/core.cc ./renderer/stb.cc ./renderer/Skybox/Skybox.cc ./engine/Window/Window.cc ./renderer/Framebuffer/Framebuffer.cc ./renderer/shaders/Shader.cc ./renderer/vertex_buffer/vertexBuffer.cc ./renderer/EBO/EBO.cc ./renderer/vertex_array/vertexArray.cc ./renderer/Texture/Texture.cc ./renderer/Camera/Camera.cc ./renderer/Mesh/Mesh.cc ./renderer/Model/Model.cc ./engine/core/core.cc ./engine/EngineSettings/engineSettings.cc ./engine/Object/Object.cc ./Util/Util.cc main.cc"

$flags = "--verbose -std=c++17 -lpthread -lglfw3 -lopengl32 -lgdi32 -mwindows -O3 -o main"

$arguments = ""

for ($i = 0; $i -lt $args.Count; $i++) {
    $arguments += $args[$i] + " "
}

$command = $deps + " " + $depfiles + " " + $enginefiles + " " + $flags + " " + $arguments

Start-Process g++ $command -Wait
