#ifndef __WALL_OBJECT__
#define __WALL_OBJECT__

class WallObject: public Engine::GameObject {
public:
    WallObject(int x, int y);
    virtual int type() { return OBJ_WALL; }

private:
    static Engine::Animation* animation;

    friend PuzzleGame;
};

#endif // __WALL_OBJECT__
