#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>

constexpr int WIDTH = 1800;
constexpr int HEIGHT = 800;
constexpr int BLOCK_SIZE = 10;
constexpr int DEFAULT_SPEED = 50;

constexpr int XBLOCKS = WIDTH/BLOCK_SIZE;
constexpr int YBLOCKS = HEIGHT/BLOCK_SIZE;

class Life {
private:
    
    std::unique_ptr<sf::RenderWindow> window;
    sf::VideoMode videoMode;
    sf::Event ev;
    bool isWindowFocused;

    // Enviroment
    std::vector<std::vector<sf::RectangleShape>> blocks;
    std::array<std::array<bool, YBLOCKS>, XBLOCKS> blocksState;
    std::array<std::array<bool, YBLOCKS>, XBLOCKS> nextBlocksState;

    void initVariables();
    void initWindow();
    void initEnviroment();
    void createGrid();

    void setBlockFromMouse(sf::Vector2f point, bool state);
    void setBlock(sf::Vector2f indexes, bool state);
    void fillBlock(sf::Vector2f indexes, bool state);
    void drawBlocks();

    
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