//
// Created by Aviv on 11/24/2022.
//

#ifndef SOLUTION_PLAYER_H
#define SOLUTION_PLAYER_H

class Team;

class Player {
    int player_id, team_id, games_played, goals, cards;
    bool isGoalie;
    Team* team;
};


#endif //SOLUTION_PLAYER_H
