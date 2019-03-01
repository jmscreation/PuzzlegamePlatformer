#include "engine.h"

namespace Engine {

    GameObject::GameObject(Animation* ani,float x,float y,float depth):
        BaseDrawable(ani,x,y,depth), BaseCollidable(x,y), _x(x), _y(y) {
        sprite.setPosition(_x,_y);
    }

    GameObject::~GameObject() {
        BaseCollidable::remove(_x,_y);
        BaseDrawable::remove(_x,_y);
    }

    void GameObject::move(float dx,float dy) {
        BaseCollidable::move(_x,_y,_x+dx,_y+dy);
        BaseDrawable::move(_x,_y,_x+dx,_y+dy);
        _x += dx; _y += dy;
        sprite.setPosition(_x,_y);
    }

    void GameObject::setPosition(float x,float y) {
        BaseCollidable::move(_x,_y,x,y);
        BaseDrawable::move(_x,_y,x,y);
        _x = x; _y = y;
        sprite.setPosition(_x,_y);
    }

    void GameObject::x(float x) {
        BaseCollidable::move(_x,_y,x,_y);
        BaseDrawable::move(_x,_y,x,_y);
        _x = x;
    }

    void GameObject::y(float y) {
        BaseCollidable::move(_x,_y,_x,y);
        BaseDrawable::move(_x,_y,_x,y);
        _y = y;
    }

}