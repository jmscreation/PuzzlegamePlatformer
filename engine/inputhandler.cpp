#include "engine.h"

namespace Engine {

    namespace Input {

        std::vector<Window*> Window::list;
        bool Window::iterating = false;

        Window::Window() {
            list.push_back(this);
        }

        Window::~Window() {
            auto pos = std::find(list.begin(),list.end(),this);
            if(iterating)
                *pos = NULL;
            else
                list.erase(pos);
        }

        std::vector<Keyboard*> Keyboard::list;
        bool Keyboard::iterating = false;
        unsigned char Keyboard::keysDown[sf::Keyboard::Key::KeyCount];

        Keyboard::Keyboard() {
            list.push_back(this);
        }

        Keyboard::~Keyboard() {
            auto pos = std::find(list.begin(),list.end(),this);
            if(iterating)
                *pos = NULL;
            else
                list.erase(pos);
        }

        bool Keyboard::isKeyDown(int ky) {
            return keysDown[ky];
        }

        std::vector<Mouse*> Mouse::list;
        bool Mouse::iterating = false;
        unsigned char Mouse::buttonsDown[sf::Mouse::Button::ButtonCount];
        int Mouse::_x = 0;
        int Mouse::_y = 0;

        Mouse::Mouse() {
            list.push_back(this);
        }

        Mouse::~Mouse() {
            auto pos = std::find(list.begin(),list.end(),this);
            if(iterating)
                *pos = NULL;
            else
                list.erase(pos);
        }

        bool Mouse::isMouseButtonDown(int btn) {
            return buttonsDown[btn];
        }

        int Mouse::mouseX() {
            return _x;
        }

        int Mouse::mouseY() {
            return _y;
        }

        std::vector<Touch*> Touch::list;
        bool Touch::iterating = false;

        Touch::Touch() {
            list.push_back(this);
        }

        Touch::~Touch() {
            auto pos = std::find(list.begin(),list.end(),this);
            if(iterating)
                *pos = NULL;
            else
                list.erase(pos);
        }

        std::vector<Joystick*> Joystick::list;
        bool Joystick::iterating = false;

        Joystick::Joystick() {
            list.push_back(this);
        }

        Joystick::~Joystick() {
            auto pos = std::find(list.begin(),list.end(),this);
            if(iterating)
                *pos = NULL;
            else
                list.erase(pos);
        }

    }

}
