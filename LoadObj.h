//
// Created by ziyinqu on 9/6/17.
//
#define TINYOBJLOADER_IMPLEMENTATION

#include <string>
#include "Lib/tiny_obj_loader.h"

#ifndef POISSONDISK_LOADOBJ_H
#define POISSONDISK_LOADOBJ_H
struct boundBox{
    boundBox(float XMIN, float XMAX, float YMIN, float YMAX): xmin(XMIN), xmax(XMAX), ymin(YMIN), ymax(YMAX) {}
    float xmin;
    float xmax;
    float ymin;
    float ymax;
};
boundBox LoadObject(std::string input){

    std::string inputfile = input;
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());

    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }

    if (!ret) {
        exit(1);
    }

    std::vector<float> objX;
    std::vector<float> objY;
    int size = (attrib.vertices.size() / 3);
    for (int i = 0; i < size; i++){
        objX.push_back(attrib.vertices[i*3]);
        objY.push_back(attrib.vertices[i*3+1]);
    }
    auto maxX = std::max_element(objX.begin(), objX.end());
    auto minX = std::min_element(objX.begin(), objX.end());
    auto maxY = std::max_element(objY.begin(), objY.end());
    auto minY = std::min_element(objY.begin(), objY.end());

    return boundBox(*minX, *maxX, *minY, *maxY);
}
#endif //POISSONDISK_LOADOBJ_H
