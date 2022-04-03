#include "engineUtil.h"

v::engine::EngineSettings v::engine::read_engineSettings(std::string path) {
    EngineSettings setts;

    auto str = v::util::get_file(v::util::normalized_path(path.c_str()).c_str());

    json obj = json::parse(str);

    setts.width = obj["width"]; v::util::log((std::string("Config: Width = ") + std::to_string(setts.width)).c_str());
    setts.height = obj["height"]; v::util::log((std::string("Config: Height = ") + std::to_string(setts.height)).c_str());

    setts.camera_position = glm::vec3(obj["camera_position"][0], 
                                      obj["camera_position"][1],
                                      obj["camera_position"][2]
                            ); 

    v::util::log((std::string("Config: Camera Position = ( ") + std::to_string(setts.camera_position.x) + ", " + std::to_string(setts.camera_position.y) + ", " + std::to_string(setts.camera_position.z) + " )").c_str()); 

    setts.cameraFOVdegrees = obj["cameraFOVdegrees"]; v::util::log((std::string("Config: FOV = ") + std::to_string(setts.cameraFOVdegrees)).c_str());
    setts.cameraNearPlane = obj["cameraNearPlane"]; v::util::log((std::string("Config: Near Plane = ") + std::to_string(setts.cameraNearPlane)).c_str());
    setts.cameraFarPlane = obj["cameraFarPlane"]; v::util::log((("Config: Far Plane = ") + std::to_string(setts.cameraFarPlane)).c_str());

    if(obj["vertexShaderPath"] == "" || obj["vertexShaderPath"] == "default") {
        setts.vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
    } else {
        std::string vertP = obj["vertexShaderPath"];
        setts.vertexShaderPath = const_cast<char*>(vertP.c_str());
    }

    v::util::log((std::string("Config: Vertex Shader = ") + setts.vertexShaderPath).c_str());

    if(obj["fragmentShaderPath"] == "" || obj["fragmentShaderPath"] == "default") {
        setts.fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str());
    } else {
        std::string fragP = obj["fragmentShaderPath"];
        setts.fragmentShaderPath = const_cast<char*>(fragP.c_str());
    }

    v::util::log((std::string("Config: Fragment Shader = ") + setts.fragmentShaderPath).c_str());

    if(obj["appName"] == "")
        setts.appName = const_cast<char*>("V");
    else {
        std::string n = obj["appName"];
        setts.appName = const_cast<char*>(n.c_str());
    }

    v::util::log((std::string("Config: Application Name = ") + setts.appName).c_str());

    for(std::string path : obj["model_paths"])
        setts.model_paths.push_back(path);

    v::util::log((std::string("Config: Queued Models for loading = ") + std::to_string(setts.model_paths.size())).c_str());

    for(json skybox : obj["skyboxes"]) {
        Skybox_container ob;
        ob.arr[0] = skybox["right"];
        ob.arr[1] = skybox["left"];
        ob.arr[2] = skybox["top"];
        ob.arr[3] = skybox["bottom"];
        ob.arr[4] = skybox["front"];
        ob.arr[5] = skybox["back"];

        setts.skybox_paths.push_back(ob);
    }

    v::util::log((std::string("Config: Queued Skyboxes for loading = ") + std::to_string(setts.skybox_paths.size())).c_str());

    setts.tickrate = obj["tickrate"]; v::util::log((std::string("Config: Tickrate = ") + std::to_string(setts.tickrate)).c_str());
    setts.fullscreen = obj["fullscreen"]; v::util::log((std::string("Config: Fullscreen = ") + std::to_string(setts.fullscreen)).c_str());
    setts.VSYNC = obj["VSYNC"]; v::util::log((std::string("Config: V-Sync = ") + std::to_string(setts.VSYNC)).c_str());

    return setts;
}

void v::engine::read_engineSettings(v::engine::EngineSettings & setts, std::string path) {
    auto str = v::util::get_file(v::util::normalized_path(path.c_str()).c_str());

    json obj = json::parse(str);

    setts.width = obj["width"]; v::util::log((std::string("Config: Width = ") + std::to_string(setts.width)).c_str());
    setts.height = obj["height"]; v::util::log((std::string("Config: Height = ") + std::to_string(setts.height)).c_str());

    setts.camera_position = glm::vec3(obj["camera_position"][0], 
                                      obj["camera_position"][1],
                                      obj["camera_position"][2]
                            ); 

    v::util::log((std::string("Config: Camera Position = ( ") + std::to_string(setts.camera_position.x) + std::to_string(setts.camera_position.y) + std::to_string(setts.camera_position.z) + " )").c_str()); 

    setts.cameraFOVdegrees = obj["cameraFOVdegrees"]; v::util::log((std::string("Config: FOV = ") + std::to_string(setts.cameraFOVdegrees)).c_str());
    setts.cameraNearPlane = obj["cameraNearPlane"]; v::util::log((std::string("Config: Near Plane = ") + std::to_string(setts.cameraNearPlane)).c_str());
    setts.cameraFarPlane = obj["cameraFarPlane"]; v::util::log((("Config: Far Plane = ") + std::to_string(setts.cameraFarPlane)).c_str());

    if(obj["vertexShaderPath"] == "" || obj["vertexShaderPath"] == "default") {
        setts.vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
    } else {
        std::string vertP = obj["vertexShaderPath"];
        setts.vertexShaderPath = const_cast<char*>(vertP.c_str());
    }

    v::util::log((std::string("Config: Vertex Shader = ") + setts.vertexShaderPath).c_str());

    if(obj["fragmentShaderPath"] == "" || obj["fragmentShaderPath"] == "default") {
        setts.fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str());
    } else {
        std::string fragP = obj["fragmentShaderPath"];
        setts.fragmentShaderPath = const_cast<char*>(fragP.c_str());
    }

    v::util::log((std::string("Config: Fragment Shader = ") + setts.fragmentShaderPath).c_str());

    if(obj["appName"] == "")
        setts.appName = const_cast<char*>("V");
    else {
        std::string n = obj["appName"];
        setts.appName = const_cast<char*>(n.c_str());
    }

    v::util::log((std::string("Config: Application Name = ") + setts.appName).c_str());

    for(std::string path : obj["model_paths"])
        setts.model_paths.push_back(path);

    v::util::log((std::string("Config: Queued Models for loading = ") + std::to_string(setts.model_paths.size())).c_str());

    for(json skybox : obj["skyboxes"]) {
        Skybox_container ob;
        ob.arr[0] = skybox["right"];
        ob.arr[1] = skybox["left"];
        ob.arr[2] = skybox["top"];
        ob.arr[3] = skybox["bottom"];
        ob.arr[4] = skybox["front"];
        ob.arr[5] = skybox["back"];

        setts.skybox_paths.push_back(ob);
    }

    v::util::log((std::string("Config: Queued Skyboxes for loading = ") + std::to_string(setts.skybox_paths.size())).c_str());

    setts.tickrate = obj["tickrate"]; v::util::log((std::string("Config: Tickrate = ") + std::to_string(setts.tickrate)).c_str());
    setts.fullscreen = obj["fullscreen"]; v::util::log((std::string("Config: Fullscreen = ") + std::to_string(setts.fullscreen)).c_str());
    setts.VSYNC = obj["VSYNC"]; v::util::log((std::string("Config: V-Sync = ") + std::to_string(setts.VSYNC)).c_str());
}

v::engine::Object::~Object() {}

void v::engine::Object::Draw(v::renderer::Shader &shader, v::renderer::Camera &cam) {
    draw(shader, cam, translation, rotation, scale);    
}

void v::engine::Object::Rotate(float degrees, const glm::vec3 & axis) { 
    rotation = glm::normalize(rotation * glm::angleAxis(glm::radians(degrees), axis));
}

void v::engine::Object::SetRotation(const glm::quat & quat) {
    rotation = quat; 
}

void v::engine::Object::SetRotation(float degrees, const glm::vec3 & axis) {
    rotation = glm::angleAxis(glm::radians(degrees), axis); 
}

void v::engine::Object::SetRotation(const glm::vec3 & eulerAngles) {
    rotation = glm::quat(eulerAngles);
}

void v::engine::Object::SetRotation(const glm::vec3 & v1, const glm::vec3 & v2) {
    rotation = glm::quat(v1, v2);
}

void v::engine::Object::SetRotation(const glm::mat3 & matrix) {
    rotation = glm::quat(matrix);
}

void v::engine::Object::SetRotation(const glm::mat4 & matrix) {
    rotation = glm::quat(matrix);
}

void v::engine::Object::SetRotation(float w, float x, float y, float z) {
    rotation = glm::quat(w, x, y, z);
}

void v::engine::Object::MoveTo(float x, float y, float z) {
    translation = glm::vec3(x, y, z);
}

void v::engine::Object::Move(float x, float y, float z) {
    translation.x += x;
    translation.y += y;
    translation.z += z;
}

void v::engine::Object::Scale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}
