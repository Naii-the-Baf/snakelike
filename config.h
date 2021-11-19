#pragma once

#include <array>
#include <functional>
#include <string>
#include <unordered_map>
#include "common_types.h"

//Require to be extern because they will be changed at runtime by ReadConfig() in main.
namespace Config{
extern u32 map_height;
extern u32 map_width;
extern u32 players;
extern u32 draw_delay;
extern u32 player_delay;
extern u32 initial_size;

extern u32 item_density;
extern u32 food_growth;
extern u32 haste_length;

extern std::array<u32, 8> snake_amounts;

extern std::array<u32, 4> ai_amounts;

//Constant, thus exempt from extern. The references don't break.
const std::unordered_map<std::string, std::reference_wrapper<u32> > config_references{
    {"map_height", map_height},
    {"map_width", map_width},
    {"players", players},
    {"draw_delay", draw_delay},
    {"player_delay", player_delay},
    {"initial_size", initial_size},

    {"item_density", item_density},
    {"food_growth", food_growth},
    {"haste_length", haste_length},

    {"ortho_amount", snake_amounts[0]},
    {"diag_amount", snake_amounts[1]},
    {"knight_amount", snake_amounts[2]},
    {"two_amount", snake_amounts[3]},
    {"king_amount", snake_amounts[4]},
    {"rando_amount", snake_amounts[5]},
    {"tele_amount", snake_amounts[6]},
    {"rain_amount", snake_amounts[7]},

    {"ai_random_amount", ai_amounts[0]},
    {"ai_item_amount", ai_amounts[1]},
    {"ai_food_amount", ai_amounts[2]},
    {"ai_survive_amount", ai_amounts[3]},
};
}//namespace Config
