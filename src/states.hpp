#ifndef STATES_HPP
#define STATES_HPP

#include <unordered_map>
#include <memory>

#include "interface.hpp"

enum STATE {
    ENTRY,
    CREATE_STARTUP,
    TOURNAMENT_08,
    TOURNAMENT_04,
    TOURNAMENT_03,
    TOURNAMENT_02,
    LEAVING
};

typedef struct DellState {
    STATE state;
    std::vector<std::shared_ptr<UIObject>> screenObjs;

    DellState() = default;
    DellState(STATE s, std::vector<std::shared_ptr<UIObject>> so)
        : state(s), screenObjs(std::move(so)) {}

} DellState;

std::unordered_map<STATE, DellState> StatesInit ();

#endif