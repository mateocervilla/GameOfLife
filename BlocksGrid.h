#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <array>
#include <memory>

class BlocksGrid {
private:
    const int width;
    const int height;
    const int blockSize;
    bool isWindowFocused;

    std::vector<std::vector<sf::RectangleShape>> blocks;
    std::vector<std::vector<bool>> blocksState;
    std::vector<std::vector<bool>> nextBlocksState;
    
    std::shared_ptr<sf::RenderWindow> window;

    void fillBlock(sf::Vector2f indexes, bool state);

public:
    BlocksGrid(int width, int height, int blockSize, std::shared_ptr<sf::RenderWindow> window);
    virtual ~BlocksGrid();
    
    void createGrid();
    
    void setBlockFromMouse(sf::Vector2f point, bool state);
    void setBlock(sf::Vector2f indexes, bool state);
    void setNextBlockState(sf::Vector2f indexes, bool state);
    void setWindowFocused(bool state);

    int getWidth();
    int getHeight();
    sf::RectangleShape getBlock(sf::Vector2f indexes);
    bool getBlockState(sf::Vector2f indexes);
    
    void drawBlocks();
    void copyNextStates();
};