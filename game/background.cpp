#include "includes.h"

using namespace std;
using namespace Engine;

Background::Background(sf::Texture* backgroundTexture, PuzzleGame &currentApp): backTexture(backgroundTexture), game(currentApp) {
    view().setSize(game.view().getSize());
    backTexture->setRepeated(true);
    background.setTexture(*backTexture);
    background.setTextureRect(sf::IntRect(0,0,6000,6000));
    background.setOrigin(3000, 3000);
    background.setScale(0.5, 0.5);
    background.setPosition(view().getCenter());
}

Background::~Background(){
}

void Background::drawScreenBack(sf::RenderWindow &win, sf::Time &delta){
    view().setRotation(game.view().getRotation());
    background.setTextureRect(sf::IntRect(game.player->x(),game.player->y(),6000, 6000));
    win.draw(background);
}

void Background::drawScreenFront(sf::RenderWindow &win, sf::Time &delta){
}
