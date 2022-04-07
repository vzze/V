#include "core.h"
#include "GLFW/glfw3.h"

v::renderer::Core::Core(v::engine::EngineSettings & settings, Callback_Functions funcs) {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   
    Window = new v::engine::Window(settings);

    if(Window->failed)
        return;

    Window->SetFramebufferSizeCallback(funcs.size_callback);
    Window->SetKeyCallback(funcs.key_callback);
    Window->SetWindowFocusCallback(funcs.focus_callback);

    Window->SetIcon("\\icon.png");

    gladLoadGL();

    glViewport(0, 0, settings.width, settings.height);

    shaderProgram = new v::renderer::Shader(settings.vertexShaderPath.c_str(), settings.fragmentShaderPath.c_str(), settings.geometryShaderPath.c_str());

    stencilProgram = new v::renderer::Shader(v::util::default_stencil_vertex_path.c_str(), v::util::default_stencil_fragment_path.c_str());

    framebufferProgram = new v::renderer::Shader(v::util::default_framebuffer_vertex_path.c_str(), v::util::default_framebuffer_fragment_path.c_str());

    skyboxProgram = new v::renderer::Shader(v::util::default_skybox_vertex_path.c_str(), v::util::default_skybox_fragment_path.c_str());

    normalsProgram = new v::renderer::Shader(settings.vertexShaderPath.c_str(), v::util::default_normal_fragment_path.c_str(), v::util::default_normal_geom_path.c_str());

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);

    lightModel = glm::translate(lightModel, lightPos);
 
    shaderProgram->Activate();

    shaderProgram->Uniform4f("lightColor", lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram->Uniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
   
    skyboxProgram->Activate();
    skyboxProgram->Uniform1i("skybox", 0);

    framebufferProgram->Activate();

    framebufferProgram->Uniform1i("screenTexture", 0);

    framebufferProgram->Uniform1f("offset_x", 1.0F / (float)(settings.width));
    framebufferProgram->Uniform1f("offset_y", 1.0F / (float)(settings.height));

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_MULTISAMPLE);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);

    camera = new v::renderer::Camera(settings.width, settings.height, settings.camera_position);

    framebuffer = new v::renderer::Framebuffer();
    
    framebuffer->Bind(settings.width, settings.height);

    if(!settings.VSYNC)
        glfwSwapInterval(0);
}

v::renderer::Core::~Core() {
    shaderProgram->Delete();
    stencilProgram->Delete();
    framebufferProgram->Delete();
    skyboxProgram->Delete();
    normalsProgram->Delete();

    delete shaderProgram;
    delete stencilProgram;
    delete framebufferProgram;
    delete skyboxProgram;
    delete normalsProgram;

    framebuffer->Delete();

    delete framebuffer;
    delete camera;

    delete Window;
}

void v::renderer::Core::MSAA(unsigned int opt) {
    MSAAsamples = opt;

    if(framebuffer) {
        framebuffer->Delete();
        delete framebuffer;
    }

    int width, height;

    glfwGetWindowSize(Window->window, &width, &height);

    framebuffer = new v::renderer::Framebuffer();

    framebuffer->Bind(width, height, MSAAsamples);
}

void v::renderer::Core::SetNormalLength(float length) {
    normalsProgram->Uniform1f("length", length);
}

void v::renderer::Core::SetNormalcolor(float r, float g, float b) {
    normalsProgram->Uniform3f("Color", r, g, b);
}

void v::renderer::Core::SetNormalcolor(std::tuple<float, float, float> rgb) {
    normalsProgram->Uniform3f("Color", std::get<0>(rgb), std::get<1>(rgb), std::get<2>(rgb));
}

void v::renderer::Core::SetNormalColor(short int r, short int g, short int b) {
    SetNormalColor(v::util::normalized_color(r), v::util::normalized_color(g), v::util::normalized_color(b));
}

void v::renderer::Core::SetNormalColor(std::tuple<short int, short int, short int> rgb) {
    SetNormalColor(v::util::normalized_rgb(rgb));
}
