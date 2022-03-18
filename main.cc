#include <V.h>

class App : public v::Engine {
    public:
        App(v::EngineSettings setts) : v::Engine(setts) {};
        App() : v::Engine() {};

        bool Init() override {
            return true;
        }

        bool Tickrate(double difftime) override {
            return true;
        }

        bool Draw() override {
            for(auto model : models)
                model->Draw(*default_shaderProgram, *camera);
            return true;
        }
};

int main() {
    
    v::EngineSettings setts;
        
    setts.model_paths.push_back("\\V\\renderer\\Resources\\Models\\statue\\scene.gltf");

    App application(setts);

    application.Run();
}
