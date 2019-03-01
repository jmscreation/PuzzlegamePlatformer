#ifndef __PUZZLEGAME_H__
#define __PUZZLEGAME_H__

class PuzzleGame: public Engine::App {
public:
    PuzzleGame();
    virtual ~PuzzleGame();
    static PuzzleGame& current(){return * (PuzzleGame*)&App::current();}
    Engine::SoundBuffer *snd_item, *snd_music, *snd_jump, *snd_swoosh;
    virtual void screenMode(bool fullscreen);
    virtual bool screenMode();

protected:
    Engine::GameObject* player;
private:
    virtual void stepBefore(sf::Time &delta);
    virtual void stepAfter(sf::Time &delta);
    virtual Engine::GameObject* instancePosition(float xx, float yy);
    Engine::SoundInstance* playing_music;
    bool isFullscreen;
    int FPS;
    float targetGameSpeed, GameSpeed;
friend class Background;
friend class ItemObject;
friend class BoxObject;
friend class WallObject;
friend class PlayerObject;
};



#endif // __PUZZLEGAME_H__
