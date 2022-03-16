#include "engine.h"

v::Engine::Engine(EngineSettings & _settings) {
    settings = _settings;
}

v::Engine::~Engine() {
    for(auto model : models)
        delete model;
}

void v::Engine::loadModels(std::string & path) {
    models.push_back(new v::renderer::Model(v::util::normalized_path(path.c_str()).c_str()));
}
void v::Engine::loadModels(std::vector<std::string> & paths) {
    for(auto path : paths)
        models.push_back(new v::renderer::Model(v::util::normalized_path(path.c_str()).c_str()));
}

v::EngineSettings v::Engine::settings = {};

void v::Engine::engine_callback(GLFWwindow * window, int width, int height) {
    settings.width = width;
    settings.height = height;
    glViewport(0, 0, width, height);
}

void v::Engine::Run() {
    glfwInit(); 
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow * window = glfwCreateWindow(settings.width, settings.height, settings.appName, settings.monitor, settings.share);
    
    if(window == NULL) {
        v::util::log("Failed to create GLFW window\n");
        glfwTerminate();
        return;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, this->engine_callback);
    
    gladLoadGL();
    
    glViewport(0, 0, settings.width, settings.height);
    
    std::string defVertSource = v::util::normalized_path(settings.vertexShaderPath);
    std::string defFragSource = v::util::normalized_path(settings.fragmentShaderPath);
    
    v::renderer::Shader * shaderProgram = new v::renderer::Shader(defVertSource.c_str(), defFragSource.c_str());

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);

	lightModel = glm::translate(lightModel, lightPos);
    
	shaderProgram->Activate();
	glUniform4f(glGetUniformLocation(shaderProgram->ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram->ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    
    camera = new v::renderer::Camera(settings.width, settings.height, settings.camera_position);

    loadModels(settings.model_paths);

    double prevTime = 0.0;
    double currTime = 0.0;
    double diffTime;

    unsigned int counter = 0;

    if(!settings.VSYNC)
        glfwSwapInterval(0);

    while(!glfwWindowShouldClose(window)) {
        
        currTime = glfwGetTime();
        diffTime = currTime - prevTime;
        
        counter++;

        if(diffTime >= settings.tickrate) {

            std::string FPS = std::to_string((1.0 / diffTime) * counter);
            std::string MS = std::to_string(((diffTime / counter) * 1000.0));
            std::string Title = "Testing - " + FPS + "FPS | " + MS + " ms";

            glfwSetWindowTitle(window, Title.c_str());
            prevTime = currTime;
            counter = 0;
        }

        glClearColor(0.10F, 0.10F, 0.10F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        camera->width = settings.width;
        camera->height = settings.height;
        
		camera->Inputs(window);
        
		camera->updateMatrix(settings.cameraFOVdegrees, settings.cameraNearPlane, settings.cameraFarPlane);

        for(auto model : models)
            model->Draw(*shaderProgram, *camera);

		glfwSwapBuffers(window);
        
		glfwPollEvents();
    }
    
    shaderProgram->Delete();

    delete shaderProgram;
    delete camera;

    glfwDestroyWindow(window);
    
    glfwTerminate();
    
    return;
}