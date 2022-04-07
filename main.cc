#include <V.h>

class App : public v::engine::Core {
    public:
        App(v::engine::EngineSettings setts) : v::engine::Core(setts) {};
        App() : v::engine::Core() {};

        bool Init() override {

            objects.at(0)->Scale(1.5F, 1.5F, 1.5F);

            setCurrentSkybox(0);

            SetBackgroundColor(0xFCC0FF);

            renderer->MSAA(v::renderer::MSAA_8X);

            renderer->SetGammaCorrection(2.2F);

            return true;
        }

        bool Tickrate(double difftime) override {

            for(auto object : objects) {
                /* object->Rotate(1.0F / 4.0F, v::util::Yaxis); */
                /* object->Rotate(1.0F / 4.0F, v::util::Xaxis); */
                /* object->Rotate(1.0F / 4.0F, v::util::Zaxis); */
            }

            return true;
        }

        bool Draw() override {
            for(auto object : objects) {
                object->DrawWithOutline(*renderer->shaderProgram, *renderer->stencilProgram, *renderer->camera, 0.08F);
            }

            return true;
        }
};

int main() {
    App application(v::engine::read_engineSettings("\\app_settings.json"));

    application.Run();
}
