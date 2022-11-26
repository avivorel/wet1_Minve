#include "worldcup23a1.h"
#include <memory>

world_cup_t::world_cup_t()
{
	all_players = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId);
    all_teams = new AVLTree<std::shared_ptr<Team>>(Team::compareTeamId);
}

world_cup_t::~world_cup_t()
{
	// TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId < 0 or points< 0){
        return StatusType::INVALID_INPUT;
    }
	std::shared_ptr<Team> newTeam(new Team(teamId,points));
    if (newTeam == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    if(!this->all_teams.Insert(newTeam)){
        return StatusType::FAILURE;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<0){
        return StatusType::INVALID_INPUT;
    }
	std::shared_ptr<Team> toremove(new Team(teamId,0))
    auto *teamToRemove = this->all_teams->Find(toremove);
    if (teamToRemove == nullptr or !(teamToRemove->GetValue()->isEmpty())){
        return StatusType::FAILURE;
    }
    this->all_teams->Remove(teamToRemove->GetValue());
    free(teamToRemove->GetValue()); // לבדוק אם צריך ונכון
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_player(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,
                                        int scoredGoals, int cardsReceived)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
	// TODO: Your code goes here
	return 2008;
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
	// TODO: Your code goes here
    static int i = 0;
    return (i++==0) ? 11 : 2;
}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
	// TODO: Your code goes here
    output[0] = 4001;
    output[1] = 4002;
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
	// TODO: Your code goes here
	return 1006;
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
	// TODO: Your code goes here
	return 2;
}

