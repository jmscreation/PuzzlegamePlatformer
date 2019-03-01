#ifndef __PLAYER_OBJECT__
#define __PLAYER_OBJECT__

class PlayerObject: public Engine::GameObject, public Engine::Input::Keyboard, public Engine::Active {
public:
    PlayerObject(float x, float y);
    virtual int type() { return OBJ_PLAYER; }
    float newdir, dir, vcx, vcy;
private:
    static Engine::Animation* animation;

    float xprev, yprev, mask_width, mask_height;
    float gravity, fric, sfric;
    bool jump, DrawMask;

    void keyHold(sf::Event::KeyEvent &key);
    void keyPress(sf::Event::KeyEvent &key);
    void keyRelease(sf::Event::KeyEvent &key);
    void step(sf::Time &delta);
    void draw(sf::RenderWindow &win, sf::Time &delta);

    friend class PuzzleGame;
    friend class ItemObject;
};

#endif // __PLAYER_OBJECT__
