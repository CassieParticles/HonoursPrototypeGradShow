#include "StaticPhysicsFloor.h"

#include <Physics/PhysicsWorld.h>
StaticPhysicsFloor::StaticPhysicsFloor():GameObject(),renderable(&transform,sf::Color::Black) {}

void StaticPhysicsFloor::Init()
{
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = transform.GetPositionb2();
    bodyDef.rotation = transform.GetRotationb2();

    bodyDef.userData = this;

    bodyId = b2CreateBody(PhysicsWorld::GetWorldId(),&bodyDef);

    b2ShapeDef shapeDef = b2DefaultShapeDef();

    b2Vec2 size = transform.GetScaleb2();
    b2Polygon box = b2MakeBox(size.x/2,size.y/2);

    b2CreatePolygonShape(bodyId,&shapeDef, &box);
}

void StaticPhysicsFloor::Update()
{

}

void StaticPhysicsFloor::Render(sf::RenderWindow* window)
{
    renderable.Render(window);
}
