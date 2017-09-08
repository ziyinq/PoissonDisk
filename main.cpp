#include <iostream>
#include <fstream>
#include <vector>
#include "Eigen/Dense"
#include "PoissonDisk.h"

using namespace std;
using Eigen::MatrixXd;

int main(int argc, char *argv[]) {

//    if (argv[1] == NULL){
//        std::cerr << "No file name!" << std::endl;
//        exit(1);
//    }
    //std::string file = "cube.obj";
    //boundBox bBox = LoadObject(file);
    boundBox bBox(0, 10, 0, 10);
    std::clock_t start;
    double duration1;
    std::vector<double> duration;
    start = std::clock();
    std::vector<Point> poissonDisk = generatePoissonDisk(bBox, 1, 30);
    duration1 = (std::clock() - start) / (double) CLOCKS_PER_SEC;
    std::cout << "duration = " << duration1 << std::endl;

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