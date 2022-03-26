#include <V.h>

class App : public v::engine::Core {
    public:
        App(v::engine::EngineSettings setts) : v::engine::Core(setts) {};
        App() : v::engine::Core() {};

        bool Init() override {
            objects.at(0)->Scale(1.5F, 1.5F, 1.5F);

            objects.at(1)->MoveTo(0.0F, 0.0F, 0.0F);

            objects.at(1)->Scale(0.25, 0.25, 0.25);

            return true;
        }

        bool Tickrate(double difftime) override {
            
            for(auto object : objects)
                object->Rotate(1.0F / 4.0F, v::util::Yaxis);

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
    setts.model_paths.push_back("\\V\\renderer\\Resources\\Models\\crow\\scene.gltf");

    setts.tickrate = 1.0 / 128.0;

    App application(setts);

    application.Run();
}
