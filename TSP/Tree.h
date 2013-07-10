//
//  Tree.h
//  TSP
//
//  Created by Alex Parienko on 5/18/13.
//  Copyright (c) 2013 Alex Parienko. All rights reserved.
//

#ifndef __TSP__Tree__
#define __TSP__Tree__

#include <iostream>
#include "Item.h"
#include <vector>

class Tree {
    Item *root;
    
public:
    Tree(std::vector<std::vector<int>> data);
    Item *findMinCostLeaf();
    Item *findMinCostLeaf(Item *stNode);
};

#endif /* defined(__TSP__Tree__) */
