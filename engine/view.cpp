#include "engine.h"

namespace Engine {

    View::View(): sf::View() {}

    View::View(const sf::FloatRect &r): sf::View(r) {}

    View::View(const sf::Vector2f &c, const sf::Vector2f &s): sf::View(c,s) {}

    View::~View() {}

    void View::screenToWorld(int sx,int sy,float &wx,float &wy) {
        sf::Vector2f v = App::current().window().mapPixelToCoords(sf::Vector2i(sx,sy),*((sf::View*)this));
        wx = v.x; wy = v.y;
    }

    void View::worldToScreen(float wx,float wy,int &sx,int &sy) {
        sf::Vector2i v = App::current().window().mapCoordsToPixel(sf::Vector2f(wx,wy),*((sf::View*)this));
        sx = v.x; sy = v.y;
    }

    /*
        [ 0  4  8  12 ]
        [ 1  5  9  13 ]
        [ 2  6  10 14 ]
        [ 3  7  11 15 ]
    */
    float View::viewMouseX() {
        const float* m = getInverseTransform().getMatrix();
        const sf::IntRect &viewport = App::current().window().getViewport(*this);

        return m[0]*(-1.f + 2.f * ((float)Input::Mouse::_x - viewport.left) / viewport.width)
             + m[4]*(1.f - 2.f * ((float)Input::Mouse::_y - viewport.top) / viewport.height)
             + m[12];
    }

    float View::viewMouseY() {
        const float* m = getInverseTransform().getMatrix();
        const sf::IntRect &viewport = App::current().window().getViewport(*this);

        return m[1]*(-1.f + 2.f * ((float)Input::Mouse::_x - viewport.left) / viewport.width)
             + m[5]*(1.f - 2.f * ((float)Input::Mouse::_y - viewport.top) / viewport.height)
             + m[13];
    }

    void View::zoomAt(int sx,int sy,float factor) {
        sf::RenderWindow& win = App::current().window();
        sf::Vector2f v = win.mapPixelToCoords(sf::Vector2i(sx,sy),*((sf::View*)this));
        zoom(factor);
        v -= win.mapPixelToCoords(sf::Vector2i(sx,sy),*((sf::View*)this));
        move(v);
    }

    void View::box(float& l,float& t,float& r,float& b) {
        sf::RenderWindow& win = App::current().window();
        const sf::FloatRect& rect = getViewport();
        const sf::Vector2u wsize = win.getSize();

        int x1 = int(rect.left*float(wsize.x)),
            y1 = int(rect.top*float(wsize.y)),
            x2 = int((rect.left+rect.width)*float(wsize.x)),
            y2 = int((rect.top+rect.height)*float(wsize.y));

        const sf::Vector2f
            &A = win.mapPixelToCoords(sf::Vector2i(x1,y1)),
            &D = win.mapPixelToCoords(sf::Vector2i(x2,y2));

        if(getRotation() == 0) {
            l = A.x; r = D.x;
            t = A.y; b = D.y;
        } else {
            const sf::Vector2f
                &B = win.mapPixelToCoords(sf::Vector2i(x2,y1)),
                &C = win.mapPixelToCoords(sf::Vector2i(x1,y2));

            l = r = A.x; t = b = A.y;
            if(B.x < l) l = B.x; if(C.x < l) l = C.x; if(D.x < l) l = D.x;
            if(B.y < t) t = B.y; if(C.y < t) t = C.y; if(D.y < t) t = D.y;
            if(B.x > r) r = B.x; if(C.x > r) r = C.x; if(D.x > r) r = D.x;
            if(B.y > b) b = B.y; if(C.y > b) b = C.y; if(D.y > b) b = D.y;
        }
    }

}