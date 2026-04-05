#include "PhysicsWorld.h"

#include <tracy/Tracy.hpp>

#include "../Core/Timer.h"

b2WorldId PhysicsWorld::worldId;

PhysicsWorld::WorldBuilder::WorldBuilder()
{
    worldDef = b2DefaultWorldDef();
}

PhysicsWorld::WorldBuilder PhysicsWorld::CreateWorldBuilder()
{
    return WorldBuilder();
}

b2WorldId PhysicsWorld::GetWorldId()
{
    return worldId;
}

void PhysicsWorld::UpdateWorld()
{
    ZoneScoped;
    float deltaTime = std::min(Timer::getDeltaTime(),0.016f);
    b2World_Step(worldId,deltaTime,4);
}
