//
// Created by Aviv on 11/24/2022.
//

#include "Player.h"
#include <memory>
#include <utility>

class Team{};


Player::Player(int playerId, int teamId, int gamesplayed, int goals, int cards, bool isGoalKeeper) :
        player_id(playerId), team_id(teamId), games_played(gamesplayed),goals(goals), cards(cards), isGoalie(isGoalKeeper),
        team(), closest_prev(),closest_next() {}

int Player::getId() const {
    return this->player_id;
}

void Player::setTeam(const std::shared_ptr<Team>& newteam)
{
    this->team = newteam;
}

std::shared_ptr<Team> Player::getTeam() {
    return this->team.lock();
}

int Player::getTeamId() const
{
    return this->team_id;
}

int Player::getGamesPlayed() const
{
    return this->games_played;

}
void Player::addTo_GamesPlayed(int games)
{
    this->games_played= this->games_played + games;
}

int Player::getGoals() const
{
    return this->goals;
}
void Player::setGoals(int goals)
{
    this->goals=goals;
}

int Player::getCards() const
{
    return this->cards;
}

void Player::setCards(int cards)
{
    this->cards=cards;
}

bool Player::isGK() const
{
    return this->isGoalie;
}


int Player::comparePlayerId(const std::shared_ptr<Player> &player1, const std::shared_ptr<Player> &player2) {
    if (player1->getId() > player2->getId()) return 1;
    else if(player1->getId() < player2->getId() ) return -1;
    return 0;
}

int Player::comparePlayerGoalsCardsId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b) {
    if (a->goals > b->goals){
        return 1;
    }
    else if (a->goals == b->goals){
        if(a->cards > b->cards){
            return -1;
        }
        else if (a->cards == b->cards){
            if (a->player_id == b->player_id){
                return 0;
            }
            else{
                return a->player_id > b->player_id ? 1 : -1;
            }
        }
        return 1;
    }
    return -1;
}

void Player::setGamesPlayed(int games) {
    this->games_played = games;
}

void Player::setNext(std::shared_ptr<Player> next) {
    //if (next == nullptr || next == NULL){
    //    this->closest_prev = nullptr;
   // }
    this->closest_next = next;
}
void Player::setPrev(std::shared_ptr<Player> prev) {
    //if (prev == nullptr || prev == NULL){
    //    this->closest_prev = nullptr;
    //}
    this->closest_prev = prev;
}
std::shared_ptr<Player> Player::getNext() {
    return this->closest_next.lock();
}
std::shared_ptr<Player> Player::getPrev() {
    return this->closest_prev.lock();
}

