//
// Created by nickm on 7/27/2023.
//

#include "Tile.h"

void Tile::OnClick() {
    if(this->untouchable) {
        return;
    }
    if(this->flagged) {
        return;
    }
    if(this->showing) {
        return;
    }
    if(this->has_mine){
        this->topper.setTexture(this->mine);
        loss = true;
        return;
    } else {
        this->untouchable = true;
        this->changetexture("files/images/tile_revealed.png");
        switch(this->adjacent_mines)
        {
            case 1:
                number.loadFromFile(numbers[1]);
                break;
            case 2:
                number.loadFromFile(numbers[2]);
                break;
            case 3:
                number.loadFromFile(numbers[3]);
                break;
            case 4:
                number.loadFromFile(numbers[4]);
                break;
            case 5:
                number.loadFromFile(numbers[5]);
                break;
            case 6:
                number.loadFromFile(numbers[6]);
                break;
            case 7:
                number.loadFromFile(numbers[7]);
                break;
            case 8:
                number.loadFromFile(numbers[8]);
                break;
            default:
                number.loadFromFile("files/images/tile_revealed.png");
                break;
        }
        number.setSmooth(true);
        this->topper.setTexture(number);
        // SHOW ADJACENT BOMBS, IF ADJ_BOMBS = 0 THEN SHOW ALL TILES WITHOUT BOMBS TOUCHING IT
    }
}

void Tile::changetexture(const std::string& fileName) {
    sf::Texture temp_texture;
    temp_texture.loadFromFile(fileName);
    temp_texture.setSmooth(true);
    this->texture = temp_texture;
    this->sprite.setTexture(this->texture);
    this->topper.setTexture(this->texture);
}

void Tile::setpostition(float x, float y) {
    this->xpos = x;
    this->ypos = y;
    this->sprite.setPosition(this->xpos, this->ypos);
    this->topper.setPosition(this->xpos, this->ypos);
}

void Tile::setdefaulttile() {
    this->numbers[1] = "files/images/number_1.png";
    this->numbers[2] = "files/images/number_2.png";
    this->numbers[3] = "files/images/number_3.png";
    this->numbers[4] = "files/images/number_4.png";
    this->numbers[5] = "files/images/number_5.png";
    this->numbers[6] = "files/images/number_6.png";
    this->numbers[7] = "files/images/number_7.png";
    this->numbers[8] = "files/images/number_8.png";
    this->untouchable = false;
    this->has_mine = false;
    this->flagged = false;
    this->adjacent_mines = 0;
    this->xpos = 0;
    this->ypos = 0;
    this->normal.loadFromFile("files/images/tile_hidden.png");
    this->normal.setSmooth(true);
    this->texture.loadFromFile("files/images/tile_hidden.png");
    this->texture.setSmooth(true);
    this->flag.loadFromFile("files/images/flag.png");
    this->flag.setSmooth(true);
    this->mine.loadFromFile("files/images/mine.png");
    this->mine.setSmooth(true);
    this->sprite.setTexture(this->texture);
    this->topper.setTexture(this->texture);
}

void Tile::RightClick() {
    if(this->untouchable) {
        return;
    }
    if(this->topper.getTexture() == &this->mine) {
        return;
    }
    if(this->showing) {
        return;
    }
    this->flagged = not this->flagged;
    if(this->flagged) {
        this->topper.setTexture(this->flag);
    } else {
        this->changetexture("files/images/tile_hidden.png");
    }
}

void Tile::ShowMine() {
    if(this->has_mine) {
        this->topper.setTexture(this->mine);
        this->showing = true;
        this->untouchable = true;
    }
}

void Tile::HideMine() {
    if(this->has_mine && not this->flagged) {
        this->topper.setTexture(this->normal);
    } else {
        this->topper.setTexture(this->flag);
    }
    this->showing = false;
    this->untouchable = false;
}

void Tile::recursive_checker() {
    this->OnClick();
    if(this->flagged) {
        return;
    }
    if(this->showing) {
        return;
    }
    if(this->visited) {
        return;
    }
    this->visited = true;
    for(auto neighbor: this->neighbors) {
        if(neighbor == nullptr) {
            continue;
        } else if(neighbor->adjacent_mines == 0) {
            neighbor->recursive_checker();
        } else if(not neighbor->has_mine) {
            neighbor->OnClick();
            neighbor->visited = true;
        }
    }
}
