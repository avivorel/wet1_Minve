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
    if(!this->all_teams->Insert(newTeam)){
        return StatusType::FAILURE;
    }
    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<0){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> toremove(new Team(teamId,0));
    auto *teamToRemove = this->all_teams->Find(toremove);
    if (teamToRemove == nullptr or !(teamToRemove->GetValue()->isEmpty())){
        return StatusType::FAILURE;
    }
    this->all_teams->Remove(teamToRemove->GetValue());
}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,
                                   int goals, int cards, bool goalKeeper)
{
    if(teamId <= 0 or playerId <= 0 or gamesPlayed<0 or goals<0 or cards< 0){
        return StatusType::INVALID_INPUT;
    }
    if (gamesPlayed == 0 and (goals>0 or cards > 0))
    {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Player> player(new Player(playerId, teamId,gamesPlayed,goals,cards,goalKeeper));

    if (player == nullptr)
        return StatusType::ALLOCATION_ERROR;

    auto *foundPlayer = this->all_players->Find(player);

    if (foundPlayer != nullptr)
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Team> team(new Team(teamId,0));

    auto* foundteam = this->all_teams->Find(team);
    if (foundteam == nullptr){
        return StatusType::FAILURE;
    }

    player->setTeam(foundteam->GetValue());
    if (foundteam->GetValue()->add_player(player))
    {
        return StatusType::SUCCESS;
    }
    return StatusType::FAILURE;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if (playerId<= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Player> toRemove(new Player(playerId,0,0,0,0,0));

    if (toRemove == nullptr) /// delete?
        return StatusType::ALLOCATION_ERROR;

    auto *foundPlayer = this->all_players->Find(toRemove);

    if (foundPlayer == nullptr)
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Team> team = foundPlayer->GetValue()->getTeam();
    (team)->removePlayer(foundPlayer->GetValue());
    foundPlayer->GetValue()->setTeam(nullptr);
    return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,int scoredGoals, int cardsReceived)
{

    if (playerId<= 0 || gamesPlayed< 0 || scoredGoals< 0 || cardsReceived< 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Player> toUpdate(new Player(playerId,0,0,0,0,0));

    if (toUpdate == nullptr) /// delete?
        return StatusType::ALLOCATION_ERROR;

    auto *foundPlayer = this->all_players->Find(toUpdate);

    if (foundPlayer == nullptr)
    {
        return StatusType::FAILURE;
    }
    std::shared_ptr<Player> player=foundPlayer->GetValue();

    if (gamesPlayed != 0)
    {
        player->setGamesPlayed(player->getGamesPlayed()+ gamesPlayed);
        /// add?
    }

    player->setGoals(player->getGoals()+ scoredGoals);
    player->setCards(player->getCards()+ cardsReceived);

    return StatusType::SUCCESS;
}

StatusType world_cup_t::play_match(int teamId1, int teamId2)
{
    if (teamId1<= 0 || teamId2<= 0  || teamId1 == teamId2)
    {
        return StatusType::INVALID_INPUT;
    }

    std::shared_ptr<Team> checkteam1(new Team(teamId1,0));
    std::shared_ptr<Team> checkteam2(new Team(teamId2,0));


    if (checkteam1 == nullptr || checkteam2 == nullptr) /// delete?
        return StatusType::ALLOCATION_ERROR;

    auto *team1 = this->all_teams->Find(checkteam1);
    auto *team2 = this->all_teams->Find(checkteam2);

    if (team1 == nullptr || team2 == nullptr || (!(team1->GetValue()->hasGk())) ||team1->GetValue()->getNumOfPlayers()<11
    || (!(team2->GetValue()->hasGk())) || team2->GetValue()->getNumOfPlayers() <11)
    {
        return StatusType::FAILURE;
    }

    
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Player> player(new Player(playerId,0,0,0,0,0));

    auto *foundplayer = this->all_players->Find(player);
    if (foundplayer == nullptr){
        return StatusType::FAILURE;
    }
    if (player == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    else{
        return foundplayer->GetValue()->getGamesPlayed();
    }
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> team(new Team(teamId,0));
    if (team == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    auto *foundteam = this->all_teams->Find(team);
    if (foundteam == nullptr) {
        return StatusType::FAILURE;
    }
    return foundteam->GetValue()->getPoints();
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
