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
    AVALIABLE,
    DESQUALIFIED,
    QUALIFIED
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
    Battle(const StartupEntry& a, const StartupEntry& b);

    const StartupEntry& GetStartupA() const;
    const StartupEntry& GetStartupB() const;
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
    std::vector<StartupEntry>& GetStartups();
    int16_t& GetStartupPointsByName(std::string name);
    void PrintStartups();
    bool HasStartupsAvaliable();
    void ResetStartups();
    void AddStartupPoints(Startup s, int16_t points);
    void UpdateStartupBattleEvent(Startup s, BattleEvent be);
    void ClearStartupBattleEvents(Startup s);
    
    std::vector<BattleEvent>& GetBattleEvents();
    std::vector<Battle>& GetBattles();
    void SetCurrentBattle(Battle b);
    Battle& GetCurrentBattle();
    int MakeBattles();
    void ClearBattles();

    static const uint8_t min_startups;
    static const uint8_t max_startups;

private:
    std::vector<BattleEvent> events;
    std::vector<StartupEntry> startups;
    std::vector<Battle> battles;
    Battle currentBattle;
};

#endif