#include "engine.h"

namespace Engine {

    App* App::currentApp = NULL;

    App::App(int width, int height, std::string title, sf::Uint32 winstyle, bool defaultView):
        videoMode(width,height) {

        if(currentApp == NULL)
            currentApp = this;
        else
            std::cout << "Warning: App was instantiated multiple times" << std::endl;

        win = new sf::RenderWindow(videoMode,title,winstyle);

        if(defaultView)
            viewStack.push_back( new View(sf::FloatRect(0,0,float(width),float(height))) );

        for(int i=0;i<sf::Keyboard::Key::KeyCount;i++)
            Input::Keyboard::keysDown[i] = 0;

        for(int i=0;i<sf::Mouse::Button::ButtonCount;i++)
            Input::Mouse::buttonsDown[i] = 0;
    }

    App::App(bool defaultView) {

        if(currentApp == NULL)
            currentApp = this;
        else
            std::cout << "Warning: App was instantiated multiple times" << std::endl;

        win = new sf::RenderWindow();

        if(defaultView)
            viewStack.push_back( new View() );

        for(int i=0;i<sf::Keyboard::Key::KeyCount;i++)
            Input::Keyboard::keysDown[i] = 0;

        for(int i=0;i<sf::Mouse::Button::ButtonCount;i++)
            Input::Mouse::buttonsDown[i] = 0;
    }

    App::~App() {
        delete win;
    }

    void App::run() {
        sf::Event event;
        sf::Clock clock;

        while(1) {
            #define ___APP_EVENT_LOOP(LIST,FUNCCALL,ITER) \
                do { \
                    ITER = true; \
                    bool drop = false; \
                    int i=0,j=0,l=LIST.size(); \
                    while(i<l) { \
                        auto itm = LIST[i]; \
                        if(itm == NULL) drop = true; \
                        else { \
                            itm->FUNCCALL; \
                            if(drop) LIST[j] = LIST[i]; \
                            j++; \
                        } \
                        i++; \
                    } \
                    if(drop) LIST.erase(LIST.begin()+j,LIST.begin()+l); \
                    ITER = false; \
                } while(0)

            #define ___APP_DRAW_LOOP(LIST,FUNCCALL) \
                do { \
                    for(auto i=LIST.first();i;i=i->n) \
                        i->item->FUNCCALL; \
                } while(0)

            while(win->pollEvent(event)) {

                // EVENT MANAGER
                switch(event.type) {
                    // WINDOW
                    case sf::Event::Closed:
                        return;
                    case sf::Event::Resized:
                        ___APP_EVENT_LOOP(Input::Window::list,windowResized(event.size),Input::Window::iterating);
                        break;
                    case sf::Event::GainedFocus:
                        ___APP_EVENT_LOOP(Input::Window::list,windowGainFocus(),Input::Window::iterating);
                        break;
                    case sf::Event::LostFocus:
                        ___APP_EVENT_LOOP(Input::Window::list,windowLostFocus(),Input::Window::iterating);
                        break;
                    case sf::Event::TextEntered:
                        ___APP_EVENT_LOOP(Input::Window::list,windowTextEntered(event.text),Input::Window::iterating);
                        break;
                    // KEYBOARD
                    case sf::Event::KeyPressed:
                        if(Input::Keyboard::keysDown[event.key.code]) break;
                        Input::Keyboard::keysDown[event.key.code] = 1
                            | (event.key.alt ? 2 : 0)
                            | (event.key.control ? 4 : 0)
                            | (event.key.shift ? 8 : 0)
                            | (event.key.system ? 16 : 0);
                        ___APP_EVENT_LOOP(Input::Keyboard::list,keyPress(event.key),Input::Keyboard::iterating);
                        break;
                    case sf::Event::KeyReleased:
                        Input::Keyboard::keysDown[event.key.code] = 0;
                        ___APP_EVENT_LOOP(Input::Keyboard::list,keyRelease(event.key),Input::Keyboard::iterating);
                        break;
                    // MOUSE
                    case sf::Event::MouseButtonPressed:
                        if(Input::Mouse::buttonsDown[event.key.code]) break;
                        Input::Mouse::buttonsDown[event.key.code] = 1;
                        Input::Mouse::_x = event.mouseButton.x; Input::Mouse::_y = event.mouseButton.y;
                        ___APP_EVENT_LOOP(Input::Mouse::list,mouseButtonPress(event.mouseButton),Input::Mouse::iterating);
                        break;
                    case sf::Event::MouseButtonReleased:
                        Input::Mouse::buttonsDown[event.key.code] = 0;
                        Input::Mouse::_x = event.mouseButton.x; Input::Mouse::_y = event.mouseButton.y;
                        ___APP_EVENT_LOOP(Input::Mouse::list,mouseButtonRelease(event.mouseButton),Input::Mouse::iterating);
                        break;
                    case sf::Event::MouseMoved:
                        Input::Mouse::_x = event.mouseMove.x; Input::Mouse::_y = event.mouseMove.y;
                        ___APP_EVENT_LOOP(Input::Mouse::list,mouseMove(event.mouseMove),Input::Mouse::iterating);
                        break;
                    case sf::Event::MouseWheelScrolled:
                        ___APP_EVENT_LOOP(Input::Mouse::list,mouseWheel(event.mouseWheelScroll),Input::Mouse::iterating);
                        break;
                    // TOUCH
                    case sf::Event::TouchBegan:
                        ___APP_EVENT_LOOP(Input::Touch::list,touchBegin(event.touch),Input::Touch::iterating);
                        break;
                    case sf::Event::TouchMoved:
                        ___APP_EVENT_LOOP(Input::Touch::list,touchMove(event.touch),Input::Touch::iterating);
                        break;
                    case sf::Event::TouchEnded:
                        ___APP_EVENT_LOOP(Input::Touch::list,touchEnd(event.touch),Input::Touch::iterating);
                        break;
                    // JOYSTICK
                    case sf::Event::JoystickConnected:
                        ___APP_EVENT_LOOP(Input::Joystick::list,joystickConnect(event.joystickConnect),Input::Joystick::iterating);
                        break;
                    case sf::Event::JoystickDisconnected:
                        ___APP_EVENT_LOOP(Input::Joystick::list,joystickDisconnect(event.joystickConnect),Input::Joystick::iterating);
                        break;
                    case sf::Event::JoystickMoved:
                        ___APP_EVENT_LOOP(Input::Joystick::list,joystickMove(event.joystickMove),Input::Joystick::iterating);
                        break;
                    case sf::Event::JoystickButtonPressed:
                        ___APP_EVENT_LOOP(Input::Joystick::list,joystickButtonPress(event.joystickButton),Input::Joystick::iterating);
                        break;
                    case sf::Event::JoystickButtonReleased:
                        ___APP_EVENT_LOOP(Input::Joystick::list,joystickButtonRelease(event.joystickButton),Input::Joystick::iterating);
                        break;
                }
            }

            event.type = sf::Event::EventType::KeyPressed;
            for(int i=0;i<sf::Keyboard::Key::KeyCount;i++)
                if(Input::Keyboard::keysDown[i]) {
                    event.key = {
                        sf::Keyboard::Key(i),
                        bool(Input::Keyboard::keysDown[i] & 2),
                        bool(Input::Keyboard::keysDown[i] & 4),
                        bool(Input::Keyboard::keysDown[i] & 8),
                        bool(Input::Keyboard::keysDown[i] & 16)
                    };
                    ___APP_EVENT_LOOP(Input::Keyboard::list,keyHold(event.key),Input::Keyboard::iterating);
                }

            event.type = sf::Event::EventType::MouseButtonPressed;
            for(int i=0;i<sf::Mouse::Button::ButtonCount;i++)
                if(Input::Mouse::buttonsDown[i]) {
                    event.mouseButton = {
                        sf::Mouse::Button(i), Input::Mouse::_x, Input::Mouse::_y
                    };
                    ___APP_EVENT_LOOP(Input::Mouse::list,mouseButtonHold(event.mouseButton),Input::Mouse::iterating);
                }

            // STEP LOOP
            dt = clock.restart();
            stepBefore(dt);
            ___APP_EVENT_LOOP(Active::list,step(dt),Active::iterating);
            ___APP_EVENT_LOOP(ActiveEnd::list,stepEnd(dt),ActiveEnd::iterating);
            stepAfter(dt);

            // DRAW LOOP

            win->clear();

            for(auto i=ScreenDrawable::list.first();i;i=i->n) {
                win->setView(*i->item->_view);
                i->item->drawScreenBack(*win,dt);
            }

            float l,r,t,b;
            for(auto &vw: viewStack) {
                win->setView(*((sf::View*)vw));
                vw->box(l,t,r,b);
                int x1 = int(floor((l-128) / DRAWABLE_REGION_SIZE)),
                    y1 = int(floor((t-128) / DRAWABLE_REGION_SIZE)),
                    x2 = int(ceil((r+128) / DRAWABLE_REGION_SIZE)),
                    y2 = int(ceil((b+128) / DRAWABLE_REGION_SIZE));

                drawStack.clear();

                for(int y=y1;y<y2;y++)
                for(int x=x1;x<x2;x++) {
                    BaseDrawable::grid.iterateCellBegin(x,y);
                    while(BaseDrawable* itm = BaseDrawable::grid.iterateCellNext())
                        drawStack.add(itm->_depth,itm);
                }

                ___APP_DRAW_LOOP(WorldDrawable::list,drawWorldBack(*win,dt));

                ___APP_DRAW_LOOP(drawStack,draw(*win,dt));

                ___APP_DRAW_LOOP(WorldDrawable::list,drawWorldFront(*win,dt));
            }

            for(auto i=ScreenDrawable::list.first();i;i=i->n) {
                win->setView(*i->item->_view);
                i->item->drawScreenFront(*win,dt);
            }

            win->display();
            #undef ___APP_EVENT_LOOP
            #undef ___APP_DRAW_LOOP
        }
    }

    View& App::view(int ind) {
        return *viewStack[ind];
    }

    int App::addView(View* vw) {
        int ind = viewStack.size();
        viewStack.push_back(vw);
        return ind;
    }

    void App::removeView(View* vw) {
        auto end = viewStack.end(),
             pos = std::find(viewStack.begin(),end,vw);
        if(pos != end)
            viewStack.erase(pos);
    }

    int App::viewIndex(View* vw) {
        auto beg = viewStack.begin(), end = viewStack.end(),
             pos = std::find(beg,end,vw);
        if(pos == end) return -1;
        return std::distance(beg,pos);
    }

    void App::screenSize(int width, int height){
        win->setSize(sf::Vector2u(width, height));
    }

    std::vector<std::string>& App::parameters() {
        return params;
    }

    void App::setAppParams(int argc,char** argv) {
        params.reserve(argc);
        for(int i=0;i<argc;i++)
            params.push_back(argv[i]);
    }

    bool App::setWindowTransparency(unsigned char alpha){
        HWND hWnd = win->getSystemHandle();
        SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        SetLayeredWindowAttributes(hWnd, 0, alpha, LWA_ALPHA);
        return true;
    }
    std::vector<std::string> App::params;

}
