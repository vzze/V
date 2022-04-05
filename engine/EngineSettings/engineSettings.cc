#include "engineSettings.h"

v::engine::EngineSettings v::engine::read_engineSettings(std::string path) {
    EngineSettings setts;

    auto str = v::util::get_file(v::util::normalized_path(path.c_str()).c_str());

    json obj = json::parse(str);

    if(!obj["width"].empty())
        setts.width = obj["width"]; 

    v::util::log((std::string("Config: Width = ") + std::to_string(setts.width)).c_str());
    
    if(!obj["height"].empty())
        setts.height = obj["height"]; 
    
    v::util::log((std::string("Config: Height = ") + std::to_string(setts.height)).c_str());

    if(!obj["camera_position"].empty())
        setts.camera_position = glm::vec3(obj["camera_position"][0], 
                                          obj["camera_position"][1],
                                          obj["camera_position"][2]
                                );

    v::util::log((std::string("Config: Camera Position = ( ") + std::to_string(setts.camera_position.x) + ", " + std::to_string(setts.camera_position.y) + ", " + std::to_string(setts.camera_position.z) + " )").c_str()); 
    
    if(!obj["cameraFOVdegrees"].empty())
        setts.cameraFOVdegrees = obj["cameraFOVdegrees"]; 

    v::util::log((std::string("Config: FOV = ") + std::to_string(setts.cameraFOVdegrees)).c_str());
    
    if(!obj["cameraNearPlane"].empty())
        setts.cameraNearPlane = obj["cameraNearPlane"]; 

    v::util::log((std::string("Config: Near Plane = ") + std::to_string(setts.cameraNearPlane)).c_str());
    
    if(!obj["cameraFarPlane"].empty())
        setts.cameraFarPlane = obj["cameraFarPlane"]; 

    v::util::log((("Config: Far Plane = ") + std::to_string(setts.cameraFarPlane)).c_str());

    if(obj["vertexShaderPath"] == "" || obj["vertexShaderPath"] == "default" || obj["vertexShaderPath"].empty()) {
        setts.vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
    } else {
        std::string vertP = obj["vertexShaderPath"];
        setts.vertexShaderPath = const_cast<char*>(vertP.c_str());
    }

    v::util::log((std::string("Config: Vertex Shader = ") + setts.vertexShaderPath).c_str());

    if(obj["fragmentShaderPath"] == "" || obj["fragmentShaderPath"] == "default" || obj["fragmentShaderPath"].empty()) {
        setts.fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str());
    } else {
        std::string fragP = obj["fragmentShaderPath"];
        setts.fragmentShaderPath = const_cast<char*>(fragP.c_str());
    }

    v::util::log((std::string("Config: Fragment Shader = ") + setts.fragmentShaderPath).c_str());

    if(obj["appName"] == "" || obj["appName"].empty())
        setts.appName = const_cast<char*>("V");
    else {
        std::string n = obj["appName"];
        setts.appName = const_cast<char*>(n.c_str());
    }

    v::util::log((std::string("Config: Application Name = ") + setts.appName).c_str());

    if(!obj["model_paths"].empty())
        for(std::string path : obj["model_paths"])
            setts.model_paths.push_back(path);

    v::util::log((std::string("Config: Queued Models for loading = ") + std::to_string(setts.model_paths.size())).c_str());

    if(!obj["skyboxes"].empty())
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

    if(!obj["shaders"].empty())
            for(json shader : obj["shaders"]) {
                Shader_container sh;
                sh.arr[0] = shader["vertex"];
                sh.arr[1] = shader["fragment"];
                sh.arr[2] = shader["geometry"];
            }

    v:util::log((std::string("Config: Shaders loaded = ") + std::to_string(setts.shader_paths.size())).c_str());

    if(!obj["tickrate"].empty())
        setts.tickrate = obj["tickrate"]; 

    v::util::log((std::string("Config: Tickrate = ") + std::to_string(setts.tickrate)).c_str());
    
    if(!obj["fullscreen"].empty())
        setts.fullscreen = obj["fullscreen"]; 

    v::util::log((std::string("Config: Fullscreen = ") + std::to_string(setts.fullscreen)).c_str());
    
    if(!obj["VSYNC"].empty())
        setts.VSYNC = obj["VSYNC"]; 

    v::util::log((std::string("Config: V-Sync = ") + std::to_string(setts.VSYNC)).c_str());


    return setts;
}

void v::engine::read_engineSettings(v::engine::EngineSettings & setts, std::string path) {
    auto str = v::util::get_file(v::util::normalized_path(path.c_str()).c_str());

    json obj = json::parse(str);

    if(!obj["width"].empty())
        setts.width = obj["width"]; 

    v::util::log((std::string("Config: Width = ") + std::to_string(setts.width)).c_str());
    
    if(!obj["height"].empty())
        setts.height = obj["height"]; 
    
    v::util::log((std::string("Config: Height = ") + std::to_string(setts.height)).c_str());

    if(!obj["camera_position"].empty())
        setts.camera_position = glm::vec3(obj["camera_position"][0], 
                                          obj["camera_position"][1],
                                          obj["camera_position"][2]
                                );

    v::util::log((std::string("Config: Camera Position = ( ") + std::to_string(setts.camera_position.x) + ", " + std::to_string(setts.camera_position.y) + ", " + std::to_string(setts.camera_position.z) + " )").c_str()); 
    
    if(!obj["cameraFOVdegrees"].empty())
        setts.cameraFOVdegrees = obj["cameraFOVdegrees"]; 

    v::util::log((std::string("Config: FOV = ") + std::to_string(setts.cameraFOVdegrees)).c_str());
    
    if(!obj["cameraNearPlane"].empty())
        setts.cameraNearPlane = obj["cameraNearPlane"]; 

    v::util::log((std::string("Config: Near Plane = ") + std::to_string(setts.cameraNearPlane)).c_str());
    
    if(!obj["cameraFarPlane"].empty())
        setts.cameraFarPlane = obj["cameraFarPlane"]; 

    v::util::log((("Config: Far Plane = ") + std::to_string(setts.cameraFarPlane)).c_str());

    if(obj["vertexShaderPath"] == "" || obj["vertexShaderPath"] == "default" || obj["vertexShaderPath"].empty()) {
        setts.vertexShaderPath = const_cast<char*>(v::util::default_vertex_shader_path.c_str());
    } else {
        std::string vertP = obj["vertexShaderPath"];
        setts.vertexShaderPath = const_cast<char*>(vertP.c_str());
    }

    v::util::log((std::string("Config: Vertex Shader = ") + setts.vertexShaderPath).c_str());

    if(obj["fragmentShaderPath"] == "" || obj["fragmentShaderPath"] == "default" || obj["fragmentShaderPath"].empty()) {
        setts.fragmentShaderPath = const_cast<char*>(v::util::default_fragment_shader_path.c_str());
    } else {
        std::string fragP = obj["fragmentShaderPath"];
        setts.fragmentShaderPath = const_cast<char*>(fragP.c_str());
    }

    v::util::log((std::string("Config: Fragment Shader = ") + setts.fragmentShaderPath).c_str());

    if(obj["appName"] == "" || obj["appName"].empty())
        setts.appName = const_cast<char*>("V");
    else {
        std::string n = obj["appName"];
        setts.appName = const_cast<char*>(n.c_str());
    }

    v::util::log((std::string("Config: Application Name = ") + setts.appName).c_str());

    if(!obj["model_paths"].empty())
        for(std::string path : obj["model_paths"])
            setts.model_paths.push_back(path);

    v::util::log((std::string("Config: Queued Models for loading = ") + std::to_string(setts.model_paths.size())).c_str());

    if(!obj["skyboxes"].empty())
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

    if(!obj["shaders"].empty())
            for(json shader : obj["shaders"]) {
                Shader_container sh;
                sh.arr[0] = shader["vertex"];
                sh.arr[1] = shader["fragment"];
                sh.arr[2] = shader["geometry"];
            }

    v:util::log((std::string("Config: Shaders loaded = ") + std::to_string(setts.shader_paths.size())).c_str());

    if(!obj["tickrate"].empty())
        setts.tickrate = obj["tickrate"]; 

    v::util::log((std::string("Config: Tickrate = ") + std::to_string(setts.tickrate)).c_str());
    
    if(!obj["fullscreen"].empty())
        setts.fullscreen = obj["fullscreen"]; 

    v::util::log((std::string("Config: Fullscreen = ") + std::to_string(setts.fullscreen)).c_str());
    
    if(!obj["VSYNC"].empty())
        setts.VSYNC = obj["VSYNC"]; 

    v::util::log((std::string("Config: V-Sync = ") + std::to_string(setts.VSYNC)).c_str());
} 
