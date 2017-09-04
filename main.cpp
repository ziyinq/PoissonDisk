#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include "Lib/tiny_obj_loader.h"
#include "Eigen/Dense"

using namespace std;
using Eigen::MatrixXd;

int main(int argc, char *argv[]) {

//    std::string inputfile = argv[1];
//    tinyobj::attrib_t attrib;
//    std::vector<tinyobj::shape_t> shapes;
//    std::vector<tinyobj::material_t> materials;
//
//    std::string err;
//    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, inputfile.c_str());
//
//    if (!err.empty()) { // `err` may contain warning message.
//        std::cerr << err << std::endl;
//    }
//
//    if (!ret) {
//        exit(1);
//    }
//    return 0;
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    cout << m << endl;
}