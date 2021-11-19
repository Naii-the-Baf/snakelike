#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif // _WIN32

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <thread>
#include "common_types.h"
#include "config.h"
#include "files.h"
#include "main.h"
#include "snake_map.h"

void ReadConfig(){
    std::FILE* cfg_file{nullptr};
    try{
        ReadFile(cfg_file, "snakelike.cfg");
    }catch(std::runtime_error& exc){
        std::printf("Error reading %s: Does the file exist?\n",
                    std::filesystem::absolute("snakelike.cfg").c_str());
        std::printf("Snakelike will run with default configuration.\n");
        std::this_thread::sleep_for(std::chrono::seconds(10));
        return;
    }
    char buf[128]{};
    std::string buf_str{};
    while(std::fscanf(cfg_file, " %[^\n]", buf) != EOF){
        buf_str = buf;
        std::size_t eq_pos = buf_str.find_first_of("=");
        if(eq_pos == std::string::npos){
            continue;
        }
        std::string var_name = buf_str.substr(0, eq_pos);
        u32 value = 0;
        try{
            value = std::stoi(buf_str.substr(eq_pos+1));
        }catch(std::exception& e){
            continue;
        }
        if(Config::config_references.find(var_name) !=
           Config::config_references.end()){
            Config::config_references.at(var_name).get() = value;
        }
    }
    CloseReadFile(cfg_file);
    ValidateConfig();
    return;
}

void ValidateConfig(){
    Config::map_height = std::clamp(Config::map_height, 10u, 100u);
    Config::map_width = std::clamp(Config::map_width, 10u, 100u);
    Config::players = std::clamp(Config::players, 1u, 100u); //Drawing only supports two digits
    Config::draw_delay = std::clamp(Config::draw_delay, 30u, 2000u);
    Config::player_delay = std::clamp(Config::player_delay, Config::draw_delay, 10000u); //Players shouldn't be faster than drawing
    Config::initial_size = std::clamp(Config::initial_size, 2u, 1000u); //Size 1 allows for remaining in one place indefinitely

    Config::item_density = std::max(Config::item_density, 20u);
    Config::food_growth = std::min(Config::food_growth, 100u);
    Config::haste_length = std::min(Config::haste_length, 100u);

    for(u32& snake_am : Config::snake_amounts){
        snake_am = std::min(snake_am, Config::players);
    }
    for(u32& ai_am : Config::ai_amounts){
        ai_am = std::min(ai_am, Config::players);
    }
    return;
}

int main(){
    #ifdef _WIN32
    Terminal::EnableEscapeSequences();
    #endif //_WIN32
    ReadConfig();
    SnakeMap snake_map{};
    snake_map.StartGame();
    return 0;
}
