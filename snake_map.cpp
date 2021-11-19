#include <chrono>
#include <functional>
#include <random>
#include <utility>
#include <thread>
#include "common.h"
#include "terminal.h"
#include "snake.h"
#include "snake_ai.h"
#include "snake_map.h"

SnakeMap::SnakeMap(){
    for(u32 i = 0; i < Config::map_height; i++){
        game_map.push_back(std::vector<u32>(Config::map_width,
                                            static_cast<u32>(MapConstants::EMPTY)));
    }
    for(u32 i = 0; i < Config::players; i++){
        player_colors.push_back(0x000000);
        player_lines.push_back({});
        growth_counter.push_back(0);
        player_status.push_back(0x0);
    }
    return;
}

void SnakeMap::StartGame(){
    players_alive = Config::players;
    std::vector<Snake*> snakes{};
    std::vector<SnakeAI*> snakes_ai{};
    std::vector<std::thread> snake_threads{};
    u32 sum = Config::ai_amounts[0];
    u32 type = 0;
    for(u32 i = 0; i < Config::players; i++){
        while(i >= sum && type < SnakeAI::MAX){
            type++;
            if(type < SnakeAI::MAX){
                sum += Config::ai_amounts[type];
            }
        }
        switch(type){
        case SnakeAI::RANDOMAI:
            snakes_ai.push_back(new RandomAI());
            break;
        case SnakeAI::ITEMAI:
            snakes_ai.push_back(new ItemAI());
            break;
        case SnakeAI::FOODAI:
            snakes_ai.push_back(new FoodAI());
            break;
        case SnakeAI::SURVIVEAI:
            snakes_ai.push_back(new SurviveAI());
            break;
        default:
            snakes_ai.push_back(new RandomAI());
            break;
        }
    }
    std::shuffle(snakes_ai.begin(), snakes_ai.end(), Common::mt_rand);
    sum = Config::snake_amounts[0];
    type = 0;
    for(u32 i = 0; i < Config::players; i++){
        while(i >= sum && type < Snake::MAX){
            type++;
            if(type < Snake::MAX){
                sum += Config::snake_amounts[type];
            }
        }
        //One case per type of snake
        switch(type){
        case Snake::ORTHOSNAKE:
            snakes.push_back(new OrthoSnake(this, snakes_ai[i], i));
            break;
        case Snake::DIAGSNAKE:
            snakes.push_back(new DiagSnake(this, snakes_ai[i], i));
            break;
        case Snake::KNIGHTSNAKE:
            snakes.push_back(new KnightSnake(this, snakes_ai[i], i));
            break;
        case Snake::TWOSNAKE:
            snakes.push_back(new TwoSnake(this, snakes_ai[i], i));
            break;
        case Snake::KINGSNAKE:
            snakes.push_back(new KingSnake(this, snakes_ai[i], i));
            break;
        case Snake::RANDOSNAKE:
            snakes.push_back(new RandoSnake(this, snakes_ai[i], i));
            break;
        case Snake::TELESNAKE:
            snakes.push_back(new TeleSnake(this, snakes_ai[i], i));
            break;
        case Snake::RAINSNAKE:
            snakes.push_back(new RainSnake(this, snakes_ai[i], i));
            break;
        default:
            snakes.push_back(new OrthoSnake(this, snakes_ai[i], i));
            break;
        }
    }
    for(u32 i = 0; i < Config::players; i++){
        player_colors[i] = colors[i%colors.size()];
        growth_counter[i] = Config::initial_size-1;
        u32 start_row;
        u32 start_column;
        if(!PickStartPosition(i, start_row, start_column)){
            continue;
        }
        player_lines[i].push(std::make_pair(start_row, start_column));
        snake_threads.push_back(std::move(std::thread(&Snake::MoveHandler, std::ref(snakes[i]), start_row, start_column)));
    }
    u32 initial_items = std::max(1u, (Config::map_width*Config::map_height)/Config::item_density); //Food!
    while(initial_items-- > 0){
        SpawnItem();
    }
    std::thread draw_thread(&SnakeMap::DrawLoop, std::ref(*this));
    can_run = true;
    for(std::thread& snake_thr : snake_threads){
        while(!snake_thr.joinable()){}
        snake_thr.join();
    }
    draw_thread.join();
    return;
}

bool SnakeMap::PickStartPosition(const u32 snake_id, u32& row_var, u32& col_var){
    if(snake_id > static_cast<u32>(MapConstants::ID_LIMIT)){
        return false;
    }
    u32 attempts = 256;
    do{
        row_var = Common::mt_rand()%Config::map_height;
        col_var = Common::mt_rand()%Config::map_width;
    }while(game_map[row_var][col_var] != static_cast<u32>(MapConstants::EMPTY) && --attempts > 0);
    if(attempts == 0){
        return false;
    }
    game_map[row_var][col_var] = snake_id;
    return true;
}

void SnakeMap::SpawnItem(){
    u32 row;
    u32 col;
    u32 attempts = 256;
    do{
        row = Common::mt_rand()%Config::map_height;
        col = Common::mt_rand()%Config::map_width;
    }while(game_map[row][col] != static_cast<u32>(MapConstants::EMPTY) && --attempts > 0);
    if(attempts == 0){
        return;
    }
    MapConstants item = MapConstants::FOOD;
    u32 rand_value = Common::mt_rand()%100;
    if (rand_value < 15) {
        item = MapConstants::HASTE;
    } else if(rand_value < 20){
        item = MapConstants::SLOW;
    } else {
        item = MapConstants::FOOD;
    }
    game_map[row][col] = static_cast<u32>(item);
    return;
}

bool SnakeMap::Update(u32 snake_id, s32 row, s32 column){
    using Position = std::pair<s32, s32>; //row, column
    if(row < 0 ||
       row >= static_cast<s32>(Config::map_height) ||
       column < 0 ||
       column >= static_cast<s32>(Config::map_width)){
        //std::printf("OOB\n");
        while(player_lines[snake_id].size() > 0){
            Position tail = player_lines[snake_id].front();
            player_lines[snake_id].pop();
            game_map[tail.first][tail.second] = static_cast<u32>(MapConstants::EMPTY);
        }
        players_alive--;
        return false;
    }
    std::lock_guard map_lock(map_mutex);
    player_status[snake_id] = 0x0;
    //std::printf("ID%d ROW%d COLUMN%d\n", snake_id, row, column);
    if(game_map[row][column] < static_cast<u32>(MapConstants::ID_LIMIT)){
        while(player_lines[snake_id].size() > 0){
            Position tail = player_lines[snake_id].front();
            player_lines[snake_id].pop();
            game_map[tail.first][tail.second] = static_cast<u32>(MapConstants::EMPTY);
        }
        players_alive--;
        return false;
    }
    if(player_lines[snake_id].size() < 1000 && growth_counter[snake_id] > 0){
        growth_counter[snake_id]--;
    } else {
        Position tail = player_lines[snake_id].front();
        player_lines[snake_id].pop();
        game_map[tail.first][tail.second] = static_cast<u32>(MapConstants::EMPTY);
    }
    switch(static_cast<MapConstants>(game_map[row][column])){
        case MapConstants::FOOD:
            growth_counter[snake_id] += Config::food_growth;
            SpawnItem();
            break;
        case MapConstants::HASTE:
            player_status[snake_id] |= static_cast<u32>(Statuses::HASTE);
            SpawnItem();
            break;
        case MapConstants::SLOW:
            player_status[snake_id] |= static_cast<u32>(Statuses::SLOW);
            SpawnItem();
            break;
        default:
            break;
    }
    game_map[row][column] = snake_id;
    player_lines[snake_id].push(std::make_pair(row, column));
    return true;
}

const std::vector<std::vector<u32> >& SnakeMap::GetMap() const{
    std::lock_guard map_lock(map_mutex);
    return game_map;
}

u32 SnakeMap::GetStatus(const u32 snake_id) const{
    return player_status[snake_id];
}

bool SnakeMap::CanRun() const{
    return can_run;
}

void SnakeMap::DrawLoop() const{
    std::printf("%s", Terminal::Display::ResizeTerminalByChars(Config::map_height+2, Config::map_width*2).c_str());
    std::fflush(stdout);
    while(players_alive > 0){
        Draw();
        std::printf("%s%d snakes alive\n", Terminal::Display::ClearLine.c_str(), players_alive);
        std::this_thread::sleep_for(std::chrono::milliseconds(Config::draw_delay));
    }
    Draw();
    std::printf("%s%d snakes alive\n", Terminal::Display::ClearLine.c_str(), players_alive);
    return;
}

void SnakeMap::Draw() const{
    std::lock_guard map_lock(map_mutex);
    std::printf("%s%s",
                Terminal::Cursor::Move(0, 0).c_str(),
                Terminal::Text::Color(0x00, 0x00, 0x00).c_str());
    for(const std::vector<u32>& row : game_map){
        for(const u32& cell : row){
            switch(static_cast<MapConstants>(cell)){
            case MapConstants::EMPTY:
                std::printf("%s  ", Terminal::Background::Color(0x00, 0x00, 0x00).c_str());
                break;
            case MapConstants::FOOD:
                std::printf("%s  ", Terminal::Background::Color(0xFF, 0x00, 0x00).c_str());
                break;
            case MapConstants::HASTE:
                std::printf("%s  ", Terminal::Background::Color(0xFF, 0xC0, 0x00).c_str());
                break;
            case MapConstants::SLOW:
                std::printf("%s  ", Terminal::Background::Color(0x00, 0x6E, 0x8A).c_str());
                break;
            default:
                std::printf("%s%02u",
                            Terminal::Background::Color(
                                static_cast<u8>((player_colors[cell]>>16)&0xFF),
                                static_cast<u8>((player_colors[cell]>>8)&0xFF),
                                static_cast<u8>(player_colors[cell]&0xFF)).c_str(),
                            cell);
                break;
            }
        }
        std::printf("%s\n", Terminal::Background::Clear.c_str());
    }
    std::printf("%s", Terminal::Format::Clear.c_str());
    std::fflush(stdout);
    return;
}
