//
// Created by Aviv on 11/24/2022.
//

#include "Player.h"

int Player::getId() const {
    return this->player_id;
}

int Player::comparePlayerId(const Player &player1, const Player &player2) {
    if (player1.getId() > player2.getId()) return 1;
    else if(player1.getId() < player2.getId() ) return -1;
    return 0;
}