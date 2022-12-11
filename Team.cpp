//
// Created by Aviv on 11/24/2022.
//

#include "Team.h"
#include "AVLTtree.h"
#include <memory>

Team::Team(int teamId, int points):team_id(teamId),points(points),games_played(0),numberOfPlayers(0),numberOfGK(0),
    hasGK(false),playMatchEquation(0) ,players(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId)),
    players_by_goals(new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerGoalsCardsId)), topScorer(nullptr){};

int Team::getId() const
{
    return this->team_id;
}

int Team::get_PM_Equation() const
{
    return this->playMatchEquation;
}
void Team::addTo_PM_Equation(int data)
{
    this->playMatchEquation+=data;
}

int Team::getGamesPlayed() const
{
    return this->games_played;
}
void Team::addTo_GamesPlayed(int games)
{
    this->games_played+=games;
}
int Team::getPoints() const
{
    return this->points;
}
void Team::addTo_points(int p)
{
    this->points+=p;
}



int Team::compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b)
{
    if(a->team_id > b->team_id) return 1;
    if(a->team_id < b->team_id) return -1;
    return 0;
}

bool Team::isEmpty() const
{
    if (this->players->IsEmpty()) return true;
    return false;
}

bool Team::add_player(std::shared_ptr<Player> playerToAdd)
{

    if (this->players->Insert(playerToAdd) and this->players_by_goals->Insert(playerToAdd)) {
        playerToAdd->setGamesPlayed(playerToAdd->getGamesPlayed()-this->games_played);
        if (topScorer == nullptr){
            this->topScorer = playerToAdd;
        }
        else{
            if (this->topScorer->getGoals() < playerToAdd->getGoals()){
                this->topScorer = playerToAdd;
            }
            else if (this->topScorer->getGoals() == playerToAdd->getGoals()){
                if (this->topScorer->getCards() > playerToAdd->getCards()){
                    topScorer = playerToAdd;
                }
                else if (this->topScorer->getCards() == playerToAdd->getCards()){
                    if (this->topScorer->getId() < playerToAdd->getId()){
                        topScorer = playerToAdd;
                    }
                }
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

bool Team::removePlayer(std::shared_ptr<Player> toRemove)
{
    if (toRemove->getId()  == topScorer->getId()){
        if (this->players->GetRoot()->GetValue()->getId() != toRemove->getId()) {
            topScorer = this->players_by_goals->Find(toRemove)->GetParent()->GetValue();
        }
        else{
            topScorer = nullptr;
        }
    }
    if (this->players->Remove(toRemove) &&  this->players_by_goals->Remove(toRemove)) {
        toRemove->setGamesPlayed(this->games_played + toRemove->getGamesPlayed());
        this->numberOfPlayers = this->numberOfPlayers - 1;
        if (toRemove->isGK()) {
            this->numberOfGK += -1;
            if (numberOfGK <= 0) {
                this->hasGK = false;
            }
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

int Team::getTopScorer() const
{
    return this->topScorer->getId();
}

void Team::PlayersToArray(int number, std::shared_ptr<Player> *playersArray) {
    if (number == 0){ // העץ לפי אידי
        this->players->ToArray(playersArray);
    }
    else if (number == 1) { // העץ לפי גולים
        this->players_by_goals->ToArray(playersArray);
    }
}

int Team::setPoints(int team1Points, int team2Points) {
    this->points = this->points+ team1Points + team2Points;
}

std::shared_ptr<Player> Team::findPlayer(int playerid) const {
    std::shared_ptr<Player> player(new Player(playerid,0,0,0,0,0));
    auto *foundPlayer = this->players->Find(player);
    if (foundPlayer == nullptr){
        return nullptr;
    }
    return foundPlayer->GetValue();
}

void Team::DelPlayers(){
    this->players->DelAll(players->GetRoot());
    this->players_by_goals->DelAll(players_by_goals->GetRoot());
}

Team::~Team() {
    delete players;
    delete players_by_goals;
}
