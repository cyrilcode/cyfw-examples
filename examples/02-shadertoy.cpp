#include <cyfw/main.h>
#include <cyfw/Shaders/Basic2DShader.h>

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    shader::Basic2DShader shader;
public:
    MyApp()
    {

    }

    void setup()
    {
        shader.init(R"GLSL(
void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
  fragColor = vec4(sin(iGlobalTime) * fragCoord.x, cos(iGlobalTime) * fragCoord.y, 0, 1);
}
)GLSL");

    }

    void draw()
    {
        window->clear();
        shader.draw(window);
    }
};

int main() {
    cy::run<MyApp>(640, 480, "a mini shadertoy");
}
