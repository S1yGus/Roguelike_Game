#include "menu.h"
#include "menuSettings.h"

Menu::Menu(const std::map <std::string, WINDOW*>& windows) : m_windows{ windows } {
    int menuWindowHeight, menuWindowWides;
    getmaxyx(m_windows["menu"], menuWindowHeight, menuWindowWides);
    //рассчет положения элементов main menu:
    m_numOfMainItems = sizeof(m_mainMenuItems) / sizeof(char*);
    m_mainYOffset = (menuWindowHeight - ((int)(sizeof(m_mainMenuItems) / sizeof(char*)) + menuSettings::spaceBeetwenTitleAndItems)) / 2 + menuSettings::menuTitleYOffset;
    m_mainXOffset = (menuWindowWides - strlen(m_mainMenuTitle)) / 2 + menuSettings::menuTitleXOffset;
    //рассчет положения элементов options menu:
    m_numOfOptionsItems = sizeof(m_optionsMenuItems) / sizeof(char*);
    m_optionsYOffset = (menuWindowHeight - ((int)(sizeof(m_optionsMenuItems) / sizeof(char*)) + menuSettings::spaceBeetwenTitleAndItems)) / 2 + menuSettings::menuTitleYOffset;
    m_optionsXOffset = (menuWindowWides - strlen(m_optionsMenuTitle)) / 2 + menuSettings::menuTitleXOffset;
    //рассчет положения элементов pause menu:
    m_numOfPauseItems = sizeof(m_pauseMenuItems) / sizeof(char*);
    m_pauseYOffset = (menuWindowHeight - ((int)(sizeof(m_pauseMenuItems) / sizeof(char*)) + menuSettings::spaceBeetwenTitleAndItems)) / 2 + menuSettings::menuTitleYOffset;
    m_pauseXOffset = (menuWindowWides - strlen(m_pauseMenuTitle)) / 2 + menuSettings::menuTitleXOffset;
}

//универсальная функция для вывода меню:
void Menu::printMenu(int YOffset, int XOffset, int selected, int numItems, const char* title, const char** items, bool isInBox) {
    wclear(m_windows["menu"]);
    if (isInBox)
        box(m_windows["menu"], 0, 0);
    //вывод загаловка:
    mvwprintw(m_windows["menu"], YOffset, XOffset, title);
    //вывод элементов меню:
    for (int i = 0; i < numItems; ++i) {
        if (i == selected)
            mvwprintw(m_windows["menu"], YOffset + menuSettings::spaceBeetwenTitleAndItems + i, XOffset, "-> ");
        else
            mvwprintw(m_windows["menu"], YOffset + menuSettings::spaceBeetwenTitleAndItems + i, XOffset, "  ");
        wprintw(m_windows["menu"], "%s", items[i]);
    }
    wrefresh(m_windows["menu"]);
}

//универсальная функция для инициализации меню:
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

//универсальная функция для выбора действия меню:
UIActionType Menu::selectMenu(Menu::MenuType type, int selected) {
    switch (type) {
    //main:
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

    //options:
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

    //pause
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