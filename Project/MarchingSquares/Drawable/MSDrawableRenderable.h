#pragma once

#include <Rendering/BaseRenderable.h>
#include "../VoxelGrid.h"

class MSDrawableRenderable: public BaseRenderable
{
public:
	MSDrawableRenderable(Transform* transform);
	void Render(sf::RenderWindow* window);

    void SetGrid(VoxelGrid* grid);

	void UpdateCell(sf::Vector2i cell);
	void Resize();
private:
	sf::VertexArray array;
	VoxelGrid* grid;

    inline int getIndexHelper(int x, int y, int triangle, int vertex) {
        //index = Cell location + triangle offset + index offset [(y * width + x) * 12 + tri * 3 + vertex]
        return (y * grid->getWidth() + x) * 12 + triangle * 3 + vertex;
    }

    //Functions borrowed from MSObject

    //Indices of triangles in each case, 8 means no more triangles, CW order, starting at lowest
    constexpr static int indexTable[16][13]
    {
        {8,8,8,8,8,8,8,8,8,8,8,8,8},//0
        {0,7,1,8,8,8,8,8,8,8,8,8,8},
        {1,3,2,8,8,8,8,8,8,8,8,8,8},
        {0,7,3,0,3,2,8,8,8,8,8,8,8},
        {3,5,4,8,8,8,8,8,8,8,8,8,8},//4
        {0,7,1,1,7,5,1,5,3,3,5,4,8},
        {1,4,2,1,5,4,8,8,8,8,8,8,8},
        {0,7,2,2,7,5,2,5,4,8,8,8,8},
        {5,7,6,8,8,8,8,8,8,8,8,8,8},//8
        {0,5,1,0,6,5,8,8,8,8,8,8,8},
        {1,3,2,1,5,3,1,7,5,5,7,6,8},
        {0,3,2,0,5,3,0,6,5,8,8,8,8},
        {3,7,4,4,7,6,8,8,8,8,8,8,8},//12
        {0,6,1,1,6,3,3,6,4,8,8,8,8},
        {1,4,2,1,7,4,4,7,6,8,8,8,8},
        {0,4,2,0,6,4,8,8,8,8,8,8,8}
    };

    inline float normalise(float a, float b, float v)
    {
        return (v - a) / (b - a);
    }
    inline sf::Vector2f findMidpointX(sf::Vector2f a, sf::Vector2f b, float aVal, float bVal)
    {
        return sf::Vector2f(a.x + normalise(aVal, bVal, 0) * (b.x - a.x), a.y);
    }

    inline sf::Vector2f findMidpointY(sf::Vector2f a, sf::Vector2f b, float aVal, float bVal)
    {
        return sf::Vector2f(a.x, a.y + normalise(aVal, bVal, 0) * (b.y - a.y));
    }
};