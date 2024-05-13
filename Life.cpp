#include "Life.h"
#include <math.h>
#include <iostream>

Life::Life() {
    this->initVariables();
    this->initWindow();
    this->initEnviroment();
}

Life::~Life() {
}

void Life::initVariables() {
    this->window = nullptr;
    this->playEnabled = false;
    this->speed = DEFAULT_SPEED;
    this->isWindowFocused = true;
}

void Life::initWindow() {
    this->videoMode.height = HEIGHT;
    this->videoMode.width = WIDTH;
    
    this->window = std::make_shared<sf::RenderWindow>(this->videoMode, "GameLife", sf::Style::Close);
    this->window->setFramerateLimit(75);
}

void Life::initEnviroment() {
    this->blocksGrid = std::make_unique<BlocksGrid>(XBLOCKS, YBLOCKS, BLOCK_SIZE, this->window);
    this->blocksGrid->createGrid();
}

void Life::update() {
    this->pollEvents();

    this->blocksGrid->drawBlocks();

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
    for (int i = 0; i <  this->blocksGrid->getWidth(); i++) {        
        for (int j = 0; j <  this->blocksGrid->getHeight(); j++) {
            this->window->draw(this->blocksGrid->getBlock(sf::Vector2f(i,j)));
        }
    }

    this->window->display();
}

void Life::pollEvents() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            case sf::Event::Closed:
                this->window->close();
                break;
            // Window focus events
            case sf::Event::GainedFocus:
                this->isWindowFocused = true;
                this->blocksGrid->setWindowFocused(true);
                break;
            case sf::Event::LostFocus:
                this->isWindowFocused = false;
                this->blocksGrid->setWindowFocused(false);
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
                    this->blocksGrid->createGrid();
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                    // DEFAULT SPEED
                    this->speed = DEFAULT_SPEED;
                }
                break;
        }
    }
}

const bool Life::running() const {
    return this->window->isOpen();
}

int Life::scanNeighbourBlocks(sf::Vector2f indexes)
{
    int sum = 0;
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x-1,indexes.y-1));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x-1,indexes.y));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x-1,indexes.y+1));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x,indexes.y+1));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x,indexes.y-1));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x+1,indexes.y-1));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x+1,indexes.y));
    sum += this->blocksGrid->getBlockState(sf::Vector2f(indexes.x+1,indexes.y+1));
    return sum;
}

void Life::scanBlocks() {
    // Skip borders (for now) (in case of that: i = 0; rm size() - 1)
    for (int i = 1; i < (XBLOCKS - 1); i++) {
        for (int j = 1; j < (YBLOCKS - 1); j++) {            
            int neigh = scanNeighbourBlocks(sf::Vector2f(i,j));
            this->blocksGrid->setNextBlockState(sf::Vector2f(i,j), this->blocksGrid->getBlockState(sf::Vector2f(i,j)));
            if (this->blocksGrid->getBlockState(sf::Vector2f(i,j))) {
                // IS ALIVE
                if (neigh > 3 || neigh < 2) {
                    // DIE
                    this->blocksGrid->setNextBlockState(sf::Vector2f(i,j), false);
                }
            }
            else {
                // IS DEAD
                if (neigh == 3) {
                    // BORN
                    this->blocksGrid->setNextBlockState(sf::Vector2f(i,j), true);
                }
            }
        }
    }
    this->blocksGrid->copyNextStates();
}
