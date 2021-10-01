#pragma once

#include <curses.h>
#include <cstring>
#include "settings.h"
#include "structs.h"

class UserInterface {
	class Windows {
		std::map<std::string, WINDOW*> m_windows;

	public:
		Windows();

		WINDOW* operator[](std::string key);
	};

	Windows m_windows;

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

	int m_battleYOffset;
	int m_numOfBattleItems;
	const char* m_battleMenuItems[2]{
		"Attack",
		"Run"
	};

public:
	enum class MenuType {
		MAIN,
		OPTIONS,
		PAUSE,
		BATTLE,
	};

	UserInterface();

	//inline getters:
	WINDOW* getMainWindow() {
		return m_windows["main"];
	}
	WINDOW* getMenuWindow() {
		return m_windows["menu"];
	}
	WINDOW* getFieldWindow() {
		return m_windows["field"];
	}
	WINDOW* getInfoWindow() {
		return m_windows["info"];
	}
	WINDOW* getStatsWindow() {
		return m_windows["stats"];
	}
	WINDOW* getActionWindow() {
		return m_windows["action"];
	}
	WINDOW* getMSGWindow() {
		return m_windows["msg"];
	}

	UIActionType inMenu(MenuType type, std::string titleText = "");
	void updateStatsMenu(int curretnHealth, int maxHealth);

	WINDOW* operator[](std::string key);

private:
	void printMenu(WINDOW* window, int YOffset, int XOffset, int selected, int numItems, const char* title, const char** items, bool isInBox = false);
	UIActionType selectMenu(UserInterface::MenuType type, int selected);
};