#include <cyfw/main.h>

using namespace std;
using namespace cy;


class MyApp : public cy::App
{
    int width = 640;
    int height = 480;
    VideoGrabber grabber;

public:

    MyApp()
    {
        if (grabber.open(0, width, height) < 0)
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
        grabber.draw(width / 4, height / 4, width / 2, height / 2);
    }

    void resize(window::ResizeEvent e)
    {
        grabber.resize(e.size.x(), e.size.y());
    }
};

int main() {
    cy::run<MyApp>(640, 480, "video capture");
}
