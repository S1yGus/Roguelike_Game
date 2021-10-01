#define PLAYER_COLOR 1

#include <curses.h>
#include <cstdlib>
#include <ctime>
#include "settings.h"
#include "UI.h"
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

    UserInterface userInterface;

    Field field(userInterface, player);

    field.getRandomPlaceInRandomRoom() = &lamp;
    field.getRandomPlaceInRandomRoom() = &enemy;
    field.getRandomPlaceInRandomRoom() = &enemy;
    field.getRandomPlaceInRandomRoom() = &enemy;

    Game game(userInterface, field, player);

    UIActionType difficulty{ UIActionType::MEDIUM }; //сложность

    UIActionType action{ userInterface.inMenu(UserInterface::MenuType::MAIN) }; //текущее действие
    while (true) {
        switch (action) {
        case UIActionType::GAME:
            action = game.inGame();
            break;
        case UIActionType::OPTIONS:
            action = userInterface.inMenu(UserInterface::MenuType::OPTIONS);
            break;
        case UIActionType::EASY:
            difficulty = UIActionType::EASY;
            action = userInterface.inMenu(UserInterface::MenuType::MAIN);
            break;
        case UIActionType::MEDIUM:
            difficulty = UIActionType::MEDIUM;
            action = userInterface.inMenu(UserInterface::MenuType::MAIN);
            break;
        case UIActionType::HARD:
            difficulty = UIActionType::HARD;
            action = userInterface.inMenu(UserInterface::MenuType::MAIN);
            break;
        case UIActionType::PAUSE:
            action = userInterface.inMenu(UserInterface::MenuType::PAUSE);
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