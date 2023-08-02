//
// Created by nickm on 7/27/2023.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


class Tile {
public:
    std::map<int, std::string> numbers;
    sf::Texture normal;
    sf::Texture number;
    sf::Texture texture;
    sf::Texture flag;
    sf::Texture mine;
    sf::Sprite sprite;
    sf::Sprite topper;
    Tile* neighbors[8];
    bool visited = false;
    bool untouchable = false;
    bool showing = false;
    bool has_mine = false;
    bool flagged = false;
    bool loss = false;
    unsigned int adjacent_mines = 0;
    float xpos = 0;
    float ypos = 0;
    void OnClick();
    void RightClick();
    void ShowMine();
    void HideMine();
    void setdefaulttile();
    void recursive_checker();
    void changetexture(const std::string& fileName);
    void setpostition(float x, float y);
};


#endif //MINESWEEPER_TILE_H
