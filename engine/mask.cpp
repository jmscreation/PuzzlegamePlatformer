#include "engine.h"

namespace Engine {

    Mask::Mask(int type,float x,float y): type(type), x(x), y(y) {}
    Mask::~Mask() {}

    void Mask::initCircle(float radius,float xo,float yo) {
        xorig = xo; yorig = yo;
        circle = {radius};
    }

    void Mask::initRect(float w,float h,float xo,float yo) {
        xorig = xo; yorig = yo;
        rect = {w,h};
    }

    bool Mask::collidesWith(Mask& m) {
        switch(type | m.type) {
            case _MASK_TEST_CIRCLECIRCLE:
                {
                    int dx = (m.x + m.circle.radius - m.xorig) - (x + circle.radius - xorig),
                        dy = (m.y + m.circle.radius - m.yorig) - (y + circle.radius - yorig),
                        dr = m.circle.radius + circle.radius;
                    return dx*dx + dy*dy < dr*dr;
                }
            case _MASK_TEST_RECTRECT:
                {
                    float _x = x-xorig, _y = y-yorig, _mx = m.x-m.xorig, _my = m.y-m.yorig;
                    return _x < _mx+m.rect.w && _x+rect.w > _mx
                        && _y < _my+m.rect.h && _y+rect.h > _my;
                }
            case _MASK_TEST_CIRCLERECT:
                {
                    float cx = type==MASK_CIRCLE ? x-xorig+circle.radius : m.x-m.xorig+m.circle.radius,
                          cy = type==MASK_CIRCLE ? y-yorig+circle.radius : m.y-m.yorig+m.circle.radius,
                          rad = type==MASK_CIRCLE ? circle.radius : m.circle.radius,
                          L = type==MASK_CIRCLE ? m.x-m.xorig : x-xorig,
                          T = type==MASK_CIRCLE ? m.y-m.yorig : y-yorig,
                          R = type==MASK_CIRCLE ? L+m.rect.w : L+rect.w,
                          B = type==MASK_CIRCLE ? T+m.rect.h : T+rect.h;

                    // test collision with sides
                    if(cx > L && cx < R) return cy+rad > T && cy-rad < B;
                    if(cy > T && cy < B) return cx+rad > L && cx-rad < R;

                    // test collision with corners
                    float corx = (cx < L ? L : R) - cx, cory = (cy < T ? T : B) - cy;
                    return corx*corx + cory*cory < rad*rad;
                }
        }
        return false;
    }

}