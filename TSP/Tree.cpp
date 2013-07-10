//
//  Tree.cpp
//  TSP
//
//  Created by Alex Parienko on 5/18/13.
//  Copyright (c) 2013 Alex Parienko. All rights reserved.
//

#include "Tree.h"

Tree::Tree(std::vector<std::vector<int>> data)
{
    root = new Item;
    root->cost = 0;
    root->costMatrix = data;
    std::vector<int> IDs(data.size());
    for (int i = 0; i < data.size(); i++) {
        IDs[i] = i;
    }
    root->horIDs = IDs;
    root->verIDs = IDs;
    root->isUsed = true;
}

Item *Tree::findMinCostLeaf()
{
    return findMinCostLeaf(root);
}

Item *Tree::findMinCostLeaf(Item *stNode)
{
    if (stNode->used == NULL && stNode->unused == NULL)
    {
        return stNode;
    } else {
        Item *used = findMinCostLeaf(stNode->used);
        Item *unused = findMinCostLeaf(stNode->unused);
        if (used->cost <= unused->cost) {
            return used;
        } else {
            return unused;
        }
    }
}