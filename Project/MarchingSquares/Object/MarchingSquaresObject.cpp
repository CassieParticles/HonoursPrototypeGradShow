#include "MarchingSquaresObject.h"

#include <iostream>

#include "../Triangle.h"
#include <tracy/Tracy.hpp>

MarchingSquaresObject::MarchingSquaresObject():voxelGrid(nullptr), isDynamic(false),renderable(&transform),physics(&transform),shouldntDestroy(false)
{

}

MarchingSquaresObject::~MarchingSquaresObject()
{
    if(!shouldntDestroy)
    {
        delete voxelGrid;
    }
}

void MarchingSquaresObject::SetGrid(float* data, int width, int height)
{
    voxelGrid = new VoxelGrid(data,width,height);
}
void MarchingSquaresObject::SetGrid(VoxelGrid* grid)
{
    this->voxelGrid = grid;
}
void MarchingSquaresObject::SetDynamic(bool isDynamic)
{
    this->isDynamic = isDynamic;
}

void MarchingSquaresObject::Init()
{
    if(!voxelGrid)
    {
        std::cout<<"ERROR: NO GRID SET\n";
        return;
    }

    Generate(isDynamic);
}

void MarchingSquaresObject::Update()
{
    if(!voxelGrid){return;}
    physics.Update();
}

MSDrawableObject* MarchingSquaresObject::MakeDrawable()
{
    ZoneScoped
    MSDrawableObject* obj = new MSDrawableObject(transform.GetPositionSf(),sf::Mouse::Button::Right,voxelGrid,-1.0f);

    //Get the physics object, then mark it as something that shouldn't be cleared up (it's being moved)
    obj->AddPhysicsStore(physics.getBody());
    physics.MarkShouldntDestroy();
    shouldntDestroy = true;

    return obj;
}

void MarchingSquaresObject::Render(sf::RenderWindow* window)
{
    if(!voxelGrid){return;}
    renderable.Render(window);
}

std::vector<MarchingSquaresObject*> MarchingSquaresObject::Separate()
{
    ZoneScoped
    std::vector<MarchingSquaresObject*> objects;
    voxelGrid->AddBorder(-1);
    std::vector<VoxelGrid*> subgrids = voxelGrid->GetSubgrids();
    delete voxelGrid;

    objects.push_back(this);
    if(subgrids.size() == 0)
    {
        voxelGrid = nullptr;
        return objects;
    }
    voxelGrid = subgrids.at(0);

    for(int i=1;i<subgrids.size();i++)
    {
        MarchingSquaresObject* newObj = new MarchingSquaresObject();
        objects.push_back(newObj);
        newObj->SetGrid(subgrids.at(i));
        newObj->SetDynamic(isDynamic);
        newObj->SetTransform(transform);
        //newObj->Init();
    }

    return objects;
}

int MarchingSquaresObject::GetTriangleCount() {return triangles.size();}

void MarchingSquaresObject::Generate(bool dynamic)
{
    ZoneScopedN("Generate MS")
    voxelGrid->AddBorder(-1.0f);

    //Set the voxel grid x and y to 0, simplifies calculating if mouse is over object
    if(voxelGrid->getX() || voxelGrid->getY())
    {
        sf::Vector2f position = transform.GetPositionSf();
        position.x += voxelGrid->getX();
        position.y += voxelGrid->getY();

        transform.SetPosition(position);

        voxelGrid->setX(0);
        voxelGrid->setY(0);
    }

    int width = voxelGrid->getWidth();
    int height = voxelGrid->getHeight();

    MarchingSquaresRenderable::MSRenderableBuilder graphicsBuilder = renderable.GetBuilder();
    MarchingSquaresPhysics::MSPhysicsBuilder physicsBuilder = physics.GetBuilder();
    physicsBuilder.SetDynamic(dynamic);

    triangles.clear();

    for(int y=0;y<height - 1; ++y)
    {
        for(int x=0;x<width - 1; ++x)
        {
            //Get the values for each of the 4 corners
            float cornerValues[4]
            {
                voxelGrid->getVoxel(x,y),
                voxelGrid->getVoxel(x+1,y),
                voxelGrid->getVoxel(x+1,y+1),
                voxelGrid->getVoxel(x,y+1)
            };

            //Get which case the cell is
            int index{};
            for(int i=0;i<4;++i)
            {
                index += (cornerValues[i] > 0) << i;
            }

            //Get the indices of the case
            const int* indexSet = indexTable[index];

            //Get the vertex position of each corner
            sf::Vector2f vertices[8];
            vertices[0] = sf::Vector2f(static_cast<float>(x + voxelGrid->getX()), static_cast<float>(y + voxelGrid->getY()) );
            vertices[2] = sf::Vector2f(static_cast<float>(x + 1 + voxelGrid->getX()), static_cast<float>(y + voxelGrid->getY()) );
            vertices[4] = sf::Vector2f(static_cast<float>(x + 1 + voxelGrid->getX()), static_cast<float>(y + 1 + voxelGrid->getY()) );
            vertices[6] = sf::Vector2f(static_cast<float>(x + voxelGrid->getX()), static_cast<float>(y + 1 + voxelGrid->getY()) );

            //Calculate the vertex position of each midpoint (lerp between positions)
            vertices[1] = findMidpointX(vertices[0],vertices[2],cornerValues[0],cornerValues[1]);
            vertices[3] = findMidpointY(vertices[2],vertices[4],cornerValues[1],cornerValues[2]);
            vertices[5] = findMidpointX(vertices[6],vertices[4],cornerValues[3],cornerValues[2]);
            vertices[7] = findMidpointY(vertices[0],vertices[6],cornerValues[0],cornerValues[3]);

            //Assemble triangles from indices using vertices
            for(int i=0;indexSet[i] != 8;i+=3)
            {
                Triangle triangle = {
                    vertices[indexSet[i + 0]],
                    vertices[indexSet[i + 1]],
                    vertices[indexSet[i + 2]]
                };

                if((triangle.A - triangle.B).length() < 0.01f || (triangle.A - triangle.C).length() < 0.01f || (triangle.B - triangle.C).length() < 0.01f)
                {
                    std::cout<<"Uh oh!\n";
                }

                triangles.push_back(triangle);
                graphicsBuilder.AddTriangle(triangle);
                physicsBuilder.AddTriangle(triangle);
            }
        }
    }

    graphicsBuilder.Build();
    physicsBuilder.Build();
}
