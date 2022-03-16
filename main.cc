#include <V.h>

int main() {
    v::EngineSettings setts;

    setts.model_paths.push_back("\\V\\renderer\\Resources\\Models\\statue\\scene.gltf");
    
    v::Engine engine(setts);

    engine.Run();
}