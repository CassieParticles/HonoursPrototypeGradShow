#include "MSDrawableObject.h"

#include <iostream>
#include <Core/InputHandler.h>

MSDrawableObject::MSDrawableObject(sf::Vector2f mousePosition, sf::Mouse::Button buttonListening,float value)
:GameObject(),complete(false),buttonListening(buttonListening),physicsStore(),value(value)
{
    transform.SetPosition(mousePosition);
    drawRadius = 3.0f;
    grid = new VoxelGrid();
}

MSDrawableObject::MSDrawableObject(sf::Vector2f mousePosition, sf::Mouse::Button buttonListening, VoxelGrid* grid,float value)
:GameObject(),complete(false),buttonListening(buttonListening),physicsStore(),value(value)
{
    transform.SetPosition(mousePosition);
    drawRadius = 3.0f;
    this->grid = grid;

    grid->setX(transform.GetPositionSf().x);
    grid->setY(transform.GetPositionSf().y);

    transform.SetPosition(sf::Vector2f{0,0});
}

MSDrawableObject::MSDrawableObject(Transform& transform, sf::Mouse::Button buttonListening, VoxelGrid* grid,
    float value):GameObject(),complete(false),buttonListening(buttonListening),physicsStore(),value(value)
{
    this->transform = transform;
    drawRadius = 3.0f;
    this->grid = grid;

    grid->setX(transform.GetPositionSf().x);
    grid->setY(transform.GetPositionSf().y);
    grid->setAngle(transform.GetRotationSf().asRadians());

    this->transform.SetPosition(sf::Vector2f{0,0});
    //this->transform.SetRotation(0.0f);
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

    //Handle grid updates from drawing
    if (grid->getResize())
    {
        std::cout << "Grid has been resized!"<<std::endl;
    }
}

void MSDrawableObject::Update()
{

}

void MSDrawableObject::AddPhysicsStore(b2BodyId bodyId)
{
    this->physicsStore = bodyId;
    b2Body_SetType(bodyId,b2_staticBody);
}
