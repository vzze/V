#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <V.h>

int main() {
    glfwInit(); 
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    v::util::width = 800;
    v::util::height = 800;

    GLFWwindow * window = glfwCreateWindow(v::util::width, v::util::height, "Testing", NULL, NULL);
    
    if(window == NULL) {
        v::util::log("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, v::util::framebuffer_size_callback);
    
    gladLoadGL();
    
    glViewport(0, 0, v::util::width, v::util::height);
    
    std::string defVertSource = v::util::normalized_path("\\V\\renderer\\shaders\\default.vert");
    std::string defFragSource = v::util::normalized_path("\\V\\renderer\\shaders\\default.frag");
    
    v::renderer::Shader shaderProgram(defVertSource.c_str(), defFragSource.c_str());

    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);

	lightModel = glm::translate(lightModel, lightPos);
    
	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    
    glEnable(GL_DEPTH_TEST); // draw the right triangles
    glEnable(GL_CULL_FACE); // face culling
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW); // counter clockwise face culling
    
    v::renderer::Camera cam(v::util::width, v::util::height, glm::vec3(0.0F, 0.0F, 2.0F));
    
    std::string modelPath = v::util::normalized_path("\\V\\renderer\\Resources\\Models\\statue\\scene.gltf");

    v::renderer::Model model(modelPath.c_str());

    double prevTime = 0.0;
    double currTime = 0.0;
    double diffTime;

    unsigned int counter = 0;

  //  glfwSwapInterval(0); // disables vsync lmao

    while(!glfwWindowShouldClose(window)) {
        
        currTime = glfwGetTime();
        diffTime = currTime - prevTime;
        
        counter++;

        if(diffTime >= 1.0 / 30.0) {
            std::string FPS = std::to_string((1.0 / diffTime) * counter);
            std::string MS = std::to_string(((diffTime / counter) * 1000.0));
            std::string Title = "Testing - " + FPS + "FPS | " + MS + " ms";

            glfwSetWindowTitle(window, Title.c_str());
            prevTime = currTime;
            counter = 0;
        }

        glClearColor(0.10F, 0.10F, 0.10F, 1.0F);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        
        cam.width = v::util::width;
        cam.height = v::util::height;
        
		cam.Inputs(window);
        
		cam.updateMatrix(45.0f, 0.1f, 100.0f);

        model.Draw(shaderProgram, cam);

		glfwSwapBuffers(window);
        
		glfwPollEvents();
    }
    
    shaderProgram.Delete();

    glfwDestroyWindow(window);
    
    glfwTerminate();
    
    return 0;
}