#define PLAYER_COLOR 1

#include "gameplay.h"

//Gameplay actors:
Actor fogOfWar('*');
Actor space(' ');
Actor wall('W', "", "", Stats(), true);

//Field:
struct Field::Graph {
    struct Cost {
        int wayCost = 0;
        int distanceCost = 0;

        Cost(int way = 0, int distance = 0) : wayCost{ way }, distanceCost{ distance }{}

        friend std::ostream& operator<<(std::ostream& out, const Cost& c) {
            out << c.wayCost + c.distanceCost;
            return out;
        }

        Cost& operator+=(const Cost& c) {
            wayCost += c.wayCost;
            distanceCost += c.distanceCost;
            return *this;
        }

        bool operator==(int i) {
            return wayCost + distanceCost == i;
        }

        bool operator>(const Cost& c) const {
            return wayCost + distanceCost > c.wayCost + c.distanceCost;
        }
    };

    Point point;
    Cost cost;
    Graph* cameFrom = nullptr;
    static Point offset[8];

    Graph(int x = 0, int y = 0, int way = 0) : point{ x, y }, cost{ way }{}

    Graph(Point point) : point{ point.x, point.y } {}

    friend std::ostream& operator<<(std::ostream& out, const Graph& g) {
        out << g.cost;
        return out;
    }

    bool operator==(const Graph& g) const {
        return point == g.point;
    }

    bool operator!=(const Graph& g) const {
        return point != g.point;
    }
    //?????????? ????????? <, ??? ?????????? ???????????? ??????? ?? ???????????:
    bool operator<(const Graph& g) const {
        return cost > g.cost;
    }
};

//?????? ?? ??????????, ??? ?????? ?????? ?????:
Point Field::Graph::offset[8]{ { 1, -1 }, { 1, 0 }, { 1, 1 }, { 0, 1 }, { -1, 1 }, { -1, 0 }, { -1, -1 }, { 0, -1 } };

Field::Field(const UserInterface& userInterface, const Player& player) : m_userInterface{ userInterface }, m_player{ player }{
    //???????????? m_col ? m_row ??? ????????? ???????? ????:
    int maxRow, maxCol;
    getmaxyx(m_userInterface.getFieldWindow(), maxRow, maxCol);
    m_col = maxCol / 2 - 1;
    m_row = maxRow - 2;

    generateField(m_col, m_row);
}

Field::~Field() {
    for (int x = 0; x < m_col; ++x)
        delete[] m_field[x];
    delete[] m_field;

    for (int x = 0; x < m_col; ++x)
        delete[] m_mask[x];
    delete[] m_mask;
}

void Field::generateField(int x, int y) {
    
    m_col = x;
    m_row = y;

    //????????? ???? ??????????? ???????:
    m_field = new Actor * *[m_col];
    for (int x = 0; x < m_col; ++x) {
        m_field[x] = new Actor * [m_row];
        for (int y = 0; y < m_row; ++y) {
            m_field[x][y] = &wall;
        }
    }

    //????????? ???????-?????:  
    m_mask = new Graph * [m_col];
    for (int x = 0; x < m_col; ++x) {
        m_mask[x] = new Graph[m_row];
        for (int y = 0; y < m_row; ++y)
            m_mask[x][y] = Graph(x, y);
    }

    //????????? ??????? ? ?????????:
    m_rooms.clear();
    generateRooms(gameplaySettings::countOfRooms);
    //????????? ????????:
    generatePassages();
}

//??????? ????????? ??????? ? ?????????? ?????????:
void Field::generateRooms(int countOfRooms) {
    for (int i = 0; i < countOfRooms; ++i)
        //???? ?? 1000 ???????? ?? ?????? ?????????? ??????:
        for (int j = 0; j < 1000; ++j) {
            int width = gameplaySettings::minRoomWidth + std::rand() % (gameplaySettings::maxRoomWidth - gameplaySettings::minRoomWidth + 1);
            int height = gameplaySettings::minRoomHight + std::rand() % (gameplaySettings::maxRoomHight - gameplaySettings::minRoomHight + 1);
            int x = 1 + std::rand() % (m_col - width - 1);
            int y = 1 + std::rand() % (m_row - height - 1);
            Room newRoom(x, y, width, height);
            auto found = std::find_if(m_rooms.begin(), m_rooms.end(),
                [&](const auto& r) { return !(newRoom.x > r.x + r.width || r.x > newRoom.x + newRoom.width
                    || newRoom.y > r.y + r.hieght || r.y > newRoom.y + newRoom.hieght); });
            if (found == m_rooms.end()) {
                m_rooms.push_back(newRoom);
                break;
            }
        }
    //?????????? m_field ?????????:
    for (const auto& room : m_rooms) {
        for (int x = 0; x < room.width; ++x)
            for (int y = 0; y < room.hieght; ++y) {
                m_field[room.x + x][room.y + y] = &space;
            }
    }
}

//??????? ????????? ???????? ????? ?????????:
void Field::generatePassages() {
    //???? ? ??????? ?????? ???? ???????: return
    if (m_rooms.size() < 2)
        return;

    for (unsigned int i = 1; i < m_rooms.size(); ++i) {
        //????????? ???????-????? ? ???????????? ? ??????? m_field:
        for (int x = 0; x < m_col; ++x)
            for (int y = 0; y < m_row; ++y) {
                //??????????? ????????? ??????? ??????:
                int wayCost;
                m_field[x][y]->isObstacle() ? wayCost = gameplaySettings::wallCost : wayCost = gameplaySettings::spaceCost;
                m_mask[x][y] = Graph(x, y, wayCost);
            }
        //????????? ???????? ????? ????? ????????? ????????? ? ???????:       
        Room currentRoom = m_rooms[i - 1];
        Room nextRoom = m_rooms[i];

        Graph start{ currentRoom.x + currentRoom.width / 2, currentRoom.y + currentRoom.hieght / 2 };
        start.cameFrom = &start;
        m_mask[start.point.x][start.point.y] = start;

        Graph finish = { nextRoom.x + nextRoom.width / 2, nextRoom.y + nextRoom.hieght / 2 };

        std::priority_queue<Graph> wave;
        wave.push(start);
        bool isFound{ false };

        while (!wave.empty()) {
            Graph current{ wave.top() };
            wave.pop();

            for (int i = 0; i < 8; ++i) {
                Graph next{ current.point + Field::Graph::offset[i] };
                if (next.point.x >= m_col || next.point.x < 0 || next.point.y >= m_row || next.point.y < 0)
                    continue;

                next = m_mask[current.point.x + Field::Graph::offset[i].x][current.point.y + Field::Graph::offset[i].y];

                if (next.cameFrom != nullptr)
                    continue;

                next.cameFrom = &m_mask[current.point.x][current.point.y];

                //??????? ????????? ??????:
                next.cost.wayCost += current.cost.wayCost;
                if (i % 2 == 0)
                    //????????? ??????????? ?? ?????????:
                    next.cost.wayCost += gameplaySettings::diagonalCost; 
                else
                    //????????? ??????????? ?? ??????????:
                    next.cost.wayCost += gameplaySettings::orthogonalCost;
                //????????? ? ??????????? ?? ????????? ?? ????:
                next.cost.distanceCost += std::abs(finish.point.x - next.point.x) + std::abs(finish.point.y - next.point.y) * 10;

                m_mask[next.point.x][next.point.y] = next;
                //???? ???? ???????:
                if (next == finish) {
                    isFound = true;
                    //??????????? ???? ?? m_field ?? ????:
                    while (next != start) {
                        for (int i = 0; i < gameplaySettings::passagesWidth; ++i) {
                            m_field[next.point.x + Field::Graph::offset[i].x][next.point.y + Field::Graph::offset[i].y] = &space;
                        }
                        next = *next.cameFrom;
                    }
                    break;
                }
                //???? ???, ?? ?????? ??????????? ? priority_queue(wave):
                else
                    wave.push(next);
            }
            if (isFound)
                break;
        }
    }
}

//??????? ????????? ????????? ????????? ?????? ??????:
Point Field::getRandomPlayerStartCoord() const {
    Field::Room startRoom = m_rooms[rand() % m_rooms.size()];
    return Point(startRoom.x + startRoom.width / 2, startRoom.y + startRoom.hieght / 2);
}

//??????? ????????? ?????????? ????? ? ????????? ???????:
Actor*& Field::getRandomPlaceInRandomRoom() {
    Field::Room randomRoom = m_rooms[rand() % m_rooms.size()];
    return get(randomRoom.x + rand() % randomRoom.width, randomRoom.y + rand() % randomRoom.hieght);
}
//debug:
void Field::printField() const {
    for (int y = 0; y < m_row; ++y) {
        for (int x = 0; x < m_col; ++x) {
            std::cout << *m_field[x][y] << " ";
        }
        std::cout << "\n";
    }
}
//debug:    
void Field::printMask() const {
    for (int y = 0; y < m_row; ++y) {
        for (int x = 0; x < m_col; ++x) {
            std::cout << m_mask[x][y] << " ";
        }
        std::cout << "\n";
    }
}

void Field::redrawField(Point newCoord) {
    //????????? ???????-?????:
    for (int x = 0; x < m_col; ++x)
        for (int y = 0; y < m_row; ++y) {
            m_mask[x][y] = Graph(x, y, 0);
        }

    bool b_isSeeSomething{ false };
    std::vector<Actor*> seenActors;

    //??????????? ?????? ???? ????????? ?? ??? X, ??? ?? ?? ????? ?? ??????? ?????:
    int fieldOfView{ m_player.getFielfOfView() };
    int leftBorder = -fieldOfView + newCoord.x < 0 ? 0 : -fieldOfView + newCoord.x;
    int rightBorder = fieldOfView + newCoord.x > m_col - 1 ? m_col - 1 : fieldOfView + newCoord.x;

    for (int x = leftBorder; x <= rightBorder; ++x) {
        int yBorder = (int)std::sqrt(std::pow(fieldOfView, 2) - std::pow(newCoord.x - x, 2));
        //??????????? ?????? ???? ????????? ?? ??? Y, ??? ?? ?? ????? ?? ??????? ?????:
        int topBorder = newCoord.y - yBorder < 0 ? 0 : newCoord.y - yBorder;
        int downBorder = newCoord.y + yBorder > m_row - 1 ? m_row - 1 : newCoord.y + yBorder;
        //?????????? ? ???????-????? ??????? ?????????:
        for (int y = topBorder; y <= downBorder; ++y) {
            m_mask[x][y].cost.wayCost = 1;
        }
    }

    for (int y = 0; y < m_row; ++y) {
        for (int x = 0; x < m_col; ++x) {
            //???? ?????????:
            if (m_mask[x][y].cost == 1) {
                //????? ???????? ? ???? ?????????:
                switch (m_field[x][y]->getType()) {
                case ActorType::ITEM:
                case ActorType::FRIEND:
                case ActorType::ENEMY:
                    b_isSeeSomething = true;
                    seenActors.push_back(m_field[x][y]);
                    break;
                default:
                    break;
                }
                //??????????? ??????:
                if (x == newCoord.x && y == newCoord.y) {
                    //?????? ??????????? ? 1 ??? ?? ???????? ??????? box():  
                    wattron(m_userInterface.getFieldWindow(), COLOR_PAIR(PLAYER_COLOR));
                    mvwaddch(m_userInterface.getFieldWindow(), 1 + y, 1 + x * 2, m_player.getSign() | A_BOLD);
                    wattroff(m_userInterface.getFieldWindow(), COLOR_PAIR(PLAYER_COLOR));
                }
                //??????????? ?????????:
                else
                    mvwprintw(m_userInterface.getFieldWindow(), 1 + y, 1 + x * 2, "%c%c", m_field[x][y]->getSign(), ' ');
            }
            //??? ???? ????????? (????? ?????):
            else
                mvwprintw(m_userInterface.getFieldWindow(), 1 + y, 1 + x * 2, "%c%c", fogOfWar.getSign(), '.');
        }
        wrefresh(m_userInterface.getFieldWindow());
        updateStatsMenu();
    }
    
    //????? ?? ????? ???????? ???????? ? ???? ?????????:
    if (b_isSeeSomething) {
        std::string infoLine1 = "You are seeing: ";
        for (const Actor* actor : seenActors) {
            infoLine1 += actor->getDescription() + " ";
        }
        m_userInterface.updateInfoMenu(infoLine1, "");
        
    }
    else
        m_userInterface.updateInfoMenu("You are seeing nothing interesting.", "");
}

void Field::clearField(Point newCoord) {
    wclear(m_userInterface.getFieldWindow());
    box(m_userInterface.getFieldWindow(), 0, 0);
    redrawField(newCoord);
}

//????? ?? ????? ???????? ????????????? ?????????:
void Field::updateStatsMenu() {
    m_userInterface.updateStatsMenu(m_player.getStats().currentHealth, m_player.getStats().maxHealth);
}

//Game:
void Game::map() const {
    system("cls");
    m_field.printField();
    std::cout << "\nMap.\n";
}

//debug:
void Game::printMask() const { m_field.printMask(); }

//?????? ???????? ? ?????????:
void Game::pickUp(Actor*& item) {
    m_player.addItemToInventory(item);
    m_field.redrawField(m_playerCoord);
    std::string pickedUpMSG{ "-> Item: " + item->getName() + " add to your inventory." };
    m_userInterface.updateInfoMenu(pickedUpMSG, "");
    item = &space;
}

//battle!
void Game::battle(Actor& attacker, Actor*& deffender) {
    std::string titleBattle{ attacker.getName() + " attacks " + deffender->getName() };
    bool inBattle{ true };
    UIActionType action{ m_userInterface.inMenu(UserInterface::MenuType::BATTLE, titleBattle) };
    do {
        switch (action) {
        case UIActionType::ATTACK:
            deffender->getStats().currentHealth -= attacker.getStats().strength;
            if (deffender->getStats().currentHealth > 0) {
                attacker.getStats().currentHealth -= deffender->getStats().strength;
                m_field.updateStatsMenu();
                titleBattle = "Enemy has left " + std::to_string(deffender->getStats().currentHealth) + " hp.";
                action = m_userInterface.inMenu(UserInterface::MenuType::BATTLE, titleBattle);
                wrefresh(m_userInterface.getInfoWindow());
            }
            else {
                wclear(m_userInterface.getFieldWindow());
                box(m_userInterface.getFieldWindow(), 0, 0);
                m_field.redrawField(m_playerCoord);
                m_userInterface.updateInfoMenu("-> Congratulations! You are win the battle!", "");
                deffender = &space;
                inBattle = false;
            }
            break;
        }
    } while (inBattle);
}

//?????????????? ? ??????????:
void Game::interact() {
    Actor*& l_actor{ m_field.get(m_playerCoord) };
    m_field.redrawField(m_playerCoord);
    switch (l_actor->interact()) {
    case InteractType::BATTLE:
        return battle(m_player, l_actor);
        break;
    case InteractType::PICKUP:
        pickUp(l_actor);
        break;
    default:
        break;
    }
}

//???????? ??????:
void Game::forward() {
    --m_playerCoord.y;
    if (m_playerCoord.y < 0) {
        ++m_playerCoord.y;
        m_field.redrawField(m_playerCoord);
        return;
    }
    if (m_field.get(m_playerCoord)->isObstacle()) {
        ++m_playerCoord.y;
        m_field.redrawField(m_playerCoord);
    }
    else
        interact();
}

//???????? ?????:
void Game::backward() {
    ++m_playerCoord.y;
    if (m_playerCoord.y >= m_field.getRow()) {
        --m_playerCoord.y;
        m_field.redrawField(m_playerCoord);
        return;
    }

    if (m_field.get(m_playerCoord)->isObstacle()) {
        --m_playerCoord.y;
        m_field.redrawField(m_playerCoord);
    }
    else
        interact();
}

//???????? ??????:    
void Game::right() {
    ++m_playerCoord.x;
    if (m_playerCoord.x >= m_field.getCol()) {
        --m_playerCoord.x;
        m_field.redrawField(m_playerCoord);
        return;
    }

    if (m_field.get(m_playerCoord)->isObstacle()) {
        --m_playerCoord.x;
        m_field.redrawField(m_playerCoord);
    }
    else
        interact();
}

//???????? ?????: 
void Game::left() {
    --m_playerCoord.x;
    if (m_playerCoord.x < 0) {
        ++m_playerCoord.x;
        m_field.redrawField(m_playerCoord);
        return;
    }

    if (m_field.get(m_playerCoord)->isObstacle()) {
        ++m_playerCoord.x;
        m_field.redrawField(m_playerCoord);
    }
    else
        interact();
}

//??????? ???????????? ?????? ????:
UIActionType Game::inGame() {
    m_field.clearField(m_playerCoord);
    while (true) {
        switch (getch()) {
        case 119:
            forward();
            break;
        case 115:
            backward();
            break;
        case 97:
            left();
            break;
        case 100:
            right();
            break;
        case 112:
            return UIActionType::PAUSE;
            break;
        }
    }
}

void Game::regenerateField(int x, int y) {
    m_field.generateField(x, y);
    m_field.getRandomPlayerStartCoord();
}