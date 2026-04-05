#include "BaseApplication.h"

#include "Timer.h"
#include <tracy/Tracy.hpp>

BaseApplication::BaseApplication():window{sf::VideoMode({800,800}),"SFML Window"},input(&window)
{
    clearColour = sf::Color::Cyan;

    Timer::Init();
}

BaseApplication::~BaseApplication()
{

}

void BaseApplication::Gameloop()
{
    while(window.isOpen())
    {
        FrameMark;

        {
            ZoneScopedN("TimerUpdate");
            Timer::Update();
        }

        {
            ZoneScopedN("WindowPoll");
            PollEvents();
        }
        {
            ZoneScopedN("Input");
            Input();
        }

        //Pre-update
        PhysicsWorld::UpdateWorld();

        {
            ZoneScopedN("Update");
            Update();
        }

        //Pre-render
        {
            ZoneScopedN("Prerender");
            window.clear(clearColour);
            window.setView(camera.getView());
        }

        {
            ZoneScopedN("Render");
            Render();
        }

        //Post-render
        {
            ZoneScopedN("Window Update");
            window.display();
        }
    }
}

void BaseApplication::PollEvents()
{
    input.PollEvents();
}
