#include "UI.h"

//Windows:
UserInterface::Windows::Windows() {
    //создание и расчет положени€ mainWindow:
    WINDOW* mainWindow = newwin(LINES - UISettings::mainWindowYOffset * 2, COLS - UISettings::mainWindowXOffset * 2, UISettings::mainWindowYOffset, UISettings::mainWindowXOffset);
    box(mainWindow, 0, 0);
    wrefresh(mainWindow);
    m_windows["main"] = mainWindow;

    //создание и расчет положени€ menuWindow:
    int menuWindowYOffset = (LINES - UISettings::menuWindowHeight) / 2 + UISettings::menuWindowYOffset;
    int menuWindowXOffset = (COLS - UISettings::menuWindowWides) / 2 + UISettings::menuWindowXOffset;
    WINDOW* menuWindow = newwin(UISettings::menuWindowHeight, UISettings::menuWindowWides, menuWindowYOffset, menuWindowXOffset);
    m_windows["menu"] = menuWindow;

    //создание и расчет положени€ fieldWindow:
    int fieldWindowHeight, fieldWindowWides;
    getmaxyx(m_windows["main"], fieldWindowHeight, fieldWindowWides);
    WINDOW* fieldWindow = newwin(fieldWindowHeight - (UISettings::infoWindowHeight + 2), fieldWindowWides - UISettings::fieldWindowXOffset * 2, UISettings::mainWindowYOffset + UISettings::fieldWindowYOffset, UISettings::mainWindowXOffset + UISettings::fieldWindowXOffset);
    m_windows["field"] = fieldWindow;

    //создание и расчет положени€ infoWindow:
    WINDOW* infoWindow = newwin(UISettings::infoWindowHeight, getmaxx(m_windows["field"]) / 2, getmaxy(m_windows["field"]) + 2, UISettings::mainWindowXOffset + UISettings::fieldWindowXOffset);
    m_windows["info"] = infoWindow;

    //создание и расчет положени€ statsWindow:
    WINDOW* statsWindow = newwin(UISettings::infoWindowHeight, getmaxx(m_windows["field"]) / 2, getmaxy(m_windows["field"]) + 2, getmaxx(m_windows["field"]) / 2 + 4);
    m_windows["stats"] = statsWindow;

    //создание и расчет положени€ actionWindow:
    int actionWindowYOffset = (LINES - UISettings::actionWindowHeight) / 2 + UISettings::actionWindowYOffset;
    int actionWindowXOffset = (COLS - UISettings::actionWindowWides) / 2 + UISettings::actionWindowXOffset;
    WINDOW* actionWindow = newwin(UISettings::actionWindowHeight, UISettings::actionWindowWides, actionWindowYOffset, actionWindowXOffset);
    m_windows["action"] = actionWindow;

    //создание и расчет положени€ msgWindow:
    int msgWindowYOffset = (LINES - UISettings::msgWindowHeight) / 2 + UISettings::msgWindowYOffset;
    int msgWindowXOffset = (COLS - UISettings::msgWindowWides) / 2 + UISettings::msgWindowXOffset;
    WINDOW* msgWindow = newwin(UISettings::msgWindowHeight, UISettings::msgWindowWides, msgWindowYOffset, msgWindowXOffset);
    m_windows["msg"] = msgWindow;
}

WINDOW* UserInterface::Windows::operator[](std::string key) {
    return m_windows[key];
}

//UserInterface:
UserInterface::UserInterface() {
    //рассчет положени€ элементов main menu:
    m_numOfMainItems = sizeof(m_mainMenuItems) / sizeof(char*);
    m_mainYOffset = (UISettings::menuWindowHeight - ((int)(sizeof(m_mainMenuItems) / sizeof(char*)) + UISettings::spaceBeetwenTitleAndItems)) / 2 + UISettings::menuTitleYOffset;
    m_mainXOffset = (UISettings::menuWindowWides - strlen(m_mainMenuTitle)) / 2 + UISettings::menuTitleXOffset;
    //рассчет положени€ элементов options menu:
    m_numOfOptionsItems = sizeof(m_optionsMenuItems) / sizeof(char*);
    m_optionsYOffset = (UISettings::menuWindowHeight - ((int)(sizeof(m_optionsMenuItems) / sizeof(char*)) + UISettings::spaceBeetwenTitleAndItems)) / 2 + UISettings::menuTitleYOffset;
    m_optionsXOffset = (UISettings::menuWindowWides - strlen(m_optionsMenuTitle)) / 2 + UISettings::menuTitleXOffset;
    //рассчет положени€ элементов pause menu:
    m_numOfPauseItems = sizeof(m_pauseMenuItems) / sizeof(char*);
    m_pauseYOffset = (UISettings::menuWindowHeight - ((int)(sizeof(m_pauseMenuItems) / sizeof(char*)) + UISettings::spaceBeetwenTitleAndItems)) / 2 + UISettings::menuTitleYOffset;
    m_pauseXOffset = (UISettings::menuWindowWides - strlen(m_pauseMenuTitle)) / 2 + UISettings::menuTitleXOffset;
    //рассчет положени€ элементов battle menu:
    m_numOfBattleItems = sizeof(m_battleMenuItems) / sizeof(char*);
    m_battleYOffset = (UISettings::actionWindowHeight - ((int)(sizeof(m_pauseMenuItems) / sizeof(char*)) + UISettings::spaceBeetwenTitleAndItems)) / 2 + UISettings::menuTitleYOffset;
}

//универсальна€ функци€ дл€ вывода меню:
void UserInterface::printMenu(WINDOW* window, int YOffset, int XOffset, int selected, int numItems, const char* title, const char** items, bool isInBox) {
    wclear(window);
    if (isInBox)
        box(window, 0, 0);
    //вывод загаловка:
    mvwprintw(window, YOffset, XOffset, title);
    //вывод элементов меню:
    for (int i = 0; i < numItems; ++i) {
        if (i == selected)
            mvwprintw(window, YOffset + UISettings::spaceBeetwenTitleAndItems + i, XOffset, "-> ");
        else
            mvwprintw(window, YOffset + UISettings::spaceBeetwenTitleAndItems + i, XOffset, "  ");
        wprintw(window, "%s", items[i]);
    }
    wrefresh(window);
}

//универсальна€ функци€ дл€ инициализации меню:
UIActionType UserInterface::inMenu(UserInterface::MenuType type, std::string titleText) {
    int selected{ 0 };
    int numOfItems{ 0 };

    do {
        switch (type) {
        case UserInterface::MenuType::MAIN:
            printMenu(m_windows["menu"], m_mainYOffset, m_mainXOffset, selected, m_numOfMainItems, m_mainMenuTitle, m_mainMenuItems);
            numOfItems = m_numOfMainItems;
            break;
        case UserInterface::MenuType::OPTIONS:
            printMenu(m_windows["menu"], m_optionsYOffset, m_optionsXOffset, selected, m_numOfOptionsItems, m_optionsMenuTitle, m_optionsMenuItems);
            numOfItems = m_numOfOptionsItems;
            break;
        case UserInterface::MenuType::PAUSE:
            printMenu(m_windows["menu"], m_pauseYOffset, m_pauseXOffset, selected, m_numOfPauseItems, m_pauseMenuTitle, m_pauseMenuItems, true);
            numOfItems = m_numOfPauseItems;
            break;
        case UserInterface::MenuType::BATTLE:
            int battleXOffset = (UISettings::actionWindowWides - titleText.length()) / 2 + UISettings::menuTitleXOffset;
            printMenu(m_windows["action"], m_battleYOffset, battleXOffset, selected, m_numOfBattleItems, titleText.c_str(), m_battleMenuItems, true);
            numOfItems = m_numOfBattleItems;
            break;
        }

        switch (getch()) {
        case 115:
            if (selected < numOfItems - 1)
                ++selected;
            break;
        case 119:
            if (selected > 0)
                --selected;
            break;
        case 10:
            return selectMenu(type, selected);
        }
    } while (true);
}

//универсальна€ функци€ дл€ выбора действи€ меню:
UIActionType UserInterface::selectMenu(UserInterface::MenuType type, int selected) {
    switch (type) {
        //main:
    case UserInterface::MenuType::MAIN:
        switch (selected) {
        case 0:
            return UIActionType::GAME;
            break;
        case 1:
            return UIActionType::OPTIONS;
            break;
        case 2:
            return UIActionType::QUIT;
            break;
        }
        break;

        //options:
    case UserInterface::MenuType::OPTIONS:
        switch (selected) {
        case 0:
            return UIActionType::EASY;
            break;
        case 1:
            return UIActionType::MEDIUM;
            break;
        case 2:
            return UIActionType::HARD;
            break;
        }
        break;

        //pause
    case UserInterface::MenuType::PAUSE:
        switch (selected) {
        case 0:
            return UIActionType::RESUME;
            break;
        case 1:
            return UIActionType::QUIT;
            break;
        }
        break;
        //battle:
    case UserInterface::MenuType::BATTLE:
        switch (selected) {
        case 0:
            return UIActionType::ATTACK;
            break;
        case 1:
            return UIActionType::RUN;
            break;
        }
    }
}

//вывод на на экран stats значений характеристик персонажа:
void UserInterface::updateStatsMenu(int currentHealth, int maxHealth) {
    wclear(m_windows["stats"]);
    box(m_windows["stats"], 0, 0);
    mvwprintw(m_windows["stats"], 1, 1, "%s%d", "HP ", currentHealth);
    double healthBarLength = (double)currentHealth / maxHealth;
    int numberOfEntrance = (getmaxx(m_windows["stats"]) - 8) * healthBarLength;
    mvwprintw(m_windows["stats"], 1, 7, "%s", std::string(numberOfEntrance, '|').c_str());
    wrefresh(m_windows["stats"]);
}

WINDOW* UserInterface::operator[](std::string key) {
    return m_windows[key];
}