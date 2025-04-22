// classes.cpp
#include "classes.hpp"
#include "raylib.h"

#include <cstdlib>
#include <algorithm>


// STARTUP
Startup::Startup() : name(""), slogan(""), year(0) {}

Startup::Startup(std::string n, std::string s, uint16_t y)
: name(n), slogan(s), year(y) {}

std::string Startup::getName() const {
    return name;
}

std::string Startup::getSlogan() const {
    return slogan;
}

uint16_t Startup::getYear() const {
    return year;
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

Battle::Battle(StartupEntry& a, StartupEntry& b)
: startup_a(a), startup_b(b), status(BattleStatus::Pending) {}

StartupEntry& Battle::GetStartupA() {
    return startup_a;
}

StartupEntry& Battle::GetStartupB() {
    return startup_b;
}

BattleStatus Battle::GetStatus() const {
    return status;
}

void Battle::SetStatus(BattleStatus s) {
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
    currentBattleIndex = -1;

    srand((unsigned int)GetTime()* 3.14159265);
}

void Tournament::AddStartup(Startup s) {
    startups.emplace_back(StartupEntry{s,(int16_t)70, Status::AVALIABLE, {}});
}

int Tournament::GetTotalStartups() {
    if (startups.empty()) return 0;
    return startups.size();
}

int Tournament::GetTotalStartupsDesqualified() {
    int total = 0;
    for(const auto& startup : startups) {
        if (startup.status == Status::DESQUALIFIED) total++;
    }
    return total;
}

std::vector<StartupEntry>& Tournament::GetStartups() {
    return startups;
}

void Tournament::UpdateStartups(StartupEntry s) {
    for(auto& startup : startups) {
        if(startup.startup.getName() == s.startup.getName())
            startup = s;
    }
}

int16_t& Tournament::GetStartupPointsByName(std::string name) {
    for (auto& [startup, value, status, events] : startups) {
        if (startup.getName() == name) return value;
    }
}

void Tournament::PrintStartups() {
    std::cout << "=========================================" << std::endl;
    for (const auto& [startup, value, status, events] : startups) {
        std::cout << "Nome: " << startup.getName() << std::endl;
        std::cout << "Points: " << value << std::endl;
        std::cout << "Status: " << status << std::endl;
        printf("\n");
    }
    std::cout << "=========================================" << std::endl;
}

bool Tournament::HasStartupsAvaliable() {
    for (const auto& startup : startups) {
        if (startup.status == Status::AVALIABLE) return true;
    }
    return false;
}

void Tournament::ResetStartups() {
    for (auto& startup : startups) {
        startup.status = Status::AVALIABLE;
        startup.totalPoints = 70;
        startup.events.clear();
    }
}

void Tournament::AddStartupPoints(Startup s, int16_t points) {
    for(auto& [startup, value, status, events] : startups) {
        if(s.getName() == startup.getName()) {
            value += points;
        }
    }
}

void Tournament::UpdateStartupStatus(Startup s, Status ss) {
    for(auto& [startup, value, status, events] : startups) {
        if(s.getName() == startup.getName()) {
            status = ss;
        }
    }
}

void Tournament::UpdateStartupBattleEvent(Startup s, BattleEvent be) {
    for(auto& [startup, value, status, events] : startups) {
        if(s.getName() == startup.getName()) {
            events.emplace_back(be);
        }
    }
}

void Tournament::ClearStartupBattleEvents(Startup s) {
    for(auto& [startup, value, status, events] : startups) {
        if(s.getName() == startup.getName()) {
            events.clear();
        }
    }
}

std::vector<BattleEvent>& Tournament::GetBattleEvents() {
    return events;
}

std::vector<Battle>& Tournament::GetBattles() {
    return battles;
}

void Tournament::UpdateBattles(Battle battle) {
    battles[currentBattleIndex] = battle;
}

void Tournament::SetCurrentBattle(Battle b) {
    this->currentBattle = b;
}

void Tournament::SetCurrentBattleIndex(int i) {
    this->currentBattleIndex = i;
}

Battle& Tournament::GetCurrentBattle() {
    return currentBattle;
}

int Tournament::GetCurrentBattleIndex(){
    return currentBattleIndex;
}

int Tournament::MakeBattles() {
    int total = GetTotalStartups();
    int i = 0;
    std::vector<StartupEntry> select_startups;

    battles.clear();

    int qtd = GetTotalStartupsDesqualified();

    // Caso especial de quando começa torneio com 6 startups!
    //      REGRA: "desempate" com as 2 piores startups
    if (qtd == 3) {
        
        // Pega startups qualificadas
        for (const auto& startup : startups) {
            if (startup.status != Status::DESQUALIFIED)
                select_startups.emplace_back(startup);
        }
        // Ordena da pior para a melhor pontuação
        std::sort(select_startups.begin(), select_startups.end(), []
            (const StartupEntry& a, const StartupEntry& b) {
                return a.totalPoints < b.totalPoints;
            });

        // Retira última startup
        select_startups.pop_back();

        Battle b = Battle(select_startups[0], select_startups[1]);
        battles.emplace_back(b);
        return -1;

    // Caso quando o torneio termina!
    } else if (qtd == total-1) {
        return 0;

    // Caso geral. Algoritmo para geração aleatória de batalhas
    } else {

        while (HasStartupsAvaliable()) {
            while (i < 2) {
                int pos = rand() % total;
                StartupEntry& s_aux = startups[pos];

                if (s_aux.status == Status::AVALIABLE) {
                    s_aux.status = Status::QUALIFIED;
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
}

void Tournament::ClearBattles() {
    this->battles.clear();
}

void Tournament::SetChampion(StartupEntry s) {
    this->champion = s;
}

StartupEntry Tournament::GetChampion() {
    return champion;
}
