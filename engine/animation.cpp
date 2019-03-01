#include "engine.h"

namespace Engine {

    Animation::Animation(sf::Texture* tex,std::vector<int>&& aniFrames,int x,int y,int w,int h,int sw):
        texture(tex), x(x), y(y), width(w), height(h), strip(sw) {

        int i = 0,l = aniFrames.size(), frame = 0;

        animationFrames.reserve(l);

        for(int i=0,l=aniFrames.size();i<l;i++) {
            animationFrames.push_back(Strip({frame, aniFrames[i]}));
            frame += aniFrames[i];
        }
    }

    Animation::~Animation() {}

    AnimatedSprite::AnimatedSprite(Animation* a):
        sf::Sprite(*a->texture), animator(a), ani(0), img(0), delay(1), d(1) {
        animation(ani,img);
    }

    AnimatedSprite::~AnimatedSprite() {}

    void AnimatedSprite::animation(int anim,int frame) {
        anim = anim < 0 ? 0 : anim >= animator->animationFrames.size() ? animator->animationFrames.size()-1 : anim;
        d = delay;
        ani = anim;
        int n = animator->animationFrames[ani].len;
        img = (frame % n + n) % n;
        frame = animator->animationFrames[ani].fr + img;

        setTextureRect(sf::IntRect(
            animator->x + frame % animator->strip * animator->width,
            animator->y + frame / animator->strip * animator->height,
            animator->width,
            animator->height
        ));
    }

    int AnimatedSprite::animation() {
        return ani;
    }

    void AnimatedSprite::frame(int frame) {
        animation(ani, frame);
    }

    int AnimatedSprite::frame() {
        return img;
    }

    void AnimatedSprite::next() {
        if(!--d) animation(ani, img+1);
    }

    void AnimatedSprite::prev() {
        if(!--d) animation(ani, img-1);
    }

    void AnimatedSprite::frameDelay(int del) {
        delay = del < 1 ? 1 : del;
        d = delay;
    }

    int AnimatedSprite::frameDelay() {
        return delay;
    }

}
