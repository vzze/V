#include <V.h>

class App : public v::engine::Core {
    public:
        App(v::engine::EngineSettings setts) : v::engine::Core(setts) {};
        App() : v::engine::Core() {};

        bool Init() override {
            return true;
        }

        float degrees = 0.0F;

        bool Tickrate(double difftime) override {
            objects[0]->Rotate(degrees, glm::vec3(0.0F, 1.0F, 0.0F));
            
            degrees += 1.0F;

            if(degrees >= 360.0F) degrees = 0.0F;

            return true;
        }

        bool Draw() override {
            for(auto object : objects)
                object->Draw(*default_shaderProgram, *camera);
            return true;
        }
};

int main() {
    v::engine::EngineSettings setts;

    setts.model_paths.push_back("\\V\\renderer\\Resources\\Models\\statue\\scene.gltf");
    setts.tickrate = 1.0 / 128.0;

    App application(setts);

    application.Run();
}
