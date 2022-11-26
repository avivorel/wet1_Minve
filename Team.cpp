//
// Created by Aviv on 11/24/2022.
//

#include "Team.h"
#include "AVLTtree.h"
#include <memory>

Team::Team(int teamId, int points) : games_played(0) , points(points), team_id(teamId), players(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId)) {};

int Team::getId() const {
    return this->team_id;
}

int Team::getGamesPlayed() const {
    return this->games_played;
}

int Team::getPoints() const {
    return this->points;
}
