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
    return boundBox(-50, 0, 0, 50);
}
#endif //POISSONDISK_LOADOBJ_H
