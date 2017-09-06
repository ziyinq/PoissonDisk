//
// Created by ziyinqu on 9/5/17.
//
#include <cstdlib>
#include <ctime>
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

gridPosition imageToGrid(Point p, float gridSize){
    int gridX = (int) (p.x / gridSize);
    int gridY = (int) (p.y / gridSize);
    return gridPosition(gridX, gridY);
}

struct gridAttribute{
    gridAttribute(int w, int h, float size): W(w), H(h), gridSize(size){
        myGrid.resize(H);

        for ( auto i = myGrid.begin() ; i != myGrid.end(); i++){
            i->resize(W);
        }
    }

    void insert(Point p, float size){
        gridPosition gridP = imageToGrid(p , gridSize);

        myGrid[gridP.x][gridP.y] = p;
    }

    int W;
    int H;
    float gridSize;
    std::vector<std::vector<Point>> myGrid;
};


float getDistance(Point p1, Point p2){
    float dis = sqrt((p2.x - p1.x) * (p2.x - p1.x) + (p2.y - p1.y) * (p2.y - p1.y));
    return dis;
}

bool inRectangle(Point thisPoint, float w, float h){

    if (thisPoint.x > 0 && thisPoint.x < w && thisPoint.y > 0 && thisPoint.y <h){
        return true;
    }
    return false;
}

bool inNeighbour(Point thisPoint, gridAttribute thisGrid, float min_dist){

    gridPosition G = imageToGrid(thisPoint, thisGrid.gridSize);

    const int d = 2;

    for (int i = G.x - d; i <= G.x + d; i++){
        for (int j = G.y - d; j<= G.y + d; j++){
            if ( i >= 0 && i < thisGrid.W && j >= 0 && j < thisGrid.W){
                Point p = thisGrid.myGrid[i][j];
                if (p.valid && getDistance(p, thisPoint) < min_dist){
                    return false;
                }
            }
        }
    }
    return true;
}

Point generateRandomPointAround(Point thisPoint, float min_dist, float w, float h){

    srand(time(NULL));
    Point newpoint;
    do {
        float r1 = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
        float r2 = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);

        float radius = (1 + r1) * min_dist;
        float angle = 2 * PI * r2;

        float newX = thisPoint.x + radius * cos(angle);
        float newY = thisPoint.y + radius * sin(angle);
        newpoint.x = newX;
        newpoint.y = newY;
    }while(!inRectangle(newpoint, w, h));
    newpoint.valid = true;
    return newpoint;
}

Point popRandom(std::vector<Point>& list){

    srand(time(NULL));

    int index = rand() % list.size();
    Point p = list[index];
    list.erase(list.begin() + index);
    return p;
}

std::vector<Point> generatePoissonDisk(float w, float h, float min_dist, int new_points_num){
    //create grid
    float gridSize = min_dist / sqrt(2);

    int gridW = (int) ceil(w / gridSize);
    int gridH = (int) ceil(h / gridSize);

    // create grid here
    gridAttribute poissonGrid(gridW, gridH, gridSize);

    std::vector<Point> processList;
    std::vector<Point> samplePoints;


    //generate first point randomly
    srand(time(NULL));
    float firstX = static_cast<float> (rand()) / static_cast<float>(RAND_MAX) * w ;
    float firstY = static_cast<float> (rand()) / static_cast<float>(RAND_MAX) * h ;
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
            Point newPoint = generateRandomPointAround(nextPoint, min_dist, w ,h);
            // check that point is in the image region
            // and no points exists in the point's neighbourhood
            if (inNeighbour(newPoint, poissonGrid, min_dist)){
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
