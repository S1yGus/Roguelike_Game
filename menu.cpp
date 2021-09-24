#include "menu.h"
#include "menuSettings.h"

Menu::Menu() {
    //рассчет положений окна и элементов меню:
    int menuWindowYOffset = (LINES - menuSettings::menuWindowHeight) / 2 + menuSettings::menuWindowYOffset;
    int menuWindowXOffset = (COLS - menuSettings::menuWindowWides) / 2 + menuSettings::menuWindowXOffset;
    m_menuWindow = newwin(menuSettings::menuWindowHeight, menuSettings::menuWindowWides, menuWindowYOffset, menuWindowXOffset);
    //Main menu:
    m_numOfMainItems = sizeof(m_mainMenuItems) / sizeof(char*);
    m_mainYOffset = (menuSettings::menuWindowHeight - ((int)(sizeof(m_mainMenuItems) / sizeof(char*)) + 2)) / 2 + menuSettings::menuTitleYOffset;
    m_mainXOffset = (menuSettings::menuWindowWides - strlen(m_mainMenuTitle)) / 2 + menuSettings::menuTitleXOffset;
    //Options menu:
    m_numOfOptionsItems = sizeof(m_optionsMenuItems) / sizeof(char*);
    m_optionsYOffset = (menuSettings::menuWindowHeight - ((int)(sizeof(m_optionsMenuItems) / sizeof(char*)) + 2)) / 2 + menuSettings::menuTitleYOffset;
    m_optionsXOffset = (menuSettings::menuWindowWides - strlen(m_optionsMenuTitle)) / 2 + menuSettings::menuTitleXOffset;
    //Pause menu:
    m_numOfPauseItems = sizeof(m_pauseMenuItems) / sizeof(char*);
    m_pauseYOffset = (menuSettings::menuWindowHeight - ((int)(sizeof(m_pauseMenuItems) / sizeof(char*)) + 2)) / 2 + menuSettings::menuTitleYOffset;
    m_pauseXOffset = (menuSettings::menuWindowWides - strlen(m_pauseMenuTitle)) / 2 + menuSettings::menuTitleXOffset;
}

//Универсальная функция для вывода меню:
void Menu::printMenu(int YOffset, int XOffset, int selected, int numItems, const char* title, const char** items, bool isInBox) {
    wclear(m_menuWindow);
    if (isInBox)
        box(m_menuWindow, 0, 0);
    //Вывод загаловка:
    mvwprintw(m_menuWindow, YOffset, XOffset, title);
    //Вывод элементов меню:
    for (int i = 0; i < numItems; ++i) {
        if (i == selected)
            mvwprintw(m_menuWindow, YOffset + 2 + i, XOffset, "-> ");
        else
            mvwprintw(m_menuWindow, YOffset + 2 + i, XOffset, "  ");
        wprintw(m_menuWindow, "%s", items[i]);
    }
    wrefresh(m_menuWindow);
}

//Универсальная функция для инициализации меню:
UIActionType Menu::inMenu(Menu::MenuType type) {
    int selected{ 0 };
    int numOfItems{ 0 };

    do {
        switch (type) {
        case Menu::MenuType::MAIN:
            printMenu(m_mainYOffset, m_mainXOffset, selected, m_numOfMainItems, m_mainMenuTitle, m_mainMenuItems);
            numOfItems = m_numOfMainItems;
            break;
        case Menu::MenuType::OPTIONS:
            printMenu(m_optionsYOffset, m_optionsXOffset, selected, m_numOfOptionsItems, m_optionsMenuTitle, m_optionsMenuItems);
            numOfItems = m_numOfOptionsItems;
            break;
        case Menu::MenuType::PAUSE:
            printMenu(m_pauseYOffset, m_pauseXOffset, selected, m_numOfPauseItems, m_pauseMenuTitle, m_pauseMenuItems, true);
            numOfItems = m_numOfPauseItems;
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

//Универсальная функция для выбора действия меню:
UIActionType Menu::selectMenu(Menu::MenuType type, int selected) {
    switch (type) {
    //Main:
    case Menu::MenuType::MAIN:
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

    //Options:
    case Menu::MenuType::OPTIONS:
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

    //Pause
    case Menu::MenuType::PAUSE:
        switch (selected) {
        case 0:
            return UIActionType::RESUME;
            break;
        case 1:
            return UIActionType::QUIT;
            break;
        }
        break;
    } 
}