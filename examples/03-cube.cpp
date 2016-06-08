#include <cyfw/main.h>
#include <cyfw/Shaders/Basic3DShader.h>

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    shader::Basic3DShader shader;
    Camera camera;
public:

    void setup()
    {
        window->allowToggleFullscreen(false);
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
        shader.drawCube();
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
    cy::run<MyApp>(640, 480, "simple 3d cube");
}
