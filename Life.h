#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "BlocksGrid.h"

constexpr int WIDTH = 1800;
constexpr int HEIGHT = 800;
constexpr int BLOCK_SIZE = 10;
constexpr int DEFAULT_SPEED = 50;

constexpr int XBLOCKS = WIDTH/BLOCK_SIZE;
constexpr int YBLOCKS = HEIGHT/BLOCK_SIZE;

class Life {
private:

    std::shared_ptr<sf::RenderWindow> window;
    sf::VideoMode videoMode;
    sf::Event ev;
    bool isWindowFocused;

    void initVariables();
    void initWindow();
    void initEnviroment();
    
    std::unique_ptr<BlocksGrid> blocksGrid;
    
    void scanBlocks();
    int scanNeighbourBlocks(sf::Vector2f indexes);

    bool playEnabled;
    int speed;

    sf::Clock clock;

public:
    Life();
    virtual ~Life();

    const bool running() const;

    void pollEvents();
    void update();
    void render();
};