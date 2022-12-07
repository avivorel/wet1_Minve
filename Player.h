//
// Created by Aviv on 11/24/2022.
//

#ifndef SOLUTION_PLAYER_H
#define SOLUTION_PLAYER_H
#include <memory>
class Team;


class Player {
    int player_id;
    int team_id;
    int games_played;
    int goals;
    int cards;
    bool isGoalie;
    std::shared_ptr<Team> team; ////todo need *???
    std::shared_ptr<Player> closest_prev;
    std::shared_ptr<Player> closest_next;

public:
    Player(int playerId, int teamId, int gamesplayed, int goals, int cards, bool isGoalKeeper);
    // להוסיף הורס
    int getId() const;
    int getTeamId() const;
    int getGamesPlayed() const;
    void setGamesPlayed(int games);
    int getGoals() const;
    int getCards() const;
    void addTo_GamesPlayed(int games);
    void setGoals(int goals);
    void setCards(int cards);
    void setNext(std::shared_ptr<Player> next);
    void setPrev(std::shared_ptr<Player> prev);
    std::shared_ptr<Player> getNext();
    std::shared_ptr<Player> getPrev();
    bool isGK() const;
    static int comparePlayerId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
    static int comparePlayerGoalsCardsId(const std::shared_ptr<Player> &a, const std::shared_ptr<Player> &b);
    void setTeam(std::shared_ptr<Team> newteam);
    std::shared_ptr<Team> getTeam();
};



#endif //SOLUTION_PLAYER_H