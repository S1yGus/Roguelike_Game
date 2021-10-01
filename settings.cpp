#include "structs.h"

namespace mainSettings {
	extern const int terminalHight = 55;
	extern const int terminalWidth = 230;
}

namespace UISettings {
	//mainWindow:
	extern const int mainWindowYOffset = 1;
	extern const int mainWindowXOffset = 2;
	//menuWindow:
	extern const int menuWindowHeight = 12;
	extern const int menuWindowWides = 40;
	extern const int menuWindowYOffset = 0;
	extern const int menuWindowXOffset = 0;
	//fieldWindow:
	extern const int fieldWindowYOffset = 1;
	extern const int fieldWindowXOffset = 2;
	//infoWindow:
	extern const int infoWindowHeight = 4;
	//actionWindow:
	extern const int actionWindowHeight = 20;
	extern const int actionWindowWides = 80;
	extern const int actionWindowYOffset = 0;
	extern const int actionWindowXOffset = 0;
	//msgWindow:
	extern const int msgWindowHeight = 10;
	extern const int msgWindowWides = 60;
	extern const int msgWindowYOffset = 0;
	extern const int msgWindowXOffset = 0;
	//mainMenu:
	extern const int menuTitleYOffset = 0;
	extern const int menuTitleXOffset = 0;
	extern const int spaceBeetwenTitleAndItems = 2;
}

namespace gameplaySettings {
	//field seeting:
	extern const int fieldX = 30;
	extern const int fieldY = 30;
	//room settings:
	extern const int countOfRooms = 15;
	extern const int minRoomWidth = 5;
	extern const int maxRoomWidth = 15;
	extern const int minRoomHight = 5;
	extern const int maxRoomHight = 15;
	extern const int passagesWidth = 3;  //от 2 до 8
	//cost settings:
	extern const int spaceCost = 0;
	extern const int wallCost = 100;
	extern const int diagonalCost = 50;
	extern const int orthogonalCost = 10;
}