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

void v::engine::Core::loadShaders(v::engine::Shader_container & cont) {
    if(cont.arr[2].empty())
        shaders.push_back(new v::renderer::Shader(cont.arr[0].c_str(), cont.arr[1].c_str(), nullptr));
    else 
        shaders.push_back(new v::renderer::Shader(cont.arr[0].c_str(), cont.arr[1].c_str(), cont.arr[2].c_str()));
}

void v::engine::Core::loadShaders(std::vector<v::engine::Shader_container> & conts) {
    for(auto cont : conts)
        if(cont.arr[2].empty())
            shaders.push_back(new v::renderer::Shader(cont.arr[0].c_str(), cont.arr[1].c_str(), nullptr));
        else 
            shaders.push_back(new v::renderer::Shader(cont.arr[0].c_str(), cont.arr[1].c_str(), cont.arr[2].c_str()));
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

    renderer->camera->width = width;
    renderer->camera->height = height;

    renderer->framebuffer->Delete();
    delete renderer->framebuffer;
    renderer->framebuffer = new v::renderer::Framebuffer();
    renderer->framebuffer->Bind(width, height, renderer->MSAAsamples);
    
    renderer->framebufferProgram->Uniform1f("offset_x", 1.0F / (float)(width));
    renderer->framebufferProgram->Uniform1f("offset_y", 1.0F / (float)(height));

    glViewport(0, 0, width, height);
}

void v::engine::Core::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        renderer->Window->SetFullscreen(!renderer->Window->isFullscreen());
        settings.fullscreen = renderer->Window->isFullscreen();
    }
}

void v::engine::Core::focus_callback(GLFWwindow * window, int focused) {
    renderer->Window->FOCUSED = (bool)(focused);
}

void v::engine::Core::SetMode(v::MODE mode) {
    v::engine::Core::mode = mode;
}

void v::engine::Core::Run() {
    renderer = new v::renderer::Core(settings, { this->window_callback, this->key_callback, this->focus_callback});

    loadModels(settings.model_paths);

    loadSkyboxes(settings.skybox_paths);

    loadShaders(settings.shader_paths);
    
    if(!Init())
        return;

    switch(mode) {
        case v::MODE::VRELEASE:
            release_thread();
        break;
        case v::MODE::VDEBUG:
            debug_thread();
        break;
    }
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

    delete renderer;
}

void v::engine::Core::release_thread() {
    double currTime;

    double camPrevTime = 0.0;
    double camDiffTime;

    double tickratePrevTime = 0.0;
    double tickrateDiffTime;

    while(!renderer->Window->ShouldClose()) {
        currTime = glfwGetTime();

        camDiffTime = currTime - camPrevTime;

        tickrateDiffTime = currTime - tickratePrevTime;

        if(tickrateDiffTime >= settings.tickrate) {
            if(!Tickrate(tickrateDiffTime))
                break;
            tickratePrevTime = currTime;
        }

        if(renderer->Window->FOCUSED) {

            glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebuffer->FBO);

            glClearColor(std::get<0>(backgroundColor), std::get<1>(backgroundColor), std::get<2>(backgroundColor), 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);

            if(camDiffTime >= 1.0 / 1000.0) {
                renderer->camera->Inputs(renderer->Window->window);
                camPrevTime = currTime;
            }

            renderer->camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

            if(current_skybox)
                current_skybox->Draw(*renderer->skyboxProgram, settings, *renderer->camera); 

            if(!Draw())
                break;
            
            renderer->framebuffer->Draw(*renderer->framebufferProgram, settings.width, settings.height);
 
            renderer->Window->SwapBuffers();
        }

        glfwPollEvents();
    }
}

void v::engine::Core::debug_thread() {
    double currTime;

    double prevTime = 0.0;
    double diffTime;

    unsigned int counter = 0;

    double camPrevTime = 0.0;
    double camDiffTime;

    double tickratePrevTime = 0.0;
    double tickrateDiffTime;

    while(!renderer->Window->ShouldClose()) {
        currTime = glfwGetTime();

        diffTime = currTime - prevTime;

        camDiffTime = currTime - camPrevTime;

        tickrateDiffTime = currTime - tickratePrevTime;

        counter++;
        if(tickrateDiffTime >= settings.tickrate) {
            if(!Tickrate(tickrateDiffTime))
                break;
            tickratePrevTime = currTime;
        }

        if(renderer->Window->FOCUSED) {
            if(diffTime >= 1.0 / 30.0) {
                std::string FPS = std::to_string((1.0 / diffTime) * counter);
                std::string MS = std::to_string(((diffTime / counter) * 1000.0));
                std::string Title = settings.appName + " - " + FPS + "FPS | " + MS + " ms";

                renderer->Window->SetTitle(Title.c_str()); 
                prevTime = currTime;
                counter = 0;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, renderer->framebuffer->FBO);

            glClearColor(std::get<0>(backgroundColor), std::get<1>(backgroundColor), std::get<2>(backgroundColor), 1.0F);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glEnable(GL_DEPTH_TEST);

            if(camDiffTime >= 1.0 / 1000.0) {
                renderer->camera->Inputs(renderer->Window->window);
                camPrevTime = currTime;
            }

            renderer->camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

            if(current_skybox)
                current_skybox->Draw(*renderer->skyboxProgram, settings, *renderer->camera); 

            if(!Draw())
                break;
            
            renderer->framebuffer->Draw(*renderer->framebufferProgram, settings.width, settings.height);
 
            renderer->Window->SwapBuffers();
        }

        glfwPollEvents();
    }
}

void v::engine::Core::SetBackgroundColor(long long hex) {
    auto l = v::util::hex_to_rgb(hex);
    backgroundColor = v::util::normalized_rgb(l);
}

void v::engine::Core::SetBackgroundColor(float r, float g, float b) {
    backgroundColor = std::tuple<float, float, float>(r, g, b);

    std::get<0>(backgroundColor) = powf(std::get<0>(backgroundColor), renderer->gamma);
    std::get<1>(backgroundColor) = powf(std::get<1>(backgroundColor), renderer->gamma);
    std::get<2>(backgroundColor) = powf(std::get<2>(backgroundColor), renderer->gamma);
}
void v::engine::Core::SetBackgroundColor(std::tuple<float, float, float> rgb) {
    backgroundColor = rgb;

    std::get<0>(backgroundColor) = powf(std::get<0>(backgroundColor), renderer->gamma);
    std::get<1>(backgroundColor) = powf(std::get<1>(backgroundColor), renderer->gamma);
    std::get<2>(backgroundColor) = powf(std::get<2>(backgroundColor), renderer->gamma);
}
void v::engine::Core::SetBackgroundColor(short int r, short int g, short int b) {
    backgroundColor = v::util::normalized_rgb(std::tuple<short, short, short>(r, g, b));

    std::get<0>(backgroundColor) = powf(std::get<0>(backgroundColor), renderer->gamma);
    std::get<1>(backgroundColor) = powf(std::get<1>(backgroundColor), renderer->gamma);
    std::get<2>(backgroundColor) = powf(std::get<2>(backgroundColor), renderer->gamma);
} 

void v::engine::Core::SetBackgroundColor(std::tuple<short int, short int, short int> rgb) {
    backgroundColor = v::util::normalized_rgb(rgb);

    std::get<0>(backgroundColor) = powf(std::get<0>(backgroundColor), renderer->gamma);
    std::get<1>(backgroundColor) = powf(std::get<1>(backgroundColor), renderer->gamma);
    std::get<2>(backgroundColor) = powf(std::get<2>(backgroundColor), renderer->gamma);
}

v::renderer::Core * v::engine::Core::renderer = nullptr;
v::engine::EngineSettings v::engine::Core::settings = {};
