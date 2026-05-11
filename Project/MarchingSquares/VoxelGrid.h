#pragma once
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>

class VoxelGrid
{
public:
    VoxelGrid();
    VoxelGrid(float* data, int width, int height, int x=0, int y=0);

    ~VoxelGrid();

    float *GetData() const;

    VoxelGrid* Separate(int x, int y, int width, int height);
    std::vector<VoxelGrid*> GetSubgrids();

    //Read values from the grid
    float getVoxel(int x, int y) const;
    float& operator[](int index) const;

    [[nodiscard]]int getWidth() const {return width;}
    [[nodiscard]]int getHeight() const {return height;}

    [[nodiscard]]int getX() const { return x; }
    [[nodiscard]]int getY() const { return y; }
    [[nodiscard]]float getAngle() const { return angle; }

    void setX(int x){this->x = x;}
    void setY(int y){this->y = y;}

    void setAngle(float angle){this->angle = angle;}

    void PrintValues();
    void AddBorder(float defaultValue = 0.0f);

    void AddValueCircle(sf::Vector2f position, float radius, float value);

    //Get if the grid has been resized since last check (then set it to false)
    bool getResize();
private:
    void AddColumnLeft(float defaultValue = 0.0f);
    void AddColumnRight(float defaultValue = 0.0f);
    void AddRowTop(float defaultValue = 0.0f);
    void AddRowBottom(float defaultValue = 0.0f);

    VoxelGrid* CreateSubgrid(int x, int y, float* gridCopy);

    float* voxelGrid;

    //Width and height of the grid
    int width;
    int height;

    //Local position of top left corner
    int x;
    int y;

    //Rotation of the grid
    float angle;

    //Handling updating of grid
    bool hasBeenResized;
};
