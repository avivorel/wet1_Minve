#include "worldcup23a1.h"
#include <memory>

world_cup_t::world_cup_t()
{
    all_players = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId);
    all_teams = new AVLTree<std::shared_ptr<Team>>(Team::compareTeamId);
    all_players_by_goals = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerGoalsCardsId);
    topScorer = nullptr;
    numberOfPlayers = 0;
}

world_cup_t::~world_cup_t()
{
    // TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId < 0 or points< 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> newTeam(new Team(teamId,points));
    if (newTeam == nullptr)
    {
        return StatusType::ALLOCATION_ERROR;
    }
    if(!this->all_teams->Insert(newTeam))
    {
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
        all_players->Insert(player);
        all_players_by_goals->Insert(player);
        AVLNode<std::shared_ptr<Player>> *PlayerNode = all_players_by_goals->Find(player);
        AVLNode<std::shared_ptr<Player>> *next = all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(),PlayerNode);
        AVLNode<std::shared_ptr<Player>> *prev = all_players_by_goals->find_closest_prev(all_players_by_goals->GetRoot(),PlayerNode);
        PlayerNode->GetValue()->setPrev(prev->GetValue());
        PlayerNode->GetValue()->setNext(next->GetValue());
        if (topScorer == nullptr){
            topScorer = player;
        }
        else
        {
            if (topScorer->getGoals() < player->getGoals())
            {
                topScorer = player;
            }
        }
        numberOfPlayers += 1;
        player->getTeam()->addTo_PM_Equation(goals-cards);
        player->addTo_GamesPlayed((-1)*(player->getTeam()->getGamesPlayed())); // good?
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
    team->addTo_PM_Equation(-foundPlayer->GetValue()->getGoals()+foundPlayer->GetValue()->getCards());
    if (topScorer == foundPlayer->GetValue()){
        topScorer = foundPlayer->GetValue()->getPrev();
        foundPlayer->GetValue()->getPrev()->setNext(nullptr);
    }
    else{
        if (foundPlayer->GetValue()->getPrev() != nullptr){
            foundPlayer->GetValue()->getPrev()->setNext(foundPlayer->GetValue()->getNext());
            foundPlayer->GetValue()->getNext()->setPrev(foundPlayer->GetValue()->getPrev());
        }
        else{
            foundPlayer->GetValue()->getNext()->setPrev(nullptr);
        }
    }
    (team)->removePlayer(foundPlayer->GetValue());
    all_players->Remove(foundPlayer->GetValue());
    all_players_by_goals->Remove(foundPlayer->GetValue());
    foundPlayer->GetValue()->setTeam(nullptr);
    numberOfPlayers -= 1;
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
    std::shared_ptr<Player> next = foundPlayer->GetValue()->getNext();
    std::shared_ptr<Player> prev = foundPlayer->GetValue()->getPrev();
    std::shared_ptr<Team> team = foundPlayer->GetValue()->getTeam();
    std::shared_ptr<Player> newPlayer(new Player(player->getId(),player->getTeam()->getId(),player->getGamesPlayed()+gamesPlayed
                                                 ,player->getGoals()+scoredGoals,player->getCards()+cardsReceived,player->isGK()));
    remove_player(playerId);
    add_player(newPlayer->getId(),newPlayer->getTeam()->getId(),newPlayer->getGamesPlayed(),newPlayer->getGoals(),newPlayer->getCards(),newPlayer->isGK());
    if (next != nullptr){
        next->setPrev((all_players_by_goals->find_closest_prev(all_players_by_goals->GetRoot(), all_players_by_goals->Find(next)))->GetValue());
    }
    if (prev != nullptr){
        prev->setNext((all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(),all_players_by_goals.Find(prev)))->GetValue());
    }
    //player->addTo_GamesPlayed(gamesPlayed);
    //player->setGoals(player->getGoals()+ scoredGoals);
    //player->setCards(player->getCards()+ cardsReceived);
    // player->getTeam()->addTo_PM_Equation(scoredGoals-cardsReceived);
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
    int eq_team1 = team1->GetValue()->getPoints()+team1->GetValue()->get_PM_Equation() ;
    int eq_team2 = team2->GetValue()->getPoints()+team2->GetValue()->get_PM_Equation() ;
    if (eq_team1 > eq_team2)
    {
        team1->GetValue()->addTo_points(3);
    }
    else
    if (eq_team1 == eq_team2)
    {
        team1->GetValue()->addTo_points(1);
        team2->GetValue()->addTo_points(1);
    }
    else{
        team2->GetValue()->addTo_points(3);
    }
    team1->GetValue()->addTo_GamesPlayed(1);
    team2->GetValue()->addTo_GamesPlayed(1);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Player> player(new Player(playerId,0,0,0,0,0));
    if (player == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    auto *foundplayer = this->all_players->Find(player);
    if (foundplayer == nullptr){
        return StatusType::FAILURE;
    }
    else{
        return foundplayer->GetValue()->getGamesPlayed() + foundplayer->GetValue()->getTeam()->getGamesPlayed();
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
    if (teamId1 <= 0 or teamId2 <= 0 or newTeamId <= 0 or teamId2 == teamId1){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> team1(new Team(teamId1,0));
    std::shared_ptr<Team> team2(new Team(teamId2,0));
    if (team1 == nullptr or team2 == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    auto *foundTeam1 = this->all_teams->Find(team1);
    auto *foundTeam2 = this->all_teams->Find(team2);
    if(foundTeam1 == nullptr or foundTeam2== nullptr){
        return StatusType::FAILURE;
    }
    std::shared_ptr<Team> newTeam(new Team(newTeamId,0));
    if (newTeam == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    std::shared_ptr<Player> team1_players_by_id[foundTeam1->GetValue()->getNumOfPlayers()];
    std::shared_ptr<Player> team2_players_by_id[foundTeam2->GetValue()->getNumOfPlayers()];
    foundTeam1->GetValue()->PlayersToArray(0,team1_players_by_id);
    for (int i = 0; i <foundTeam1->GetValue()->getNumOfPlayers();i++ ){
        team1_players_by_id[i]->setTeam(newTeam);
        newTeam->add_player(team1_players_by_id[i]);
    }
    foundTeam2->GetValue()->PlayersToArray(0,team2_players_by_id);
    for (int i = 0; i <foundTeam2->GetValue()->getNumOfPlayers();i++ ){
        team2_players_by_id[i]->setTeam(newTeam);
        newTeam->add_player(team2_players_by_id[i]);
    }
    newTeam->setPoints(foundTeam1->GetValue()->getPoints(),foundTeam2->GetValue()->getPoints());
    // לבדוק מחיקות
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    if (teamId>0){
        std::shared_ptr<Team> team(new Team(teamId,0));
        auto *foundTeam = this->all_teams->Find(team);
        if (foundTeam == nullptr){
            return StatusType::FAILURE;
        }
        if (foundTeam->GetValue()->getNumOfPlayers() == 0){
            return StatusType::FAILURE;
        }
        return foundTeam->GetValue()->getTopScorer();
    }
    else{
        if (topScorer == nullptr){
            return StatusType::FAILURE;
        }
        return topScorer->getId();
    }
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
    std::shared_ptr<Team> team(new Team(teamId,0));
    if (teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    if (teamId > 0){
        auto *foundTeam = this->all_teams->Find(team);
        if (foundTeam == nullptr) return StatusType::FAILURE;
        else{
            return foundTeam->GetValue()->getNumOfPlayers();
        }
    }
    else{
        return this->numberOfPlayers;
    }

}

StatusType world_cup_t::get_all_players(int teamId, int *const output)
{
    if (output == nullptr or teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    if (teamId>0){
        std::shared_ptr<Team> team(new Team(teamId,0));
        if (team == nullptr){
            return StatusType::ALLOCATION_ERROR;
        }
        auto *foundTeam = this->all_teams->Find(team);
        if (foundTeam == nullptr){
            return StatusType::FAILURE;
        }
        else{
            if (foundTeam->GetValue()->isEmpty()){
                return StatusType::FAILURE;
            }
            else{
                int numbOfPlayers = foundTeam->GetValue()->getNumOfPlayers();
                std::shared_ptr<Player> players[numbOfPlayers];
                foundTeam->GetValue()->PlayersToArray(1,players);
                for (int i = 0; i<numbOfPlayers; i++){
                    output[i] = players[numbOfPlayers-i-1]->getId();
                }
            }
        }
    }
    else{
        if (this->all_players_by_goals->IsEmpty()){
            return StatusType::FAILURE;
        }
        else{
            std::shared_ptr<Player> players[numberOfPlayers];
            this->all_players_by_goals->ToArray(players);
            for (int i = 0; i<numberOfPlayers; i++){
                output[i] = players[numberOfPlayers-i-1]->getId();
            }
        }
    }
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if (playerId <= 0 || teamId <= 0){
        return StatusType::INVALID_INPUT;
    }
    std::shared_ptr<Team> team(new Team(teamId,0));
    if (team == nullptr){
        return StatusType::ALLOCATION_ERROR;
    }
    auto *foundTeam = this->all_teams->Find(team);
    if (team == nullptr){
        return StatusType::FAILURE;
    }
    std::shared_ptr<Player> foundPlayer = foundTeam->GetValue()->findPlayer(playerId);
    if (foundPlayer == nullptr || numberOfPlayers == 1){
        return StatusType::FAILURE;
    }
    // if next is null then edit topscorer
    // if prev is null then nothing

}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    // TODO: Your code goes here
    return 2;
}