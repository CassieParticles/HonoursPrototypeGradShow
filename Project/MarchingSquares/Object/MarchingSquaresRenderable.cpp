#include "MarchingSquaresRenderable.h"

#include <tracy/Tracy.hpp>

#include "../Triangle.h"

MarchingSquaresRenderable::MarchingSquaresRenderable(Transform* transform):BaseRenderable(transform)
{
    triangles.setPrimitiveType(sf::PrimitiveType::Triangles);

    renderState = sf::RenderStates();
}

void MarchingSquaresRenderable::Render(sf::RenderWindow* RenderWindow)
{
    renderState.transform = sf::Transform();
    renderState.transform.translate(transform->GetPositionSf());
    renderState.transform.scale(transform->GetScaleSf());
    renderState.transform.rotate(transform->GetRotationSf());

    RenderWindow->draw(triangles,renderState);
}

MarchingSquaresRenderable::MSRenderableBuilder::MSRenderableBuilder(MarchingSquaresRenderable* renderable):renderable(renderable) {}

void MarchingSquaresRenderable::MSRenderableBuilder::Build()
{
    ZoneScopedN("BuildRenderable")
    //Set the size of the vertex array
    renderable->triangles.clear();
    renderable->triangles.resize(triangles.size() * 3);

    for(int i=0;i<triangles.size();++i)
    {
        renderable->triangles[i * 3 + 0].position = triangles[i].A;
        renderable->triangles[i * 3 + 1].position = triangles[i].B;
        renderable->triangles[i * 3 + 2].position = triangles[i].C;
        renderable->triangles[i * 3 + 0].color = sf::Color::Blue;
        renderable->triangles[i * 3 + 1].color = sf::Color::Blue;
        renderable->triangles[i * 3 + 2].color = sf::Color::Blue;
    }
}


MarchingSquaresRenderable::MSRenderableBuilder MarchingSquaresRenderable::GetBuilder()
{
    return {this};
}

