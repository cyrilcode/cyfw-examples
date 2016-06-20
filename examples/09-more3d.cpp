#include <cyfw/main.h>
#include <cyfw/Shaders/CyrilShader.h>

// A bit like the Cinder 3D example from:
// https://www.libcinder.org/docs/guides/opengl/part2.html

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    shader::CyrilShader shader;
    Camera camera;
public:

    void setup()
    {
        window->setClearColor({0,0,0,1});
        shader.init();
        camera.reset({-5, -2, -5}, {0,1,0});
    }

    void draw()
    {
        camera.setViewport(window->getWindowSize());
        window->clear();

        shader.bind(window);
        shader.setProjectionMatrix(camera.projectionMatrix());
        shader.setViewMatrix(camera.viewMatrix());
        shader.setLightPosition(camera.getPosition());

        int numSpheres = 64;
        float maxAngle = math::PI * 7;
        float spiralRadius = 1;
        float height = 3;
        float offsetT = cy::time();

        for (int s = 0; s < numSpheres; ++s)
        {
            float rel = s / (float)numSpheres;
            float angle = (rel - offsetT) * maxAngle;
            float y = rel * height;
            float r = rel * spiralRadius * spiralRadius;
            vec3f offset{r * cos(angle), y, r * sin(angle)};

            shader.pushMatrix();
            shader.move(offset);
            shader.setColor(color::hsb(rel, 1, 1));
            shader.drawBall(0.1f);
            shader.popMatrix();
        }
    }
    void key(window::KeyEvent e)
    {
        camera.handleKeyEvent(e);
        if (e.action == window::action::PRESSED && e.key == window::key::Q)
        {
            camera.reset({-5, -2, -5}, {0,1,0});
        }
    }

    void scroll(window::ScrollEvent e)
    {
        camera.handleScrollEvent(e);
    }

    void mouseButton(window::MouseButtonEvent e)
    {
        camera.handleMouseButtonEvent(e);
    }

    void cursorMove(window::CursorMoveEvent e)
    {
        camera.handleCursorMoveEvent(e);
    }
};

int main() {
    cy::run<MyApp>(640, 480, "more 3d");
}
