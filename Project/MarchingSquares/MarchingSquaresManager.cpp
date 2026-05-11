#include "MarchingSquaresManager.h"

#include <Core/InputHandler.h>
#include <tracy/Tracy.hpp>

#include "Object/MarchingSquaresObject.h"
#include "Drawable/MSDrawableObject.h"

MarchingSquaresManager::MarchingSquaresManager():mouseLeftPressed{false}
{

}
MarchingSquaresManager::~MarchingSquaresManager()
{
    for(auto* obj : MSObjects)
    {
        delete obj;
    }
}

void MarchingSquaresManager::Add(float* data, int width, int height, bool dynamic, Transform& transform)
{
    ZoneScoped
    MarchingSquaresObject* obj = new MarchingSquaresObject();
    obj->SetGrid(data,width,height);
    obj->SetDynamic(dynamic);
    obj->GetTransform().SetData(transform);

    std::vector<MarchingSquaresObject*> objects = obj->Separate();
    for(auto* obj:objects)
    {
        obj->Init();
        MSObjects.push_back(obj);
    }
}

void MarchingSquaresManager::Add(VoxelGrid* data, bool dynamic, Transform& transform)
{
    ZoneScoped
    MarchingSquaresObject* obj = new MarchingSquaresObject();
    obj->SetGrid(data);
    obj->SetDynamic(dynamic);
    obj->GetTransform().SetData(transform);

    std::vector<MarchingSquaresObject*> objects = obj->Separate();
    for(auto* obj:objects)
    {
        obj->Init();
        MSObjects.push_back(obj);
    }
}

void MarchingSquaresManager::TakeInput(InputHandler* inputHandler)
{
    if(inputHandler->getMouseButton(sf::Mouse::Button::Left) && !mouseLeftPressed)
    {
        //Create new drawable
        MSDrawableObject* drawable = new MSDrawableObject(inputHandler->getMousePositionWorld(), sf::Mouse::Button::Left, 1.0f);
        MSDrawables.push_back(drawable);
        mouseLeftPressed = true;
    }
    else if(!inputHandler->getMouseButton(sf::Mouse::Button::Left))
    {
        mouseLeftPressed = false;
    }
    if(inputHandler->getMouseButton(sf::Mouse::Button::Right) && !mouseRightPressed)
    {
        for(auto it = MSObjects.begin();it!=MSObjects.end();)
        {
            auto* obj = *it;
            MSDrawables.push_back(obj->MakeDrawable());
            delete obj;
            it = MSObjects.erase(it);
        }
        mouseRightPressed = true;
    }
    else if(!inputHandler->getMouseButton(sf::Mouse::Button::Right))
    {
        mouseRightPressed = false;
    }


    for(auto* obj : MSDrawables)
    {
        obj->TakeInput(inputHandler);
    }
}

void MarchingSquaresManager::Update()
{
    //Update and clear empty objects
    for(auto it = MSObjects.begin();it!=MSObjects.end();)
    {
        auto* obj = (*it);
        if(obj->GetTriangleCount()==0)
        {
            delete obj;
            it = MSObjects.erase(it);
            continue;
        }
        obj->Update();
        ++it;
    }

    for(auto it = MSDrawables.begin();it!=MSDrawables.end();)
    {
        auto* obj = (*it);
        if(obj->isComplete())
        {
            VoxelGrid* grid = obj->getGrid();
            Add(grid,true, obj->GetTransform());
            delete obj;
            it=MSDrawables.erase(it);
            continue;
        }
        obj->Update();
        ++it;
    }
}

void MarchingSquaresManager::Render(sf::RenderWindow* window)
{
    for(auto* obj: MSObjects)
    {
        obj->Render(window);
    }
    for (auto* obj : MSDrawables)
    {
        obj->Render(window);
    }
}
