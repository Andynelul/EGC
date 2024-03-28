#include "object2D.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateRectangle(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color)
    };

    Mesh* rectangle = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        rectangle->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    rectangle->InitFromData(vertices, indices);
    return rectangle;
}
Mesh* object2D::CreateDiamond(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float height,
    glm::vec3 color,
    bool fill)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat( glm::vec3(-length, 0, 1), color),
       VertexFormat( glm::vec3(0, height, 1), color),
       VertexFormat(glm::vec3(length, 0, 1), color),
        VertexFormat( glm::vec3(0,-height, 1), color)

    };
    Mesh* diamond = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3,0,2 };
    diamond->InitFromData(vertices, indices);
    return diamond;
}
Mesh* object2D::CreateTank(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float length,
    float height,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices2 =
    {
           VertexFormat(glm::vec3(0, - height  / 4, 1), color),
           VertexFormat( glm::vec3(0, height  / 4, 1), color),
           VertexFormat(glm::vec3(length  , height  / 4, 1), color),
          VertexFormat( glm::vec3(length  ,-height / 4, 1), color)
    };
    Mesh* tank = new Mesh(name);
    std::vector<unsigned int> indices2 = { 0, 1, 2, 3 };


    if (!fill) {
        tank->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices2.push_back(0);
        indices2.push_back(2);
    }

    tank->InitFromData(vertices2, indices2);
    return tank;
}
Mesh* object2D::CreateStar(
    const std::string& name,
    glm::vec3 leftBottomCorner,
    float R,
    glm::vec3 color,
    bool fill)
{
    float height = R;
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner + glm::vec3(height*cos(RADIANS(0)), sin(RADIANS(0)) * height, 0), color),
        VertexFormat(corner + glm::vec3(cos(RADIANS(72)) * height, sin(RADIANS(72)) * height, 0), color),
        VertexFormat(corner + glm::vec3(cos(RADIANS(144)) * height, sin(RADIANS(144)) * height , 0), color),
        VertexFormat(corner + glm::vec3(cos(RADIANS(216)) * height ,sin(RADIANS(216)) * height, 0), color),
        VertexFormat(corner + glm::vec3(cos(RADIANS(288)) * height ,sin(RADIANS(288)) * height, 0), color),
        VertexFormat(corner + glm::vec3(height * 2 * (cos(RADIANS(0)) + cos(RADIANS(72))), (sin(RADIANS(72)) + sin(RADIANS(0))) * height * 2, 0), color),
        VertexFormat(corner + glm::vec3(height * 2 * (cos(RADIANS(144)) + cos(RADIANS(72))), (sin(RADIANS(72)) + sin(RADIANS(144))) * height * 2, 0), color),
        VertexFormat(corner + glm::vec3(height * 2 * (cos(RADIANS(144)) + cos(RADIANS(216))), (sin(RADIANS(144)) + sin(RADIANS(216))) * height *2, 0), color),
        VertexFormat(corner + glm::vec3(height* 2 * (cos(RADIANS(216)) + cos(RADIANS(288))), (sin(RADIANS(216)) + sin(RADIANS(288))) * height * 2, 0), color),
        VertexFormat(corner + glm::vec3(height * 2 * (cos(RADIANS(0)) + cos(RADIANS(288))), (sin(RADIANS(0)) + sin(RADIANS(288))) * height * 2, 0), color),

    };
    Mesh* star = new Mesh(name);
    std::vector<unsigned int> indices = { 0,2,1,0,3,2,0,4,3,
    0, 1,5,  // Triangle 1
    1, 2, 6,  // Triangle 2
    2, 3, 7,  // Triangle 3
    3, 4, 8,  // Triangle 4
    4, 0, 9,  // Triangle 5
    };
    star->InitFromData(vertices, indices);
    return star;
}
Mesh* object2D::CreatePolygon(
    const std::string& name,
    glm::vec3 center,
    float radius,
    glm::vec3 color)
{
    std::vector<VertexFormat> vertices={
        VertexFormat(center + glm::vec3(radius * cos(RADIANS(0)), sin(RADIANS(0)) * radius, 0), color),
        VertexFormat(center + glm::vec3(cos(RADIANS(60)) * radius, sin(RADIANS(60)) * radius, 0), color),
        VertexFormat(center + glm::vec3(cos(RADIANS(120)) * radius, sin(RADIANS(120)) * radius , 0), color),
        VertexFormat(center + glm::vec3(cos(RADIANS(180)) * radius ,sin(RADIANS(180)) * radius, 0), color),
        VertexFormat(center + glm::vec3(cos(RADIANS(240)) * radius ,sin(RADIANS(240)) * radius, 0), color),
        VertexFormat(center + glm::vec3(radius * cos(RADIANS(300)), sin(RADIANS(300)) * radius, 0), color),
       
    };
    Mesh* polygon = new Mesh(name);
    std::vector<unsigned int> indices = { 0,2,1,0,3,2,0,4,3,0,5,4 };

    polygon->InitFromData(vertices, indices);

    return polygon;
}