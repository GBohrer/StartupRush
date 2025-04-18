#ifndef CLASSES_HPP
#define CLASSES_HPP

#include <string>
#include <cstdint>
#include <vector>
#include <tuple>
#include <iostream>

class Startup {
    public:
        Startup() : name(""), slogan(""), year(0), is_competing(true) {}

        Startup(std::string n, std::string s, uint16_t y)
        : name(n), slogan(s), year(y), is_competing(true) {}

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

        bool IsCompeting() const {
            return is_competing;
        }

    private:
        std::string name;
        std::string slogan;
        uint16_t year;
        bool is_competing;
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

class Battle {
    public:

    private:
        std::tuple<Startup, std::vector<BattleEvent>> startup_a;
        std::tuple<Startup, std::vector<BattleEvent>> startup_b;
};

class Tournament {
    public:
        Tournament() {}

        static const uint8_t min_startups = 4;
        static const uint8_t max_startups = 8;

        //bool AddBattleEvent() {}

        void AddStartup(Startup s) {
            startups.emplace_back(std::make_tuple(s, 70));
        }

        void PrintStartups() {
            for (const auto& [startup, value] : startups) {
                std::cout << "Nome: " << startup.getName() << std::endl;
                std::cout << "Slogan: " << startup.getSlogan() << std::endl;
                std::cout << "Year: " << startup.getYear() << std::endl;
                printf("\n");
            }
        }

        int GetTotalStartups() {
            return startups.size();
        }

    private:
        std::vector<BattleEvent> events;
        std::vector<std::tuple<Startup, uint16_t>> startups;
        std::vector<Battle> Battles;
};


#endif