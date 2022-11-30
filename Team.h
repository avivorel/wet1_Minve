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
    AVLTree<std::shared_ptr<Player>> *players_by_goals;
    std::shared_ptr<Player> topScorer;
    int team_id;
    int points;
    int games_played;
    int numberOfPlayers;
    int numberOfGK;
    bool hasGK;

public:
    explicit Team(int teamid, int points);
    int getGamesPlayed() const;
    int getId() const;
    bool hasGk() const;
    int getNumOfPlayers() const;
    int getPoints() const;
    Player* findPlayer(int playerid) const;
    bool isEmpty() const;
    bool add_player(std::shared_ptr<Player> playerToAdd); // true if succeeds
    bool removePlayer(std::shared_ptr<Player> toRemove);
    static int compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b);
    int getTopScorer() const;

};


#endif //SOLUTION_TEAM_H