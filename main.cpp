#include <iostream>
#include <fstream>
#include <cctype>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <map>
#include "Tile.h"
#include "Board.h"

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

bool isTileClicked(const Board& tempboard, const sf::RenderWindow& tempwindow) {
    auto mouse_pos = sf::Mouse::getPosition(tempwindow);
    auto translated_pos = tempwindow.mapPixelToCoords(mouse_pos);
    for(unsigned int i = 0; i < tempboard.num_tiles; i++) {
        if(tempboard.board[i].sprite.getGlobalBounds().contains(translated_pos)) {
            return true;
        }
    }
    return false;
}

Tile* returnTile(const Board& tempboard, const sf::RenderWindow& tempwindow) {
    auto mouse_pos = sf::Mouse::getPosition(tempwindow);
    auto translated_pos = tempwindow.mapPixelToCoords(mouse_pos);
    for(unsigned int i = 0; i < tempboard.num_tiles; i++) {
        if(tempboard.board[i].sprite.getGlobalBounds().contains(translated_pos)) {
            return &tempboard.board[i];
        }
    }
    return nullptr;
}

bool isSpriteClicked(const sf::Sprite& tempsprite, const sf::RenderWindow& tempwindow) {
    auto mouse_pos = sf::Mouse::getPosition(tempwindow);
    auto translated_pos = tempwindow.mapPixelToCoords(mouse_pos);
    if(tempsprite.getGlobalBounds().contains(translated_pos)) {
        return true;
    }
    return false;
}

sf::Sprite* returnSprite(sf::Sprite& tempsprite, const sf::RenderWindow& tempwindow) {
    auto mouse_pos = sf::Mouse::getPosition(tempwindow);
    auto translated_pos = tempwindow.mapPixelToCoords(mouse_pos);
    if(tempsprite.getGlobalBounds().contains(translated_pos)) {
        return &tempsprite;
    }
    return nullptr;
}

int main()
{
    // Reading Board Config

    std::ifstream config("files/board_config.cfg");
    std::string config_reader;
    if(not config.is_open()){
        std::cout << "Unable to Read Board Config" << std::endl;
        return 0;
    }
    std::getline(config, config_reader);
    const float WIDTH = std::stof(config_reader) * 32;
    const float COLS = std::stof(config_reader);
    std::getline(config, config_reader);
    const float HEIGHT = (std::stof(config_reader) * 32) + 100;
    const float ROWS = std::stof(config_reader);
    std::getline(config, config_reader);
    const unsigned int MINECOUNT = std::stoi(config_reader);
    const float TILECOUNT = COLS * ROWS;

    // Initializing Welcome Window

    sf::RenderWindow welcome_window(sf::VideoMode(WIDTH, HEIGHT), "Welcome", sf::Style::Close);

    // Creating Welcome Screen Text

    sf::Font font;
    font.loadFromFile("files/font.ttf");
    sf::Text welcome_message;
    welcome_message.setFont(font);
    welcome_message.setString("WELCOME TO MINESWEEPER!");
    welcome_message.setCharacterSize(24);
    welcome_message.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcome_message, WIDTH/2.0f, (HEIGHT/2.0f) - 150);

    sf::Text welcome_prompt;
    welcome_prompt.setFont(font);
    welcome_prompt.setString("Enter your name:");
    welcome_prompt.setCharacterSize(20);
    welcome_prompt.setStyle(sf::Text::Bold);
    setText(welcome_prompt, WIDTH/2.0f, (HEIGHT/2.0f) - 75);

    sf::Text input_text;
    std::string username = "|";
    input_text.setFont(font);
    input_text.setString(username);
    input_text.setCharacterSize(18);
    input_text.setStyle(sf::Text::Bold);
    setText(input_text, WIDTH/2.0f, (HEIGHT/2.0f) - 45);

    // Welcome Window

    while (welcome_window.isOpen())
    {
        sf::Event event;
        while (welcome_window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    welcome_window.close();
                    return 0;
                }
                case sf::Event::TextEntered:
                {
                    if(isalpha(event.text.unicode) && username.length() < 11)
                    {
                        username.pop_back();
                        username += (static_cast<char>(event.text.unicode));
                        username += '|';
                        input_text.setString(username);
                        setText(input_text, WIDTH/2.0f, (HEIGHT/2.0f) - 45);
                    }
                    break;
                }
                case sf::Event::KeyPressed:
                {
                    if(event.key.code == sf::Keyboard::BackSpace && username.length() > 1)
                    {
                        username.pop_back();
                        username.pop_back();
                        username += '|';
                        input_text.setString(username);
                        setText(input_text, WIDTH/2.0f, (HEIGHT/2.0f) - 45);
                    }
                    if(event.key.code == sf::Keyboard::Enter)
                    {
                        if(username.length() == 1)
                        {
                            break;
                        }
                        username.pop_back();
                        std::transform(username.begin(), username.end(), username.begin(), ::tolower);
                        std::transform(username.begin(), username.begin() + 1, username.begin(), ::toupper);
                        welcome_window.close();
                    }
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        welcome_window.clear(sf::Color::Blue);
        welcome_window.draw(welcome_message);
        welcome_window.draw(welcome_prompt);
        welcome_window.draw(input_text);
        welcome_window.display();
    }

    // Initializing Game Window

    sf::RenderWindow game_window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper");

    Board test(COLS, ROWS, TILECOUNT, MINECOUNT, game_window);

    // Initializing Clock

    std::map<int, sf::IntRect> rect;
    rect[0] = sf::IntRect(0,0,21,32);
    rect[1] = sf::IntRect(21,0,21,32);
    rect[2] = sf::IntRect(42,0,21,32);
    rect[3] = sf::IntRect(63,0,21,32);
    rect[4] = sf::IntRect(84,0,21,32);
    rect[5] = sf::IntRect(105,0,21,32);
    rect[6] = sf::IntRect(126,0,21,32);
    rect[7] = sf::IntRect(147,0,21,32);
    rect[8] = sf::IntRect(168,0,21,32);
    rect[9] = sf::IntRect(189,0,21,32);
    rect[10] = sf::IntRect(210,0,21,32);

    sf::Clock pause_clock;
    sf::Clock clock;
    float paused = 0.0f;
    bool isPaused = false;
    sf::Texture digits;
    digits.loadFromFile("files/images/digits.png");
    digits.setSmooth(true);
    sf::Sprite minute_left;
    minute_left.setPosition((COLS * 32.0f) - 97, (32*(ROWS + 0.5f)) + 16);
    minute_left.setTexture(digits);
    minute_left.setTextureRect(sf::IntRect(0,0,21,32));
    sf::Sprite minute_right;
    minute_right.setPosition((COLS * 32.0f) - 76, (32*(ROWS + 0.5f)) + 16);
    minute_right.setTexture(digits);
    minute_right.setTextureRect(sf::IntRect(0,0,21,32));
    sf::Sprite second_left;
    second_left.setPosition((COLS * 32.0f) - 54, (32*(ROWS + 0.5f)) + 16);
    second_left.setTexture(digits);
    second_left.setTextureRect(sf::IntRect(0,0,21,32));
    sf::Sprite second_right;
    second_right.setPosition((COLS * 32.0f) - 33, (32*(ROWS + 0.5f)) + 16);
    second_right.setTexture(digits);
    second_right.setTextureRect(sf::IntRect(0,0,21,32));

    // Debug Button

    sf::Texture debug_texture;
    debug_texture.loadFromFile("files/images/debug.png");
    debug_texture.setSmooth(true);
    sf::Sprite debug;
    debug.setPosition((COLS * 32.0f) - 304, 32 * (ROWS + 0.5f));
    debug.setTexture(debug_texture);

    // Mine Counter

    sf::Sprite counter_left;
    counter_left.setPosition(12, (32*(ROWS + 0.5f)) + 16);
    counter_left.setTexture(digits);
    counter_left.setTextureRect(rect[(int)(MINECOUNT / 100)]);
    sf::Sprite counter_middle;
    counter_middle.setPosition(33, (32*(ROWS + 0.5f)) + 16);
    counter_middle.setTexture(digits);
    counter_middle.setTextureRect(rect[(int)((MINECOUNT % 100) / 10)]);
    sf::Sprite counter_right;
    counter_right.setPosition(54, (32*(ROWS + 0.5f)) + 16);
    counter_right.setTexture(digits);
    counter_right.setTextureRect(rect[(int)(MINECOUNT % 10)]);

    // Face Button

    sf::Texture sad;
    sad.loadFromFile("files/images/face_lose.png");
    sad.setSmooth(true);
    sf::Texture happy;
    happy.loadFromFile("files/images/face_happy.png");
    happy.setSmooth(true);
    sf::Texture winner;
    winner.loadFromFile("files/images/face_win.png");
    winner.setSmooth(true);
    sf::Sprite face;
    face.setPosition(((COLS / 2.0f) * 32.0f) - 32.0f,32*(ROWS + 0.5f));
    face.setTexture(happy);

    // Play / Pause Button

    sf::Texture pause_texture;
    pause_texture.loadFromFile("files/images/pause.png");
    pause_texture.setSmooth(true);
    sf::Texture play_texture;
    play_texture.loadFromFile("files/images/play.png");
    play_texture.setSmooth(true);
    sf::Sprite resume;
    resume.setPosition((COLS * 32) - 176, 32*(ROWS + 0.5f));
    resume.setTexture(pause_texture);

    // Game Window
    clock.restart();
    pause_clock.restart();
    while (game_window.isOpen())
    {
        // Timer

        sf::Time elapsed = clock.getElapsedTime();
        sf::Time pause_time = pause_clock.getElapsedTime();
        float actual_time = elapsed.asSeconds() - paused;
        int time_seconds = (int)(actual_time) % 60;
        int time_minutes = (int)(actual_time) / 60;
        if(not isPaused && not test.checkforloss()) {
            switch(time_seconds % 10)
            {
                case 0:
                    second_right.setTextureRect(rect[0]);
                    break;
                case 1:
                    second_right.setTextureRect(rect[1]);
                    break;
                case 2:
                    second_right.setTextureRect(rect[2]);
                    break;
                case 3:
                    second_right.setTextureRect(rect[3]);
                    break;
                case 4:
                    second_right.setTextureRect(rect[4]);
                    break;
                case 5:
                    second_right.setTextureRect(rect[5]);
                    break;
                case 6:
                    second_right.setTextureRect(rect[6]);
                    break;
                case 7:
                    second_right.setTextureRect(rect[7]);
                    break;
                case 8:
                    second_right.setTextureRect(rect[8]);
                    break;
                case 9:
                    second_right.setTextureRect(rect[9]);
                    break;
            }
            switch(time_seconds / 10)
            {
                case 0:
                    second_left.setTextureRect(rect[0]);
                    break;
                case 1:
                    second_left.setTextureRect(rect[1]);
                    break;
                case 2:
                    second_left.setTextureRect(rect[2]);
                    break;
                case 3:
                    second_left.setTextureRect(rect[3]);
                    break;
                case 4:
                    second_left.setTextureRect(rect[4]);
                    break;
                case 5:
                    second_left.setTextureRect(rect[5]);
                    break;
                case 6:
                    second_left.setTextureRect(rect[6]);
                    break;
                case 7:
                    second_left.setTextureRect(rect[7]);
                    break;
                case 8:
                    second_left.setTextureRect(rect[8]);
                    break;
                case 9:
                    second_left.setTextureRect(rect[9]);
                    break;
            }
            switch(time_minutes % 10)
            {
                case 0:
                    minute_right.setTextureRect(rect[0]);
                    break;
                case 1:
                    minute_right.setTextureRect(rect[1]);
                    break;
                case 2:
                    minute_right.setTextureRect(rect[2]);
                    break;
                case 3:
                    minute_right.setTextureRect(rect[3]);
                    break;
                case 4:
                    minute_right.setTextureRect(rect[4]);
                    break;
                case 5:
                    minute_right.setTextureRect(rect[5]);
                    break;
                case 6:
                    minute_right.setTextureRect(rect[6]);
                    break;
                case 7:
                    minute_right.setTextureRect(rect[7]);
                    break;
                case 8:
                    minute_right.setTextureRect(rect[8]);
                    break;
                case 9:
                    minute_right.setTextureRect(rect[9]);
                    break;
            }
            switch(time_minutes / 10)
            {
                case 0:
                    minute_left.setTextureRect(rect[0]);
                    break;
                case 1:
                    minute_left.setTextureRect(rect[1]);
                    break;
                case 2:
                    minute_left.setTextureRect(rect[2]);
                    break;
                case 3:
                    minute_left.setTextureRect(rect[3]);
                    break;
                case 4:
                    minute_left.setTextureRect(rect[4]);
                    break;
                case 5:
                    minute_left.setTextureRect(rect[5]);
                    break;
                case 6:
                    minute_left.setTextureRect(rect[6]);
                    break;
                case 7:
                    minute_left.setTextureRect(rect[7]);
                    break;
                case 8:
                    minute_left.setTextureRect(rect[8]);
                    break;
                case 9:
                    minute_left.setTextureRect(rect[9]);
                    break;
            }
        } else if(time_minutes > 99) {
            minute_left.setTextureRect(rect[10]);
            minute_right.setTextureRect(rect[10]);
            second_left.setTextureRect(rect[10]);
            second_right.setTextureRect(rect[10]);
        }

        // Counter

        test.count_flagged();
        switch(abs(test.counter) / 100)
        {
            case 0:
                counter_left.setTextureRect(rect[0]);
                break;
            case 1:
                counter_left.setTextureRect(rect[1]);
                break;
            case 2:
                counter_left.setTextureRect(rect[2]);
                break;
            case 3:
                counter_left.setTextureRect(rect[3]);
                break;
            case 4:
                counter_left.setTextureRect(rect[4]);
                break;
            case 5:
                counter_left.setTextureRect(rect[5]);
                break;
            case 6:
                counter_left.setTextureRect(rect[6]);
                break;
            case 7:
                counter_left.setTextureRect(rect[7]);
                break;
            case 8:
                counter_left.setTextureRect(rect[8]);
                break;
            case 9:
                counter_left.setTextureRect(rect[9]);
                break;
        }
        switch((abs(test.counter % 100) / 10))
        {
            case 0:
                counter_middle.setTextureRect(rect[0]);
                break;
            case 1:
                counter_middle.setTextureRect(rect[1]);
                break;
            case 2:
                counter_middle.setTextureRect(rect[2]);
                break;
            case 3:
                counter_middle.setTextureRect(rect[3]);
                break;
            case 4:
                counter_middle.setTextureRect(rect[4]);
                break;
            case 5:
                counter_middle.setTextureRect(rect[5]);
                break;
            case 6:
                counter_middle.setTextureRect(rect[6]);
                break;
            case 7:
                counter_middle.setTextureRect(rect[7]);
                break;
            case 8:
                counter_middle.setTextureRect(rect[8]);
                break;
            case 9:
                counter_middle.setTextureRect(rect[9]);
                break;
        }
        switch(abs(test.counter % 10))
        {
            case 0:
                counter_right.setTextureRect(rect[0]);
                break;
            case 1:
                counter_right.setTextureRect(rect[1]);
                break;
            case 2:
                counter_right.setTextureRect(rect[2]);
                break;
            case 3:
                counter_right.setTextureRect(rect[3]);
                break;
            case 4:
                counter_right.setTextureRect(rect[4]);
                break;
            case 5:
                counter_right.setTextureRect(rect[5]);
                break;
            case 6:
                counter_right.setTextureRect(rect[6]);
                break;
            case 7:
                counter_right.setTextureRect(rect[7]);
                break;
            case 8:
                counter_right.setTextureRect(rect[8]);
                break;
            case 9:
                counter_right.setTextureRect(rect[9]);
                break;
        }
        if(test.counter < 0) {
            counter_left.setTextureRect(rect[10]);
        }

        // Check Winner

        if(test.checkforwinner()) {
            test.end_game();
            face.setTexture(winner);
            isPaused = true;
        }

        // Check Loser

        if(test.checkforloss()) {
            test.end_game();
            face.setTexture(sad);
        }

        // Game Events

        sf::Event event{};
        while (game_window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    game_window.close();
                    return 0;
                }
                case sf::Event::MouseButtonPressed:
                {
                    if(event.mouseButton.button == sf::Mouse::Left) {
                        if(isTileClicked(test, game_window)) {
                            if(returnTile(test, game_window)->has_mine) {
                                returnTile(test, game_window)->OnClick();
                            } else if(returnTile(test, game_window)->adjacent_mines == 0) {
                                returnTile(test, game_window)->recursive_checker();
                            } else {
                                returnTile(test, game_window)->OnClick();
                                returnTile(test,game_window)->visited = true;
                            }
                        } else if(isSpriteClicked(debug, game_window)) {
                            if(not isPaused) {
                                if(test.showing_mines) {
                                    test.hide_mines();
                                } else {
                                    test.show_mines();
                                }
                                test.showing_mines = not test.showing_mines;
                            }
                        } else if(isSpriteClicked(face, game_window)) {
                            test.setup_board();
                            face.setTexture(happy);
                            clock.restart();
                            pause_clock.restart();
                            paused = 0.0f;
                            isPaused = false;
                            resume.setTexture(pause_texture);
                        } else if(isSpriteClicked(resume, game_window)) {
                            isPaused = not isPaused;
                            if(isPaused) {
                                test.display_allhidden();
                                resume.setTexture(play_texture);
                                pause_clock.restart();
                            } else {
                                test.resume_game();
                                resume.setTexture(pause_texture);
                                paused += pause_time.asSeconds();
                            }
                        }
                    } else if(event.mouseButton.button == sf::Mouse::Right) {
                        if(isTileClicked(test, game_window)) {
                            returnTile(test, game_window)->RightClick();
                        }
                    }
                }
                default:
                {
                    break;
                }
            }
        }
        game_window.clear(sf::Color::White);
        game_window.draw(minute_left);
        game_window.draw(minute_right);
        game_window.draw(second_left);
        game_window.draw(second_right);
        game_window.draw(counter_left);
        game_window.draw(counter_middle);
        game_window.draw(counter_right);
        game_window.draw(face);
        game_window.draw(resume);
        game_window.draw(debug);
        test.draw(game_window);
        game_window.display();
    }
    // TODO: Leaderboard Window
    return 0;
}