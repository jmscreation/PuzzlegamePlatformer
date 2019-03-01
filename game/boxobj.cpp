#include "includes.h"
using namespace Engine;
using namespace std;

Animation* BoxObject::animation;

BoxObject::BoxObject(float xx, float yy):
    GameObject(animation,xx,yy), xprev(xx), yprev(yy) {
        mask = new Mask(MASK_RECT, xx, yy);
        mask->initRect(32,32,16,16);
        sprite.setOrigin(16,16);
        gravity = 32;
        fric = 0.9;
        dir = (rand()%4)*90;
        depth(0.5);
    }

void BoxObject::draw(sf::RenderWindow &win, sf::Time &delta){
    float spd = delta.asSeconds();
    float GameSpeed = PuzzleGame::current().GameSpeed;

    vcx += cos(dir*M_PI/180)*gravity * GameSpeed;
    vcy += -sin(dir*M_PI/180)*gravity * GameSpeed;

    vcx*=fric + (1-fric)*(1-GameSpeed);
    vcy*=fric + (1-fric)*(1-GameSpeed);


    move(vcx*spd*GameSpeed,vcy*spd*GameSpeed);


    vector<BaseCollidable*> collide_list;
    collide_list.reserve(4);
    beginCollision();
    while(auto obj = iterateCollidable()) {
        int type = obj->type();
        if(type == OBJ_WALL || type == OBJ_BOX || type == OBJ_PLAYER){
            BaseCollidable& wall = *obj;
            if(!mask->collidesWith(*wall.mask)) continue;
            if(fabs(x() - wall.x()) < wall.mask->rect.w/2 || fabs(y() - wall.y()) < wall.mask->rect.h/2) collide_list.push_back(&wall);
        }
    }
    beginCollision();
    while(auto obj = iterateCollidable()) {
        int type = obj->type();
        if(type == OBJ_WALL || type == OBJ_BOX || type == OBJ_PLAYER){
            BaseCollidable& wall = *obj;
            if(!mask->collidesWith(*wall.mask)) continue;
            if(fabs(x() - wall.x()) >= wall.mask->rect.w/2 && fabs(y() - wall.y()) >= wall.mask->rect.h/2) collide_list.push_back(&wall);
        }
    }

    for(int i=0;i<collide_list.size();i++){
        BaseCollidable* obj = collide_list[i];
        switch(obj->type()){
            case OBJ_WALL:
                {
                    WallObject& wall = *((WallObject*)obj);
                    if(!mask->collidesWith(*wall.mask)) continue;
                    float vx = xprev - wall.x(), vy = yprev - wall.y();
                    if((vx + vy < 0) ^ (vx > vy)){
                        if(vx<0) {vcx = 0;if(dir==0) vcy *= fric/2;} //left
                        if(vx>0) {vcx = 0;if(dir==180) vcy *= fric/2;} //right
                        if(dir == 270 || dir == 90) {x(xprev);continue;}
                        x(vx<0 ? wall.x() - wall.mask->rect.w/2 - mask->rect.w/2 : wall.x() + wall.mask->rect.w/2 + mask->rect.w/2);
                        x(xprev);
                    } else{
                        if(vy<0) {vcy = 0;if(dir==270) vcx *= fric/2;} //top
                        if(vy>0) {vcy = 0;if(dir==90) vcx *= fric/2;} //bottom
                        if(dir == 180 || dir == 0) {y(yprev);continue;}
                        y(yprev);
                        y(vy<0 ? wall.y() - wall.mask->rect.h/2 - mask->rect.h/2: wall.y() + wall.mask->rect.h/2 + mask->rect.h/2);
                    }
                }
                break;
            case OBJ_BOX:
                {
                    BoxObject& wall = *((BoxObject*)obj);
                    float vx = xprev - wall.x(), vy = yprev - wall.y();
                    if(!mask->collidesWith(*wall.mask)) continue;
                    if((vx + vy < 0) ^ (vx > vy)){
                        vcy = wall.vcy;
                        wall.vcx += vcx*0.9;vcx = 0;
                        if(dir == 270 || dir == 90) {x(xprev);continue;}
                        x(vx<0 ? wall.x() - wall.mask->rect.w/2 - mask->rect.w/2 : wall.x() + wall.mask->rect.w/2 + mask->rect.w/2);
                    } else{
                        vcx = wall.vcx;
                        wall.vcy += vcy*0.9;vcy = 0;
                        if(dir == 180 || dir == 0) {y(yprev);continue;}
                        y(vy<0 ? wall.y() - wall.mask->rect.h/2 - mask->rect.h/2: wall.y() + wall.mask->rect.h/2 + mask->rect.h/2);
                    }
                }
                break;
            case OBJ_PLAYER:
                {
                    PlayerObject& wall = *((PlayerObject*)obj);
                    if(!mask->collidesWith(*wall.mask)) continue;
                    float vx = xprev - wall.x(), vy = yprev - wall.y();
                    if((vx + vy < 0) ^ (vx > vy)){
                        if(wall.newdir != 0 && wall.newdir != 180) {wall.vcx += vcx*1.1;vcx*=0.6;} else vcx = 0;
                        if(dir == 270 || dir == 90) {x(xprev);continue;}
                    } else{
                        if(wall.newdir != 270 && wall.newdir != 90) {wall.vcy += vcx*1.1;vcy*=0.6;} else vcy = 0;
                        if(dir == 180 || dir == 0) {y(yprev);continue;}
                    }
                }
                break;
        }
    }
    xprev = x(); yprev = y();
    BaseDrawable::draw(win, delta);//Inherit Draw Event
}
