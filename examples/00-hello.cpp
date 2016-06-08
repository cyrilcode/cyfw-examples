#include <cyfw/main.h>

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    Shader shader;
public:
    MyApp()
    {
        // MyApp constructor is called once.
        // Do one-time initialization here.
    }

    void setup()
    {
        // Setup is where you'll do most initialization.
        // It might be called more than once, for example,
        // when the App switches to fullscreen the OpenGL
        // context needs resetting, create and load shaders, etc.
        shader = {};
        shader.init("example",
R"GLSL(
#version 330 core
in vec3 position;
in vec3 color;

out vec3 fragColor;

void main()
{
    gl_Position = vec4(position, 1.0f);
    fragColor = color;
}
)GLSL",
R"GLSL(
#version 330 core
in vec3 fragColor;

out vec4 color;

void main()
{
    color = vec4(fragColor, 1.0f);
}
)GLSL");

        mat_u indices(3, 2);
        indices.col(0) << 0, 1, 2;
        indices.col(1) << 2, 1, 3;
        mat_f positions(3, 4);
        positions.col(0) << -1, -1, 0;
        positions.col(1) <<  1, -1, 0;
        positions.col(2) << -1,  1, 0;
        positions.col(3) <<  1,  1, 0;
        mat_f colors(3, 4);
        colors.col(0) << 1, 1, 0;
        colors.col(1) << 1, 0, 0;
        colors.col(2) << 0, 1, 0;
        colors.col(3) << 0, 0, 1;
        shader.bind();
        shader.uploadIndices(indices);
        shader.uploadAttrib("position", positions);
        shader.uploadAttrib("color", colors);
    }

    void draw()
    {
        // This is called once per frame to render your
        // app. You probably want to clear the window first.
        window->clear();

        shader.bind();
        shader.drawIndexed(shader::mode::TRIANGLES, 0, 2);

    }
};

int main() {
    cy::run<MyApp>();
}
