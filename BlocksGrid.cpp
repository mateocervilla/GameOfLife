#include "BlocksGrid.h"
#include <math.h>
#include <iostream>


BlocksGrid::BlocksGrid(int width, int height, int blockSize, std::shared_ptr<sf::RenderWindow> window)
: width(width), height(height), blockSize(blockSize)
{
    this->window = window;
    this->isWindowFocused = true;

    this->blocks.resize(this->width);
    this->blocksState.resize(this->width);
    this->nextBlocksState.resize(this->width);
    for (int i = 0; i < this->width; ++i) {
        this->blocks[i].resize(this->height);
        this->blocksState[i].resize(this->height);
        this->nextBlocksState[i].resize(this->height);
    }    
}

BlocksGrid::~BlocksGrid() {
}

int BlocksGrid::getWidth() {
    return this->width;
}

int BlocksGrid::getHeight() {
    return this->height;
}

sf::RectangleShape BlocksGrid::getBlock(sf::Vector2f indexes){
    return this->blocks[indexes.x][indexes.y];
}

bool BlocksGrid::getBlockState(sf::Vector2f indexes){
    return this->blocksState[indexes.x][indexes.y];
}

void BlocksGrid::createGrid() {
    // Initialize all blocks to 0
    for (int i = 0; i < this->width; ++i) {
        for (int j = 0; j < this->height; ++j) {
            this->blocksState[i][j] = 0;
            this->nextBlocksState[i][j] = 0;
            this->fillBlock(sf::Vector2f(i, j), false);
            if(i < this->width-1) continue;
            this->blocks[0][j].setFillColor(sf::Color::Red);
            this->blocks[this->width-1][j].setFillColor(sf::Color::Red);
        }
        this->blocks[i][0].setFillColor(sf::Color::Red);
        this->blocks[i][this->height-1].setFillColor(sf::Color::Red);
    }
}

sf::Vector2f operator*(const sf::Vector2f& vector, float scalar) {
    return sf::Vector2f(vector.x * scalar, vector.y * scalar);
}

void BlocksGrid::fillBlock(sf::Vector2f indexes, bool state) {
    this->blocks[indexes.x][indexes.y].setSize(sf::Vector2f(this->blockSize, this->blockSize));
    this->blocks[indexes.x][indexes.y].setPosition(indexes*this->blockSize);
    this->blocks[indexes.x][indexes.y].setFillColor(state ? sf::Color::Black : sf::Color::White);
    this->blocks[indexes.x][indexes.y].setOutlineColor(sf::Color::Black);
    this->blocks[indexes.x][indexes.y].setOutlineThickness(0.8f);
}

void BlocksGrid::setBlockFromMouse(sf::Vector2f point, bool state) {
    // Get upper left corner
    int xTrunc = static_cast<int>(std::floor(point.x / this->blockSize)) * this->blockSize;    
    int yTrunc = static_cast<int>(std::floor(point.y / this->blockSize)) * this->blockSize;

    int xInd = xTrunc/10;    
    int yInd = yTrunc/10;

    this->setBlock(sf::Vector2f(xInd,yInd), state);
}

void BlocksGrid::setBlock(sf::Vector2f indexes, bool state) {
    this->fillBlock(indexes, state);
    this->blocksState[indexes.x][indexes.y] = state;    
}

void BlocksGrid::drawBlocks() {
    if (!this->isWindowFocused) return;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i position = sf::Mouse::getPosition(*this->window);
        if (position.x >= this->blockSize && position.x < this->window->getSize().x - this->blockSize &&
            position.y >= this->blockSize && position.y < this->window->getSize().y - this->blockSize) {
            this->setBlockFromMouse(sf::Vector2f(position), true);
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i position = sf::Mouse::getPosition(*this->window);
        if (position.x >= this->blockSize && position.x < this->window->getSize().x - this->blockSize &&
            position.y >= this->blockSize && position.y < this->window->getSize().y - this->blockSize) {
            this->setBlockFromMouse(sf::Vector2f(position), false);
        }
    }
}

void BlocksGrid::setNextBlockState(sf::Vector2f indexes, bool state) {
    this->nextBlocksState[indexes.x][indexes.y] = state;
}

void BlocksGrid::setWindowFocused(bool state) {
    this->isWindowFocused = state;
}

void BlocksGrid::copyNextStates() {    
    // Copy next states
    for (int i = 1; i < (this->blocksState.size() - 1); i++) {
        for (int j = 1; j < (this->blocksState[i].size() - 1); j++) {
            setBlock(sf::Vector2f(i,j), this->nextBlocksState[i][j]);
        }
    }
}

