#include "engineUtil.h"

v::engine::EngineSettings v::engine::read_engineSettings(std::string path) {
    EngineSettings setts;

    auto str = v::util::get_file(v::util::normalized_path(path.c_str()).c_str());

    json obj = json::parse(str);

    setts.width = obj["width"];
    setts.height = obj["height"];

    setts.camera_position = glm::vec3(obj["camera_position"][0], 
                                      obj["camera_position"][1],
                                      obj["camera_position"][2]
                            );
    setts.cameraFOVdegrees = obj["cameraFOVdegrees"];
    setts.cameraNearPlane = obj["cameraNearPlane"];
    setts.cameraFarPlane = obj["cameraFarPlane"];

    if(obj["vertexShaderPath"] == "" || obj["vertexShaderPath"] == "default") {
        setts.vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
    } else {
        std::string vertP = obj["vertexShaderPath"];
        setts.vertexShaderPath = const_cast<char*>(vertP.c_str());
    }

    if(obj["fragmentShaderPath"] == "" || obj["fragmentShaderPath"] == "default") {
        setts.fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str());
    } else {
        std::string fragP = obj["fragmentShaderPath"];
        setts.fragmentShaderPath = const_cast<char*>(fragP.c_str());
    }

    for(std::string path : obj["model_paths"])
        setts.model_paths.push_back(path);

    setts.tickrate = obj["tickrate"];
    setts.fullscreen = obj["fullscreen"];
    return setts;
}

void v::engine::read_engineSettings(v::engine::EngineSettings & setts, std::string path) {
    auto str = v::util::get_file(v::util::normalized_path(path.c_str()).c_str());

    json obj = json::parse(str);

    setts.width = obj["width"];
    setts.height = obj["height"];

    setts.camera_position = glm::vec3(obj["camera_position"][0], 
                                      obj["camera_position"][1],
                                      obj["camera_position"][2]
                            );
    setts.cameraFOVdegrees = obj["cameraFOVdegrees"];
    setts.cameraNearPlane = obj["cameraNearPlane"];
    setts.cameraFarPlane = obj["cameraFarPlane"];

    if(obj["vertexShaderPath"] == "" || obj["vertexShaderPath"] == "default") {
        setts.vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
    } else {
        std::string vertP = obj["vertexShaderPath"];
        setts.vertexShaderPath = const_cast<char*>(vertP.c_str());
    }

    if(obj["fragmentShaderPath"] == "" || obj["fragmentShaderPath"] == "default") {
        setts.fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str());
    } else {
        std::string fragP = obj["fragmentShaderPath"];
        setts.fragmentShaderPath = const_cast<char*>(fragP.c_str());
    }

    for(std::string path : obj["model_paths"])
        setts.model_paths.push_back(path);

    setts.tickrate = obj["tickrate"];
    setts.fullscreen = obj["fullscreen"];
}

v::engine::Object::~Object() {}

void v::engine::Object::Draw(v::renderer::Shader &shader, v::renderer::Camera &cam) {
    draw(shader, cam, translation, rotation, scale);    
}

void v::engine::Object::Rotate(float degrees, glm::vec3 axis) { 
    rotation = glm::normalize(rotation * glm::angleAxis(glm::radians(degrees), axis));
}

void v::engine::Object::MoveTo(float x, float y, float z) {
    translation = glm::vec3(x, y, z);
}

void v::engine::Object::Scale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}
