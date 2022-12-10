#include "worldcup23a1.h"
#include <memory>


world_cup_t::world_cup_t()
{

    all_players = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerId);
    all_teams = new AVLTree<std::shared_ptr<Team>>(Team::compareTeamId);
    all_players_by_goals = new AVLTree<std::shared_ptr<Player>>(Player::comparePlayerGoalsCardsId);
    all_viable_teams =  new AVLTree<std::shared_ptr<Team>>(Team::compareTeamId);
    topScorer = nullptr;
    numberOfPlayers = 0;

}

world_cup_t::~world_cup_t()
{
    // TODO: Your code goes here
}


StatusType world_cup_t::add_team(int teamId, int points)
{
    if(teamId <= 0 or points< 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try
    {
        std::shared_ptr<Team> newTeam(new Team(teamId,points));
        if (newTeam == nullptr)
        {
            return StatusType::ALLOCATION_ERROR;
        }
        if(!this->all_teams->Insert(newTeam))
        {
            return StatusType::FAILURE;
        }
    } catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}

    return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }

    try {
        std::shared_ptr<Team> toRemove(new Team(teamId, 0));
        auto *all_teams_remove = this->all_teams->Find(toRemove);
        if ((all_teams_remove == nullptr || !(all_teams_remove->GetValue()->isEmpty()))) {
            return StatusType::FAILURE;
        }
        this->all_teams->Remove(all_teams_remove->GetValue());
        return StatusType::SUCCESS;
    }  catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}

}

StatusType world_cup_t::add_player(int playerId, int teamId, int gamesPlayed,int goals, int cards, bool goalKeeper)
{
    if(teamId <= 0 or playerId <= 0 or gamesPlayed<0 or goals<0 or cards< 0){
        return StatusType::INVALID_INPUT;
    }
    if (gamesPlayed == 0 and (goals>0 or cards > 0))
    {
        return StatusType::INVALID_INPUT;
    }
    try
    {
        std::shared_ptr<Player> player(new Player(playerId, teamId, gamesPlayed, goals, cards, goalKeeper));

        if (player == nullptr)
        return StatusType::ALLOCATION_ERROR;

        auto *foundPlayer = this->all_players->Find(player);

        if (foundPlayer != nullptr)
        {
        return StatusType::FAILURE;
        }

        std::shared_ptr<Team> team(new Team(teamId,0));


        auto* foundteam = this->all_teams->Find(team);

        if (foundteam == nullptr)
        {
            return StatusType::FAILURE;
        }

        player->setTeam(foundteam->GetValue());

        if (foundteam->GetValue()->add_player(player))
        {
            all_players->Insert(player);
            all_players_by_goals->Insert(player);
            if (foundteam->GetValue()->getNumOfPlayers() >= 11 && foundteam->GetValue()->hasGk())
            {
                this->all_viable_teams->Insert(foundteam->GetValue());
            }
            AVLNode<std::shared_ptr<Player>> *PlayerNode = all_players_by_goals->Find(player);
            AVLNode<std::shared_ptr<Player>> *next = all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(),PlayerNode);
            AVLNode<std::shared_ptr<Player>> *prev = all_players_by_goals->find_closest_prev
                (all_players_by_goals->GetRoot(),PlayerNode);
            if (prev != nullptr){
                PlayerNode->GetValue()->setPrev(prev->GetValue());
            }
            if (next != nullptr) {
                PlayerNode->GetValue()->setNext(next->GetValue());
            }
            if (topScorer == nullptr)
            {
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
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
    return StatusType::FAILURE;
}

StatusType world_cup_t::remove_player(int playerId)
{
    if (playerId<= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Player> toRemove(new Player(playerId, 0, 0, 0, 0, 0));
        if (toRemove == nullptr) /// delete?
            return StatusType::ALLOCATION_ERROR;
        auto *foundPlayer_all = this->all_players->Find(toRemove);
        if (foundPlayer_all == nullptr) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Player> toRemove_goals(new Player(foundPlayer_all->GetValue()->getId(), foundPlayer_all->GetValue()->getTeamId(),
                                                          foundPlayer_all->GetValue()->getGamesPlayed(), foundPlayer_all->GetValue()->getGoals(),
                                                          foundPlayer_all->GetValue()->getCards(), foundPlayer_all->GetValue()->isGK()));
        auto *foundPlayer_goals = this->all_players_by_goals->Find(toRemove_goals);
        if (topScorer == foundPlayer_all->GetValue()) {
            topScorer = foundPlayer_all->GetValue()->getPrev();
            if (foundPlayer_all->GetValue()->getPrev() != nullptr) {
                foundPlayer_all->GetValue()->getPrev()->setNext(nullptr);
            }
        } else {
            if (foundPlayer_all->GetValue()->getPrev() != nullptr && foundPlayer_all->GetValue()->getNext()!= nullptr) {
                foundPlayer_all->GetValue()->getPrev()->setNext(foundPlayer_all->GetValue()->getNext());
                foundPlayer_all->GetValue()->getNext()->setPrev(foundPlayer_all->GetValue()->getPrev());
            }
            else {
                if (foundPlayer_all->GetValue()->getNext() != nullptr) {
                    foundPlayer_all->GetValue()->getNext()->setPrev(nullptr);
                }
            }

        }
        std::shared_ptr<Team> team = foundPlayer_all->GetValue()->getTeam();
        team->addTo_PM_Equation(-foundPlayer_all->GetValue()->getGoals() + foundPlayer_all->GetValue()->getCards());
        (team)->removePlayer(foundPlayer_all->GetValue());
        all_players->Remove(foundPlayer_all->GetValue());
        all_players_by_goals->Remove(foundPlayer_goals->GetValue());
        //foundPlayer_all->GetValue()->setTeam(nullptr);
        if (team->getNumOfPlayers() < 11 or !team->hasGk()) {
            all_viable_teams->Remove(team);
        }

        numberOfPlayers -= 1;
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
        return StatusType::SUCCESS;
}

StatusType world_cup_t::update_player_stats(int playerId, int gamesPlayed,int scoredGoals, int cardsReceived)
{

    if (playerId<= 0 || gamesPlayed< 0 || scoredGoals< 0 || cardsReceived< 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Player> toUpdate(new Player(playerId, 0, 0, 0, 0, 0));

        if (toUpdate == nullptr) /// delete?
            return StatusType::ALLOCATION_ERROR;

        auto *foundPlayer = this->all_players->Find(toUpdate);

        if (foundPlayer == nullptr) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Player> player = foundPlayer->GetValue();
        std::shared_ptr<Player> next = foundPlayer->GetValue()->getNext();
        std::shared_ptr<Player> prev = foundPlayer->GetValue()->getPrev();
        std::shared_ptr<Team> team = foundPlayer->GetValue()->getTeam();
        std::shared_ptr<Player> newPlayer(
                new Player(player->getId(), player->getTeam()->getId(), player->getGamesPlayed() + gamesPlayed,
                           player->getGoals() + scoredGoals, player->getCards() + cardsReceived, player->isGK()));
        int team_id = player->getTeam()->getId();
        remove_player(playerId);

        add_player(newPlayer->getId(), team_id, newPlayer->getGamesPlayed(),
                   newPlayer->getGoals(),
                   newPlayer->getCards(), newPlayer->isGK());
        if (next != nullptr) {
            AVLNode<std::shared_ptr<Player>> *new_prev = all_players_by_goals->find_closest_prev(all_players_by_goals->GetRoot(),
            all_players_by_goals->Find(next));
            if (new_prev != nullptr) {
                next->setPrev(new_prev->GetValue());
            }
        }
        if (prev != nullptr) {
            AVLNode<std::shared_ptr<Player>> *new_next = all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(),
            all_players_by_goals->Find(prev));
            if(new_next != nullptr){
                prev->setNext(new_next->GetValue());
            }
        }

    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
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
    try {
        std::shared_ptr<Team> checkteam1(new Team(teamId1, 0));
        std::shared_ptr<Team> checkteam2(new Team(teamId2, 0));

        if (checkteam1 == nullptr || checkteam2 == nullptr) /// delete?
            return StatusType::ALLOCATION_ERROR;

        auto *team1 = this->all_teams->Find(checkteam1);
        auto *team2 = this->all_teams->Find(checkteam2);

        if (team1 == nullptr || team2 == nullptr || (!(team1->GetValue()->hasGk())) ||
            team1->GetValue()->getNumOfPlayers() < 11
            || (!(team2->GetValue()->hasGk())) || team2->GetValue()->getNumOfPlayers() < 11) {
            return StatusType::FAILURE;
        }
        int eq_team1 = team1->GetValue()->getPoints() + team1->GetValue()->get_PM_Equation();
        int eq_team2 = team2->GetValue()->getPoints() + team2->GetValue()->get_PM_Equation();
        if (eq_team1 > eq_team2) {
            team1->GetValue()->addTo_points(3);
        } else if (eq_team1 == eq_team2) {
            team1->GetValue()->addTo_points(1);
            team2->GetValue()->addTo_points(1);
        } else {
            team2->GetValue()->addTo_points(3);
        }
        team1->GetValue()->addTo_GamesPlayed(1);
        team2->GetValue()->addTo_GamesPlayed(1);
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_num_played_games(int playerId)
{
    if(playerId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Player> player(new Player(playerId, 0, 0, 0, 0, 0));
        if (player == nullptr)
        {
            return StatusType::ALLOCATION_ERROR;
        }
        auto *foundplayer = this->all_players->Find(player);

        if (foundplayer == nullptr)
        {
            return StatusType::FAILURE;
        }
        else {
            return foundplayer->GetValue()->getGamesPlayed() + foundplayer->GetValue()->getTeam()->getGamesPlayed();
        }
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Team> team(new Team(teamId, 0));
        if (team == nullptr) {
            return StatusType::ALLOCATION_ERROR;
        }
        auto *foundteam = this->all_teams->Find(team);
        if (foundteam == nullptr) {
            return StatusType::FAILURE;
        }
        return foundteam->GetValue()->getPoints();
    }catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

StatusType world_cup_t::unite_teams(int teamId1, int teamId2, int newTeamId)
{
    try {
    if (teamId1 <= 0 or teamId2 <= 0 or newTeamId <= 0 or teamId2 == teamId1){
        return StatusType::INVALID_INPUT;
        }
        std::shared_ptr<Team> team1(new Team(teamId1, 0));
        std::shared_ptr<Team> team2(new Team(teamId2, 0));
        if (team1 == nullptr or team2 == nullptr) {
            return StatusType::ALLOCATION_ERROR;
        }
        auto *foundTeam1 = this->all_teams->Find(team1);
        auto *foundTeam1_viable = this->all_viable_teams->Find(team1);
        auto *foundTeam2 = this->all_teams->Find(team2);
        auto *foundTeam2_viable = this->all_viable_teams->Find(team2);
        if (foundTeam1 == nullptr || foundTeam2 == nullptr) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Team> newTeam(new Team(newTeamId, foundTeam1->GetValue()->getPoints()+foundTeam2->GetValue()->getPoints()));
        if (newTeam == nullptr) {
            return StatusType::ALLOCATION_ERROR;
        }
        std::shared_ptr<Player> team1_players_by_id[foundTeam1->GetValue()->getNumOfPlayers()];
        std::shared_ptr<Player> team2_players_by_id[foundTeam2->GetValue()->getNumOfPlayers()];
        foundTeam1->GetValue()->PlayersToArray(0, team1_players_by_id);
        for (int i = 0; i < foundTeam1->GetValue()->getNumOfPlayers(); i++) {
            team1_players_by_id[i]->setTeam(newTeam);
            newTeam->add_player(team1_players_by_id[i]);
            newTeam->addTo_PM_Equation(team1_players_by_id[i]->getGoals()-team1_players_by_id[i]->getCards());
        }
        foundTeam2->GetValue()->PlayersToArray(0, team2_players_by_id);
        for (int i = 0; i < foundTeam2->GetValue()->getNumOfPlayers(); i++) {
            team2_players_by_id[i]->setTeam(newTeam);
            newTeam->add_player(team2_players_by_id[i]);
            newTeam->addTo_PM_Equation(team2_players_by_id[i]->getGoals()-team2_players_by_id[i]->getCards());
        }
        this->all_teams->Remove(foundTeam1->GetValue());
        this->all_viable_teams->Remove(foundTeam1_viable->GetValue());
        this->all_teams->Remove(foundTeam2->GetValue());
        this->all_viable_teams->Remove(foundTeam2_viable->GetValue());
        this->all_teams->Insert(newTeam);
        if (newTeam->getNumOfPlayers() >= 11 && newTeam->hasGk()){
            all_viable_teams->Insert(newTeam);
        }
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_top_scorer(int teamId)
{
    if (teamId == 0){
        return StatusType::INVALID_INPUT;
    }
    try {
        if (teamId > 0)
        {
            std::shared_ptr<Team> team(new Team(teamId, 0));
            auto *foundTeam = this->all_teams->Find(team);
            if (foundTeam == nullptr)
            {
                return StatusType::FAILURE;
            }
            if (foundTeam->GetValue()->getNumOfPlayers() == 0)
            {
                return StatusType::FAILURE;
            }
            return foundTeam->GetValue()->getTopScorer();
        }
        else {
            if (topScorer == nullptr)
            {
                return StatusType::FAILURE;
            }
            return topScorer->getId();
        }
    }catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_all_players_count(int teamId)
{
   try {
       std::shared_ptr<Team> team(new Team(teamId, 0));
       if (teamId == 0)
       {
           return StatusType::INVALID_INPUT;
       }
       if (teamId > 0)
       {
           auto *foundTeam = this->all_teams->Find(team);
           if (foundTeam == nullptr) return StatusType::FAILURE;
           else {
               return foundTeam->GetValue()->getNumOfPlayers(); }
       } else {
           return this->numberOfPlayers;
       }
   } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

StatusType world_cup_t::get_all_players(int teamId, int *const output) {
    if (output == nullptr or teamId == 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        if (teamId > 0) {
            std::shared_ptr<Team> team(new Team(teamId, 0));
            if (team == nullptr) {
                return StatusType::ALLOCATION_ERROR;
            }
            auto *foundTeam = this->all_teams->Find(team);
            if (foundTeam == nullptr) {
                return StatusType::FAILURE;
            } else {
                if (foundTeam->GetValue()->isEmpty()) {
                    return StatusType::FAILURE;
                } else {
                    int numbOfPlayers = foundTeam->GetValue()->getNumOfPlayers();
                    std::shared_ptr<Player> players[numbOfPlayers];
                    foundTeam->GetValue()->PlayersToArray(1, players);
                    for (int i = 0; i < numbOfPlayers; i++) {
                        //output[i] = players[numbOfPlayers - i - 1]->getId();
                        output[i] = players[i]->getId();
                    }
                }
            }
        } else {
            if (this->all_players_by_goals->IsEmpty()) {
                return StatusType::FAILURE;
            } else {
                std::shared_ptr<Player> players[numberOfPlayers];
                this->all_players_by_goals->ToArray(players);
                for (int i = 0; i < numberOfPlayers; i++) {
                    output[i] = players[numberOfPlayers - i - 1]->getId();
                }
            }
        }
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::get_closest_player(int playerId, int teamId)
{
    if (playerId <= 0 || teamId <= 0){
        return {StatusType::INVALID_INPUT};
    }
    try {
        std::shared_ptr<Team> team(new Team(teamId, 0));
        if (team == nullptr) {
            return {StatusType::ALLOCATION_ERROR};
        }
        auto *foundTeam = this->all_teams->Find(team);
        if (foundTeam == nullptr) {
            return {StatusType::FAILURE};
        }
        std::shared_ptr<Player> foundPlayer = foundTeam->GetValue()->findPlayer(playerId);
        if (foundPlayer == nullptr || numberOfPlayers == 1) {
            return {StatusType::FAILURE};
        }
        if (foundPlayer->getPrev() != nullptr && foundPlayer->getNext() != nullptr) {
            if (foundPlayer->getGoals() - foundPlayer->getPrev()->getGoals() ==
                foundPlayer->getNext()->getGoals() - foundPlayer->getGoals())
            {
                if (abs(foundPlayer->getPrev()->getCards() - foundPlayer->getCards()) >
                    abs(foundPlayer->getNext()->getCards() - foundPlayer->getCards()))
                {
                    return {foundPlayer->getNext()->getId()};
                }
                else if (abs(foundPlayer->getPrev()->getCards() - foundPlayer->getCards()) <
                           abs(foundPlayer->getNext()->getCards() - foundPlayer->getCards()))
                {
                    return {foundPlayer->getPrev()->getId()};
                } else {
                    if (abs(foundPlayer->getPrev()->getId() - foundPlayer->getId()) >
                        abs(foundPlayer->getNext()->getId() - foundPlayer->getId()))
                    {
                        return {foundPlayer->getNext()->getId()};
                    } else if (abs(foundPlayer->getPrev()->getId() - foundPlayer->getId()) <
                               abs(foundPlayer->getNext()->getId() - foundPlayer->getId()))
                    {
                        return {foundPlayer->getPrev()->getId()};
                    } else {
                        return {std::max(foundPlayer->getPrev()->getId(), foundPlayer->getNext()->getId())};
                    }
                }
            } else {
                return (foundPlayer->getGoals() - foundPlayer->getPrev()->getGoals() <
                        foundPlayer->getNext()->getGoals() - foundPlayer->getGoals()) ?
                       output_t<int>(foundPlayer->getPrev()->getId()) : output_t<int>(foundPlayer->getNext()->getId());
            }
        }
        else{
            if (foundPlayer->getPrev() != nullptr){
                return output_t<int>(foundPlayer->getPrev()->getId());
            }
            else{
                return output_t<int>(foundPlayer->getNext()->getId());
            }
        }
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

output_t<int> world_cup_t::knockout_winner(int minTeamId, int maxTeamId)
{
    try{
    std::shared_ptr<Team> team_min(new Team(minTeamId,0));
    std::shared_ptr<Team> team_max(new Team(maxTeamId,0));
    if (minTeamId< 0 || maxTeamId< 0 || maxTeamId< minTeamId){
        return {StatusType::INVALID_INPUT};
    }
    auto *list = new LinkedList<std::shared_ptr<Team>>();
    this->all_viable_teams->inorder_search_ll(this->all_viable_teams->GetRoot(),list,team_min,team_max);
    while (list->getHead() != nullptr && list->getHead()->next != nullptr){
        Node<std::shared_ptr<Team>> *first_team = list->getHead();
        Node<std::shared_ptr<Team>> *second_team = list->getHead()->next;
        while (first_team != nullptr && second_team!= nullptr){
            int eq_team1 = first_team->data->getPoints() + first_team->data->get_PM_Equation();
            int eq_team2 = second_team->data->getPoints() + second_team->data->get_PM_Equation();
            if (eq_team1 > eq_team2){
                first_team->data->addTo_points(3);
                unite_teams(first_team->data->getId(),second_team->data->getId(),first_team->data->getId());
                std::shared_ptr<Team> newTeam(new Team(first_team->data->getId(),first_team->data->getPoints()+second_team->data->getPoints()));
                first_team->setData((this->all_teams->Find(newTeam)->GetValue()));
                list->remove(second_team);
                first_team = second_team->next;
                if (first_team != nullptr){
                    second_team = first_team->next;
                }
            }
            else if (eq_team1 < eq_team2){
                second_team->data->addTo_points(3);
                unite_teams(first_team->data->getId(),second_team->data->getId(),second_team->data->getId());
                std::shared_ptr<Team> newTeam(new Team(second_team->data->getId(),first_team->data->getPoints()+second_team->data->getPoints()));
                second_team->setData((this->all_teams->Find(newTeam)->GetValue()));
                list->remove(first_team);
                first_team = second_team->next;
                if (first_team != nullptr){
                    second_team = first_team->next;
                }
            }
            else{
                if(first_team->data->getId() > second_team->data->getId()){
                    first_team->data->addTo_points(3);
                    unite_teams(first_team->data->getId(),second_team->data->getId(),first_team->data->getId());
                    std::shared_ptr<Team> newTeam(new Team(first_team->data->getId(),first_team->data->getPoints()+second_team->data->getPoints()));
                    first_team->setData((this->all_teams->Find(newTeam)->GetValue()));
                    list->remove(second_team);
                    first_team = second_team->next;
                    if (first_team != nullptr){
                        second_team = first_team->next;
                    }
                }
                else{
                    second_team->data->addTo_points(3);
                    unite_teams(first_team->data->getId(),second_team->data->getId(),second_team->data->getId());
                    std::shared_ptr<Team> newTeam(new Team(second_team->data->getId(),first_team->data->getPoints()+second_team->data->getPoints()));
                    first_team->setData((this->all_teams->Find(newTeam)->GetValue()));
                    list->remove(first_team);
                    first_team = second_team->next;
                    if (first_team != nullptr){
                        second_team = first_team->next;
                    }
                }
            }
        }
    }
    int winning_team_id = list->getHead()->data->getId();
    list->deleteList();
    return {winning_team_id};
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}