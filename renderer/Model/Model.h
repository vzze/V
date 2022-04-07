#pragma once

#include <json.h>

#include "../../Util/Util.h"
#include "../Mesh/Mesh.h"

using json = nlohmann::json;

namespace v {
    namespace renderer {
        class Model {
            private:
                const char * file;
                std::vector<unsigned char> data;
                json JSON;

                std::vector<v::renderer::Mesh> meshes;

                unsigned int instances;

                std::vector<glm::mat4> instanceMatrix;
                
                std::vector<glm::vec3> translationsMeshes;
                std::vector<glm::quat> rotationsMeshes;
                std::vector<glm::vec3> scalesMeshes;
                std::vector<glm::mat4> matricesMeshes;

                void loadMesh(unsigned int indMesh);

                void TraverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0F));

                std::vector<std::string> loadedTexN;
                std::vector<v::renderer::Texture> loadedTex;

                std::vector<unsigned char> getData();

                std::vector<float> getFloats(json accessor);
                std::vector<GLuint> getIndices(json accessor);

                std::vector<Vertex> assembleVertex(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);
                std::vector<Texture> getTextures();

                std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
                std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
                std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);

            protected:
                Model(const char * file, unsigned int instancing = 1, std::vector<glm::mat4> instanceMatrix = {});

                void draw(Shader & shader, Camera & cam, glm::vec3 translation, glm::quat rotation, glm::vec3 scale);

                virtual ~Model();
        };
    }
}
