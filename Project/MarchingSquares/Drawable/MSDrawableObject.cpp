#include "MSDrawableObject.h"

#include <iostream>
#include <Core/InputHandler.h>

MSDrawableObject::MSDrawableObject(sf::Vector2f mousePosition, sf::Mouse::Button buttonListening,float value)
:GameObject(),complete(false),buttonListening(buttonListening),physicsStore(),value(value), renderable(&transform)
{
    drawRadius = 3.0f;
    grid = new VoxelGrid(mousePosition.x,mousePosition.y);

    renderable.SetGrid(grid);
}

MSDrawableObject::MSDrawableObject(sf::Vector2f mousePosition, sf::Mouse::Button buttonListening, VoxelGrid* grid,float value)
:GameObject(),complete(false),buttonListening(buttonListening),physicsStore(),value(value), renderable(&transform)
{
    transform.SetPosition(mousePosition);
    drawRadius = 3.0f;
    this->grid = grid;

    grid->setX(transform.GetPositionSf().x);
    grid->setY(transform.GetPositionSf().y);

    transform.SetPosition(sf::Vector2f{0,0});

    renderable.SetGrid(grid);
}

MSDrawableObject::MSDrawableObject(Transform& transform, sf::Mouse::Button buttonListening, VoxelGrid* grid,
    float value):GameObject(),complete(false),buttonListening(buttonListening),physicsStore(),value(value), renderable(&transform)
{
    this->transform = transform;
    drawRadius = 3.0f;
    this->grid = grid;

    grid->setX(transform.GetPositionSf().x);
    grid->setY(transform.GetPositionSf().y);
    grid->setAngle(transform.GetRotationSf().asRadians());

    this->transform.SetPosition(sf::Vector2f{0,0});
    //this->transform.SetRotation(0.0f);

    renderable.SetGrid(grid);
}

MSDrawableObject::~MSDrawableObject()
{
    //If physics existed destroy it (new one is being created)
    if(b2Body_IsValid(physicsStore))
    {
        b2DestroyBody(physicsStore);
    }
}

void MSDrawableObject::TakeInput(InputHandler* input)
{
    if(!input->getMouseButton(buttonListening))
    {
        complete = true;
        return;
    }

    grid->AddValueCircle(input->getMousePositionWorld(),drawRadius,value);
}

void MSDrawableObject::Update()
{
    bool resized = false;
    if (grid->getResize())
    {
        renderable.Resize();
        resized = true;
    }

    sf::Vector2i modifiedCell;
    while ((modifiedCell = grid->getModifiedCell()) != sf::Vector2i(-1,-1))
    {
        if (!resized)
        {
            renderable.UpdateCell(modifiedCell);
        }
    }
}

void MSDrawableObject::Render(sf::RenderWindow* window)
{
    renderable.Render(window);
}

void MSDrawableObject::AddPhysicsStore(b2BodyId bodyId)
{
    this->physicsStore = bodyId;
    b2Body_SetType(bodyId,b2_staticBody);
}
