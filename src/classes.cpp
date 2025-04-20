// classes.cpp
#include "classes.hpp"
#include "raylib.h"

#include <cstdlib>


// STARTUP
Startup::Startup() : name(""), slogan(""), year(0), competing(false) {}

Startup::Startup(std::string n, std::string s, uint16_t y)
: name(n), slogan(s), year(y), competing(false) {}

std::string Startup::getName() const {
    return name;
}

std::string Startup::getSlogan() const {
    return slogan;
}

uint16_t Startup::getYear() const {
    return year;
}

bool Startup::GetCompeting() const {
    return competing;
}

void Startup::SetCompeting(bool v) {
    this->competing = v;
}

// BATTLE EVENT
BattleEvent::BattleEvent(EventID id, int8_t val)
: id(id), value(val) {}

EventID BattleEvent::getID() const {
    return id;
}

int8_t BattleEvent::getValue() const {
    return value;
}

void BattleEvent::setID(EventID newID) {
    this->id = newID;
}

void BattleEvent::setValue(int8_t newValue) {
    this->value = newValue;
}

// BATTLE
Battle::Battle() {}

Battle::Battle(const Startup& a, const Startup& b)
: startup_a(a), startup_b(b), status(Status::Pending) {}

const Startup& Battle::GetStartupA() const {
    return startup_a;
}

const Startup& Battle::GetStartupB() const {
    return startup_b;
}

Status Battle::GetStatus() const {
    return status;
}

void Battle::SetStatus(Status s) {
    status = s;
}

// TOURNAMENT
const uint8_t Tournament::min_startups = 4;
const uint8_t Tournament::max_startups = 8;

Tournament::Tournament() {}

void Tournament::Init() {
    startups.clear();
    startups.reserve(8);

    events.clear();
    events.emplace_back(BattleEvent(EventID::GoodPitch, 6));
    events.emplace_back(BattleEvent(EventID::WithBugs, -4));
    events.emplace_back(BattleEvent(EventID::UserDriven, 3));
    events.emplace_back(BattleEvent(EventID::AngryInvestor, -6));
    events.emplace_back(BattleEvent(EventID::FakeNews, -8));

    battles.clear();
    battles.reserve(4);

    srand((unsigned int)GetTime());
}

void Tournament::AddStartup(Startup s) {
    startups.emplace_back(StartupEntry{s,(int16_t)70, {}});
}

int Tournament::GetTotalStartups() {
    if (startups.empty()) return 0;
    return startups.size();
}

std::vector<StartupEntry>& Tournament::GetStartups() {
    return startups;
}

int16_t Tournament::GetStartupPointsByName(std::string name) {
    for (const auto& [startup, value, events] : startups) {
        if (startup.getName() == name) {
            return value;
        }
    }
    return 0;
}

void Tournament::PrintStartups() {
    for (const auto& [startup, value, events] : startups) {
        std::cout << "Nome: " << startup.getName() << std::endl;
        std::cout << "Slogan: " << startup.getSlogan() << std::endl;
        std::cout << "Year: " << startup.getYear() << std::endl;
        std::cout << "Points: " << value << std::endl;
        printf("\n");
    }
}

bool Tournament::HasStartupsAvaliable() {
    for (const auto& [startup, value, events] : startups) {
        if (!(startup.GetCompeting())) return true;
    }
    return false;
}

void Tournament::ResetStartups() {
    for (auto& [startup, value, events] : startups) {
        startup.SetCompeting(false);
        value = 70;
    }
}

void Tournament::UpdateStartupPoints(Startup s, int16_t points) {
    for(auto& [startup, value, events] : startups) {
        if(s.getName() == startup.getName()) {
            value = points;
        }
    }
}

void Tournament::UpdateStartupBattleEvent(Startup s, BattleEvent be) {
    for(auto& [startup, value, battleEvents] : startups) {
        if(s.getName() == startup.getName()) {
            battleEvents.emplace_back(be);
        }
    }
}

std::vector<BattleEvent>& Tournament::GetBattleEvents() {
    return events;
}

std::vector<Battle>& Tournament::GetBattles() {
    return battles;
}

void Tournament::SetCurrentBattle(Battle b) {
    this->currentBattle = b;
}

Battle& Tournament::GetCurrentBattle() {
    return currentBattle;
}

int Tournament::MakeBattles() {
    int total = GetTotalStartups();
    int i = 0;
    std::vector<Startup> select_startups;

    while (HasStartupsAvaliable()) {
        while (i < 2) {
            int pos = rand() % total;
            Startup& s_aux = startups[pos].startup;

            if (!(s_aux.GetCompeting())) {
                s_aux.SetCompeting(true);
                select_startups.emplace_back(s_aux);
                i++;
            }
        }
        Battle b = Battle(select_startups[0], select_startups[1]);
        battles.emplace_back(b);

        select_startups.clear();
        i = 0;
    }

    return battles.size();
}

void Tournament::ClearBattles() {
    this->battles.clear();
}
