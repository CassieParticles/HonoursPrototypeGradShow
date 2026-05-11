#include "InputHandler.h"

#include <iostream>

InputHandler::InputHandler(sf::RenderWindow* window):window{window}
{
    keys.resize(sf::Keyboard::KeyCount);
    mouseButtons.resize(sf::Mouse::ButtonCount);
    scroll = 0;
}

InputHandler::~InputHandler()
{

}

void InputHandler::PollEvents()
{
    while(const std::optional event = window->pollEvent())
    {
        scroll = 0;

        //Close the window
        if(event->is<sf::Event::Closed>())
        {
            window->close();
        }
        //Mouse position
        if(event->is<sf::Event::MouseLeft>())
        {
            mousePosition = sf::Vector2i(-1,-1);
            mouseOnScreen = false;
        }
        if(event->is<sf::Event::MouseEntered>())
        {
            mouseOnScreen = true;
        }
        if(mouseOnScreen && event->is<sf::Event::MouseMoved>())
        {
            mousePosition = event->getIf<sf::Event::MouseMoved>()->position;
        }
        //Keyboard pressed/released
        if(event->is<sf::Event::KeyPressed>())
        {
            int code = static_cast<int>(event->getIf<sf::Event::KeyPressed>()->code);
            if (code == -1) { return; }
            keys.at(code) = true;
        }
        if(event->is<sf::Event::KeyReleased>())
        {
            int code = static_cast<int>(event->getIf<sf::Event::KeyReleased>()->code);
            if (code == -1) { return; }
            keys.at(code) = false;
        }
        //Mouse buttons pressed/released
        if(event->is<sf::Event::MouseButtonPressed>())
        {
            int button = static_cast<int>(event->getIf<sf::Event::MouseButtonPressed>()->button);
            if (button == -1) { return; }
            mouseButtons.at(button) = true;
        }
        if(event->is<sf::Event::MouseButtonReleased>())
        {
            int button = static_cast<int>(event->getIf<sf::Event::MouseButtonReleased>()->button);
            if (button == -1) { return; }
            mouseButtons.at(button) = false;
        }
        if(event->is<sf::Event::MouseWheelScrolled>())
        {
            scroll = event->getIf<sf::Event::MouseWheelScrolled>()->delta;
        }
    }
}

bool InputHandler::getKey(sf::Keyboard::Key key) {return keys[static_cast<int>(key)];}
bool InputHandler::getMouseButton(sf::Mouse::Button button) {return mouseButtons[static_cast<int>(button)];}

sf::Vector2f InputHandler::getMousePositionWorld()
{
    return window->mapPixelToCoords(mousePosition);
}


float InputHandler::getScroll() {return scroll;}

