//
// Created by Aviv on 11/24/2022.
//

#include "Team.h"
#include "AVLTtree.h"
#include <memory>

Team::Team(int teamId, int points):team_id(teamId),points(points),games_played(0),numberOfPlayers(0),numberOfGK(0),
hasGK(false) ,players(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId))
,players_by_goals(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerGoalsCardsId)), topScorer(nullptr){};

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
    if (this->players->Insert(playerToAdd) and this->players_by_goals->Insert(playerToAdd)) {
        if (topScorer == nullptr){
            this->topScorer = playerToAdd;
        }
        else{
            if (this->topScorer->getGoals() < playerToAdd->getGoals()){
                this->topScorer = playerToAdd;
            }
        }
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
    this->players_by_goals->Remove(toRemove);
    this->numberOfPlayers = this->numberOfPlayers -1;
    if (toRemove->isGK()){
        this->numberOfGK += -1;
        if (numberOfGK<= 0){
            this->hasGK = false;
        }
    }
}

bool Team::hasGk() const
{
    return this->hasGK;
}

int  Team::getNumOfPlayers() const
{
    return this->numberOfPlayers;
}

int Team::getTopScorer() const {
    return this->topScorer->getId();
}

