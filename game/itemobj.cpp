

#include "includes.h"
using namespace Engine;
using namespace std;

Animation* ItemObject::animation;
sf::Clock ItemObject::timer;

ItemObject::ItemObject(float xx, float yy):
    GameObject(animation,xx,yy) {
        mask = new Mask(MASK_RECT, xx, yy);
        mask->initRect(16,16,8,8);
        sprite.setOrigin(16,16);
        sprite.animation(0,rand()%3000);
        depth(1);
        xstart = x();
        ystart = y();
        alpha = 255;
        chain = -1;
        gather = false;
        item_type = rand()%4;
        offset = rand()%100;
    }

void ItemObject::draw(sf::RenderWindow &win, sf::Time &delta){
    float spd = delta.asSeconds();
    float dir = ((PlayerObject*)PuzzleGame::current().player)->dir;
    sprite.setRotation(270-dir);
    float GameSpeed = PuzzleGame::current().GameSpeed;
    if(alpha > 0) {
        if(!gather){
            float vcx, vcy;
            vcx = cos((dir)*M_PI/180);
            vcy = -sin((dir)*M_PI/180);
            float len = sin(((float)timer.getElapsedTime().asMilliseconds()/300 + offset))*8;
            setPosition(xstart + vcx*len, ystart + vcy*len);
        } else {
            float vcx, vcy;
            float len = alpha/130;
            vcx = cos((dir+180)*M_PI/180);
            vcy = -sin((dir+180)*M_PI/180);

            move(vcx*len*GameSpeed,vcy*len*GameSpeed);
            alpha -= 160*spd * GameSpeed;
        }
    } else {delete this;return;}

    if(chain > 0 && collectMe.getElapsedTime().asSeconds() > chain){
        get_item();
        chain = -1;
    }

    sprite.setColor(sf::Color(255,255,255,alpha));
    BaseDrawable::draw(win, delta);//Inherit Draw Event
}

void ItemObject::get_item_collect(int distance) {
    if(!gather && !(chain > 0)){
        chain = (float(distance + 1) + float((rand() % 1000)/100))/8;
        collectMe.restart();

        PositionIterator It;
        beginCollision(It);
        while(BaseCollidable* other = iterateCollidable(It)) {
            if(other->type() == OBJ_ITEM) {
                ItemObject &o = *(ItemObject*)other;
                if(abs(o.x() - x()) < 80 && abs(o.y() - y()) < 80) {
                    o.get_item_collect(distance + 1);
                }
            }
        }

    }
}

int ItemObject::get_item() {
    if(!gather){
        PuzzleGame &app = PuzzleGame::current();
        app.soundList.push_back(app.snd_item->play(false));
        app.soundList.back()->play(app.GameSpeed);
        // cout << app.soundList.size() << endl;
        gather = true;
        return item_type;
    } else {
        return -1;
    }
}
