//
// Created by Aviv on 11/24/2022.
//

#include "Team.h"
#include "Player.h"


Team::Team(int teamid, int points) : players(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId)), points(points), team_id(teamid) {};

int Team::getId() const {
    return this->team_id;
}

int Team::getGamesPlayed() const {
    return this->games_played;
}

int Team::getPoints() const {
    return this->points;
}
