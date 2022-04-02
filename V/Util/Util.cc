#include "Util.h"

std::string v::util::get_file(const char * file) {
    std::ifstream f(file, std::ios::binary);

    if(f) {
        std::string content;

        f.seekg(0, std::ios::end);
        content.resize(f.tellg());
        f.seekg(0, std::ios::beg);

        f.read(&content[0], content.size());

        f.close();

        return content;
    } else {
        std::string l = "Invalid Path: ";
        l += file; l += '\n';

        v::util::log(l.c_str());

        throw std::invalid_argument("Invalid Path"); 
    }
}

std::string v::util::normalized_path(const char * path) {
    return std::filesystem::current_path().string() + path;
}

std::tuple<short int, short int, short int> v::util::hex_to_rgb(long long hex) {
    return std::make_tuple((short int)((hex >> sizeof(hex) * 2) & 0xFF), (short int)(((hex >> sizeof(hex)) & 0xFF)), (short int)(hex & 0xFF));
}

std::tuple<float, float, float> v::util::normalized_rgb(std::tuple<short int, short int, short int> rgb) {
    const float op = 1.0F / 255.0F;
    return std::make_tuple(op * (float)(std::get<0>(rgb)), op * (float)(std::get<1>(rgb)), op * (float)(std::get<2>(rgb)));
}

float v::util::normalized_color(short int color) {
    return 1.0F / 255.0F * (float)(color);
}

void v::util::log(const char * output) {
    static std::ofstream out("app.log"); 

    std::chrono::time_point t = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t(t);

    std::tm tm; localtime_s(&tm, &tt);

    std::stringstream ss;

    ss << std::put_time(&tm, "%H:%M:%S");

    out << '[' << ss.str() << "]: " << output << '\n';
}

const std::string v::util::default_fragment_shader_path = v::util::normalized_path("\\V\\renderer\\shaders\\default.frag");
const std::string v::util::default_vertex_shader_path = v::util::normalized_path("\\V\\renderer\\shaders\\default.vert"); 

const glm::vec3 v::util::Xaxis = glm::vec3(1.0F, 0.0F, 0.0F);
const glm::vec3 v::util::Yaxis = glm::vec3(0.0F, 1.0F, 0.0F);
const glm::vec3 v::util::Zaxis = glm::vec3(0.0F, 0.0F, 1.0F);
