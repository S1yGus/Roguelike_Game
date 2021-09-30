#ifndef STRUCTS_H
#define STRUCTS_H

#include <iostream>
#include <map>

enum class UIActionType {
    GAME,
    OPTIONS,
        EASY,
        MEDIUM,
        HARD,

    PAUSE,
    RESUME,
    QUIT,

        ATTACK,
        RUN
};

enum class ActorType {
    ACTOR,
    ITEM,
    FRIEND,
    ENEMY,
    PLAYER
};

enum class InteractType {
    INFO,
    PICKUP,
    TALK,
    BATTLE
};

struct Point {
    int x;
    int y;

    Point(int x = 0, int y = 0) : x(x), y(y) {}

    Point operator+(const Point& p) const {
        return Point(x + p.x, y + p.y);
    }

    bool operator==(const Point& p) const {
        return x == p.x && y == p.y;
    }

    bool operator!=(const Point& p) const {
        return x != p.x || y != p.y;
    }

    friend std::ostream& operator<<(std::ostream& out, const Point& point) {
        out << "(" << point.x << ", " << point.y << ")";
        return out;
    }
};

class Actor;

struct Stats {
    int maxHealth;
    int currentHealth;
    int strength;
    int fieldOfView;
    std::map<Actor*, int> inventory;

    Stats(int health = 0, int strenght = 0, int fieldOfView = 0)
        : maxHealth{ health }, strength{ strenght }, fieldOfView{ fieldOfView } {
        currentHealth = maxHealth;
    }

    Stats& operator+=(Stats s) {
        currentHealth += s.currentHealth;
        strength += s.strength;
        fieldOfView += s.fieldOfView;
        return *this;
    }
};

#endif