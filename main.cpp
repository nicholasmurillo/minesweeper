#include <iostream>
#include <fstream>
#include <cctype>
#include <SFML/Graphics.hpp>

void setText(sf::Text &text, float x, float y) {
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    text.setPosition(sf::Vector2f(x, y));
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
    std::getline(config, config_reader);
    const float HEIGHT = (std::stof(config_reader) * 32) + 100;
    std::getline(config, config_reader);
    const unsigned int MINECOUNT = std::stoi(config_reader);
    const unsigned int TILECOUNT = WIDTH * HEIGHT;

    // Initializing Welcome Window

    sf::RenderWindow welcome_window(sf::VideoMode(WIDTH, HEIGHT), "Minesweeper", sf::Style::Close);

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
                    break;
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

    // TODO: Game Window
    // TODO: Leaderboard Window
    return 0;
}