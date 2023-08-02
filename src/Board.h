//
// Created by nickm on 7/27/2023.
//

#ifndef MINESWEEPER_BOARD_H
#define MINESWEEPER_BOARD_H

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Board {
public:
    unsigned int num_mines;
    unsigned int num_tiles;
    unsigned int num_cols;
    unsigned int num_rows;
    bool first_init = false;
    int counter;
    bool showing_mines = false;
    Tile* board;
    void setup_mines() const;
    void show_mines() const;
    void hide_mines() const;
    void count_flagged();
    void setup_middle_neighbors() const;
    void setup_left_neighbors() const;
    void setup_top_neighbors() const;
    void setup_right_neighbors() const;
    void setup_bottom_neighbors() const;
    void setup_corner_neighbors() const;
    void setup_board();
    void display_allhidden() const;
    void resume_game() const;
    bool checkforloss() const;
    bool checkforwinner() const;
    void end_game() const;
    void draw(sf::RenderWindow& temp) const;
    Board();
    Board(const float& num_c, const float& num_r, const float& num_t, const unsigned int& num_m);
    ~Board();
};


#endif //MINESWEEPER_BOARD_H
