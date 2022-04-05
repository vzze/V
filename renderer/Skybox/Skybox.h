#include <glad/glad.h>

#include <string>
#include <stb_image.h>

#include "../../Util/Util.h"
#include "../shaders/Shader.h"
#include "../../engine/EngineSettings/engineSettings.h"
#include "../Camera/Camera.h"

namespace v {
    namespace renderer {
        class Skybox {
            private:
                float skyboxVertices[24] = {
                    -1.0F, -1.0F,  1.0F,
                     1.0F, -1.0F,  1.0F,
                     1.0F, -1.0F, -1.0F,
                    -1.0F, -1.0F, -1.0F,
                    -1.0F,  1.0F,  1.0F,
                     1.0F,  1.0F,  1.0F,
                     1.0F,  1.0F, -1.0F,
                    -1.0F,  1.0F, -1.0F
                };

                unsigned int skyboxIndices[36] = {
                     1, 2, 6,
                     6, 5, 1,

                     0, 4, 7,
                     7, 3, 0,

                     4, 5, 6,
                     6, 7, 4,

                     0, 3, 2,
                     2, 1, 0,

                     0, 1, 5,
                     5, 4, 0,

                     3, 7, 6,
                     6, 2, 3
                };

                unsigned int sVAO, sVBO, sEBO;

                unsigned int cbTexture;
            
            public:
                Skybox();

                void Bind(std::string cubeMaps[6]);
                void Draw(v::renderer::Shader & skyboxProgram, v::engine::EngineSettings & settings, v::renderer::Camera & camera);
        };
    }
}
