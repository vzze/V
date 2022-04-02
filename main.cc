#include <V.h>

class App : public v::engine::Core {
    public:
        App(v::engine::EngineSettings setts) : v::engine::Core(setts) {};
        App() : v::engine::Core() {};

        bool Init() override {
            objects.at(0)->Scale(1.5F, 1.5F, 1.5F);
            return true;
        }

        bool Tickrate(double difftime) override {

            for(auto object : objects) {
                object->Rotate(1.0F / 4.0F, v::util::Yaxis);
                object->Rotate(1.0F / 4.0F, v::util::Xaxis);
                object->Rotate(1.0F / 4.0F, v::util::Zaxis);
            }

            return true;
        }

        bool Draw() override {
            for(auto object : objects)
                object->Draw(*shaderProgram, *camera);
            return true;
        }
};

int main() {
    App application(v::engine::read_engineSettings("\\app_settings.json"));

    application.Run();
}
