#include <cyfw/main.h>


using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    Gui gui;
public:
    void setup()
    {
        gui.init(window);
    }

    void draw()
    {
        window->clear();
        gui.clear();



        gui.draw();
    }

    void key(window::KeyEvent e) { gui.key(e); }
    void scroll(window::ScrollEvent e) { gui.scroll(e); }
    void textInput(window::CharEvent e) { gui.character(e); }
    void mouseButton(window::MouseButtonEvent e ) { gui.mouse(e); }
};

int main() {
    cy::run<MyApp,ConsoleLogger>(640, 480, "testing editor", true);
}
