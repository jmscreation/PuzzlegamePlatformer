#ifndef BOXOBJ_H_INCLUDED
#define BOXOBJ_H_INCLUDED

class BoxObject: public Engine::GameObject, public Engine::Active {
public:
    BoxObject(float x, float y);
    virtual int type() { return OBJ_BOX; }
    int dist;
    float vcx, vcy;
private:
    static Engine::Animation* animation;

    float xprev, yprev;
    float dir, moment, gravity, fric;

    //void step(sf::Time &delta);
    void draw(sf::RenderWindow &win, sf::Time &delta);
    friend PuzzleGame;
};

#endif // BOXOBJ_H_INCLUDED
