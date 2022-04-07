#include "Mesh.h"

v::renderer::Mesh::Mesh(
    std::vector<Vertex> & vertices, 
    std::vector<GLuint> & indices, 
    std::vector<Texture> & textures, 
    unsigned int instancing,
    std::vector<glm::mat4> instanceMatrix 
) {

    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    instances = instancing;

    vao.Bind();

    v::util::log(
        (
            std::string("Loading Mesh: ") + "Vertices: " + 
            std::to_string(vertices.size()) + "Indices: " + 
            std::to_string(indices.size()) + "Textures: " + 
            std::to_string(textures.size()) + "Instances: " +
            std::to_string(instances)
        ).c_str()
    );

    v::renderer::VBO instanceVBO(instanceMatrix);
    v::renderer::VBO vbo(vertices);

    v::renderer::EBO ebo(indices);

    vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);
    vao.LinkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));
    vao.LinkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(6 * sizeof(float)));
    vao.LinkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), (void *)(9 * sizeof(float)));
    if(instances != 1) {
        instanceVBO.Bind();

        vao.LinkAttrib(instanceVBO, 4, 4, GL_FLOAT, sizeof(glm::mat4), (void *)0);
        vao.LinkAttrib(instanceVBO, 5, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(1 * sizeof(glm::vec4)));
        vao.LinkAttrib(instanceVBO, 6, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(2 * sizeof(glm::vec4)));
        vao.LinkAttrib(instanceVBO, 7, 4, GL_FLOAT, sizeof(glm::mat4), (void *)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
    }
    
    vao.Unbind();
    vbo.Unbind();
    instanceVBO.Unbind();
    ebo.Unbind();
}

void v::renderer::Mesh::Draw(
    Shader & shader, Camera & camera, 
    glm::mat4 matrix, 
    glm::vec3 translation, 
    glm::quat rotation,
    glm::vec3 scale
) {
    shader.Activate();
    vao.Bind();

    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    for(unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].type;
	
        if(type == "diffuse") { 
            num = std::to_string(numDiffuse++);
        } else if (type == "specular") 
            num = std::to_string(numSpecular++);
        
        textures[i].texUnit(shader, (type + num).c_str(), i); 
	textures[i].Bind();
    }

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    
    if(instances == 1) {
	glm::mat4 trans = glm::mat4(1.0f);
	glm::mat4 rot = glm::mat4(1.0f);
        glm::mat4 sca = glm::mat4(1.0f);

        trans = glm::translate(trans, translation);
        rot = glm::mat4_cast(rotation);
	sca = glm::scale(sca, scale);

	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));
        shader.Uniform1i("ifinstanced", 0);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    } else {
        shader.Uniform1i("ifinstanced", 1);

        glDrawElementsInstanced(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0, instances);
    }
}
