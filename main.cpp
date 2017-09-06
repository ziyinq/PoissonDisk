#define TINYOBJLOADER_IMPLEMENTATION
#include <iostream>
#include <fstream>
#include <vector>
#include "Lib/tiny_obj_loader.h"
#include "Eigen/Dense"
#include "PoissonDisk.h"

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
    std::vector<Point> poissonDisk = generatePoissonDisk(50, 50, 1, 30);

    ofstream outfile;
    outfile.open("points.txt");

    int size = poissonDisk.size();
    cout << size << endl;
    for (int i = 0; i < size; i++){
        outfile << poissonDisk[i].x << " " << poissonDisk[i].y << endl;
    }
    outfile.close();

    return 0;
}