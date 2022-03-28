#include "Camera.h"

v::renderer::Camera::~Camera() {}

v::renderer::Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
    
    Camera::height = height;
    
    Position = position;
}

void v::renderer::Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane) {
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Sets new camera matrix
    cameraMatrix = projection * view;
}

void v::renderer::Camera::Matrix(Shader & shader, const char * uniform) {
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void v::renderer::Camera::Inputs(GLFWwindow * window) {
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Position += speed * Orientation;
	
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
	Position += speed * -glm::normalize(glm::cross(Orientation, Up));
	
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	Position += speed * -Orientation;
	
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        Position += speed * glm::normalize(glm::cross(Orientation, Up));
	
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
	Position += speed * Up;
	
    if(glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) 
	Position += speed * -Up;
	
    if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
	speed = 0.4f;
	
    else if(glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) 
	speed = 0.1f;
	//  \/ you will need to remove this if and comment out firstClick for the camera to work all the time
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if(firstClick) {
    	    glfwSetCursorPos(window, (width / 2), (height / 2)); // NOLINT
	    firstClick = false;
	}

	double mouseX;
        double mouseY;

        glfwGetCursorPos(window, &mouseX, &mouseY);

	float rotX = sensitivity * (float)(mouseY - (height / 2)) / height; // NOLINT
	float rotY = sensitivity * (float)(mouseX - (width / 2)) / width; // NOLINT

	glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

	if(abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <= glm::radians(85.0f))
	    Orientation = newOrientation;
		

	Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

	glfwSetCursorPos(window, (width / 2), (height / 2)); // NOLINT
    } else if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            
        firstClick = true;
    }
}
