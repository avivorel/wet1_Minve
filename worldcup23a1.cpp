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
    delete all_teams;
    delete all_players;
    delete all_players_by_goals;
    delete all_viable_teams;
    topScorer = nullptr;

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
        return StatusType::SUCCESS;
    } catch (const std::bad_alloc &) {return  StatusType::ALLOCATION_ERROR;}
}

StatusType world_cup_t::remove_team(int teamId)
{
    if (teamId<=0){
        return StatusType::INVALID_INPUT;
    }

    try {
        std::shared_ptr<Team> toRemove(new Team(teamId, 0));
        auto *all_teams_remove = this->all_teams->Find(toRemove);
        if ((all_teams_remove == nullptr || !(all_teams_remove->Get()->isEmpty()))) {
            return StatusType::FAILURE;
        }
        this->all_teams->Remove(all_teams_remove->Get());
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
        std::shared_ptr<Player> player = std::make_shared<Player>(playerId, teamId, gamesPlayed, goals, cards, goalKeeper);
        if (player == nullptr)
        return StatusType::ALLOCATION_ERROR;
        auto *foundPlayer = this->all_players->Find(player);
        if (foundPlayer != nullptr)
        {
        return StatusType::FAILURE;
        }
        std::shared_ptr<Team> team(new Team(teamId,0));
        auto *foundteam = this->all_teams->Find(team);
        if (foundteam == nullptr)
        {
            return StatusType::FAILURE;
        }
        player->setTeam(foundteam->Get());
        if (foundteam->Get()->add_player(player))
        {
            all_players_by_goals->Insert(player);
            all_players->Insert(player);
            if (foundteam->Get()->getNumOfPlayers() >= 11 && foundteam->Get()->hasGk())
            {
                this->all_viable_teams->Insert(foundteam->Get());
            }
            AVLNode<std::shared_ptr<Player>> *PlayerNode = all_players_by_goals->Find(player);
            AVLNode<std::shared_ptr<Player>> *next = all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(),PlayerNode);
            AVLNode<std::shared_ptr<Player>> *prev = all_players_by_goals->find_closest_prev
                (all_players_by_goals->GetRoot(),PlayerNode);

            if (prev != nullptr){
                PlayerNode->Get()->setPrev(prev->Get());
                prev->Get()->setNext(
                        (all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(), prev))->Get());
            }
            if (next != nullptr) {
                PlayerNode->Get()->setNext(next->Get());
                next->Get()->setPrev(
                        (all_players_by_goals->find_closest_prev(all_players_by_goals->GetRoot(), next))->Get());
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
        std::shared_ptr<Player> toRemove_goals(new Player(foundPlayer_all->Get()->getId(), foundPlayer_all->Get()->getTeamId(),
                                                          foundPlayer_all->Get()->getGamesPlayed(),
                                                          foundPlayer_all->Get()->getGoals(),
                                                          foundPlayer_all->Get()->getCards(),
                                                          foundPlayer_all->Get()->isGK()));
        auto *foundPlayer_goals = this->all_players_by_goals->Find(toRemove_goals);
        if (topScorer == foundPlayer_all->Get()) {
            topScorer = foundPlayer_all->Get()->getPrev();
            if (foundPlayer_all->Get()->getPrev() != nullptr) {
                foundPlayer_all->Get()->getPrev()->setNext(nullptr);
            }
        } else {
            if (foundPlayer_all->Get()->getPrev() != nullptr && foundPlayer_all->Get()->getNext() != nullptr) {
                foundPlayer_all->Get()->getPrev()->setNext(foundPlayer_all->Get()->getNext());
                foundPlayer_all->Get()->getNext()->setPrev(foundPlayer_all->Get()->getPrev());
            }
            else {
                if (foundPlayer_all->Get()->getNext() != nullptr) {
                    foundPlayer_all->Get()->getNext()->setPrev(nullptr);
                }
            }

        }
        std::shared_ptr<Team> team = foundPlayer_all->Get()->getTeam();
        team->addTo_PM_Equation(-foundPlayer_all->Get()->getGoals() + foundPlayer_all->Get()->getCards());
        (team)->removePlayer(foundPlayer_all->Get());
        all_players->Remove(foundPlayer_all->Get());
        all_players_by_goals->Remove(foundPlayer_goals->Get());
        //foundPlayer_all->Get()->setTeam(nullptr);
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
        std::shared_ptr<Player> player = foundPlayer->Get();
        std::shared_ptr<Player> next = foundPlayer->Get()->getNext();
        std::shared_ptr<Player> prev = foundPlayer->Get()->getPrev();
        std::shared_ptr<Team> team = foundPlayer->Get()->getTeam();
        std::shared_ptr<Player> newPlayer(
                new Player(player->getId(), player->getTeam()->getId(), player->getGamesPlayed() + gamesPlayed,
                           player->getGoals() + scoredGoals, player->getCards() + cardsReceived, player->isGK()));
        int team_id = player->getTeam()->getId();
        std::shared_ptr<Team> savedTeam = player->getTeam();
        remove_player(playerId);
        addUpdate(newPlayer->getId(), team_id, newPlayer->getGamesPlayed(),
                   newPlayer->getGoals(),
                   newPlayer->getCards(), newPlayer->isGK(), savedTeam);

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

        if (team1 == nullptr || team2 == nullptr || (!(team1->Get()->hasGk())) ||
                team1->Get()->getNumOfPlayers() < 11
            || (!(team2->Get()->hasGk())) || team2->Get()->getNumOfPlayers() < 11) {
            return StatusType::FAILURE;
        }
        int eq_team1 = team1->Get()->getPoints() + team1->Get()->get_PM_Equation();
        int eq_team2 = team2->Get()->getPoints() + team2->Get()->get_PM_Equation();
        if (eq_team1 > eq_team2) {
            team1->Get()->addTo_points(3);
        } else if (eq_team1 == eq_team2) {
            team1->Get()->addTo_points(1);
            team2->Get()->addTo_points(1);
        } else {
            team2->Get()->addTo_points(3);
        }
        team1->Get()->addTo_GamesPlayed(1);
        team2->Get()->addTo_GamesPlayed(1);
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
            return foundplayer->Get()->getGamesPlayed() + foundplayer->Get()->getTeam()->getGamesPlayed();
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
        return foundteam->Get()->getPoints();
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
        team1 = foundTeam1->Get();
        team2 = foundTeam2->Get();
        std::shared_ptr<Team> newTeam = std::make_shared<Team>(newTeamId,
                                               foundTeam1->Get()->getPoints() + foundTeam2->Get()->getPoints());
        if (this->all_teams->Find(newTeam) != nullptr && (newTeam->getId() != teamId1 && newTeam->getId() != teamId2)){
            return StatusType::FAILURE;
        }
        if (newTeam == nullptr) {
            return StatusType::ALLOCATION_ERROR;
        }
        int team1_players_count = foundTeam1->Get()->getNumOfPlayers();
        int team2_players_count = foundTeam2->Get()->getNumOfPlayers();
        std::shared_ptr<Player> team1_players_by_id[foundTeam1->Get()->getNumOfPlayers()];
        std::shared_ptr<Player> team2_players_by_id[foundTeam2->Get()->getNumOfPlayers()];
        team1->PlayersToArray(0, team1_players_by_id);

        for (int i = 0; i < team1_players_count; i++) {
            team1_players_by_id[i]->setTeam(newTeam);
            newTeam->add_player(team1_players_by_id[i]);
            newTeam->addTo_PM_Equation(team1_players_by_id[i]->getGoals()-team1_players_by_id[i]->getCards());
        }

        team2->PlayersToArray(0, team2_players_by_id);

        for (int i = 0; i < team2_players_count; i++) {
            team2_players_by_id[i]->setTeam(newTeam);
            newTeam->add_player(team2_players_by_id[i]);
            newTeam->addTo_PM_Equation(team2_players_by_id[i]->getGoals()-team2_players_by_id[i]->getCards());
        }

        this->all_teams->Remove(foundTeam1->Get());
        if (foundTeam1_viable != nullptr) {
            this->all_viable_teams->Remove(foundTeam1_viable->Get());
        }
        this->all_teams->Remove(foundTeam2->Get());
        if (foundTeam2_viable != nullptr) {
            this->all_viable_teams->Remove(foundTeam2_viable->Get());
        }
        this->all_teams->Insert(newTeam);
        if (newTeam->getNumOfPlayers() >= 11 && newTeam->hasGk()){
            all_viable_teams->Insert(newTeam);
        }
        return StatusType::SUCCESS;
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
            if (foundTeam->Get()->getNumOfPlayers() == 0)
            {
                return StatusType::FAILURE;
            }
            return foundTeam->Get()->getTopScorer();
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
               return foundTeam->Get()->getNumOfPlayers(); }
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
                if (foundTeam->Get()->isEmpty()) {
                    return StatusType::FAILURE;
                } else {
                    int numbOfPlayers = foundTeam->Get()->getNumOfPlayers();
                    std::shared_ptr<Player> players[numbOfPlayers];
                    foundTeam->Get()->PlayersToArray(1, players);
                    for (int i = 0; i < numbOfPlayers; i++) {
                        //output[i] = players[numbOfPlayers - i - 1]->getId();
                        output[i] = players[i]->getId();
                    }
                }
            }
        } else {
            if (this->all_players_by_goals->CheckEmpty()) {
                return StatusType::FAILURE;
            } else {
                std::shared_ptr<Player> players[numberOfPlayers];
                this->all_players_by_goals->ToArray(players);
                for (int i = 0; i < numberOfPlayers; i++) {
                    output[i] = players[i]->getId();
                }
            }
        }
        return StatusType::SUCCESS;
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
        std::shared_ptr<Player> foundPlayer = foundTeam->Get()->findPlayer(playerId);
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
            else if (foundPlayer->getNext() != nullptr){
                return output_t<int>(foundPlayer->getNext()->getId());
            }
            else{
                return {StatusType::FAILURE};
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
            int eq_team1 = first_team->data->getPoints() + first_team->data->get_PM_Equation() + first_team->score;
            int eq_team2 = second_team->data->getPoints() + second_team->data->get_PM_Equation() + second_team->score;
            if (eq_team1 > eq_team2){
                first_team->addToScore(eq_team2 + second_team->score);
                first_team = second_team->next;
                list->remove(second_team);
                if (first_team != nullptr){
                    second_team = first_team->next;
                }
            }
            else if (eq_team1 < eq_team2){
                second_team->addToScore(eq_team1 + first_team->score);
                list->remove(first_team);
                first_team = second_team->next;
                if (first_team != nullptr){
                    second_team = first_team->next;
                }
            }
            else{
                if(first_team->data->getId() > second_team->data->getId()){
                    first_team->addToScore(eq_team2 + second_team->score);
                    first_team = second_team->next;
                    list->remove(second_team);
                    if (first_team != nullptr){
                        second_team = first_team->next;
                    }
                }
                else{
                    second_team->addToScore(eq_team1 + first_team->score);
                    list->remove(first_team);
                    first_team = second_team->next;
                    if (first_team != nullptr){
                        second_team = first_team->next;
                    }
                }
            }
        }
    }
    if (list->getHead() != nullptr) {
        int winning_team_id = list->getHead()->data->getId();
        list->remove(list->getHead());
        //list->deleteList();
        delete list;
        return {winning_team_id};
    }
    //list->deleteList();
    delete list;
    return {StatusType::FAILURE};
    } catch (const std::bad_alloc &) { return  StatusType::ALLOCATION_ERROR;}
}

StatusType world_cup_t::addUpdate(int playerId, int teamId, int gamesPlayed, int goals, int cards, bool goalKeeper,
                            std::shared_ptr<Team> currTeam){
    if(teamId <= 0 or playerId <= 0 or gamesPlayed<0 or goals<0 or cards< 0){
        return StatusType::INVALID_INPUT;
    }
    if (gamesPlayed == 0 and (goals>0 or cards > 0))
    {
        return StatusType::INVALID_INPUT;
    }
    try
    {
        std::shared_ptr<Player> player = std::make_shared<Player>(playerId, teamId, gamesPlayed, goals, cards, goalKeeper);
        if (player == nullptr)
            return StatusType::ALLOCATION_ERROR;
        auto *foundPlayer = this->all_players->Find(player);
        if (foundPlayer != nullptr)
        {
            return StatusType::FAILURE;
        }
        player->setTeam(currTeam);
        if (currTeam->add_player(player))
        {
            all_players_by_goals->Insert(player);
            all_players->Insert(player);
            if (currTeam->getNumOfPlayers() >= 11 && currTeam->hasGk())
            {
                this->all_viable_teams->Insert(currTeam);
            }
            AVLNode<std::shared_ptr<Player>> *PlayerNode = all_players_by_goals->Find(player);
            AVLNode<std::shared_ptr<Player>> *next = all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(),PlayerNode);
            AVLNode<std::shared_ptr<Player>> *prev = all_players_by_goals->find_closest_prev
                    (all_players_by_goals->GetRoot(),PlayerNode);

            if (prev != nullptr){
                PlayerNode->Get()->setPrev(prev->Get());
                prev->Get()->setNext(
                        (all_players_by_goals->find_closest_next(all_players_by_goals->GetRoot(), prev))->Get());
            }
            if (next != nullptr) {
                PlayerNode->Get()->setNext(next->Get());
                next->Get()->setPrev(
                        (all_players_by_goals->find_closest_prev(all_players_by_goals->GetRoot(), next))->Get());
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
