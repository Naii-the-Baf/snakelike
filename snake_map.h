#pragma once

#include <array>
#include <mutex>
#include <queue>
#include <utility>
#include <vector>
#include "common_types.h"
#include "config.h"

enum class MapConstants : u32{
    ID_LIMIT = 100, //Max 100 players

    EMPTY,
    FOOD,
    HASTE,
    SLOW,
};

enum class Statuses : u32{
    HASTE = 0x1,
    SLOW = 0x2,
};

class SnakeMap{
public:
    SnakeMap();

    void StartGame(); //Creates and calls snakes. Undefined snakes are always OrthoSnakes.
    bool Update(u32 snake_id, s32 row, s32 column); //Return value is true if the calling snake remains alive

    void DrawLoop() const; //Loops drawing. Better than drawing on every update since that'll slow down snakes considerably

    const std::vector<std::vector<u32> >& GetMap() const; //Gives current map to snakes
    u32 GetStatus(const u32 snake_id) const;
    bool CanRun() const;

private:
    mutable std::mutex map_mutex{};

    bool can_run = false; //So all snakes will start running at the same time
    std::vector<std::vector<u32> > game_map{};
    bool PickStartPosition(const u32 snake_id, u32& row_var, u32& col_var);

    void SpawnItem(); //Spawns stuff like food and improvements. Uses the same impl as PickStartPosition.

    u32 players_alive{0};
    std::vector<std::queue<std::pair<s32, s32> > > player_lines{}; //row, column
    std::vector<u32> player_colors{};
    std::vector<u32> growth_counter{};
    std::vector<u32> player_status{};

    void Draw() const;

    static constexpr std::array<u32, 23> colors{ //RRGGBB
        0xFFFFFF, //WHITE
        0xFF0000, //RED
        0x00FF00, //GREEN
        0xFFFF00, //YELLOW
        0x0000FF, //BLUE
        0xFF00FF, //MAGENTA
        0x00FFFF, //CYAN
        0xFF7F00, //ORANGE
        0x8B4513, //BROWN
        0x7F00FF, //PURPLE
        0x7FFFFF, //SKYBLUE
        0x7F7F7F, //GREY
        0x00FF7F, //TURQUOISE
        0x007F00, //DARKGREEN
        0x7F0000, //DARKRED
        0xE1C699, //BEIGE
        0x7F7F00, //MOSSYELLOW
        0x7F007F, //VIOLET
        0x00FFFF, //TEAL
        0xFF7FFF, //PASTELPINK
        0x007FFF, //NICEBLUE
        0x3F7F3F, //GRAYGREEN
        0xDAA520,  //DARKYELLOW
    };
};
