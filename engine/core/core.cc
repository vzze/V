#include "core.h"
#include "GLFW/glfw3.h"
#include "imgui.h"

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
        current_skybox = skyboxes.at(index);
        skybox_slider = index + 1;
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

    if(mode == v::MODE::VDEBUG)
        delete props;
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

    ImGuiIO & io = Init(renderer->Window->window, "#version 330");

    color[0] = std::get<0>(backgroundColor);
    color[1] = std::get<1>(backgroundColor);
    color[2] = std::get<2>(backgroundColor);

    props = new ObjectProps[objects.size()];

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
            Prep();

            MainWindow();
            GammaWindow(); 
            SkyboxWindow();
            BackgroundColorWindow();
            ObjectWindow();

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
                if(!io.WantCaptureMouse && !io.WantCaptureKeyboard)
                    renderer->camera->Inputs(renderer->Window->window);
                camPrevTime = currTime;
            }

            renderer->camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

            if(current_skybox)
                current_skybox->Draw(*renderer->skyboxProgram, settings, *renderer->camera); 

            if(!Draw())
                break;
            
            renderer->framebuffer->Draw(*renderer->framebufferProgram, settings.width, settings.height);

            Render();
 
            renderer->Window->SwapBuffers();
        }

        glfwPollEvents();
    }

    Terminate();
}

ImGuiIO & v::engine::Core::Init(GLFWwindow * window, const char * version) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(version);
    return ImGui::GetIO();
}

void v::engine::Core::Prep() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void v::engine::Core::MainWindow() {
    ImGui::Begin("V"); 
    ImGui::Text("V Engine");
    ImGui::Text("Width: %u", settings.width); 
    ImGui::Text("Height: %u", settings.height); 
    ImGui::End();
}

void v::engine::Core::GammaWindow() {
    ImGui::Begin("Gamma");
    ImGui::SliderFloat("Gamma", &renderer->gamma, 0.45F, 2.5F);
    ImGui::End();

    renderer->SetGammaCorrection(renderer->gamma);
}

void v::engine::Core::SkyboxWindow() {
    ImGui::Begin("Skybox");
    ImGui::Checkbox("Draw Skybox", &skybox_draw);
    if(skybox_draw)
        ImGui::SliderInt("Sky Select", &skybox_slider, 1, skyboxes.size());
    ImGui::End();

    if(!skybox_draw)
        setCurrentSkybox(-1);
    else
        setCurrentSkybox(skybox_slider - 1);
}

void v::engine::Core::BackgroundColorWindow() {
    ImGui::Begin("Background Color");
    ImGui::ColorEdit3("Bg. Col.", color);
    ImGui::End();

    SetBackgroundColor(color[0], color[1], color[2]);
}

void v::engine::Core::ObjectWindow() {
    if(!objects.empty()) {
        ImGui::Begin("Objects");
        ImGui::SliderInt("Obj. Select", &object_slider, 1, objects.size());

        ImGui::Checkbox("Draw", &props[object_slider - 1].draw);
        ImGui::Checkbox("Draw Outline", &props[object_slider - 1].DrawOutline);
        ImGui::Checkbox("Draw Normals", &props[object_slider - 1].DrawNormals);

        ImGui::SliderFloat("Out. Thickness", &props[object_slider - 1].thickness, 0.0F, 1.0F);
        ImGui::ColorEdit3("Out. Color", props[object_slider - 1].colors); 
        ImGui::SliderFloat("Out. Alpha", &props[object_slider - 1].alpha, 0.0F, 1.0F);

        ImGui::SliderFloat("X Axis", &props[object_slider - 1].X_rot, 0.0F, 360.0F);
        ImGui::SliderFloat("Y Axis", &props[object_slider - 1].Y_rot, 0.0F, 360.0F);
        ImGui::SliderFloat("Z Axis", &props[object_slider - 1].Z_rot, 0.0F, 360.0F);

        ImGui::SliderFloat("X Scale", &props[object_slider - 1].X_scale, 0.0F, 50.0F);
        ImGui::SliderFloat("Y Scale", &props[object_slider - 1].Y_scale, 0.0F, 50.0F);
        ImGui::SliderFloat("Z Scale", &props[object_slider - 1].Z_scale, 0.0F, 50.0F);

        ImGui::SliderFloat("X Pos", &props[object_slider - 1].X_translate, props[object_slider - 1].X_translate - 0.5F, props[object_slider - 1].X_translate + 0.5F);
        ImGui::SliderFloat("Y Pos", &props[object_slider - 1].Y_translate, props[object_slider - 1].Y_translate - 0.5F, props[object_slider - 1].Y_translate + 0.5F);
        ImGui::SliderFloat("Z Pos", &props[object_slider - 1].Z_translate, props[object_slider - 1].Z_translate - 0.5F, props[object_slider - 1].Z_translate + 0.5F);

        ImGui::End();
    }
}

void v::engine::Core::DebugTickrate() {
    for(unsigned int i = 0; i < objects.size(); i++) {

        auto X = glm::angleAxis(glm::radians(props[i].X_rot), v::util::Xaxis);
        auto Y = glm::angleAxis(glm::radians(props[i].Y_rot), v::util::Yaxis);
        auto Z = glm::angleAxis(glm::radians(props[i].Z_rot), v::util::Zaxis);

        objects[i]->SetRotation(glm::normalize(X * Y * Z));

        objects[i]->MoveTo(props[i].X_translate, props[i].Y_translate, props[i].Z_translate);

        objects[i]->Scale(props[i].X_scale, props[i].Y_scale, props[i].Z_scale);
    }
}

void v::engine::Core::DebugDraw() {
    for(unsigned int i = 0; i < objects.size(); i++) {
        if(props[i].draw) {
            if(props[i].DrawOutline) {
                std::tuple<float, float, float> rgb = { props[i].colors[0], props[i].colors[1], props[i].colors[2] };
                rgb = v::util::gamma_corrected_rgb(rgb, renderer->gamma);
                objects[i]->DrawWithOutline(*renderer->shaderProgram, *renderer->stencilProgram, *renderer->camera, rgb, props[i].alpha, props[i].thickness); 
            } else
                objects[i]->Draw(*renderer->shaderProgram, *renderer->camera);

            if(props[i].DrawNormals)
                objects[i]->Draw(*renderer->normalsProgram, *renderer->camera);    
        } 
    }
}

void v::engine::Core::Render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void v::engine::Core::Terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
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
