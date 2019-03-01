

#include "includes.h"
using namespace Engine;

Animation* WallObject::animation;

WallObject::WallObject(int xx, int yy): GameObject(animation,xx,yy){
    mask = new Mask(MASK_RECT, xx, yy);
    mask->initRect(32,32,16,16);
    sprite.setScale(32.0/200.0, 32.0/200.0);
    sprite.setOrigin(100,100);
    sprite.animation(0,rand()%36);
    depth(0.5);
}
