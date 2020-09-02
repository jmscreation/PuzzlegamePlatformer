#include "includes.h"
using namespace Engine;
using namespace std;
#define SPEED 148
#define VDIR(x, mx, mn) ((1-fabs(sin(x*DTR))-fabs(cos(x*DTR)))/0.414+1) * (mx-mn) + mn

Animation* PlayerObject::animation;

PlayerObject::PlayerObject(float xx, float yy):
    DrawMask(false),jump(false),
    GameObject(animation,xx,yy), xprev(xx), yprev(yy) {

    mask_width = 16, mask_height = 16;
    mask = new Mask(MASK_RECT, xx, yy);
    mask->initRect(mask_width,mask_height,mask_width/2,mask_height/2);
    sprite.setOrigin(24,54);
    sprite.setScale(32.0/48.0, 32.0/48.0);
    sprite.frameDelay(25);

    newdir = 270;
    dir = 270;
    vcx = vcy = 0;
    gravity = 32;
    fric = 0.65;
    sfric = 0.95;
    depth(0.6);

}

void PlayerObject::step(sf::Time &delta){
    float spd = delta.asSeconds() * PuzzleGame::current().GameSpeed;
    if(spd >= 0.15) spd = 0.15; //Prevent player from falling through walls when game freezes
    xprev = x(); yprev = y();
    sprite.next();

    if(dir != newdir){
        float dist = fabs(newdir-dir);
        dir += (dist<180 ? 1 : -1)*(newdir>dir?1:-1) * (dist<180 ? dist : 360-dist)/16*spd*75;
        dir = dir-floor(dir/360)*360;
        if(dist <= 1 || dist >= 359) dir = newdir;
        sprite.setRotation(270-dir);
    }

    vcx += cos(newdir*DTR)*gravity*60 * spd;
    vcy += -sin(newdir*DTR)*gravity*60 * spd;
    float termVelFac = 60 / gravity;
    vcx *= termVelFac / (spd + termVelFac);
    vcy *= termVelFac / (spd + termVelFac);
    move(vcx*spd,vcy*spd);
    jump = true;

    vector<BaseCollidable*> collide_list;
    collide_list.reserve(4);
    beginCollision();
    while(auto obj = iterateCollidable()) {
        int type = obj->type();
        if(type == OBJ_WALL || type == OBJ_BOX){
            BaseCollidable& wall = *obj;
            if(!mask->collidesWith(*wall.mask)) continue;
            if(fabs(x() - wall.x()) < wall.mask->rect.w/2 || fabs(y() - wall.y()) < wall.mask->rect.h/2) collide_list.push_back(&wall);
        }
    }
    //beginCollision();// DON'T DO THIS FOR SOME REASON
    //while(auto obj = iterateCollidable()) {
    //    int type = obj->type();
    //    if(type == OBJ_WALL){
    //        WallObject& wall = *((WallObject*)obj);
    //        if(!mask->collidesWith(*wall.mask)) continue;
    //        if(fabs(x() - wall.x()) >= wall.mask->rect.w/2 && fabs(y() - wall.y()) >= wall.mask->rect.h/2) collide_list.push_back(&wall);
    //    }
    //}

    for(int i=0;i<collide_list.size();i++){
        BaseCollidable* obj = collide_list[i];
        if(obj->mask == NULL) continue;
        switch(obj->type()){
            case OBJ_WALL:
                {
                    WallObject& wall = *((WallObject*)obj);
                    if(!mask->collidesWith(*wall.mask)) continue;
                    float vx = xprev - wall.x(), vy = yprev - wall.y();

                    if((vx + vy < 0) ^ (vx > vy)){
                        if(vx<0) {if(dir == 0) {vcx = 0;jump = false;x(wall.x()-wall.mask->rect.w/2-8);vcy*=fric;continue;}if(vcx>0){vcx = 0;vcy*=sfric;}} //left
                        if(vx>0) {if(dir == 180) {vcx = 0;jump = false;x(wall.x()+wall.mask->rect.w/2+8);vcy*=fric;continue;}if(vcx<0)vcx = 0;vcy*=sfric;} //right
                        setPosition(vx<0 ? wall.x() - wall.mask->rect.w/2-mask_width/2 : wall.x() + wall.mask->rect.w/2+mask_width/2, y());
                    } else{
                        if(vy<0) {if(dir == 270) {vcy = 0;jump = false;y(wall.y()-wall.mask->rect.h/2-8);vcx*=fric;continue;}if(vcy>0)vcy = 0;vcx*=sfric;} //top
                        if(vy>0) {if(dir == 90) {vcy = 0;jump = false;y(wall.y()+wall.mask->rect.h/2+8);vcx*=fric;continue;}if(vcy<0)vcy = 0;vcx*=sfric;} //bottom

                        setPosition(x(), vy<0 ? wall.y() - wall.mask->rect.h/2-mask_height/2 : wall.y() + wall.mask->rect.h/2+mask_height/2);
                    }

                }
                break;
            case OBJ_BOX:
                {
                    BoxObject& wall = *((BoxObject*)obj);
                    if(!mask->collidesWith(*wall.mask)) continue;
                    float vx = xprev - wall.x(), vy = yprev - wall.y();
                    if((vx + vy < 0) ^ (vx > vy)){

                        if(vx<0) {wall.vcx += vcx*0.9;if(dir == 0) {vcx = 0;jump = false;x(wall.x()-wall.mask->rect.w/2-8);vcy*=fric;continue;}if(vcx>0){vcx = 0;vcy*=sfric;}} //left
                        if(vx>0) {wall.vcx += vcx*0.9;if(dir == 180) {vcx = 0;jump = false;x(wall.x()+wall.mask->rect.w/2+8);vcy*=fric;continue;}if(vcx<0)vcx = 0;vcy*=sfric;} //right
                        setPosition(vx<0 ? wall.x() - wall.mask->rect.w/2-mask_width/2 : wall.x() + wall.mask->rect.w/2+mask_width/2, y());
                    } else{
                        if(vy<0) {wall.vcy += vcy*0.9;if(dir == 270) {vcy = 0;jump = false;y(wall.y()-wall.mask->rect.h/2-8);vcx*=fric;continue;}if(vcy>0)vcy = 0;vcx*=sfric;} //top
                        if(vy>0) {wall.vcy += vcy*0.9;if(dir == 90) {vcy = 0;jump = false;y(wall.y()+wall.mask->rect.h/2+8);vcx*=fric;continue;}if(vcy<0)vcy = 0;vcx*=sfric;} //bottom

                        setPosition(x(), vy<0 ? wall.y() - wall.mask->rect.h/2-mask_height/2 : wall.y() + wall.mask->rect.h/2+mask_height/2);
                    }
                }
                break;
        }
    }

    beginCollision();
    while(auto obj = iterateCollidable()) {
        if(obj->mask == NULL) continue;
        switch(obj->type()) {
            case OBJ_ITEM:
                {
                    ItemObject& itm = *((ItemObject*)obj);
                    if(!mask->collidesWith(*itm.mask)) continue;

                    PositionIterator It;
                    beginCollision(It);
                    while(BaseCollidable* other = iterateCollidable(It)) {
                        if(other->type() == OBJ_ITEM) {
                            ItemObject &o = *(ItemObject*)other;
                            if(abs(o.x() - itm.x()) < 50 && abs(o.y() - itm.y()) < 50) {
                                o.get_item();
                                //cout << o.getID() << endl;
                            }
                        }
                    }
                    itm.get_item(); //returns what item you got
                }
                break;
        }
    }
}
void PlayerObject::draw(sf::RenderWindow &win, sf::Time &delta){
    BaseDrawable::draw(win, delta);
    if(DrawMask){
        sprite.setOrigin(sprite.getOrigin().x + sf::Keyboard::isKeyPressed(sf::Keyboard::D) - sf::Keyboard::isKeyPressed(sf::Keyboard::A), sprite.getOrigin().y + sf::Keyboard::isKeyPressed(sf::Keyboard::W) - sf::Keyboard::isKeyPressed(sf::Keyboard::S));
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) cout << "X: " << sprite.getOrigin().x << " Y: " << sprite.getOrigin().y << "                          \r";
        sf::Image PixelImg = sf::Image();
        sf::RectangleShape MaskImg = sf::RectangleShape(sf::Vector2f(mask_width, mask_height));
        PixelImg.create(1,1,sf::Color::White);
        sf::Texture PixelTex = sf::Texture();
        PixelTex.loadFromImage(PixelImg);
        sf::Sprite Pixel = sf::Sprite(PixelTex);
        Pixel.setPosition(x(),y());
        MaskImg.setFillColor(sf::Color(255,0,0,120));
        MaskImg.setPosition(mask->x, mask->y);
        MaskImg.setOrigin(mask->xorig, mask->yorig);
        win.draw(Pixel); //DRAW THE X AND Y COORD FOR OUR GAME OBJECT
        win.draw(MaskImg); //DRAW THE X AND Y COORD FOR OUR MASK
    }
}
void PlayerObject::keyRelease(sf::Event::KeyEvent &key){
    if(key.code == sf::Keyboard::Key::Left || key.code == sf::Keyboard::Key::Right){
        if(sprite.animation() != 0);
            sprite.animation(0,0);
    }
}

void PlayerObject::keyPress(sf::Event::KeyEvent &key){
    if(key.code == sf::Keyboard::Key::M){
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl)) DrawMask = !DrawMask;
    }

    if(key.code == sf::Keyboard::F4){
        PuzzleGame::current().screenMode(!PuzzleGame::current().screenMode());
    }

    if(key.code == sf::Keyboard::Tab){
        PuzzleGame::current().targetGameSpeed = (PuzzleGame::current().targetGameSpeed == 1) ? 0.2 : 1;
    }
}

void PlayerObject::keyHold(sf::Event::KeyEvent &key){
    PuzzleGame &app = PuzzleGame::current();

    float spd = app.delta().asSeconds() * app.GameSpeed;
    float d = 0;
    if(key.code == sf::Keyboard::Key::Left){
        d = -90;
        if(sprite.animation() != 1)
            sprite.animation(1,0);
    } else
        if(key.code == sf::Keyboard::Key::Right){
            d = 90;
            if(sprite.animation() != 2)
                sprite.animation(2,0);
        }
    if(d!=0){
        float vx, vy;
        if(vcx > -6000*spd && vcx < 6000*spd) vcx += cos((dir+d)*DTR)*SPEED;
        if(vcy > -6000*spd && vcy < 6000*spd) vcy += -sin((dir+d)*DTR)*SPEED;
    }
    if(key.code == sf::Keyboard::Key::Down){
        switch((int)dir){
            case 0:
            case 90:
            case 180:
            case 270:
                app.soundList.push_back(app.snd_swoosh->play(false));
                app.soundList.back()->play(app.GameSpeed);
                newdir += 90;
                newdir = newdir - floor(newdir/360)*360;
            break;
        }
    }
    if(key.code == sf::Keyboard::Key::Up && !jump){
        float GameSpeed = app.GameSpeed;

        app.soundList.push_back(app.snd_jump->play(false));
        app.soundList.back()->play(GameSpeed);

        vcx += cos((dir+180)*DTR)*750;
        vcy += -sin((dir+180)*DTR)*750;
    }
}
