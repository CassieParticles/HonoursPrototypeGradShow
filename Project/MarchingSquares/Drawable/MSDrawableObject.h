#pragma once
#include <GameObjects/GameObject.h>

#include "../VoxelGrid.h"
#include "MSDrawableRenderable.h"



class MSDrawableObject: public GameObject
{
public:
    MSDrawableObject(sf::Vector2f mousePosition, sf::Mouse::Button buttonListening,float value);
    MSDrawableObject(sf::Vector2f mousePosition, sf::Mouse::Button buttonListening, VoxelGrid* grid,float value);
    MSDrawableObject(Transform& transform, sf::Mouse::Button buttonListening, VoxelGrid* grid, float value);
    ~MSDrawableObject();

    void TakeInput(InputHandler* input) override;

    void Update() override;

    void Render(sf::RenderWindow* window) override;

    void AddPhysicsStore(b2BodyId bodyId);

    bool isComplete() const { return complete; }
    VoxelGrid* getGrid(){return grid;}

private:
    VoxelGrid* grid;
    sf::Mouse::Button buttonListening;

    MSDrawableRenderable renderable;

    b2BodyId physicsStore;

    float drawRadius;
    float value;

    bool complete;
};
