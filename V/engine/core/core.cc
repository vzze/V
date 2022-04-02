#include "core.h"
#include "GLFW/glfw3.h"

v::engine::Core::Core(v::engine::EngineSettings & _settings) {
    settings = _settings;
}

v::engine::Core::Core() {
    v::engine::EngineSettings setts;
    settings = setts;
}

void v::engine::Core::loadModels(std::string & path) {
    objects.push_back(new v::engine::Object(v::util::normalized_path(path.c_str()).c_str()));
}
void v::engine::Core::loadModels(std::vector<std::string> & paths) {
    for(auto path : paths)
        objects.push_back(new v::engine::Object(v::util::normalized_path(path.c_str()).c_str()));
}

void v::engine::Core::loadSkyboxes(Skybox_container & cont) {
    skyboxes.push_back(new v::renderer::Skybox());
    skyboxes[skyboxes.size() - 1]->Bind(cont.arr);
}

void v::engine::Core::loadSkyboxes(std::vector<Skybox_container> & conts) {
    for(auto cont : conts) {
        skyboxes.push_back(new v::renderer::Skybox());
        skyboxes[skyboxes.size() - 1]->Bind(cont.arr);
    }
}

void v::engine::Core::setCurrentSkybox(unsigned int index) {
    try {
        current_skybox = skyboxes[index];
    } catch(...) {
        current_skybox = nullptr;
    }
}

void v::engine::Core::window_callback(GLFWwindow * window, int width, int height) {
    settings.width = width;
    settings.height = height;

    camera->width = width;
    camera->height = height;

    framebuffer->Delete();
    delete framebuffer;
    framebuffer = new v::renderer::Framebuffer();
    framebuffer->Bind(width, height);
    
    framebufferProgram->Uniform1f("offset_x", 1.0F / (float)(width));
    framebufferProgram->Uniform1f("offset_y", 1.0F / (float)(height));

    glViewport(0, 0, width, height);
}

void v::engine::Core::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        Window->SetFullscreen(!Window->isFullscreen());
        settings.fullscreen = Window->isFullscreen();
    }
}

void v::engine::Core::Run() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
   
    Window = new v::engine::Window(settings);

    if(Window->failed)
        return;

    Window->SetFramebufferSizeCallback(this->window_callback);
    Window->SetKeyCallback(this->key_callback);

    Window->SetIcon("\\V\\icon.png");

    gladLoadGL();

    glViewport(0, 0, settings.width, settings.height);

    shaderProgram = new v::renderer::Shader(settings.vertexShaderPath.c_str(), settings.fragmentShaderPath.c_str());

    std::string frag_path = v::util::normalized_path("\\V\\renderer\\shaders\\framebuffer.frag");
    std::string vert_path = v::util::normalized_path("\\V\\renderer\\shaders\\framebuffer.vert");

    framebufferProgram = new v::renderer::Shader(vert_path.c_str(), frag_path.c_str());

    frag_path.clear();
    vert_path.clear();

    frag_path = v::util::normalized_path("\\V\\renderer\\shaders\\skybox.frag");
    vert_path = v::util::normalized_path("\\V\\renderer\\shaders\\skybox.vert");

    skyboxProgram = new v::renderer::Shader(vert_path.c_str(), frag_path.c_str());

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

    loadModels(settings.model_paths);

    loadSkyboxes(settings.skybox_paths);

    framebuffer = new v::renderer::Framebuffer();

    framebuffer->Bind(settings.width, settings.height);

    if(!settings.VSYNC)
        glfwSwapInterval(0);
    
    if(Init())
        main_thread();
}

v::engine::Core::~Core() {
    for(auto object : objects)
        delete object;
    
    for(auto shader : shaders) {
        shader->Delete();
        delete shader;
    }

    for(auto skybox : skyboxes)
        delete skybox;

    shaderProgram->Delete();
    framebufferProgram->Delete();
    skyboxProgram->Delete();

    delete shaderProgram;
    delete framebufferProgram;
    delete skyboxProgram;

    framebuffer->Delete();

    delete framebuffer;
    delete camera;

    delete Window;
}

void v::engine::Core::main_thread() {
    double currTime;

    double prevTime = 0.0;
    double diffTime;

    unsigned int counter = 0;

    double camPrevTime = 0.0;
    double camDiffTime;

    double tickratePrevTime = 0.0;
    double tickrateDiffTime;

    while(!Window->ShouldClose()) {
        currTime = glfwGetTime();

        diffTime = currTime - prevTime;

        camDiffTime = currTime - camPrevTime;

        tickrateDiffTime = currTime - tickratePrevTime;

        counter++;

        if(diffTime >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / diffTime) * counter);
            std::string MS = std::to_string(((diffTime / counter) * 1000.0));
            std::string Title = settings.appName + " - " + FPS + "FPS | " + MS + " ms";

            Window->SetTitle(Title.c_str()); 
            prevTime = currTime;
            counter = 0;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->FBO);

        glClearColor(0.10F, 0.10F, 0.10F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);

        if(tickrateDiffTime >= settings.tickrate) {
            if(!Tickrate(diffTime))
                break;
            tickratePrevTime = currTime;
        }

        if(camDiffTime >= 1.0 / 1000.0) {
            camera->Inputs(Window->window);
            camPrevTime = currTime;
        }

        camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

        if(!Draw())
            break;

        if(current_skybox)
            current_skybox->Draw(*skyboxProgram, settings, *camera); 

        framebuffer->Draw(*framebufferProgram);

        Window->SwapBuffers();

        glfwPollEvents();
    }
}

// statics
v::renderer::Camera * v::engine::Core::camera = nullptr;
v::engine::EngineSettings v::engine::Core::settings = {};
v::renderer::Framebuffer * v::engine::Core::framebuffer = nullptr;
v::renderer::Shader * v::engine::Core::framebufferProgram = nullptr;
v::engine::Window * v::engine::Core::Window = nullptr;
