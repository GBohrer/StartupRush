#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <stdlib.h>
#include <string>
#include <cstdint>
#include <vector>
#include <tuple>
#include <iostream>

class Startup {
    public:
        Startup() : name(""), slogan(""), year(0), competing(false) {}

        Startup(std::string n, std::string s, uint16_t y)
        : name(n), slogan(s), year(y), competing(false) {}

        // Getters
        std::string getName() const {
            return name;
        }

        std::string getSlogan() const {
            return slogan;
        }

        uint16_t getYear() const {
            return year;
        }

        bool GetCompeting() const {
            return competing;
        }

        void SetCompeting(bool v) {
            this->competing = v;
        }

    private:
        std::string name;
        std::string slogan;
        uint16_t year;
        bool competing;

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
        BattleEvent(EventID id, const std::string& desc, int8_t val)
        : id(id), description(desc), value(val) {}

        // Getters
        EventID getID() const {
         return id;
        }

        std::string getDescription() const {
         return description;
        }

        int8_t getValue() const {
         return value;
        }

        // Setters
        void setID(EventID newID) {
            this->id = newID;
        }

        void setDescription(const std::string& newDesc) {
            this->description = newDesc;
        }

        void setValue(int8_t newValue) {
            this->value = newValue;
        }

    private:
        EventID id;
        std::string description;
        int8_t value;

};

enum Status {
    Pending,
    Complete
};

class Battle {
    public:
        Battle() {}
        Battle(const Startup& a, const Startup& b)
        : startup_a(a, {}), startup_b(b, {}), status(Status::Pending) {}
    
        // Getters
        const std::tuple<Startup, std::vector<BattleEvent>>& GetStartupA() const {
            return startup_a;
        }

        const std::tuple<Startup, std::vector<BattleEvent>>& GetStartupB() const {
            return startup_b;
        }

        Status GetStatus() const {
            return status;
        }

        void SetStatus(Status s) {
            status = s;
        }

        void AddEventToA(const BattleEvent& event) {
            std::get<1>(startup_a).push_back(event);
        }

        void AddEventToB(const BattleEvent& event) {
            std::get<1>(startup_b).push_back(event);
        }

    private:
        std::tuple<Startup, std::vector<BattleEvent>> startup_a;
        std::tuple<Startup, std::vector<BattleEvent>> startup_b;
        Status status;

};

class Tournament {
    public:
        Tournament() {}
        void Init() {
            startups.clear();
            startups.reserve(8);

            events.clear();
            events.emplace_back(BattleEvent(EventID::GoodPitch, "Pitch convincente", 6));
            events.emplace_back(BattleEvent(EventID::WithBugs, "Produto com bugs", -4));
            events.emplace_back(BattleEvent(EventID::UserDriven, "Boa tração de usuários", 3));
            events.emplace_back(BattleEvent(EventID::AngryInvestor, "Investidor irritado", -6));
            events.emplace_back(BattleEvent(EventID::FakeNews, "Fake news no pitch", -8));

            battles.clear();
            battles.reserve(4);

            srand((unsigned int)GetTime());
        }

        static const uint8_t min_startups = 4;
        static const uint8_t max_startups = 8;

        void AddStartup(Startup s) {
            startups.emplace_back(std::make_tuple(s, (int16_t)70));
        }

        int GetTotalStartups() {
            if (startups.empty()) return 0;
            return startups.size();
        }

        std::vector<std::tuple<Startup, int16_t>>& GetStartups() {
            return startups;
        }

        int16_t GetStartupPointsByName(std::string name) {
            for (const auto& [startup, value] : startups) {
                if (startup.getName() == name) {
                    return value;
                }
            }
            return 0;
        }

        void PrintStartups() {
            for (const auto& [startup, value] : startups) {
                std::cout << "Nome: " << startup.getName() << std::endl;
                std::cout << "Slogan: " << startup.getSlogan() << std::endl;
                std::cout << "Year: " << startup.getYear() << std::endl;
                std::cout << "Points: " << value << std::endl;
                printf("\n");
            }
        }

        bool HasStartupsAvaliable() {
            for (const auto& [startup, value] : startups) {
                if (!(startup.GetCompeting())) return true;
            }
            return false;
        }

        void ResetStartups() {
            for (auto& [startup, value] : startups) {
                startup.SetCompeting(false);
                value = 70;
            }
        }

        void UpdateStartupPoints(Startup s, int16_t points) {
            for(auto& [startup, value] : startups) {
                if(s.getName() == startup.getName()) {
                    value = points;
                }
            }
        }

        std::vector<BattleEvent>& GetBattleEvents() {
            return events;
        }

        std::vector<Battle>& GetBattles() {
            return battles;
        }

        void SetCurrentBattle(Battle b) {
            this->currentBattle = b;
        }

        Battle& GetCurrentBattle() {
            return currentBattle;
        }

        int MakeBattles() {
            int total = GetTotalStartups();
            int i = 0;
            std::vector<Startup> select_startups;

            // Enquanto houver startups competing==false
            while (HasStartupsAvaliable()) {

                // Escolher 2 startups
                while (i < 2) {
                    //Pegar random pos do vector startups
                    int pos = rand() % total;
                    Startup& s_aux = std::get<0>(startups[pos]);

                    // Set a startup pega como competing
                    if (!(s_aux.GetCompeting())) {
                        s_aux.SetCompeting(true);
                        select_startups.emplace_back(s_aux);
                        i++;
                    }
                }
                select_startups.clear();
                i = 0;
                
                // Cria battle com as startups selecionadas
                Battle b = Battle(select_startups[0], select_startups[1]);
                battles.emplace_back(b);
            }

            return battles.size();
        }

        void ClearBattles() {
            this->battles.clear();
        }

    private:
        std::vector<BattleEvent> events;
        std::vector<std::tuple<Startup, int16_t>> startups;
        std::vector<Battle> battles;
        Battle currentBattle;
};


#endif