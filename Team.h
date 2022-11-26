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

    AVLTree<std::shared_ptr<Player>> *players;
    int team_id, points;
    int games_played;

public:
    Team(int teamid, int points);
    int getGamesPlayed() const;
    int getId() const;
    int getPoints() const;
    Player* findPlayer(int playerid) const;


};


#endif //SOLUTION_TEAM_H
