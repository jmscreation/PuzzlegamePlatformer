#ifndef __ITEM_OBJECT__
#define __ITEM_OBJECT__

class ItemObject: public Engine::GameObject, public Engine::Active {
public:
    ItemObject(float x, float y);
    virtual int type() { return OBJ_ITEM; }
    int get_item() {if(!gather){(PuzzleGame::current().snd_item->play())->play(PuzzleGame::current().GameSpeed);gather = true;return item_type;} else return -1;}

private:
    static Engine::Animation* animation;
    static sf::Clock timer;

    void draw(sf::RenderWindow &win, sf::Time &delta);
    float xstart,ystart, offset, alpha;
    bool gather;
    int item_type;
    friend PuzzleGame;
};

#endif // __ITEM_OBJECT__
