#include "MSDrawableRenderable.h"

#include <Core/Timer.h>

MSDrawableRenderable::MSDrawableRenderable(Transform* transform):BaseRenderable(transform)
{
	array.setPrimitiveType(sf::PrimitiveType::Triangles);
    timestep = 0;
}

void MSDrawableRenderable::Render(sf::RenderWindow* window)
{
    state.transform = sf::Transform();
    state.transform.translate(sf::Vector2f(static_cast<float>(grid->getX()), static_cast<float>(grid->getY())));
    //state.transform.rotate(sf::radians(timestep));
    state.transform.rotate(sf::radians(grid->getAngle()));
    timestep += Timer::getDeltaTime();

    window->draw(array,state);
}

void MSDrawableRenderable::SetGrid(VoxelGrid* grid)
{
    this->grid = grid;
}

void MSDrawableRenderable::UpdateCell(sf::Vector2i cell)
{
	float cornerValues[4]
	{
		grid->getVoxel(cell.x,cell.y),
		grid->getVoxel(cell.x + 1,cell.y),
		grid->getVoxel(cell.x + 1,cell.y + 1),
		grid->getVoxel(cell.x,cell.y + 1)
	};

    //Get which case the cell is
    int index{};
    for (int i = 0;i < 4;++i)
    {
        index += (cornerValues[i] > 0) << i;
    }

    //Get the indices of the case
    const int* indexSet = indexTable[index];

    //Get the vertex position of each corner
    sf::Vector2f vertices[8];
    vertices[0] = sf::Vector2f(static_cast<float>(cell.x), static_cast<float>(cell.y));
    vertices[2] = sf::Vector2f(static_cast<float>(cell.x + 1), static_cast<float>(cell.y));
    vertices[4] = sf::Vector2f(static_cast<float>(cell.x + 1), static_cast<float>(cell.y + 1));
    vertices[6] = sf::Vector2f(static_cast<float>(cell.x), static_cast<float>(cell.y + 1));

    //Calculate the vertex position of each midpoint (lerp between positions)
    vertices[1] = findMidpointX(vertices[0], vertices[2], cornerValues[0], cornerValues[1]);
    vertices[3] = findMidpointY(vertices[2], vertices[4], cornerValues[1], cornerValues[2]);
    vertices[5] = findMidpointX(vertices[6], vertices[4], cornerValues[3], cornerValues[2]);
    vertices[7] = findMidpointY(vertices[0], vertices[6], cornerValues[0], cornerValues[3]);

    //Iterate through all triangles (to remove any unnecesary)
    for (int i = 0;i < 4;i++)
    {
        //Triangle not present
        if (indexSet[i * 3] == 8)
        {
            array[getIndexHelper(cell.x, cell.y, i, 0)].position = sf::Vector2f(0, 0);
            array[getIndexHelper(cell.x, cell.y, i, 1)].position = sf::Vector2f(0, 0);
            array[getIndexHelper(cell.x, cell.y, i, 2)].position = sf::Vector2f(0, 0);
            continue;
        }
        array[getIndexHelper(cell.x, cell.y, i, 0)].position = vertices[indexSet[i * 3 + 0]];
        array[getIndexHelper(cell.x, cell.y, i, 1)].position = vertices[indexSet[i * 3 + 1]];
        array[getIndexHelper(cell.x, cell.y, i, 2)].position = vertices[indexSet[i * 3 + 2]];
    }
}

void MSDrawableRenderable::Resize()
{
	array.clear();

	//Size of grid x 4 triangles per cell x 3 vertices per triangle
	array.resize(grid->getWidth() * grid->getHeight() * 4 * 3);
    for (int y = 0;y < grid->getHeight()-1;++y)
    {
        for (int x = 0;x < grid->getWidth()-1;++x)
        {
            UpdateCell(sf::Vector2i(x, y));
        }
    }
}
