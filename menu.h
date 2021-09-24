#ifndef MENU_H
#define MENU_H

#include <curses.h>
#include <cstring>
#include "structs.h"

class Menu {
	WINDOW* m_menuWindow;

	int m_mainYOffset;
	int m_mainXOffset;
	int m_numOfMainItems;
	const char* m_mainMenuTitle{ "MAIN MENU" };
	const char* m_mainMenuItems[3]{
		"Start",
		"Options",
		"Quit"
	};

	int m_optionsYOffset;
	int m_optionsXOffset;
	int m_numOfOptionsItems;
	const char* m_optionsMenuTitle{ "DIFFICULTY" };
	const char* m_optionsMenuItems[3]{
		"Easy",
		"Medium",
		"Hard"
	};

	int m_pauseYOffset;
	int m_pauseXOffset;
	int m_numOfPauseItems;
	const char* m_pauseMenuTitle{ "PAUSE MENU" };
	const char* m_pauseMenuItems[2]{
		"Resume",
		"Quit"
	};

public:
	enum class MenuType {
		MAIN,
		OPTIONS,
		PAUSE
	};

	Menu();

	void printMenu(int YOffset, int XOffset, int selected, int numItems, const char* title, const char** items, bool isInBox = false);
	UIActionType selectMenu(Menu::MenuType type, int selected);
	UIActionType inMenu(MenuType type);

};

#endif