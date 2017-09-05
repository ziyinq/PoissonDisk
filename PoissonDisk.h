//
// Created by ziyinqu on 9/5/17.
//
#include <cstdlib>
#include <ctime>
#ifndef POISSONDISK_POISSONDISK_H
#define POISSONDISK_POISSONDISK_H

#define PI 3.141592653589

struct Point {
    Point(): x(0.f),y(0.f), valid(false)
    {}
    Point(float X, float Y): x(X), y(Y), valid(true)
    {}

    float x;
    float y;
    bool valid;
};

class PoissonDisk {

};

Point generateRandomPointAround(Point thisPoint, float min_dist){

    srand(time(NULL));

    float r1 = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);
    float r2 = static_cast<float> (rand()) / static_cast<float> (RAND_MAX);

    float radius = (1 + r1) * min_dist;
    float angle = 2 * PI * r2;

    float newX = thisPoint.x + radius * cos(angle);
    float newY = thisPoint.y * sin(angle);

    return Point(newX, newY);
}

bool inRectangle(Point thisPoint){

}

bool inNeighbour(Point thisPoint){

}

std::vector<Point> generatePoissonDisk(float w, float h, float min_dist, int new_points_num){
    //create grid
    float gridSize = min_dist / sqrt(2);

    int gridW = (int) ceil(w / gridSize);
    int gridH = (int) ceil(h / gridSize);

    //TODO create grid here

    std::vector<Point> processList;
    std::vector<Point> samplePoints;

    //generate first point randomly
    srand(time(NULL));
    float firstX = static_cast<float> (rand()) * static_cast<float>(gridW) ;
    float firstY = static_cast<float> (rand()) * static_cast<float>(gridH) ;
    Point firstPoint = Point(firstX, firstY);

    //update containers
    processList.push_back(firstPoint);
    samplePoints.push_back(firstPoint);

    //generate other points from points in queue
    while(!processList.empty()){
        Point nextPoint;
        for (int i = 0; i < new_points_num; i++){
            Point newPoint = generateRandomPointAround(nextPoint, min_dist);
            // check that point is in the image region
            // and no points exists in the point's neighbourhood
            if (inRectangle(newPoint) && inNeighbour(newPoint)){
                //update containers
                processList.push_back(newPoint);
                samplePoints.push_back(newPoint);
                //TODO add grid here
            }
        }
    }
    return samplePoints;
}



#endif //POISSONDISK_POISSONDISK_H
