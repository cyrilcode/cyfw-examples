// simple text input example

#include <cyfw/main.h>

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
public:
    void setup()
    {
    }

    void draw()
    {
    }

    void key(window::KeyEvent e)
    {
        if (e.action == window::action::PRESSED && e.key == window::key::ENTER)
        {
            cout << endl;
        }
    }

    void textInput(window::CharEvent e)
    {
        cout << (char)e.codepoint;
    }
};

int main() {
    cy::run<MyApp>(640, 480, "testing text input");
}
