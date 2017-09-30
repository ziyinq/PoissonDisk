//
// Created by ziyinqu on 9/29/17.
//
#include <cstdlib>
#include <ctime>
#include <random>
#include "LoadObj.h"
#ifndef POISSONDISK_POISSONDISK3D_H
#define POISSONDISK_POISSONDISK3D_H

#define PI 3.141592653589

struct Point {
    Point(): x(0.f),y(0.f),z(0.f), valid(false) {}
    Point(float X, float Y, float Z): x(X), y(Y), z(Z), valid(true) {}

    float x;
    float y;
    float z;
    bool valid;
};

struct gridPosition{
    gridPosition(int X, int Y, int Z): x(X), y(Y), z(Z){}

    int x;
    int y;
    int z;
};

gridPosition imageToGrid(Point p, float gridSize, boundBox bBox){
    int gridX = (int) ((p.x - bBox.xmin) / gridSize);
    int gridY = (int) ((p.y - bBox.ymin) / gridSize);
    int gridZ = (int) ((p.z - bBox.zmin) / gridSize);
    return gridPosition(gridX, gridY, gridZ);
}

struct gridAttribute{
    gridAttribute(int w, int h, int l, float size, boundBox B): W(w), H(h), L(l), gridSize(size), bBox(B){

        myGrid.resize(H);

        for (auto i = myGrid.begin(); i != myGrid.end(); i++){
            i->resize(W);
            for (auto j = i->begin(); j != i->end(); j++){
                j->resize(L);
            }
        }
    }

    void insert(Point p){
        gridPosition gridP = imageToGrid(p , gridSize, bBox);

        myGrid[gridP.x][gridP.y][gridP.z] = p;
    }

    int W;
    int H;
    int L;
    boundBox bBox;
    float gridSize;
    // TODO optimize data structure?
    std::vector<std::vector<std::vector<Point>>> myGrid;
};


float getDistance(Point p1, Point p2){
    float dis = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y) + (p2.z - p1.z) * (p2.z - p1.z));
    return dis;
}

bool inRectangle(Point thisPoint, const boundBox& bBox){

    if (thisPoint.x > bBox.xmin && thisPoint.x < bBox.xmax && thisPoint.y > bBox.ymin && thisPoint.y < bBox.ymax && thisPoint.z > bBox.zmin && thisPoint.z < bBox.zmax){
        return true;
    }
    return false;
}

bool inNeighbour(const Point& thisPoint, const gridAttribute& thisGrid, float min_dist, const boundBox& bBox){

    gridPosition G = imageToGrid(thisPoint, thisGrid.gridSize, bBox);

    const int d = 2;

    for (int i = G.x - d; i <= G.x + d; i++){
        for (int j = G.y - d; j<= G.y + d; j++){
            for (int k = G.z -d; k <= G.z +d; k++){
                if ( i >= 0 && i < thisGrid.W && j >= 0 && j < thisGrid.H && k >=0 && k <thisGrid.L){
                    Point p = thisGrid.myGrid[i][j][k];
                    if (p.valid && (getDistance(p, thisPoint) < min_dist)){
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

Point generateRandomPointAround(Point thisPoint, float min_dist, const boundBox& bBox, std::mt19937& mt, std::uniform_real_distribution<float>& dist){
    Point newpoint;
    do {
        float r1 = dist(mt);
        float r2 = dist(mt);
        float r3 = dist(mt);

        float radius = (1 + r1) * min_dist;
        float angle1 = 2 * PI * r2;
        float angle2 = 2 * PI * r3;

        float newX = thisPoint.x + radius * cos(angle1) * sin(angle2);
        float newY = thisPoint.y + radius * sin(angle1) * sin(angle2);
        float newZ = thisPoint.z + cos(angle2);

        newpoint.x = newX;
        newpoint.y = newY;
        newpoint.z = newZ;
        newpoint.valid = true;
    }while(!inRectangle(newpoint, bBox));
    return newpoint;
}

Point popRandom(std::vector<Point>& list, std::mt19937& mt, int& index){

    int size = list.size();
    std::uniform_real_distribution<> dist(0, size);

    index = dist(mt);
    Point p = list[index];
    return p;
}

std::vector<Point> generatePoissonDisk3D(boundBox bBox, float min_dist, int new_points_num){
    //create grid
    float gridSize = min_dist / sqrt(2);

    int gridW = (int) ceil((bBox.xmax - bBox.xmin) / gridSize);
    int gridH = (int) ceil((bBox.ymax - bBox.ymin) / gridSize);
    int gridL = (int) ceil((bBox.zmax - bBox.zmin) / gridSize);
    // create grid here
    gridAttribute poissonGrid(gridW, gridH, gridL, gridSize, bBox);

    std::vector<Point> processList;
    std::vector<Point> samplePoints;

    //generate first point randomly
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    float r1 = dist(mt);
    float r2 = dist(mt);
    float r3 = dist(mt);
    float firstX = bBox.xmin + r1 * (bBox.xmax - bBox.xmin);
    float firstY = bBox.ymin + r2 * (bBox.ymax - bBox.ymin);
    float firstZ = bBox.zmin + r3 * (bBox.zmax - bBox.zmin);
    Point firstPoint = Point(firstX, firstY, firstZ);

    //update containers
    processList.push_back(firstPoint);
    samplePoints.push_back(firstPoint);
    poissonGrid.insert(firstPoint);

    int random_index = 0;

    //generate other points from points in queue
    while(!processList.empty()){
        // fetch a new point from processList
        Point nextPoint = popRandom(processList, mt, random_index);
        // if nice point found, stop, if not, erase from processList
        bool found_point=false;
        for (int i = 0; i < new_points_num; i++){
            Point newPoint = generateRandomPointAround(nextPoint, min_dist, bBox, mt, dist);
            // check that point is in the image region
            // and no points exists in the point's neighbourhood
            if (!inNeighbour(newPoint, poissonGrid, min_dist, bBox)){
                //update containers
                processList.push_back(newPoint);
                samplePoints.push_back(newPoint);
                poissonGrid.insert(newPoint);
                found_point = true;
                break;
            }
        }
        if(!found_point) {
            processList.erase(processList.begin()+random_index);
        }
    }
    return samplePoints;
}

#endif //POISSONDISK_POISSONDISK3D_H
