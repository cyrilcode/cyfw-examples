#include <cyfw/main.h>

using namespace std;
using namespace cy;


class MyApp : public cy::App
{
    vec2i size;
    VideoGrabber grabber;

public:

    void setup()
    {
        size = window->getFramebufferSize();
        if (grabber.open(0, size.x(), size.y()) < 0)
            throw "Failed to open capture device";
        if (grabber.start() < 0)
            throw "Failed to start capture device";
    }
    ~MyApp()
    {
        grabber.stop();
        grabber.close();
    }

    void draw()
    {
        grabber.update();
        grabber.draw(size.x() / 4, size.y() / 4, size.x() / 2, size.y() / 2);
    }

    void resize(window::ResizeEvent e)
    {
        grabber.resize(e.size.x(), e.size.y());
    }
};

int main() {
    cy::run<MyApp>(640, 480, "video capture");
}
