#include "includes.h"

using namespace Engine;
using namespace std;

static sf::Texture* playerTexture, *wallTexture, *itemTexture, *boxTexture;

PuzzleGame::PuzzleGame(): player(NULL), isFullscreen(true), FPS(60), targetGameSpeed(1), GameSpeed(1),
                        App(640, 480, "PuzzleGame"), soundList({}) {

    srand(time(NULL));
    screenMode(false);
    setWindowTransparency(0);
    //Load Textures
    AudioResources* aresources = new AudioResources({PATH_SND_MUSIC, PATH_SND_ITEM, PATH_SND_JUMP, PATH_SND_SWOOSH});
    TextureResources* resources = new TextureResources({PATH_PLAYER, PATH_WALL, PATH_ITEM, PATH_BOX, PATH_BACKGROUND});

    aresources->failureSubstitute(PATH_SND_ERROR);
    sf::Image error;error.create(200,200,sf::Color::White);
    resources->failureSubstitute(error);

    playerTexture = resources->getTexture(0);
    wallTexture = resources->getTexture(1);
    itemTexture = resources->getTexture(2);
    boxTexture = resources->getTexture(3);
    new Background(resources->getTexture(4), *this);

    snd_music = aresources->getSound(0);snd_music->defaultVolume(0.9);
    snd_item = aresources->getSound(1);snd_item->defaultDestroy(true);snd_item->defaultVolume(1.1);
    snd_jump = aresources->getSound(2);snd_jump->defaultDestroy(true);snd_jump->defaultVolume(0.5);
    snd_swoosh = aresources->getSound(3);snd_swoosh->defaultDestroy(true);snd_swoosh->defaultVolume(2.2);

    playing_music = snd_music->loop();

    PlayerObject::animation = new Animation(playerTexture,{3,3,3,3},0,0,48,64,3);
    WallObject::animation = new Animation(wallTexture,{36},0,0,200,200,6);
    ItemObject::animation = new Animation(itemTexture,{3051},0,0,32,32,64);
    BoxObject::animation = new Animation(boxTexture,{1},0,0,32,32,1);

    int s = 100;

    player = new PlayerObject(16, 16);
    for(int i=0;i<s;i++){
        float xx, yy;
        xx = (i-s/2)*32 + 16;
        yy = -(s/2)*32 + 16;
        new WallObject(xx, yy);
        yy = (s/2)*32 + 16;
        new WallObject(xx, yy);

        yy = (i-s/2)*32 + 16;
        xx = -(s/2+1)*32 + 16;
        new WallObject(xx, yy);
        xx = (s/2)*32 + 16;
        new WallObject(xx, yy);

    }

    for(int i=0;i<2000;i++){
        float xx = (rand()%s - s/2)*32 + 16, yy = (rand()%s - s/2)*32 + 16;
        if(instancePosition(xx, yy) != NULL) continue;
        new WallObject(xx, yy);
    }
    for(int i=0;i<2000;i++){
        float xx = (rand()%s - s/2)*32 + 16, yy = (rand()%s - s/2)*32 + 16;
        if(instancePosition(xx, yy) != NULL) continue;
        new ItemObject(xx, yy);
    }
    for(int i=0;i<500;i++){
        float xx = (rand()%s - s/2)*32 + 16, yy = (rand()%s - s/2)*32 + 16;
        if(instancePosition(xx, yy) != NULL) continue;
        new BoxObject(xx, yy);
    }
    view().zoom(1.1);
    /*
    view().setViewport(sf::FloatRect(0.5,0,0.5,0.5));
    addView(new View());
    view(1).setViewport(sf::FloatRect(0,0,0.5,0.5));
    view(1).setSize(640, 480);
    */
}

PuzzleGame::~PuzzleGame() {
    delete player;
}

void PuzzleGame::screenMode(bool fullscreen){
    if(fullscreen){
        if(!isFullscreen) window().create(sf::VideoMode(1024, 768), "", sf::Style::Fullscreen);
        isFullscreen = true;
    } else {
        if(isFullscreen) window().create(sf::VideoMode(1024, 768), "", sf::Style::Titlebar | sf::Style::Close);
        isFullscreen = false;
    }
    window().setFramerateLimit(FPS);
}

bool PuzzleGame::screenMode(){
    return isFullscreen;
}

void PuzzleGame::stepBefore(sf::Time &delta){
    static int alpha = 0;
    if(alpha < 100){
        alpha++;
        setWindowTransparency(255*alpha/100);
    }
    static sf::Clock timer;
    view().setCenter(player->x(), player->y());
    view().setRotation(270-((PlayerObject*)player)->dir);
    window().setTitle(to_string(1/delta.asSeconds()));
    if((fabs(GameSpeed-targetGameSpeed) > 0.01) && timer.getElapsedTime().asMilliseconds() > 30){
        GameSpeed += (GameSpeed<targetGameSpeed)?0.01:-0.01;
        playing_music->loop(GameSpeed/1.f);
        timer.restart();
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
        for(int i=0;i<=100;i++){
            //MoveWindow(window().getSystemHandle(), rand()%1048, rand()%800, rand()%1024, rand()%768, true);
            setWindowTransparency(255*(100-i)/100);
            Sleep(5);
        }
        exit(0);
    }
}

void PuzzleGame::stepAfter(sf::Time &delta){
    for(SoundInstance* &snd : soundList){
        if(snd == NULL) continue;
        if(!snd->isPlaying()) {delete snd; snd = NULL; continue;}
        snd->play(GameSpeed);
    }
    for(unsigned int i=soundList.size();i--;){
        if(soundList[i] == NULL) soundList.erase(soundList.begin() + i);
    }
}

GameObject* PuzzleGame::instancePosition(float xx, float yy){
    BaseCollidable::beginPosition(xx,yy);
    while(auto obj = BaseCollidable::iteratePosition()){
        if(obj->x() == xx && obj->y() == yy){
            return (GameObject*)obj;
        }
    }
    return NULL;
}
