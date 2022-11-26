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
    std::shared_ptr<Team> *team;
public:
    Player(int playerId, int teamId, int gamesplayed, int goals, int cards, bool isGoalKeeper);
    int getId() const;
    int getTeamId() const;
    int getGamesPlayed() const;
    int getGoals() const;
    int getCards() const;
    bool isGK() const;
    static int comparePlayerId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
    void setTeam(std::shared_ptr<Team> *newteam);
    std::shared_ptr<Team>* getTeam();
};



#endif //SOLUTION_PLAYER_H
