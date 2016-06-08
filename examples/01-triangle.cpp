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

uniform float rotation = 0;
out vec3 fragColor;

mat3 rotate(float r) {
    return mat3(
        vec3( cos(r),  sin(r),  0.0),
        vec3(-sin(r),  cos(r),  0.0),
        vec3(    0.0,     0.0,  1.0));
}
void main()
{
    gl_Position = vec4(rotate(rotation) * position, 1.0f);
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

        // See here for details on how Eigen matrices work
        // http://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html

        mat_u indices(3, 1);
        indices << 0, 1, 2;

        mat_f positions(3, 3);
        positions.col(0) << -0.5f, -0.5f, 0.0f;
        positions.col(1) <<  0.5f, -0.5f, 0.0f;
        positions.col(2) <<  0.0f,  0.5f, 0.0f;

        mat_f colors(3, 3);
        colors.col(0) << 1, 0, 0;
        colors.col(1) << 0, 1, 0;
        colors.col(2) << 0, 0, 1;

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
        shader.setUniform("rotation", float(cy::time()));
        shader.drawIndexed(shader::mode::TRIANGLES, 0, 1);

    }
};

int main() {
    cy::run<MyApp>();
}
