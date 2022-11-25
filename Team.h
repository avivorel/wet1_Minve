//
// Created by Aviv on 11/24/2022.
//

#ifndef SOLUTION_TEAM_H
#define SOLUTION_TEAM_H

#include "AVLTtree.h"
#include "Player.h"
#include <memory>
#include <iostream>

class Team {
    AVLTree<std::shared_ptr<Player*>> players;
    int team_id, points;
    int games_played;
};


#endif //SOLUTION_TEAM_H
