#include "core.h"

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
    framebufferProgram->Delete();
    skyboxProgram->Delete();
    normalsProgram->Delete();

    delete shaderProgram;
    delete framebufferProgram;
    delete skyboxProgram;
    delete normalsProgram;

    framebuffer->Delete();

    delete framebuffer;
    delete camera;

    delete Window;
}
