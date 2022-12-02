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

    int team_id;
    int points;
    int games_played;
    int numberOfPlayers;
    int numberOfGK;
    bool hasGK;
    int playMatchEquation;
    AVLTree<std::shared_ptr<Player>> *players;
    AVLTree<std::shared_ptr<Player>> *players_by_goals;
    std::shared_ptr<Player> topScorer;

public:
    explicit Team(int teamid, int points);
    // להוסיף הורס
    int getGamesPlayed() const;
    void addTo_GamesPlayed(int games);
    int get_PM_Equation() const;
    void addTo_PM_Equation(int data);
    int getId() const;
    bool hasGk() const;
    int getNumOfPlayers() const;
    int getPoints() const;
    void addTo_points(int p);
    Player* findPlayer(int playerid) const;
    bool isEmpty() const;
    bool add_player(std::shared_ptr<Player> playerToAdd); // true if succeeds
    bool removePlayer(std::shared_ptr<Player> toRemove);
    static int compareTeamId(const std::shared_ptr<Team> &a, const std::shared_ptr<Team> &b);
    int getTopScorer() const;
    void PlayersToArray(int number, std::shared_ptr<Player> *playersArray);

};


#endif //SOLUTION_TEAM_H