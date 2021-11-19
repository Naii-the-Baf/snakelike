#include <queue>
#include <vector>
#include "common.h"
#include "snake_ai.h"

bool SnakeAI::IsMovePossible(const std::vector<std::vector<u32> >& own_map,
                             const s32 row,
                             const s32 column) const{
    //std::printf("Ismove\n");
    /*std::printf("%d %d %d %d\n",
                row >= 0, row < static_cast<s32>(Config::map_height),
                column >= 0, column < static_cast<s32>(Config::map_width));*/

    return row >= 0 && row < static_cast<s32>(Config::map_height) &&
           column >= 0 && column < static_cast<s32>(Config::map_width) &&
           own_map[row][column] > static_cast<u32>(MapConstants::ID_LIMIT);
}

std::pair<s32, s32> RandomAI::Move(const std::vector<std::vector<u32> >& own_map,
                                   const std::vector<std::pair<s32, s32> >& moves,
                                   const s32 row,
                                   const s32 column) const{
    u32 attempts = 20;
    while(attempts-- > 0){
        std::size_t selected = Common::mt_rand()%moves.size();
        if(IsMovePossible(own_map, row+moves[selected].first, column+moves[selected].second)){
            return std::make_pair(row+moves[selected].first,
                                  column+moves[selected].second);
        }
    }
    return std::make_pair(static_cast<u32>(row), static_cast<u32>(column));
}

std::pair<s32, s32> ItemAI::Move(const std::vector<std::vector<u32> >& own_map,
                                 const std::vector<std::pair<s32, s32> >& moves,
                                 const s32 row,
                                 const s32 column) const{
    //Performs a BFS until it finds an item. If none found, defaults to first move in list that is valid.
    std::vector<std::vector<u32> > map_move(own_map.size(), std::vector<u32>(own_map[0].size(), moves.size()));
    using Move = std::pair<s32, s32>; //row, column
    std::queue<Move> bfs_queue{};
    bfs_queue.push(std::make_pair(row, column));
    bool is_first = true;
    while(bfs_queue.size() > 0){
        Move curr = bfs_queue.front();
        bfs_queue.pop();
        if(own_map[curr.first][curr.second] > static_cast<u32>(MapConstants::EMPTY)){
            return std::make_pair(row+moves[map_move[curr.first][curr.second]].first,
                                  column+moves[map_move[curr.first][curr.second]].second);
        }
        for(std::size_t i = 0; i < moves.size(); i++){
            Move next = std::make_pair(curr.first+moves[i].first, curr.second+moves[i].second);
            if(!IsMovePossible(own_map, next.first, next.second)){
                continue;
            }
            if(map_move[next.first][next.second] == moves.size()){
                bfs_queue.push(next);
                map_move[next.first][next.second] = is_first ? i : map_move[curr.first][curr.second];
            }
        }
        is_first = false;
    }
    for(const Move& movement : moves){
        Move next = std::make_pair(row+movement.first, column+movement.second);
        if(IsMovePossible(own_map, next.first, next.second)){
            return std::make_pair(row+movement.first, column+movement.second);
        }
    }
    return std::make_pair(row, column);
}

std::pair<s32, s32> FoodAI::Move(const std::vector<std::vector<u32> >& own_map,
                                 const std::vector<std::pair<s32, s32> >& moves,
                                 const s32 row,
                                 const s32 column) const{
    //Performs a BFS until it finds food. If none found, defaults to first move in list that is valid.
    std::vector<std::vector<u32> > map_move(own_map.size(), std::vector<u32>(own_map[0].size(), moves.size()));
    using Move = std::pair<s32, s32>; //row, column
    std::queue<Move> bfs_queue{};
    bfs_queue.push(std::make_pair(row, column));
    bool is_first = true;
    while(bfs_queue.size() > 0){
        Move curr = bfs_queue.front();
        bfs_queue.pop();
        if(own_map[curr.first][curr.second] == static_cast<u32>(MapConstants::FOOD)){
            return std::make_pair(row+moves[map_move[curr.first][curr.second]].first,
                                  column+moves[map_move[curr.first][curr.second]].second);
        }
        for(std::size_t i = 0; i < moves.size(); i++){
            Move next = std::make_pair(curr.first+moves[i].first, curr.second+moves[i].second);
            if(!IsMovePossible(own_map, next.first, next.second)){
                continue;
            }
            if(map_move[next.first][next.second] == moves.size()){
                bfs_queue.push(next);
                map_move[next.first][next.second] = is_first ? i : map_move[curr.first][curr.second];
            }
        }
        is_first = false;
    }
    for(const Move& movement : moves){
        Move next = std::make_pair(row+movement.first, column+movement.second);
        if(IsMovePossible(own_map, next.first, next.second)){
            return std::make_pair(row+movement.first, column+movement.second);
        }
    }
    return std::make_pair(row, column);
}

std::pair<s32, s32> SurviveAI::Move(const std::vector<std::vector<u32> >& own_map,
                                    const std::vector<std::pair<s32, s32> >& moves,
                                    const s32 row,
                                    const s32 column) const{
    //std::printf("Begin\n");
    //Performs a BFS on the entire map. Will count how many tiles have a particular movement, and move to the one with most.
    std::vector<std::vector<u32> > map_move(own_map.size(), std::vector<u32>(own_map[0].size(), moves.size()));
    std::vector<u32> move_count(moves.size(), 0);
    using Move = std::pair<s32, s32>; //row, column
    std::queue<Move> bfs_queue{};
    bfs_queue.push(std::make_pair(row, column));
    bool is_first = true;
    //std::printf("Init complete\n");
    while(bfs_queue.size() > 0){
        //std::printf("Loop\n");
        Move curr = bfs_queue.front();
        bfs_queue.pop();
        for(std::size_t i = 0; i < moves.size(); i++){
            Move next = std::make_pair(curr.first+moves[i].first, curr.second+moves[i].second);
            //std::printf("Select\n");
            if(!IsMovePossible(own_map, next.first, next.second)){
                //std::printf("FAIL\n");
                continue;
            }
            //std::printf("Passed\n");
            if(map_move[next.first][next.second] == moves.size() &&
               own_map[next.first][next.second] != static_cast<u32>(MapConstants::FOOD)){
                bfs_queue.push(next);
                if(is_first){
                    map_move[next.first][next.second] = i;
                    move_count[i]++;
                } else {
                    map_move[next.first][next.second] = map_move[curr.first][curr.second];
                    move_count[map_move[curr.first][curr.second]]++;
                }
                //std::printf("Queued\n");
            }
        }
        is_first = false;
    }
    std::size_t selected = 0;
    for(std::size_t i = 1; i < move_count.size(); i++){
        if(move_count[selected] < move_count[i]){
            selected = i;
        }
    }
    //std::printf("Selected movement %u: %d %d\n", selected, moves[selected].first, moves[selected].second);
    return std::make_pair(row+moves[selected].first, column+moves[selected].second);
}
