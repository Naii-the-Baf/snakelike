#include <algorithm>
#include <chrono>
#include <utility>
#include <thread>
#include "common.h"
#include "snake.h"

Snake::Snake(SnakeMap* parent_game_, SnakeAI* ai_object_, u32 id_) :
             parent_game(parent_game_),
             ai_object(ai_object_),
             id(id_){}

void Snake::MoveHandler(u32 row_, u32 column_){
    if(parent_game == nullptr || ai_object == nullptr){
        throw std::runtime_error("Fatal: Player has null parent game or AI");
    }
    row = row_;
    column = column_;
    while(!(parent_game->CanRun())){
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    using Position = std::pair<u32, u32>; //row, column
    Position next_move = std::make_pair(row, column);
    do{
        ApplyStatus(parent_game->GetStatus(id));
        if(hasten > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(Config::draw_delay));
            hasten--;
        } else if(slow > 0){
            std::this_thread::sleep_for(std::chrono::milliseconds(Config::player_delay*2));
            slow--;
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(Config::player_delay));
        }
        own_map = parent_game->GetMap();
        next_move = ai_object->Move(own_map,
                                    GetPossibleMoves(),
                                    static_cast<s32>(row),
                                    static_cast<s32>(column));
        row = next_move.first;
        column = next_move.second;
    }while(parent_game->Update(id, next_move.first, next_move.second));
    return;
}

void Snake::ApplyStatus(const u32 status){
    if((status & static_cast<u32>(Statuses::HASTE)) > 0){
        slow = 0;
        hasten = Config::haste_length;
    }
    if((status & static_cast<u32>(Statuses::SLOW)) > 0){
        hasten = 0;
        slow = Config::haste_length;
    }
    return;
}

const std::vector<std::pair<s32, s32> > OrthoSnake::movement{ //row, column
    {0, -1},    //Left
    {0, 1},     //Right
    {1, 0},     //Up
    {-1, 0}     //Down
};

const std::vector<std::pair<s32, s32> >& OrthoSnake::GetPossibleMoves() const{
    return movement;
}

const std::vector<std::pair<s32, s32> > DiagSnake::movement{ //row, column
    {1, -1},    //Up-left
    {1, 1},     //Up-right
    {-1, 1},    //Down-right
    {-1, -1},   //Down-left
};

const std::vector<std::pair<s32, s32> >& DiagSnake::GetPossibleMoves() const{
    return movement;
}

const std::vector<std::pair<s32, s32> > KnightSnake::movement{ //row, column
    {-2, -1},   //Up-left
    {-2, 1},    //Up-right
    {-1, 2},    //Right-up
    {1, 2},     //Right-down
    {2, 1},     //Down-right
    {2, -1},    //Down-left
    {-1, -2},   //Left-down
    {1, -2},    //Left-up
};

const std::vector<std::pair<s32, s32> >& KnightSnake::GetPossibleMoves() const{
    return movement;
}

const std::vector<std::pair<s32, s32> > TwoSnake::movement{ //row, column
    {0, -2},    //Left
    {0, 2},     //Right
    {2, 0},     //Up
    {-2, 0},    //Down
    {-1, -1},    //Left-up
    {-1, 1},     //Right-up
    {1, 1},     //Right-down
    {1, -1},    //Left-down
};

const std::vector<std::pair<s32, s32> >& TwoSnake::GetPossibleMoves() const{
    return movement;
}

const std::vector<std::pair<s32, s32> > KingSnake::movement{ //row, column
    {0, -1},    //Left
    {0, 1},     //Right
    {1, 0},     //Up
    {-1, 0},    //Down
    {-1, -1},    //Left-up
    {-1, 1},     //Right-up
    {1, 1},     //Right-down
    {1, -1},    //Left-down
};

const std::vector<std::pair<s32, s32> >& KingSnake::GetPossibleMoves() const{
    return movement;
}

std::vector<std::pair<s32, s32> > RandoSnake::InitMovement(){
    std::vector<std::pair<s32, s32> > result(4);
    for(std::size_t i = 0; i < result.size(); i++){
        s32 row = (Common::mt_rand()%7)-3;
        const s32 abs_row = std::max(row, -row);
        s32 column = (Common::mt_rand()%(7-(abs_row*2)))-3+abs_row;
        result[i] = std::make_pair(row, column);
    }
    return result;
}

const std::vector<std::pair<s32, s32> >& RandoSnake::GetPossibleMoves() const{
    return movement;
}

std::vector<std::pair<s32, s32> > TeleSnake::InitMovement(){
    std::vector<std::pair<s32, s32> > result{ //row, column
        {0, -1},    //Left
        {0, 1},     //Right
        {1, 0},     //Up
        {-1, 0}     //Down
    };
    s32 row = (Common::mt_rand()%12)-6;
    s32 column = (Common::mt_rand()%12)-6;
    result.push_back(std::make_pair(row, column));
    return result;
}

const std::vector<std::pair<s32, s32> >& TeleSnake::GetPossibleMoves() const{
    return movement;
}

const std::vector<std::pair<s32, s32> > RainSnake::movement{ //row, column
    {-1, 0},    //Up
    {2, 1},     //Down-right
    {2, -1},     //Down-left
};

const std::vector<std::pair<s32, s32> >& RainSnake::GetPossibleMoves() const{
    return movement;
}
