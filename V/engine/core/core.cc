#include "core.h"
#include "GLFW/glfw3.h"

v::engine::Core::Core(v::engine::EngineSettings & _settings) {
    settings = _settings;
}

v::engine::Core::Core() {
    v::engine::EngineSettings setts;
    settings = setts;
}

v::engine::Core::~Core() {
    for(auto object : objects)
        delete object;
    for(auto shader : shaders) {
        shader->Delete();
        delete shader;
    }
}

void v::engine::Core::loadModels(std::string & path) {
    objects.push_back(new v::engine::Object(v::util::normalized_path(path.c_str()).c_str()));
}
void v::engine::Core::loadModels(std::vector<std::string> & paths) {
    for(auto path : paths)
        objects.push_back(new v::engine::Object(v::util::normalized_path(path.c_str()).c_str()));
}

v::renderer::Camera * v::engine::Core::camera = nullptr;
v::engine::EngineSettings v::engine::Core::settings = {};

GLFWwindow * v::engine::Core::window = nullptr;
GLFWmonitor * v::engine::Core::monitor = nullptr;
GLFWwindow * v::engine::Core::share = nullptr;

v::renderer::Framebuffer * v::engine::Core::framebuffer = nullptr;
v::renderer::Shader * v::engine::Core::default_framebufferProgram = nullptr;

void v::engine::Core::window_callback(GLFWwindow * window, int width, int height) {
    settings.width = width;
    settings.height = height;

    camera->width = width;
    camera->height = height;

    framebuffer->Delete();
    delete framebuffer;
    framebuffer = new v::renderer::Framebuffer();
    framebuffer->Bind(width, height);

    glUniform1f(glGetUniformLocation(default_framebufferProgram->ID, "offset_x"), 1.0F / (float)(width));
    glUniform1f(glGetUniformLocation(default_framebufferProgram->ID, "offset_y"), 1.0F / (float)(height));

    glViewport(0, 0, width, height);
}

void v::engine::Core::key_callback(GLFWwindow * window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_F11 && action == GLFW_PRESS) {
        SetFullscreen(!isFullscreen());
        settings.fullscreen = isFullscreen();
    }
}

bool v::engine::Core::isFullscreen() {
    return glfwGetWindowMonitor(window) != nullptr;
}

void v::engine::Core::SetFullscreen(bool fullscreen) {
    if(isFullscreen() == fullscreen)
        return;

    if(fullscreen) {
        const auto mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
    } else {
        const auto mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(window, nullptr, 50, 50, (int)((float)(mode->width) * 3.0F / 4.0F), (int)((float)(mode->height) * 3.0F / 4.0F), mode->refreshRate);
    }

}

void v::engine::Core::Run() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    monitor = glfwGetPrimaryMonitor();

    const auto mode = glfwGetVideoMode(monitor);

    if(!settings.fullscreen) {
        window = glfwCreateWindow(settings.width, settings.height, settings.appName.c_str(), nullptr, share);
    
        int xpos, ypos; glfwGetWindowPos(window, &xpos, &ypos);

        glfwSetWindowMonitor(window, nullptr, xpos, ypos, settings.width, settings.height, mode->refreshRate);
    } else {
        window = glfwCreateWindow(mode->width, mode->height, settings.appName.c_str(), monitor, share);

        int xpos, ypos; glfwGetWindowPos(window, &xpos, &ypos);
        
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

        settings.width = mode->width;
        settings.height = mode->height;
    }

    if(window == NULL) {
        v::util::log("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, this->window_callback);
    glfwSetKeyCallback(window, this->key_callback);

    gladLoadGL();

    glViewport(0, 0, settings.width, settings.height);

    default_shaderProgram = new v::renderer::Shader(settings.vertexShaderPath.c_str(), settings.fragmentShaderPath.c_str());

    std::string frag_path = v::util::normalized_path("\\V\\renderer\\shaders\\framebuffer.frag");
    std::string vert_path = v::util::normalized_path("\\V\\renderer\\shaders\\framebuffer.vert");

    default_framebufferProgram = new v::renderer::Shader(vert_path.c_str(), frag_path.c_str());

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);

    lightModel = glm::translate(lightModel, lightPos);
 
    default_shaderProgram->Activate();
    glUniform4f(glGetUniformLocation(default_shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(default_shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    default_framebufferProgram->Activate();
    glUniform1i(glGetUniformLocation(default_framebufferProgram->ID, "screenTexture"), 0);

    glUniform1f(glGetUniformLocation(default_framebufferProgram->ID, "offset_x"), 1.0F / (float)(settings.width));
    glUniform1f(glGetUniformLocation(default_framebufferProgram->ID, "offset_y"), 1.0F / (float)(settings.height));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    
    camera = new v::renderer::Camera(settings.width, settings.height, settings.camera_position);

    loadModels(settings.model_paths);

    framebuffer = new v::renderer::Framebuffer();

    framebuffer->Bind(settings.width, settings.height);

    if(!settings.VSYNC)
        glfwSwapInterval(0);
    
    if(Init())
        main_thread();
 
    default_shaderProgram->Delete();
    default_framebufferProgram->Delete();

    delete default_shaderProgram;
    delete default_framebufferProgram;

    framebuffer->Delete();

    delete framebuffer;
    delete camera;

    glfwDestroyWindow(window);
    
    glfwTerminate();
    
    return;
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

    while(!glfwWindowShouldClose(window)) {
        currTime = glfwGetTime();

        diffTime = currTime - prevTime;

        camDiffTime = currTime - camPrevTime;

        tickrateDiffTime = currTime - tickratePrevTime;

        counter++;

        if(diffTime >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / diffTime) * counter);
            std::string MS = std::to_string(((diffTime / counter) * 1000.0));
            std::string Title = settings.appName + " - " + FPS + "FPS | " + MS + " ms";

            glfwSetWindowTitle(window, Title.c_str());
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
            camera->Inputs(window);
            camPrevTime = currTime;
        }

        camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

        if(!Draw())
            break;

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        default_framebufferProgram->Activate();
        glBindVertexArray(framebuffer->rectVAO);

        glDisable(GL_DEPTH_TEST);

        glBindTexture(GL_TEXTURE_2D, framebuffer->framebufferTexture);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }
}
