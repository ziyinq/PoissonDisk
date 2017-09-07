//
// Created by ziyinqu on 9/5/17.
//
#include <cstdlib>
#include <ctime>
#include <random>
#include "LoadObj.h"
#ifndef POISSONDISK_POISSONDISK_H
#define POISSONDISK_POISSONDISK_H

#define PI 3.141592653589

struct Point {
    Point(): x(0.f),y(0.f), valid(false) {}
    Point(float X, float Y): x(X), y(Y), valid(true) {}

    float x;
    float y;
    bool valid;
};

struct gridPosition{
    gridPosition(int X, int Y): x(X), y(Y) {}

    int x;
    int y;
};

gridPosition imageToGrid(Point p, float gridSize, boundBox bBox){
    int gridX = (int) (p.x - bBox.xmin / gridSize);
    int gridY = (int) (p.y - bBox.ymin / gridSize);
    return gridPosition(gridX, gridY);
}

struct gridAttribute{
    gridAttribute(int w, int h, float size, boundBox B): W(w), H(h), gridSize(size), bBox(B){
        myGrid.resize(H);

        for ( auto i = myGrid.begin() ; i != myGrid.end(); i++){
            i->resize(W);
        }
    }

    void insert(Point p, float size){
        gridPosition gridP = imageToGrid(p , gridSize, bBox);

        myGrid[gridP.x][gridP.y] = p;
    }

    int W;
    int H;
    boundBox bBox;
    float gridSize;
    std::vector<std::vector<Point>> myGrid;
};


float getDistance(Point p1, Point p2){
    float dis = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    return dis;
}

bool inRectangle(Point thisPoint, boundBox bBox){

    if (thisPoint.x > bBox.xmin && thisPoint.x < bBox.xmax && thisPoint.y > bBox.ymin && thisPoint.y < bBox.ymax){
        return true;
    }
    return false;
}

bool inNeighbour(Point thisPoint, gridAttribute thisGrid, float min_dist, boundBox bBox){

    gridPosition G = imageToGrid(thisPoint, thisGrid.gridSize, bBox);

    const int d = 5;

    for (int i = G.x - d; i <= G.x + d; i++){
        for (int j = G.y - d; j<= G.y + d; j++){
            if ( i >= 0 && i < thisGrid.W && j >= 0 && j < thisGrid.H){
                Point p = thisGrid.myGrid[i][j];
                if (p.valid && (getDistance(p, thisPoint) < min_dist)){
                    return false;
                }
            }
        }
    }
    return true;
}

Point generateRandomPointAround(Point thisPoint, float min_dist, boundBox bBox){

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    Point newpoint;
    do {
        float r1 = dist(mt);
        float r2 = dist(mt);

        float radius = (1 + r1) * min_dist;
        float angle = 2 * PI * r2;

        float newX = thisPoint.x + radius * cos(angle);
        float newY = thisPoint.y + radius * sin(angle);
        newpoint.x = newX;
        newpoint.y = newY;
    }while(!inRectangle(newpoint, bBox));
    newpoint.valid = true;
    return newpoint;
}

Point popRandom(std::vector<Point>& list){

    std::random_device rd;
    std::mt19937 mt(rd());
    int size = list.size();
    std::uniform_real_distribution<> dist(0, size);

    int index = dist(mt);
    Point p = list[index];
    list.erase(list.begin() + index);
    return p;
}

std::vector<Point> generatePoissonDisk(boundBox bBox, float min_dist, int new_points_num){
    //create grid
    float gridSize = min_dist / sqrt(2);

    int gridW = (int) ceil((bBox.xmax - bBox.xmin) / gridSize);
    int gridH = (int) ceil((bBox.ymax - bBox.ymin) / gridSize);

    // create grid here
    gridAttribute poissonGrid(gridW, gridH, gridSize, bBox);

    std::vector<Point> processList;
    std::vector<Point> samplePoints;


    //generate first point randomly
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.f, 1.f);
    float r1 = dist(mt);
    float r2 = dist(mt);
    float firstX = bBox.xmin + r1 * (bBox.xmax - bBox.xmin);
    float firstY = bBox.ymin + r2 * (bBox.ymax - bBox.ymin);
    Point firstPoint = Point(firstX, firstY);

    //update containers
    processList.push_back(firstPoint);
    samplePoints.push_back(firstPoint);
    poissonGrid.insert(firstPoint, gridSize);

    //generate other points from points in queue
    while(!processList.empty()){
        // fetch a new point from processList
        Point nextPoint = popRandom(processList);
        for (int i = 0; i < new_points_num; i++){
            Point newPoint = generateRandomPointAround(nextPoint, min_dist, bBox);
            // check that point is in the image region
            // and no points exists in the point's neighbourhood
            if (inNeighbour(newPoint, poissonGrid, min_dist, bBox)){
                //update containers
                processList.push_back(newPoint);
                samplePoints.push_back(newPoint);
                poissonGrid.insert(newPoint, gridSize);
            }
        }
    }
    return samplePoints;
}

#endif //POISSONDISK_POISSONDISK_H
