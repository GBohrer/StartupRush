#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <tuple>
#include <iostream>

class Startup {
public:
    Startup();
    Startup(std::string n, std::string s, uint16_t y);

    std::string getName() const;
    std::string getSlogan() const;
    uint16_t getYear() const;

private:
    std::string name;
    std::string slogan;
    uint16_t year;
};

enum EventID {
    GoodPitch,
    WithBugs,
    UserDriven,
    AngryInvestor,
    FakeNews
};

class BattleEvent {
public:
    BattleEvent() {};
    BattleEvent(EventID id, int8_t val);

    EventID getID() const;
    int8_t getValue() const;
    void setID(EventID newID);
    void setValue(int8_t newValue);

private:
    EventID id;
    int8_t value;
};

enum BattleStatus {
    Pending,
    Complete
};

enum Status {
    AVALIABLE = 0,
    DESQUALIFIED = -1,
    QUALIFIED = 1
};

typedef struct StartupEntry {
    Startup startup;
    int16_t totalPoints;
    Status status;
    std::vector<BattleEvent> events;

} StartupEntry;

class Battle {
public:
    Battle();
    Battle(StartupEntry& a,StartupEntry& b);

    StartupEntry& GetStartupA();
    StartupEntry& GetStartupB();
    BattleStatus GetStatus() const;
    void SetStatus(BattleStatus s);

private:
    StartupEntry startup_a;
    StartupEntry startup_b;
    BattleStatus status;
};

class Tournament {
public:
    Tournament();
    void Init();

    void AddStartup(Startup s);
    int GetTotalStartups();
    int GetTotalStartupsDesqualified();
    std::vector<StartupEntry>& GetStartups();
    void UpdateStartups(StartupEntry s);
    int16_t& GetStartupPointsByName(std::string name);

    void PrintStartups();
    void PrintAllEventsFromStartup();

    bool HasStartupsAvaliable();
    void ResetStartups();
    void AddStartupPoints(Startup s, int16_t points);
    void UpdateStartupStatus(Startup s, Status ss);

    void UpdateStartupBattleEvent(Startup s, BattleEvent be);
    void ClearStartupBattleEvents(Startup s);
    
    std::vector<BattleEvent>& GetBattleEvents();
    int GetEventCount(const StartupEntry& s, EventID id);

    std::vector<Battle>& GetBattles();
    void UpdateBattles(Battle battle);

    void SetCurrentBattle(Battle b);
    void SetCurrentBattleIndex(int i);
    Battle& GetCurrentBattle();
    int GetCurrentBattleIndex();

    int MakeBattles();
    void ClearBattles();

    void SetChampion(StartupEntry s);
    StartupEntry GetChampion();

    static const uint8_t min_startups;
    static const uint8_t max_startups;

private:
    std::vector<BattleEvent> events;
    std::vector<StartupEntry> startups;
    StartupEntry champion;
    std::vector<Battle> battles;
    Battle currentBattle;
    int currentBattleIndex;
};

#endif