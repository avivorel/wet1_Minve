//
// Created by Aviv on 11/24/2022.
//

#include "Team.h"
#include "AVLTtree.h"
#include <memory>

Team::Team(int teamId, int points) :  numberOfGK(0),numberOfPlayers(0),hasGK(false),games_played(0) , points(points), team_id(teamId), players(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId)) {};

int Team::getId() const {
    return this->team_id;
}

int Team::getGamesPlayed() const {
    return this->games_played;
}

int Team::getPoints() const {
    return this->points;
}

int Team::compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b) {
    if(a->team_id > b->team_id) return 1;
    if(a->team_id < b->team_id) return -1;
    return 0;
}

bool Team::isEmpty() const {
    if (this->players->IsEmpty()) return true;
    return false;
}

bool Team::add_player(std::shared_ptr<Player> playerToAdd) {
    if (this->players->Insert(playerToAdd)) {
        this->numberOfPlayers = this->numberOfPlayers + 1;
        if (playerToAdd->isGK()){
            this->hasGK = true;
            this->numberOfGK += 1;
        }
        return true;
    }
    return false;
}

bool Team::removePlayer(std::shared_ptr<Player> toRemove) {
    this->players->Remove(toRemove);
    this->numberOfPlayers = this->numberOfPlayers -1;
    if (toRemove->isGK()){
        this->numberOfGK += -1;
        if (numberOfGK<= 0){
            this->hasGK = false;
        }
    }
}