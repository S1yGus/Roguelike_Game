#define PLAYER_COLOR 1

#include <curses.h>
#include <cstdlib>
#include <ctime>
#include "mainSettings.h"
#include "menu.h"
#include "actors.h"
#include "gameactors.h"
#include "gameplay.h"

int main() {
    std::srand(std::time(0));

    //переход в режим curses и установка размера окна терминала:
    initscr();
    resize_term(mainSettings::terminalHight, mainSettings::terminalWidth);
    //отключение курсора и отображения вводимых символов:
    curs_set(0);
    noecho();
    refresh();
    //если изменить размер терминала не удалось, то выводится сообщение об ошибке:
    /*if (!is_termresized()) {
        endwin();
        const char* error{ "ERROR! Fail resize terminal!" };
        const char* exit{ "Press any key to exit." };
        mvprintw(LINES / 2, (COLS - strlen(error)) / 2, "%s\n", error);
        mvprintw(LINES / 2 + 1, (COLS - strlen(error)) / 2, "%s\n", exit);
        getch();
        return -1;
    }*/
    //если терминал не поддерживает изменения цветов, то выводится сообщение об ошибке:
    if (!has_colors()) {
        endwin();
        printf("Your terminal does not support color\n");
        return -1;
    }
    //инициализация цветовых пар:
    start_color();
    init_pair(PLAYER_COLOR, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLUE);
    init_pair(3, COLOR_BLACK, COLOR_WHITE);
    init_pair(4, COLOR_RED, COLOR_MAGENTA);

    std::map<std::string, WINDOW*> windows;

    //создание и расчет положения mainWindow:
    WINDOW* mainWindow = newwin(LINES - mainSettings::mainWindowYOffset * 2, COLS - mainSettings::mainWindowXOffset * 2, mainSettings::mainWindowYOffset, mainSettings::mainWindowXOffset);
    box(mainWindow, 0, 0);
    wrefresh(mainWindow);
    windows["main"] = mainWindow;

    //создание и расчет положения menuWindow: 
    int menuWindowYOffset = (LINES - mainSettings::menuWindowHeight) / 2 + mainSettings::menuWindowYOffset;
    int menuWindowXOffset = (COLS - mainSettings::menuWindowWides) / 2 + mainSettings::menuWindowXOffset;
    WINDOW* menuWindow = newwin(mainSettings::menuWindowHeight, mainSettings::menuWindowWides, menuWindowYOffset, menuWindowXOffset);
    windows["menu"] = menuWindow;

    //создание и расчет положения fieldWindow:
    int fieldWindowHeight, fieldWindowWides;
    getmaxyx(windows["main"], fieldWindowHeight, fieldWindowWides);
    WINDOW* fieldWindow = newwin(fieldWindowHeight - (mainSettings::infoWindowHeight + 2), fieldWindowWides - mainSettings::fieldWindowXOffset * 2, mainSettings::mainWindowYOffset + mainSettings::fieldWindowYOffset, mainSettings::mainWindowXOffset + mainSettings::fieldWindowXOffset);
    windows["field"] = fieldWindow;

    //создание и расчет положения infoWindow:
    WINDOW* infoWindow = newwin(mainSettings::infoWindowHeight, getmaxx(windows["main"]) / 2, getmaxy(windows["field"]) + 2, mainSettings::mainWindowXOffset + mainSettings::fieldWindowXOffset);
    windows["info"] = infoWindow;

    Field field(windows, player);

    field.getRandomPlaceInRandomRoom() = &lamp;
    field.getRandomPlaceInRandomRoom() = &enemy;
    field.getRandomPlaceInRandomRoom() = &enemy;
    field.getRandomPlaceInRandomRoom() = &enemy;

    Game game(windows, field, player);

    Menu menu(windows);
    UIActionType difficulty{ UIActionType::MEDIUM }; //сложность

    UIActionType action{ menu.inMenu(Menu::MenuType::MAIN) }; //текущее действие
    while (true) {
        switch (action) {
        case UIActionType::GAME:
            action = game.inGame();
            break;
        case UIActionType::OPTIONS:
            action = menu.inMenu(Menu::MenuType::OPTIONS);
            break;
        case UIActionType::EASY:
            difficulty = UIActionType::EASY;
            action = menu.inMenu(Menu::MenuType::MAIN);
            break;
        case UIActionType::MEDIUM:
            difficulty = UIActionType::MEDIUM;
            action = menu.inMenu(Menu::MenuType::MAIN);
            break;
        case UIActionType::HARD:
            difficulty = UIActionType::HARD;
            action = menu.inMenu(Menu::MenuType::MAIN);
            break;
        case UIActionType::PAUSE:
            action = menu.inMenu(Menu::MenuType::PAUSE);
            break;
        case UIActionType::RESUME:
            action = game.inGame();
            break;
        case UIActionType::QUIT:
            std::exit(0);
        default:
            std::exit(0);
        }
    }

    endwin();
}