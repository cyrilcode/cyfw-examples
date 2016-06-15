#include <cyfw/main.h>
#include <cyfw/Shaders/CyrilShader.h>

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
        camera.reset({0,0,-3});
    }

    void draw()
    {
        camera.setViewport(window->getWindowSize());
        window->clear();

        shader.bind(window);
        shader.setProjectionMatrix(camera.projectionMatrix());
        shader.setViewMatrix(camera.viewMatrix());
        shader.setLightPosition(camera.getPosition());
        shader.drawCube({0.5,0.25,1});
        shader.move({1,1,0});
        shader.setColor({1,0,0,1});
        shader.drawCube({0.5,0.5,0.5});
    }

    void key(window::KeyEvent e)
    {
        camera.handleKeyEvent(e);
        if (e.action == window::action::PRESSED && e.key == window::key::Q)
        {
            camera.reset({0,0,-3});
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
    cy::run<MyApp>(640, 480, "simple 3d example");
}
