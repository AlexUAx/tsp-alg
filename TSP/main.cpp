//
//  main.cpp
//  Lab8
//
//  Created by Alex Parienko on 4/21/13.
//  Copyright (c) 2013 Alex Parienko. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Point.h"
#include <math.h>
#include "Tree.h"

#define inf 999999

using namespace std;

vector <Point *> *points;     //total points

void getData()              //read points from file to vector
{
    int test = 0;
    cout << "Enter № of file: ";
    cin >> test;
    FILE *inFile;    //input file
    switch (test) {
        case 1:
            inFile = fopen("/data/graph_12_1.txt", "r");    //input file
            break;
        case 2:
            inFile = fopen("/data/graph_12_2.txt", "r");    //input file
            break;
        case 3:
            inFile = fopen("/data/graph_12_3.txt", "r");    //input file
            break;
    }
    points = new vector<Point *>;             //memory allocation
    int total = 0;
    fscanf(inFile, "%d",&total);         //search for data
    
    for (int i = 0; i < total; i++)  {                 //until eof
        Point *point = new Point;              //create new hole
        fscanf(inFile, "%d%d",&point->x,&point->y);         //search for data
        points->push_back(point);             //add new hole
    }
}

float directpointsDistance(Point *start, Point *end)       //get distance beetwen 2 points
//Hole *start, Hole *end - adjacency points
{
    int xSq = pow(start->x-end->x,2);   //square of every coordinate
    int ySq = pow(start->y-end->y,2);
    float result = sqrt(xSq+ySq);    //total distance
    if (start != end) {           //if it has distance
        return result;
    } else {
        return inf;
    }
}

vector<vector<int>> generateAdjacencyMatrix(vector<Point *> *points)
//vector<Hole *> *points - data source
{
    vector<vector<int>> adj(points->size());        //memory allocation
    for (int i = 0; i < points->size(); i++) {
        vector<int> pvector(points->size());
        for (int ii = 0; ii < points->size(); ii++) {
            pvector[ii] = directpointsDistance(points->at(i), points->at(ii));  //calculate distance from i's point to ii's
        }
        adj[i] = pvector;
    }
    return adj;
}

int deleteMinElements(vector<vector<int>> &matrix)
{
    int min = inf;
    int sum = 0;
    for (int i = 0; i < matrix.size(); i++) {
        min = inf;
        for (int ii = 0; ii < matrix.size(); ii++) {
            if (matrix[i][ii] < min) {
                min = matrix[i][ii];
            }
        }
        for (int ii = 0; ii < matrix.size(); ii++) {
            if (matrix[i][ii] != inf) {
                matrix[i][ii] -= min;
            }
        }
        sum += min;
    }
    
    for (int i = 0; i < matrix.size(); i++) {
        min = inf;
        for (int ii = 0; ii < matrix.size(); ii++) {
            if (matrix[ii][i] < min) {
                min = matrix[ii][i];
            }
        }
        for (int ii = 0; ii < matrix.size(); ii++) {
            if (matrix[ii][i] != inf) {
                matrix[ii][i] -= min;
            }
        }
        sum += min;
    }
    
    return sum;
}

int getMinRowColumnElement(vector<vector<int>> matrix, int row, int column)
{
    int sum = 0;
    int min = inf;
    for (int i = 0; i < matrix.size(); i++) {
        if (matrix[i][column] < min && i != row) {
            min = matrix[i][column];
        }
    }
    sum += min;
    min = inf;
    for (int i = 0; i < matrix.size(); i++) {
        if (matrix[row][i] < min && i != column) {
            min = matrix[row][i];
        }
    }
    sum += min;
    if (sum > inf) {
        sum = inf;
    }
    return sum;
}

vector<Point *> zeroPenalty(vector<vector<int>> matrix)
{
    vector<Point *> criticalPoints;
    for (int i = 0; i < matrix.size(); i++) {
        for (int ii = 0; ii < matrix.size(); ii++) {
            if (matrix[i][ii] == 0) {
                Point *point = new Point;
                point->x = i;
                point->y = ii;
                point->penalty = getMinRowColumnElement(matrix, i, ii);
                criticalPoints.push_back(point);
            }
        }
    }
    return criticalPoints;
}

Point *getMaxPoint(vector<Point *> arr, vector<int> ver, vector<int> hor, Item *curr)
{
    int max = -inf;
    Point *maxPoint;
    for (int i = 0; i < arr.size(); i++) {
        if (arr[i]->penalty >= max) {
            max = arr[i]->penalty;
            maxPoint = arr[i];
        }
    }
    return maxPoint;
}

vector<vector<int>> copyMatrix(vector<vector<int>> matrix)
{
    vector<vector<int>> newM(matrix.size());
    for (int i = 0; i < matrix.size(); i++) {
        for (int ii = 0; ii < matrix.size(); ii++) {
            newM[i].push_back(matrix[i][ii]);
        }
    }
    return newM;
}

int positionInVector(vector<int> vec, int num)
{
    int i;
    for (i = 0; i < vec.size(); i++) {
        if (num == vec[i]) {
            return i;
        }
    }
    return inf;
}

vector<vector<int>> matrixForUsed(vector<vector<int>> current, Point *maxPoint, vector<int> hor, vector<int> ver)
{
    vector<vector<int>> matrix = copyMatrix(current);
    int exactX = ver[maxPoint->x];
    int shiftedX = positionInVector(hor, exactX);
    int exactY = hor[maxPoint->y];
    int shiftedY = positionInVector(ver, exactY);
    if (shiftedX != inf && shiftedY != inf) {
        matrix[shiftedY][shiftedX] = inf;
    }
    matrix.erase(matrix.begin() + maxPoint->x);
    for (int i = 0; i < matrix.size(); i++) {
        matrix[i].erase(matrix[i].begin() + maxPoint->y);
    }
    return matrix;
}

vector<Point *> getPointsInPath(Item *current)
{
    vector<Point *> result;
    while (current->parent != NULL) {
        if (current->isUsed) {
            result.push_back(current->point);
        }
        current = current->parent;
    }
    return result;
}

vector<vector<int>> findPaths(vector<Point *> usedPoints)
{
    vector<vector<int>> paths;
    bool found = false;
    while (usedPoints.size()) {
        vector<int> currPath;
        currPath.push_back(usedPoints[0]->x);
        currPath.push_back(usedPoints[0]->y);
        usedPoints.erase(usedPoints.begin());
        do {
            found = false;
            for (int g = 0; g < usedPoints.size(); g++) {
                Point *i = usedPoints[g];
                if (i->x == *(currPath.end() - 1)) {
                    currPath.push_back(i->y);
                    usedPoints.erase(usedPoints.begin() + g);
                    g--;
                    found = true;
                }
                if (i->y == *(currPath.begin())) {
                    currPath.insert(currPath.begin(), i->x);
                    usedPoints.erase(usedPoints.begin() + g);
                    g--;
                    found = true;
                }
            }
        } while (found);
        paths.push_back(currPath);
    }
    return paths;
}

void deleteCycleElements(Item *current)
{
    vector<Point *> usedPoints = getPointsInPath(current);
    if (usedPoints.size()) {
        vector<vector<int>> paths = findPaths(usedPoints);
        for (const auto& i: paths) {
            int start = i[0];
            int end = *(i.end() - 1);
            int corrStart = positionInVector(current->horIDs, start);
            int corrEnd = positionInVector(current->verIDs, end);
            if (corrEnd != inf && corrEnd != inf) {
                current->costMatrix[corrEnd][corrStart] = inf;
            }
        }
    }
}

vector<Point *> compute(Tree *decisions)
{
    Item *current = decisions->findMinCostLeaf();
    int shift = deleteMinElements(current->costMatrix);
    current->cost = shift;
    vector<Point *> penalty;
    Point *maxPoint;
    while (current->costMatrix.size() != 1) {
        if (current->isUsed) {
            deleteCycleElements(current);
            shift = deleteMinElements(current->costMatrix);
            current->cost += shift;
        }
        penalty = zeroPenalty(current->costMatrix);
        maxPoint = getMaxPoint(penalty, current->verIDs, current->horIDs, current);
        
        if (maxPoint) {
            current->used = new Item;
            current->unused = new Item;
            current->used->isUsed = true;
            current->unused->isUsed = false;
            current->used->parent = current;
            current->unused->parent = current;
            current->used->verIDs = current->verIDs;
            current->used->horIDs = current->horIDs;
            current->unused->verIDs = current->verIDs;
            current->unused->horIDs = current->horIDs;
            Point *point = new Point;
            point->x = current->verIDs[maxPoint->x];
            point->y = current->horIDs[maxPoint->y];
            point->penalty = 0;
            current->used->point = point;
            current->unused->point = point;
        } //new nodes init
                
        //unused
        current->unused->cost = current->cost + maxPoint->penalty;
        vector<vector<int>> unusedMatrix = copyMatrix(current->costMatrix);
        unusedMatrix[maxPoint->x][maxPoint->y] = inf;
        int newShift = deleteMinElements(unusedMatrix);
        newShift = 0;
        current->unused->costMatrix = unusedMatrix;
        
        //used
        vector<vector<int>> usedMatrix = matrixForUsed(current->costMatrix, maxPoint, current->used->horIDs, current->used->verIDs);
        int newCost = deleteMinElements(usedMatrix);
        current->used->costMatrix = usedMatrix;
        current->used->cost = current->cost + newCost;
        current->used->horIDs.erase(current->used->horIDs.begin() + maxPoint->y);
        current->used->verIDs.erase(current->used->verIDs.begin() + maxPoint->x);
        
        
        current = decisions->findMinCostLeaf();
    }
    vector<Point *> result = getPointsInPath(current);

    if (current->isUsed) {
        Point *point = new Point;
        point->x = current->verIDs[0];
        point->y = current->horIDs[0];
        result.push_back(point);
    }
    
    return result;
    
}

vector<int> exactCycle(vector<Point *> result)
{
    vector<int> cycle;
    cycle.push_back(result[0]->x + 1);
    int current = result[0]->y + 1;
    result.erase(result.begin());
    while (result.size()) {
        for (int g = 0; g < result.size(); g++) {
            Point *i = result[g];
            if (current - 1 == i->x) {
                cycle.push_back(current);
                current = i->y + 1;
                result.erase(result.begin() + g);
                break;
            }
        }
    }
    cycle.push_back(current);
    return cycle;
}

void printVector(vector<int> vec, vector<vector<int>> matrix)
{
    cout << "Cycle is: ";
    int cost = 0;
    for (const auto& i: vec) {
        cout << i << " ";
    }
    cout << endl;
    for (int i = 0; i < vec.size()-1; i++) {
        cost += matrix[vec[i]-1][vec[i+1]-1];
    }
    cout << "Total cost: " << cost << endl;
}

int main(int argc, const char * argv[])
{
    getData();                                              //read points from file to vector
    vector<vector<int>> adjMatrix = generateAdjacencyMatrix(points);
    Tree *decisions = new Tree(adjMatrix);
    vector<Point *> result = compute(decisions);
    vector<int> formResult = exactCycle(result);
    printVector(formResult, adjMatrix);
}