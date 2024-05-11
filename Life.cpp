#include "Life.h"
#include <math.h>
#include <iostream>

void Life::initVariables() {
    this->window = nullptr;
    this->playEnabled = false;
    this->speed = DEFAULT_SPEED;
    this->isWindowFocused = true;
}

void Life::initWindow() {
    this->videoMode.height = HEIGHT;
    this->videoMode.width = WIDTH;
    
    this->window = std::make_unique<sf::RenderWindow>(this->videoMode, "GameLife", sf::Style::Close);
    this->window->setFramerateLimit(75);
}

void Life::createGrid() { 
    // Initialize all blocks to 0
    for (int i = 0; i < XBLOCKS; ++i) {
        for (int j = 0; j < YBLOCKS; ++j) {
            this->blocksState[i][j] = 0;
            this->nextBlocksState[i][j] = 0;
        }
    }

    blocks.resize(XBLOCKS);
    for (int i = 0; i < XBLOCKS; ++i) {
        blocks[i].resize(YBLOCKS);
        for (int j = 0; j < YBLOCKS; ++j) {
            fillBlock(sf::Vector2f(i, j), false);
        }
    }
    for (int i = 0; i < XBLOCKS; ++i) {
        this->blocks[i][0].setFillColor(sf::Color::Red);
        this->blocks[i][YBLOCKS-1].setFillColor(sf::Color::Red);
    }
    for (int j = 0; j < YBLOCKS; ++j) {
        this->blocks[0][j].setFillColor(sf::Color::Red);
        this->blocks[XBLOCKS-1][j].setFillColor(sf::Color::Red);
    }
}

sf::Vector2f operator*(const sf::Vector2f& vector, float scalar) {
    return sf::Vector2f(vector.x * scalar, vector.y * scalar);
}

void Life::fillBlock(sf::Vector2f indexes, bool state) {
    this->blocks[indexes.x][indexes.y].setSize(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
    this->blocks[indexes.x][indexes.y].setPosition(indexes*BLOCK_SIZE);
    this->blocks[indexes.x][indexes.y].setFillColor(state ? sf::Color::Black : sf::Color::White);
    this->blocks[indexes.x][indexes.y].setOutlineColor(sf::Color::Black);
    this->blocks[indexes.x][indexes.y].setOutlineThickness(0.8f);
}

void Life::setBlockFromMouse(sf::Vector2f point, bool state) {
    // Get upper left corner
    int xTrunc = static_cast<int>(std::floor(point.x / BLOCK_SIZE)) * BLOCK_SIZE;    
    int yTrunc = static_cast<int>(std::floor(point.y / BLOCK_SIZE)) * BLOCK_SIZE;

    int xInd = xTrunc/10;    
    int yInd = yTrunc/10;

    this->setBlock(sf::Vector2f(xInd,yInd), state);
}

void Life::setBlock(sf::Vector2f indexes, bool state) {
    this->fillBlock(indexes, state);
    this->blocksState[indexes.x][indexes.y] = state;    
}

void Life::drawBlocks() {
    if (!this->isWindowFocused) return;
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        sf::Vector2i position = sf::Mouse::getPosition(*this->window);
        if (position.x >= BLOCK_SIZE && position.x < this->window->getSize().x - BLOCK_SIZE &&
            position.y >= BLOCK_SIZE && position.y < this->window->getSize().y - BLOCK_SIZE) {
            this->setBlockFromMouse(sf::Vector2f(position), true);
        }
    }
    else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2i position = sf::Mouse::getPosition(*this->window);
        if (position.x >= BLOCK_SIZE && position.x < this->window->getSize().x - BLOCK_SIZE &&
            position.y >= BLOCK_SIZE && position.y < this->window->getSize().y - BLOCK_SIZE) {
            this->setBlockFromMouse(sf::Vector2f(position), false);
        }
    }
}

void Life::initEnviroment() {
    this->createGrid();
}

Life::Life() {
    this->initVariables();
    this->initWindow();
    this->initEnviroment(); // Must be called before actors
}

Life::~Life() {
}

void Life::update() {
    this->pollEvents();

    this->drawBlocks();

    
    if (playEnabled) {
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed.asMilliseconds() > this->speed) {
            scanBlocks();
            clock.restart();
        }
    }
}

void Life::render() {

    /*
        - clear old frame
        - render objects
        - display frame in window

        Renders the game objects    
    */


    this->window->clear(sf::Color::White);

    //Draw objects

    for (int i = 0; i <  this->blocks.size(); i++) {        
        for (int j = 0; j <  this->blocks[i].size(); j++) {
            this->window->draw(this->blocks[i][j]);
        }
    }

    this->window->display();
}

void Life::pollEvents() {
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            
            // Window focus events
            case sf::Event::GainedFocus:
                this->isWindowFocused = true;
                break;
            case sf::Event::LostFocus:
                this->isWindowFocused = false;
                break;

            case sf::Event::KeyPressed:
                if (!this->isWindowFocused) break;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                    // STEP
                    if (!this->playEnabled) {
                        scanBlocks();
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                    // TOGGLE PLAY
                    this->playEnabled = !this->playEnabled;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    // SPEED UP
                    if (this->speed >= 10) {
                        this->speed -= 5;
                    }
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    // SPEED DOWN
                    this->speed += 5;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                    // RESET BOARD
                    this->playEnabled = false;
                    this->createGrid();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    // DEFAULT SPEED
                    this->speed = DEFAULT_SPEED;
                }
                break;
        }
    }
}

const bool Life::running() const
{
    return this->window->isOpen();
}


int Life::scanNeighbourBlocks(sf::Vector2f indexes)
{
    int sum = 0;
    sum += this->blocksState[indexes.x-1][indexes.y-1];
    sum += this->blocksState[indexes.x-1][indexes.y];
    sum += this->blocksState[indexes.x-1][indexes.y+1];
    sum += this->blocksState[indexes.x][indexes.y+1];
    sum += this->blocksState[indexes.x][indexes.y-1];
    sum += this->blocksState[indexes.x+1][indexes.y-1];
    sum += this->blocksState[indexes.x+1][indexes.y];
    sum += this->blocksState[indexes.x+1][indexes.y+1];
    return sum;
}


void Life::scanBlocks()
{
    // Skip borders (for now) (in case of that: i = 0; rm size() - 1)
    for (int i = 1; i < (this->blocksState.size() - 1); i++) {
        for (int j = 1; j < (this->blocksState[i].size() - 1); j++) {            
            int neigh = scanNeighbourBlocks(sf::Vector2f(i,j));
            this->nextBlocksState[i][j] = this->blocksState[i][j];
            if (this->blocksState[i][j]) {
                // IS ALIVE
                if (neigh > 3 || neigh < 2) {
                    // DIE
                    this->nextBlocksState[i][j] = false;
                }
            }
            else {
                // IS DEAD
                if (neigh == 3) {
                    // BORN
                    this->nextBlocksState[i][j] = true;
                }
            }
        }
    }

    // Copy next states
    for (int i = 1; i < (this->blocksState.size() - 1); i++) {
        for (int j = 1; j < (this->blocksState[i].size() - 1); j++) {
            setBlock(sf::Vector2f(i,j), this->nextBlocksState[i][j]);
        }
    }
}
