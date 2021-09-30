#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <curses.h>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>
#include <cstdlib>
#include "actors.h"
#include "UI.h"


class Field {
    struct Room {
        int x;
        int y;
        int width;
        int hieght;

        Room(int x, int y, int w, int h) : x{ x }, y{ y }, width{ w }, hieght{ h } {}
    };

    struct Graph;

    UserInterface m_userInterface;
    const Player& m_player;
    int m_row;
    int m_col;
    std::vector<Room> m_rooms;
    Actor*** m_field;
    Graph** m_mask;

public:
    Field(const UserInterface& userInterface, const Player& player);
    ~Field();

    //getters:
    int getRow() const { return m_row; }
    int getCol() const { return m_col; }
    Actor*& get(int x, int y) { return m_field[x][y]; }
    Actor*& get(Point p) { return m_field[p.x][p.y]; }

    void generateField(int x, int y);

    void generateRooms(int countOfRooms);

    void generatePassages();

    Point getRandomPlayerStartCoord() const;

    Actor*& getRandomPlaceInRandomRoom();

    void printField() const;

    void printMask() const;

    void redrawField(Point newCoord);

    void clearField(Point newCoord);

    void updateStatsMenu();
};

class Game {
    UserInterface m_userInterface;
    Field& m_field;
    Player& m_player;
    Point m_playerCoord;

public:
    Game(const UserInterface& userInterface, Field& field, Player& player, UIActionType difficulty = UIActionType::MEDIUM) : m_userInterface{ userInterface }, m_field { field }, m_player{ player }{
        m_playerCoord = m_field.getRandomPlayerStartCoord();
    }

    const Player& getPlayer() const { return m_player; }

    void map() const;

    void printMask() const;

    void pickUp(Actor*& item);

    void battle(Actor& attacker, Actor*& deffender);

    void interact();

    void forward();

    void backward();

    void right();

    void left();

    UIActionType inGame();

    void regenerateField(int x, int y);
};

#endif