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

    //Переходим в режим curses и устанавливаемя размеры окна терминала:
    initscr();
    resize_term(mainSettings::terminalHight, mainSettings::terminalWidth);
    //Убираем курсор и отключаем отображение вводимых символов:
    curs_set(0);
    noecho();
    refresh();
    //Если изменить размер терминала не удалось, то вываодим сообщение об ошибке:
    /*if (!is_termresized()) {
        const char* error{ "ERROR! Fail resize terminal!" };
        const char* exit{ "Press any key to exit." };
        mvprintw(LINES / 2, (COLS - strlen(error)) / 2, "%s\n", error);
        mvprintw(LINES / 2 + 1, (COLS - strlen(error)) / 2, "%s\n", exit);
        getch();
        std::exit(1);
    }*/

    //Создаем и выделяем окно в терминале:
    WINDOW* mainWindow = newwin(LINES - mainSettings::mainWindowYOffset * 2, COLS - mainSettings::mainWindowXOffset * 2, mainSettings::mainWindowYOffset, mainSettings::mainWindowXOffset);
    box(mainWindow, 0, 0);
    wrefresh(mainWindow);

    Field field(COLS, LINES, mainSettings::mainWindowXOffset, mainSettings::mainWindowYOffset, player);

    field.getRandomPlaceInRandomRoom() = &lamp;
    field.getRandomPlaceInRandomRoom() = &enemy;
    field.getRandomPlaceInRandomRoom() = &enemy;
    field.getRandomPlaceInRandomRoom() = &enemy;

    Game game(field, player);

    Menu menu;
    UIActionType difficulty{ UIActionType::MEDIUM }; //Сложность

    UIActionType action{ menu.inMenu(Menu::MenuType::MAIN) }; //Текущее действие
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