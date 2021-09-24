#ifndef ACTORS_H
#define ACTORS_H

#include <string>
#include "structs.h"

class Actor_base {
protected:
    char m_sign;
    std::string m_name;
    std::string m_description;
    Stats m_stats;
    bool b_isObstacle;

public:
    Actor_base(char sign, std::string name, std::string description, Stats stats, bool isObstacle)
        : m_sign{ sign }, m_name{ name }, m_description{ description }, m_stats{ stats }, b_isObstacle{ isObstacle }{}

    //getters:
    const char getSign() const { return m_sign; }
    const std::string getName() const { return m_name; }
    const std::string getDescription() const { return m_description; }
    const Stats& getStats() const { return m_stats; }
    Stats& getStats() { return m_stats; }
    const int getFielfOfView() const { return m_stats.fieldOfView; }
    bool isObstacle() const { return b_isObstacle; }
    virtual const ActorType getType() const = 0;

    //setters:
    void setSign(char sign) { m_sign = sign; }
    void toggleObstacle() { b_isObstacle = !b_isObstacle; }

    virtual const InteractType interact() const = 0;

    friend std::ostream& operator<<(std::ostream& out, const Actor_base& actor);
};

class Actor : public Actor_base {
public:
    Actor(char sign = ' ', std::string name = "", std::string description = "", Stats stats = Stats(), bool isObstacle = false)
        : Actor_base(sign, name, description, stats, isObstacle) {}

    virtual const ActorType getType() const override { return ActorType::ACTOR; }

    virtual const InteractType interact() const override { return InteractType::INFO; }

    friend bool operator<( const Actor& a, const Actor& b) {
        return a.m_sign < b.m_sign;
    }
};

class Item : public Actor {
public:
    Item(char sign = 'I', std::string name = "Item", std::string description = "This is an item.", Stats stats = Stats(), bool isObstacle = false)
        : Actor(sign, name, description, stats, isObstacle) {}

    virtual const ActorType getType() const override { return ActorType::ITEM; }

    virtual const InteractType interact() const override { return InteractType::PICKUP; }
};

class Character : public Actor {
public:
    Character(char sign, std::string name, std::string description, Stats stats, bool isObstacle)
        : Actor(sign, name, description, stats, isObstacle) {}
};

class Friend : public Character {
public:
    Friend(char sign = 'F', std::string name = "Friend", std::string description = "This is a friend.", Stats stats = Stats(1, 1, 1), bool isObstacle = false)
        : Character(sign, name, description, stats, isObstacle) {}

    virtual const ActorType getType() const override { return ActorType::FRIEND; }

    virtual const InteractType interact() const override {
        std::cout << "Hello! My name is " << m_name << "." << "\n";
        return InteractType::TALK;
    }
};

class Enemy : public Character {
public:
    Enemy(char sign = 'F', std::string name = "Friend", std::string description = "This is a friend.", Stats stats = Stats(1, 1, 1), bool isObstacle = false)
        : Character(sign, name, description, stats, isObstacle) {}

    virtual const ActorType getType() const override { return ActorType::ENEMY; }

    virtual const InteractType interact() const override { return InteractType::BATTLE; }
};

class Player : public Character {
public:
    Player(char sign = 'H', std::string name = "Hero", std::string description = "This is a hero.", Stats stats = Stats(1, 1, 2), bool isObstacle = false)
        : Character(sign, name, description, stats, isObstacle) {}

    virtual const ActorType getType() const override { return ActorType::PLAYER; }

    std::map<Actor*, int> getInventory();

    void updateStats(const Actor* item);

    void addItemToInventory(Actor*& item);

    void showInventory() const;
};

#endif