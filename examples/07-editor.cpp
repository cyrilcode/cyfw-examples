#include <cyfw/main.h>
#include <cyfw/Gui.h>

using namespace std;
using namespace cy;

class MyApp : public cy::App
{
    Gui gui;
    bool guiOpen;
    char text[1024*16];
public:
    MyApp() {
        char * t = "rotate\nbox\0";
        memcpy(text, t, 11);
    }

    void setup()
    {
        window->setClearColor({0,0,0,1});
        gui.init(window);
        ImGui::GetIO().FontGlobalScale = 3;
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0;
        style.WindowPadding = {0, 0};
        style.Colors[ImGuiCol_Text] = {1,1,1,1};
        style.Colors[ImGuiCol_WindowBg] = {0,0,0,0};
        style.Colors[ImGuiCol_FrameBg] = {0,0,0,0};
    }

    void draw()
    {
        window->clear();
        gui.clear();

        doGui();

        gui.draw();
    }

    void doGui()
    {

        ImGui::SetNextWindowPos(ImVec2(10,10));
        ImGuiWindowFlags windowFlags =
                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse;
        if (!ImGui::Begin("Buffer", &guiOpen, windowFlags))
        {
            ImGui::End();
            return;
        }
        vec2i dim = window->getWindowSize();
        ImGui::SetWindowSize(ImVec2(dim.x() - 20, dim.y() - 20));

        ImGui::InputTextMultiline("source", text, sizeof(text), ImVec2(-1.0f, -1.0f), 0);
        ImGui::SetKeyboardFocusHere(-1);

        ImGui::End();
    }

    void key(window::KeyEvent e) { gui.key(e); }
    void scroll(window::ScrollEvent e) { gui.scroll(e); }
    void textInput(window::CharEvent e) { gui.character(e); }
    void mouseButton(window::MouseButtonEvent e ) { gui.mouse(e); }
};

int main() {
    cy::run<MyApp,ConsoleLogger>(640, 480, "testing editor");
}
