#pragma once

#include <utility>
#include <vector>
#include "common_types.h"
#include "config.h"
#include "snake_ai.h"
#include "snake_map.h"

class Snake{
public:
    Snake() = delete;
    Snake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_);
    virtual ~Snake(){};
    void MoveHandler(u32 row, u32 column); //Calls Update() in map. Should kill thread if Update() returns false

    enum SnakeTypes : u32{
        ORTHOSNAKE, //Moves orthogonally
        DIAGSNAKE, //Moves diagonally
        KNIGHTSNAKE, //Moves like a chess knight
        TWOSNAKE, //Moves two cells orthogonally, or one diagonally (two ortho moves).
        KINGSNAKE, //Another chess portmanteau; can move diagonally and orthogonally.
        RANDOSNAKE, //Rolls 4 random moves within 0 and 3 ortho steps.
        TELESNAKE, //Rolls one large random move; May teleport (use random move) because it feels like it. Otherwise ortho
        RAINSNAKE, //Moves 1 cell up, or down two and right/left one

        MAX,
    };

protected:
    SnakeMap* const parent_game{nullptr};
    SnakeAI* const ai_object{nullptr};
    const u32 id = static_cast<u32>(MapConstants::ID_LIMIT);

    //Since other snakes will be updating the map as well, snakes will only be able to see
    //the current map when calling GetMap().
    std::vector<std::vector<u32> > own_map{};
    u32 row = 0;
    u32 column = 0;
    virtual const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const = 0;

    u32 hasten = 0;
    u32 slow = 0;

    void ApplyStatus(const u32 status);
};

class OrthoSnake : public Snake{
public:
    OrthoSnake() = delete;
    OrthoSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
               Snake(parent_game_, ai_object_, id_){}

private:
    static const std::vector<std::pair<s32, s32> > movement;
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};

class DiagSnake : public Snake{
public:
    DiagSnake() = delete;
    DiagSnake(SnakeMap* parent_game_, SnakeAI* ai_object_, u32 id_) :
              Snake(parent_game_, ai_object_, id_){}

private:
    static const std::vector<std::pair<s32, s32> > movement;
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};

class KnightSnake : public Snake{
public:
    KnightSnake() = delete;
    KnightSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
                Snake(parent_game_, ai_object_, id_){}

private:
    static const std::vector<std::pair<s32, s32> > movement;
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};

class TwoSnake : public Snake{
public:
    TwoSnake() = delete;
    TwoSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
             Snake(parent_game_, ai_object_, id_){}

private:
    static const std::vector<std::pair<s32, s32> > movement;
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};

class KingSnake : public Snake{
public:
    KingSnake() = delete;
    KingSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
              Snake(parent_game_, ai_object_, id_){}

private:
    static const std::vector<std::pair<s32, s32> > movement;
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};

class RandoSnake : public Snake{
public:
    RandoSnake() = delete;
    RandoSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
               Snake(parent_game_, ai_object_, id_),
               movement(InitMovement()){}

private:
    std::vector<std::pair<s32, s32> > InitMovement();

    const std::vector<std::pair<s32, s32> > movement{};
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};

class TeleSnake : public Snake{
public:
    TeleSnake() = delete;
    TeleSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
              Snake(parent_game_, ai_object_, id_),
              movement(InitMovement()){}

private:
    std::vector<std::pair<s32, s32> > InitMovement();

    const std::vector<std::pair<s32, s32> > movement{};
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};


class RainSnake : public Snake{
public:
    RainSnake() = delete;
    RainSnake(SnakeMap* const parent_game_, SnakeAI* const ai_object_, u32 id_) :
              Snake(parent_game_, ai_object_, id_){}

private:
    static const std::vector<std::pair<s32, s32> > movement;
    const std::vector<std::pair<s32, s32> >& GetPossibleMoves() const override;
};
