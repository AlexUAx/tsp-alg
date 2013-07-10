//
//  Item.h
//  TSP
//
//  Created by Alex Parienko on 5/18/13.
//  Copyright (c) 2013 Alex Parienko. All rights reserved.
//

#ifndef __TSP__Item__
#define __TSP__Item__

#include <iostream>
#include <vector>
#include "Point.h"

class Item {
public:
    Item *parent;
    Item *unused;
    Item *used;
    
    Item();
    
    int cost;
    Point *point;
    bool isUsed;
    std::vector<std::vector<int>> costMatrix;
    std::vector<int> horIDs;
    std::vector<int> verIDs;
};

#endif /* defined(__TSP__Item__) */
