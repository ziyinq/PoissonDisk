#include <iostream>
#include <fstream>
#include <vector>
#include "Eigen/Dense"
//#include "PoissonDisk2D.h"
#include "PoissonDisk3D.h"


using namespace std;
using Eigen::MatrixXd;

int main(int argc, char *argv[]) {

//    if (argv[1] == NULL){
//        std::cerr << "No file name!" << std::endl;
//        exit(1);
//    }
    //std::string file = "cube.obj";
    //boundBox bBox = LoadObject(file);
    boundBox bBox(0, 50, 0, 50, 0, 50);
    std::vector<Point> poissonDisk = generatePoissonDisk3D(bBox, 1, 30);

    ofstream outfile;
    outfile.open("points.obj");

    int size = poissonDisk.size();
    cout << size << endl;
    for (int i = 0; i < size; i++){
        outfile << "v" << " " << poissonDisk[i].x << " " << poissonDisk[i].y << " " << poissonDisk[i].z << endl;
    }
    outfile.close();

    return 0;
}