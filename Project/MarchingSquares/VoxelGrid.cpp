#include "VoxelGrid.h"

#include <iostream>
#include <stack>
#include <SFML/Graphics.hpp>
#include <tracy/Tracy.hpp>

VoxelGrid::VoxelGrid(): width(1),height(1),x(0),y(0),voxelGrid(new float[1])
{
    voxelGrid[0] = -1.0f;
}

VoxelGrid::VoxelGrid(float* data, int width, int height, int x, int y): width(width), height(height), x(x), y(y)
{
    if(data!=nullptr)
    {
        voxelGrid = new float[width * height];
        std::copy(data,data + width * height,voxelGrid);
    }
}

VoxelGrid::~VoxelGrid()
{
    delete[] voxelGrid;
}

VoxelGrid* VoxelGrid::Separate(int x, int y, int width, int height)
{
    ZoneScoped;
    VoxelGrid* newGrid = new VoxelGrid();

    newGrid->width = width;
    newGrid->height = height;
    newGrid->x = x + this->x;
    newGrid->y = y + this->y;
    newGrid->voxelGrid = new float[width * height];

    for(int row = 0; row < height; row++)
    {
        std::copy_n(voxelGrid + (row + y) * this->width + x, width, newGrid->voxelGrid + row * width);
    }

    return newGrid;
}

std::vector<VoxelGrid*> VoxelGrid::GetSubgrids()
{
    ZoneScoped;
    std::vector<VoxelGrid*> subgrids;
    //Create copy of the grid to check(this may be expensive)
    //Iterate through grid until +ve value reached
    //-Add position to "check list"
    //-Iterate through neighbors, adding +ve to checklist (set them to -1 to prevent rechecking)
    //-Keep track of upper/lower bounds of x & y
    //-Once "check list" is empty, create subgrid using bounds, and add it to subgrids vector

    //Copy grid, this will be destroyed during operation
    float* gridCopy = new float[width * height];
    std::copy(voxelGrid,voxelGrid + width * height,gridCopy);

    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            //Skip values less than 0
            if(gridCopy[y * width + x] < 0){continue;}
            float val = gridCopy[y * width + x];
            subgrids.push_back(CreateSubgrid(x,y, gridCopy));
        }
    }

    delete[] gridCopy;

    return subgrids;
}

VoxelGrid* VoxelGrid::CreateSubgrid(int x, int y, float* gridCopy)
{
    ZoneScoped
    std::stack<sf::Vector2i> checkList;
    std::stack<sf::Vector2i> points;
    sf::Vector2i lowerBound{x,y};
    sf::Vector2i upperBound{x,y};

    //Corners (for easier iteration)
    constexpr sf::Vector2i neighbors[8]
    {
      { 1, 0},
      { 1, 1},
      { 0, 1},
      {-1, 1},
      {-1, 0},
      {-1,-1},
      { 0,-1},
      { 1,-1}
    };

    checkList.push({x,y});
    while(checkList.size() > 0)
    {
        //Get checked position(compare against bounds)
        sf::Vector2i current = checkList.top();
        checkList.pop();
        points.push(current);

        lowerBound.x = std::min(lowerBound.x,current.x);
        lowerBound.y = std::min(lowerBound.y,current.y);

        upperBound.x = std::max(upperBound.x,current.x);
        upperBound.y = std::max(upperBound.y,current.y);

        //Check neighbors
        for(int i=0;i<8;++i)
        {
            sf::Vector2i neighbor = current + neighbors[i];
            //Bounds checking(should never occur if boundary is set up properly)
            if(neighbor.x < 0 || neighbor.x >= width || neighbor.y < 0 || neighbor.y >= height){continue;}
            //Add +ve neighbors to checklist
            if(gridCopy[neighbor.y * width + neighbor.x] > 0)
            {
                checkList.push(neighbor);
            }
        }
        //Set checked value to -1 so it's not checked again
        gridCopy[current.y * width + current.x] = -1;
    }

    sf::Vector2i size = upperBound - lowerBound;
    VoxelGrid* grid = Separate(lowerBound.x - 1,lowerBound.y - 1, size.x + 3, size.y + 3);

    //Update actual grid to reflect changes
    while(points.size() > 0)
    {
        sf::Vector2i point = points.top();
        points.pop();
        voxelGrid[point.y * width + point.x] = -1;
    }

    return grid;
}

float *VoxelGrid::GetData() const {return voxelGrid;}
float VoxelGrid::getVoxel(int x, int y) const {return voxelGrid[y * width + x];}
float& VoxelGrid::operator[](int index) const {return voxelGrid[index];}

void VoxelGrid::PrintValues()
{
    for(int i=0;i<height;++i)
    {
        std::cout<<"[";
        for(int j=0;j<width;++j)
        {
            std::cout<<voxelGrid[i * width + j];
            if(j + 1 != width) { std::cout<<", "; }
        }
        std::cout<<"]"<<std::endl;
    }
}

void VoxelGrid::AddBorder(float defaultValue)
{
    ZoneScoped
    AddColumnLeft(defaultValue);
    AddColumnRight(defaultValue);
    AddRowTop(defaultValue);
    AddRowBottom(defaultValue);
}



void VoxelGrid::AddValueCircle(sf::Vector2f position, float radius, float value)
{
    ZoneScoped
    //If point is out of bounds, extend the grid
    sf::Vector2i lowerBound = sf::Vector2i{x,y};
    sf::Vector2i upperBound = sf::Vector2i{x+width,y+height};

    //Add to the left
    if(position.x - radius < lowerBound.x)
    {
        for(int i=0;i<-(position.x - radius - lowerBound.x);i++)
        {
            AddColumnLeft(-1.0f);
        }
    }
    //Add to the right
    if(position.x + radius > upperBound.x)
    {
        for(int i=0;i<position.x + radius - lowerBound.x;i++)
        {
            AddColumnRight(-1.0f);
        }
    }
    //Add to the top
    if(position.y - radius < lowerBound.y)
    {
        for(int i=0;i<-(position.y - radius - lowerBound.y);i++)
        {
            AddRowTop(-1.0f);
        }
    }
    //Add to the bottom
    if(position.y +  radius > upperBound.y)
    {
        for(int i=0;i<position.y + radius - lowerBound.y;i++)
        {
            AddRowBottom(-1.0f);
        }
    }

    for(int y=-radius;y<radius;y++)
    {
        for(int x=-radius;x<radius;x++)
        {
            float distanceSqr = ((x * x) + (y * y)) / (radius * radius);
            if(distanceSqr > 1){continue;}
            float scalar = 1 - distanceSqr;

            int index = (y + static_cast<int>(position.y) - this->y) * width + (x + static_cast<int>(position.x) - this->x);

            voxelGrid[index] = std::clamp(voxelGrid[index] + value * scalar,-1.0f,1.0f);
            if(abs(voxelGrid[index]) < 0.05f)
            {
                voxelGrid[index] = signbit(voxelGrid[index]) ? -0.05f: 0.05f;
            }
        }
    }
}

void VoxelGrid::AddColumnLeft(float defaultValue)
{
    ZoneScoped
    float* newArr = new float[(width + 1) * height];

    //Copy old data (one row at a time)
    for(int row=0;row < height;row++)
    {
        newArr[row * (width + 1)] = defaultValue;
        //Set default value in new space
        std::copy(voxelGrid + row * width, voxelGrid + (row + 1) * width, newArr + row * (width + 1) + 1);
    }

    //Overwrite with new array
    delete[] voxelGrid;
    voxelGrid = newArr;

    width++;
    x--;
}

void VoxelGrid::AddColumnRight(float defaultValue)
{
    ZoneScoped
    float* newArr = new float[(width + 1) * height];

    //Copy old data(one row at a time)
    for(int row=0;row<height;row++)
    {
        std::copy(voxelGrid + row * width, voxelGrid + (row + 1) * width, newArr + row * (width + 1));
        //Set default value in new space
        newArr[(row + 1) * (width + 1) - 1] = defaultValue;
    }

    //Overwrite with new array
    delete[] voxelGrid;
    voxelGrid = newArr;

    width++;
}

void VoxelGrid::AddRowTop(float defaultValue)
{
    ZoneScoped
    //Create larger array
    float* newArr = new float[width * (height + 1)];

    //Copy old data into array (offset by width)
    std::copy(voxelGrid,voxelGrid + (width * height), newArr + width);

    //Set default value in new space
    for(int i=0;i<width;++i)
    {
        newArr[i] = defaultValue;
    }

    //Overwrite with new array
    delete[] voxelGrid;
    voxelGrid = newArr;

    height++;
    y--;
}

void VoxelGrid::AddRowBottom(float defaultValue)
{
    ZoneScoped
    //Create larger array
    float* newArr = new float[width * (height + 1)];

    //Copy old data into array
    std::copy(voxelGrid,voxelGrid + (width * height), newArr);

    //Set default value in new space
    for(int i=0;i<width;++i)
    {
        newArr[width * height + i] = defaultValue;
    }

    //Overwrite with new array
    delete[] voxelGrid;
    voxelGrid = newArr;

    height++;
}
