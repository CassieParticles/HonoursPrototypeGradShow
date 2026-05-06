#include "Application.h"

#include <iostream>

#include "CmdVarPass.h"

Application::Application():controller{&camera}
{
    //Set up controller
    controller.SetMoveSpeed(50);
    controller.SetScrollSpeed(0.001);

    PhysicsWorld::CreateWorldBuilder().SetGravity(b2Vec2(0,30.0f))->Build();

    //Create box for testing
    floorObj.GetTransform().SetPosition(-1,50);
    floorObj.GetTransform().SetRotation(sf::degrees(00));
    floorObj.GetTransform().SetScale(100,10);

    wallAObj.GetTransform().SetPosition(-50,0);
    wallAObj.GetTransform().SetScale(10,100);

    wallBObj.GetTransform().SetPosition( 50,0);
    wallBObj.GetTransform().SetScale(10,100);

    int length = CmdVarPass::sideLength;

    float value = 1.0f;

    Transform transform;
    transform.SetPosition(0,0);

    for(int x=0;x<length;x+=2)
    {
        for(int y=0;y<length;y+=2)
        {
            transform.SetPosition(x,y);
            MSManager.Add(&value,1,1,true,transform);
        }
    }

    floorObj.Init();

    wallAObj.Init();
    wallBObj.Init();

    boxObj.GetTransform().SetPosition(5,-2);
    boxObj.Init();
}

Application::~Application()
{
}

void Application::Input()
{
    controller.TakeInput(&input);
    MSManager.TakeInput(&input);
}

void Application::Update()
{
    controller.Update();

    boxObj.Update();
    wallAObj.Update();
    wallBObj.Update();

    MSManager.Update();
}

void Application::Render()
{
    MSManager.Render(&window);

    floorObj.Render(&window);
    wallAObj.Render(&window);
    wallBObj.Render(&window);
    boxObj.Render(&window);
}

