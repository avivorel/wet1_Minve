//
// Created by Aviv on 11/24/2022.
//

#include "Player.h"
#include <memory>


Player::Player(int playerId, int teamId, int gamesplayed, int goals, int cards, bool isGoalKeeper) : player_id(playerId), team_id(teamId), games_played(gamesplayed),
goals(goals), cards(cards), isGoalie(isGoalKeeper), team(nullptr) {};

int Player::getId() const {
    return this->player_id;
}

void Player::setTeam(std::shared_ptr<Team> *newteam) {
    this->team = newteam;
}

std::shared_ptr<Team>* Player::getTeam() {
    return this->team;
}

int Player::comparePlayerId(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    if (player1->getId() > player2->getId()) return 1;
    else if(player1->getId() < player2->getId() ) return -1;
    return 0;
}
