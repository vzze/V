#include "Window.h"
#include "GLFW/glfw3.h"

v::engine::Window::Window(v::engine::EngineSettings & settings) {
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
        v::util::log("Failed to create GLFW Window");
        
        failed = true;

        glfwTerminate();

        return;
    }

    glfwMakeContextCurrent(window);
}

bool v::engine::Window::isFullscreen() {
    return glfwGetWindowMonitor(window) != nullptr;
}

void v::engine::Window::SetFullscreen(bool fullscreen) {
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

void v::engine::Window::SetFramebufferSizeCallback(GLFWframebuffersizefun fn) {
    glfwSetFramebufferSizeCallback(window, fn);
}

void v::engine::Window::SetKeyCallback(GLFWkeyfun fn) {
    glfwSetKeyCallback(window, fn);
}

bool v::engine::Window::ShouldClose() {
    return glfwWindowShouldClose(window);
}

void v::engine::Window::SetTitle(const char * title) {
    glfwSetWindowTitle(window, title);
}

void v::engine::Window::SetIcon(const char * image) {
    int width, height, nrChannels;
    
    stbi_set_flip_vertically_on_load(false);
    
    unsigned char * data = stbi_load(v::util::normalized_path(image).c_str(), &width, &height, &nrChannels, 0);

    if(data) {
        GLFWimage img[1];

        img[0].width = width;
        img[0].height = height;
        img[0].pixels = data;

        glfwSetWindowIcon(window, 1, img);

        v::util::log((std::string("Set Window Icon: ") + image).c_str());

        stbi_image_free(data);
    } else {
        v::util::log((std::string("Failed to set Window Icon: ") + image).c_str());    
    }
}

void v::engine::Window::SwapBuffers() {
    return glfwSwapBuffers(window);
}

v::engine::Window::~Window() {
    if(!failed) {
        glfwDestroyWindow(window);
    
        glfwTerminate();
    }
}
