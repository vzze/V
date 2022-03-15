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
                
          //      glm::vec3 translation = glm::vec3(0.0F, 0.0F, 0.0F);
          //      glm::quat rotation = glm::quat(1.0F, 0.0F, 0.0F, 0.0F);
         //       glm::vec3 scale = glm::vec3(1.0F, 1.0F, 1.0F);

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

            public:
                Model(const char * file);

                void Draw(Shader & shader, Camera & cam);

        //        void Rotate(glm::quat rotation);
        };
    }
}