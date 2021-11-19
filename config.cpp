#include "config.h"

//Values in here should all have default values.
namespace Config{
u32 map_height = 20;
u32 map_width = 20;
u32 players = 2;
u32 draw_delay = 50;
u32 player_delay = 100;
u32 initial_size = 4;

u32 item_density = 50;
u32 food_growth = 3;
u32 haste_length = 7;

std::array<u32, 8> snake_amounts{};

std::array<u32, 4> ai_amounts{};
}//namespace Config
