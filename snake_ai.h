#pragma once

#include <utility>
#include <vector>
#include "common_types.h"
#include "snake_map.h"

class SnakeAI{
public:
    SnakeAI(){};
    virtual ~SnakeAI(){};

    enum AITypes : u32 {
        RANDOMAI, //Picks a random possible move and performs it.
        ITEMAI, //Goes towards the nearest item
        FOODAI, //A variant of ItemAI that only cares about food
        SURVIVEAI, //Avoids enclosing itself and food.

        MAX,
    };

    virtual std::pair<s32, s32> Move(const std::vector<std::vector<u32> >& own_map,
                                     const std::vector<std::pair<s32, s32> >& moves,
                                     const s32 row,
                                     const s32 column) const = 0;

protected:
    bool IsMovePossible(const std::vector<std::vector<u32> >& own_map,
                        const s32 row,
                        const s32 column) const;
};

class RandomAI : public SnakeAI{
public:
    RandomAI(){};

    std::pair<s32, s32> Move(const std::vector<std::vector<u32> >& own_map,
                             const std::vector<std::pair<s32, s32> >& moves,
                             const s32 row,
                             const s32 column) const override;
};

class ItemAI : public SnakeAI{
public:
    ItemAI(){};

    std::pair<s32, s32> Move(const std::vector<std::vector<u32> >& own_map,
                             const std::vector<std::pair<s32, s32> >& moves,
                             const s32 row,
                             const s32 column) const override;
};

class FoodAI : public SnakeAI{
public:
    FoodAI(){};

    std::pair<s32, s32> Move(const std::vector<std::vector<u32> >& own_map,
                             const std::vector<std::pair<s32, s32> >& moves,
                             const s32 row,
                             const s32 column) const override;
};

class SurviveAI : public SnakeAI{
public:
    SurviveAI(){};

    std::pair<s32, s32> Move(const std::vector<std::vector<u32> >& own_map,
                             const std::vector<std::pair<s32, s32> >& moves,
                             const s32 row,
                             const s32 column) const override;
};
