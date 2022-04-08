#include <V.h>

class App : public v::engine::Core {
    public:
        App(v::engine::EngineSettings setts) : v::engine::Core(setts) {};
        App() : v::engine::Core() {};

        bool Init() override {

            renderer->SetGammaCorrection(2.2F);

            objects.at(0)->Scale(1.5F, 1.5F, 1.5F);

            setCurrentSkybox(0);

            SetBackgroundColor(0xFCC0FF);

            renderer->MSAA(v::MSAA::V8X);

            SetMode(v::MODE::VDEBUG);

            return true;
        }

        bool Tickrate(double difftime) override {
            DebugTickrate();
            return true;
        }

        bool Draw() override {
            DebugDraw();
            return true;
        }
};

int main() {
    App application(v::engine::read_engineSettings("\\app_settings.json"));

    application.Run();
}
