#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__


class Background: public Engine::ScreenDrawable {
    public:
        Background(sf::Texture* backgroundTexture, PuzzleGame &currentApp);
        virtual ~Background();

    private:
        sf::Texture* backTexture;
        sf::Sprite background;
        PuzzleGame &game;
        void drawScreenBack(sf::RenderWindow &win, sf::Time &delta);
        void drawScreenFront(sf::RenderWindow &win, sf::Time &delta);
};

#endif // __BACKGROUND_H__
