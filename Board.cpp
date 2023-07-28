//
// Created by nickm on 7/27/2023.
//

#include "Board.h"
#include <random>

Board::Board() {
    this->num_cols = 0;
    this->num_rows = 0;
    this->num_tiles = 0;
    this->num_mines = 0;
}

Board::~Board() {
    delete[] this->board;
}

Board::Board(const float& num_c, const float& num_r, const float& num_t, const unsigned int& num_m, sf::RenderWindow& game) {
    this->num_cols = (unsigned int)(num_c);
    this->num_rows = (unsigned int)(num_r);
    this->num_tiles = (unsigned int)(num_t);
    this->num_mines = num_m;
    this->counter = (int)this->num_mines;
    setup_board();
}

void Board::setup_mines() const {
    unsigned int count = 0;
    while(count < this->num_mines) {
        unsigned int random = rand() % this->num_tiles;
        if(this->board[random].has_mine) {
            continue;
        } else {
            this->board[random].has_mine = true;
            count++;
        }
    }
}

void Board::draw(sf::RenderWindow& temp) const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        temp.draw(this->board[i].sprite);
        temp.draw(this->board[i].topper);
    }
}

void Board::show_mines() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(this->board[i].has_mine) {
            this->board[i].ShowMine();
        }
    }
}

void Board::hide_mines() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(this->board[i].has_mine) {
            this->board[i].HideMine();
        }
    }
}

void Board::count_flagged() {
    this->counter = (int)this->num_mines;
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(this->board[i].flagged) {
            this->counter--;
        }
    }
}

void Board::setup_board() {
    if(this->showing_mines) {
        this->showing_mines = false;
        this->hide_mines();
    }
    float xcount = 0.0f;
    float ycount = 0.0f;
    if(this->first_init) {
        delete[] board;
    }
    this->board = new Tile[this->num_tiles];
    this->first_init = true;
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        this->board[i].setdefaulttile();
        this->board[i].setpostition(xcount, ycount);
        xcount += 32.0f;
        if(xcount >= (float)this->num_cols * 32.0f) {
            xcount = 0.0f;
            ycount += 32.0f;
        }
    }
    setup_mines();
    setup_middle_neighbors();
    setup_top_neighbors();
    setup_right_neighbors();
    setup_bottom_neighbors();
    setup_corner_neighbors();
    setup_left_neighbors();
}

bool Board::checkforloss() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(this->board[i].loss) {
            return true;
        }
    }
    return false;
}

void Board::end_game() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(this->board[i].has_mine) {
            this->board[i].ShowMine();
        }
        this->board[i].untouchable = true;
    }
}

void Board::setup_middle_neighbors() const {
    for(unsigned int i = this->num_cols + 1; i < this->num_tiles - this->num_cols - 1; i++) {
        if(i % this->num_cols == this->num_cols - 1 || i % this->num_cols == 0) {
            continue;
        }
        this->board[i].neighbors[0] = &this->board[i - this->num_cols - 1];
        this->board[i].neighbors[1] = &this->board[i - this->num_cols];
        this->board[i].neighbors[2] = &this->board[i - this->num_cols + 1];
        this->board[i].neighbors[3] = &this->board[i - 1];
        this->board[i].neighbors[4] = &this->board[i + 1];
        this->board[i].neighbors[5] = &this->board[i + this->num_cols - 1];
        this->board[i].neighbors[6] = &this->board[i + this->num_cols];
        this->board[i].neighbors[7] = &this->board[i + this->num_cols + 1];
        for(auto &neighbor : this->board[i].neighbors) {
            if(neighbor->has_mine) {
                this->board[i].adjacent_mines++;
            }
        }
    }
}

void Board::setup_left_neighbors() const {
    for(unsigned int i = this->num_cols; i < (this->num_tiles - (2 * this->num_cols)) + 1; i += this->num_cols) {
        this->board[i].neighbors[0] = nullptr;
        this->board[i].neighbors[1] = &this->board[i - this->num_cols];
        this->board[i].neighbors[2] = &this->board[i - this->num_cols + 1];
        this->board[i].neighbors[3] = nullptr;
        this->board[i].neighbors[4] = &this->board[i + 1];
        this->board[i].neighbors[5] = nullptr;
        this->board[i].neighbors[6] = &this->board[i + this->num_cols];
        this->board[i].neighbors[7] = &this->board[i + this->num_cols + 1];
        for(auto &neighbor : this->board[i].neighbors) {
            if(neighbor != nullptr && neighbor->has_mine) {
                this->board[i].adjacent_mines++;
            }
        }
    }
}

void Board::setup_top_neighbors() const {
    for(unsigned int i = 1; i < this->num_cols - 1; i++) {
        this->board[i].neighbors[0] = nullptr;
        this->board[i].neighbors[1] = nullptr;
        this->board[i].neighbors[2] = nullptr;
        this->board[i].neighbors[3] = &this->board[i - 1];
        this->board[i].neighbors[4] = &this->board[i + 1];
        this->board[i].neighbors[5] = &this->board[i + this->num_cols - 1];
        this->board[i].neighbors[6] = &this->board[i + this->num_cols];
        this->board[i].neighbors[7] = &this->board[i + this->num_cols + 1];
        for(auto &neighbor : this->board[i].neighbors) {
            if(neighbor != nullptr && neighbor->has_mine) {
                this->board[i].adjacent_mines++;
            }
        }
    }
}

void Board::setup_right_neighbors() const {
    for(unsigned int i = (2 * this->num_cols) - 1; i < this->num_tiles - this->num_cols; i += this->num_cols) {
        this->board[i].neighbors[0] = &this->board[i - this->num_cols - 1];
        this->board[i].neighbors[1] = &this->board[i - this->num_cols];
        this->board[i].neighbors[2] = nullptr;
        this->board[i].neighbors[3] = &this->board[i - 1];
        this->board[i].neighbors[4] = nullptr;
        this->board[i].neighbors[5] = &this->board[i + this->num_cols - 1];
        this->board[i].neighbors[6] = &this->board[i + this->num_cols];
        this->board[i].neighbors[7] = nullptr;
        for(auto &neighbor : this->board[i].neighbors) {
            if(neighbor != nullptr && neighbor->has_mine) {
                this->board[i].adjacent_mines++;
            }
        }
    }
}

void Board::setup_bottom_neighbors() const {
    for(unsigned int i = this->num_tiles - this->num_cols + 1; i < this->num_tiles - 1; i++) {
        this->board[i].neighbors[0] = &this->board[i - this->num_cols - 1];
        this->board[i].neighbors[1] = &this->board[i - this->num_cols];
        this->board[i].neighbors[2] = &this->board[i - this->num_cols + 1];
        this->board[i].neighbors[3] = &this->board[i - 1];
        this->board[i].neighbors[4] = &this->board[i + 1];
        this->board[i].neighbors[5] = nullptr;
        this->board[i].neighbors[6] = nullptr;
        this->board[i].neighbors[7] = nullptr;
        for(auto &neighbor : this->board[i].neighbors) {
            if(neighbor != nullptr && neighbor->has_mine) {
                this->board[i].adjacent_mines++;
            }
        }
    }
}

void Board::setup_corner_neighbors() const {
    // Top Left
    this->board[0].neighbors[0] = nullptr;
    this->board[0].neighbors[1] = nullptr;
    this->board[0].neighbors[2] = nullptr;
    this->board[0].neighbors[3] = nullptr;
    this->board[0].neighbors[4] = &this->board[1];
    this->board[0].neighbors[5] = nullptr;
    this->board[0].neighbors[6] = &this->board[this->num_cols];
    this->board[0].neighbors[7] = &this->board[this->num_cols + 1];
    for(auto &neighbor : this->board[0].neighbors) {
        if(neighbor != nullptr && neighbor->has_mine) {
            this->board[0].adjacent_mines++;
        }
    }

    // Top Right

    this->board[this->num_cols - 1].neighbors[0] = nullptr;
    this->board[this->num_cols - 1].neighbors[1] = nullptr;
    this->board[this->num_cols - 1].neighbors[2] = nullptr;
    this->board[this->num_cols - 1].neighbors[3] = &this->board[this->num_cols - 2];
    this->board[this->num_cols - 1].neighbors[4] = nullptr;
    this->board[this->num_cols - 1].neighbors[5] = &this->board[(2 * this->num_cols) - 2];
    this->board[this->num_cols - 1].neighbors[6] = &this->board[(2 * this->num_cols) - 1];
    this->board[this->num_cols - 1].neighbors[7] = nullptr;
    for(auto &neighbor : this->board[this->num_cols - 1].neighbors) {
        if(neighbor != nullptr && neighbor->has_mine) {
            this->board[this->num_cols - 1].adjacent_mines++;
        }
    }

    // Bottom Left

    this->board[this->num_tiles - this->num_cols].neighbors[0] = nullptr;
    this->board[this->num_tiles - this->num_cols].neighbors[1] = &this->board[this->num_tiles - (2 *this->num_cols)];
    this->board[this->num_tiles - this->num_cols].neighbors[2] = &this->board[this->num_tiles - (2 *this->num_cols) + 1];
    this->board[this->num_tiles - this->num_cols].neighbors[3] = nullptr;
    this->board[this->num_tiles - this->num_cols].neighbors[4] = &this->board[this->num_tiles - this->num_cols + 1];
    this->board[this->num_tiles - this->num_cols].neighbors[5] = nullptr;
    this->board[this->num_tiles - this->num_cols].neighbors[6] = nullptr;
    this->board[this->num_tiles - this->num_cols].neighbors[7] = nullptr;
    for(auto &neighbor : this->board[this->num_tiles - this->num_cols].neighbors) {
        if(neighbor != nullptr && neighbor->has_mine) {
            this->board[this->num_tiles - this->num_cols].adjacent_mines++;
        }
    }

    // Bottom Right

    this->board[this->num_tiles - 1].neighbors[0] = &this->board[this->num_tiles - this->num_cols - 2];
    this->board[this->num_tiles - 1].neighbors[1] = &this->board[this->num_tiles - this->num_cols - 1];
    this->board[this->num_tiles - 1].neighbors[2] = nullptr;
    this->board[this->num_tiles - 1].neighbors[3] = &this->board[this->num_tiles - 2];
    this->board[this->num_tiles - 1].neighbors[4] = nullptr;
    this->board[this->num_tiles - 1].neighbors[5] = nullptr;
    this->board[this->num_tiles - 1].neighbors[6] = nullptr;
    this->board[this->num_tiles - 1].neighbors[7] = nullptr;
    for(auto &neighbor : this->board[this->num_tiles - 1].neighbors) {
        if(neighbor != nullptr && neighbor->has_mine) {
            this->board[this->num_tiles - 1].adjacent_mines++;
        }
    }
}

bool Board::checkforwinner() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(not (this->board[i].visited || this->board[i].has_mine)) {
            return false;
        }
    }
    return true;
}

void Board::display_allhidden() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        this->board[i].untouchable = true;
        this->board[i].changetexture("files/images/tile_revealed.png");
    }
}

void Board::resume_game() const {
    for(unsigned int i = 0; i < this->num_tiles; i++) {
        if(this->board[i].visited && this->board[i].adjacent_mines == 0) {
            this->board[i].changetexture("files/images/tile_revealed.png");
        } else if(this->board[i].showing) {
            this->board[i].sprite.setTexture(this->board[i].normal);
            this->board[i].topper.setTexture(this->board[i].mine);
        } else if(this->board[i].flagged) {
            this->board[i].untouchable = false;
            this->board[i].sprite.setTexture(this->board[i].normal);
            this->board[i].topper.setTexture(this->board[i].flag);
        } else if(this->board[i].visited) {
            this->board[i].sprite.setTexture(this->board[i].texture);
            this->board[i].topper.setTexture(this->board[i].number);
        } else {
            this->board[i].untouchable = false;
            this->board[i].changetexture("files/images/tile_hidden.png");
        }
    }
}
