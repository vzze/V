#include "core.h"

v::Engine::Engine(v::engine::EngineSettings & _settings) {
    settings = _settings;
}

v::Engine::Engine() {
    v::engine::EngineSettings setts;
    settings = setts;
}

v::Engine::~Engine() {
    for(auto model : models)
        delete model;
    for(auto shader : shaders) {
        shader->Delete();
        delete shader;
    }
}

void v::Engine::loadModels(std::string & path) {
    models.push_back(new v::renderer::Model(v::util::normalized_path(path.c_str()).c_str()));
}
void v::Engine::loadModels(std::vector<std::string> & paths) {
    for(auto path : paths)
        models.push_back(new v::renderer::Model(v::util::normalized_path(path.c_str()).c_str()));
}

v::renderer::Camera * v::Engine::camera = nullptr;
v::engine::EngineSettings v::Engine::settings = {};
std::mutex v::Engine::mtx;

void v::Engine::engine_callback(GLFWwindow * window, int width, int height) {
    settings.width = width;
    settings.height = height;

    camera->width = width;
    camera->height = height;
    glViewport(0, 0, width, height);
}

void v::Engine::Run() {
    glfwInit(); 
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(settings.width, settings.height, settings.appName, settings.monitor, settings.share);
    
    if(window == NULL) {
        v::util::log("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, this->engine_callback); 
    
    gladLoadGL();
    
    glViewport(0, 0, settings.width, settings.height);
    
    default_shaderProgram = new v::renderer::Shader(settings.vertexShaderPath, settings.fragmentShaderPath);

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::mat4 lightModel = glm::mat4(1.0f);

    lightModel = glm::translate(lightModel, lightPos);
    
    default_shaderProgram->Activate();
    glUniform4f(glGetUniformLocation(default_shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(default_shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    
    camera = new v::renderer::Camera(settings.width, settings.height, settings.camera_position);

    loadModels(settings.model_paths);

    

    if(!settings.VSYNC)
        glfwSwapInterval(0);
    
    if(Init())
        main_thread();
 
    default_shaderProgram->Delete();

    delete default_shaderProgram;
    delete camera;

    glfwDestroyWindow(window);
    
    glfwTerminate();
    
    return;
}

void v::Engine::main_thread() {
    double prevTime = 0.0;
    double currTime = 0.0;
    double diffTime;

    unsigned int counter = 0;

    double camPrevTime = 0.0;
    double camDiffTime;

    while(!glfwWindowShouldClose(window)) {
        currTime = glfwGetTime();
        diffTime = currTime - prevTime;

        counter++;

        camDiffTime = currTime - camPrevTime;

        if(diffTime >= settings.tickrate) {
            std::string FPS = std::to_string((1.0 / diffTime) * counter);
            std::string MS = std::to_string(((diffTime / counter) * 1000.0));
            std::string Title = std::string(settings.appName) + std::string(" - ") + FPS + "FPS | " + MS + " ms";
            
            if(!Tickrate(diffTime))
                break;

            glfwSetWindowTitle(window, Title.c_str());
            prevTime = currTime;
            counter = 0;
        }

        glClearColor(0.10F, 0.10F, 0.10F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        if(camDiffTime >= 1.0 / 1000.0)
            camera->Inputs(window);
        
        camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

        if(!Draw())
            break;

        glfwSwapBuffers(window);
        
        glfwPollEvents();
    }
}
