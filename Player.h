//
// Created by Aviv on 11/24/2022.
//

#ifndef SOLUTION_PLAYER_H
#define SOLUTION_PLAYER_H
#include <memory>
class Team;

class Player {
    int player_id, team_id, games_played, goals, cards;
    bool isGoalie;
    Team* team;
public:
    int getId() const;
    int getTeamId() const;
    int getGamesPlayed() const;
    int getGoals() const;
    int getCards() const;
    bool isGK() const;
    static int comparePlayerId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
};



#endif //SOLUTION_PLAYER_H
