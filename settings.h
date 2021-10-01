#pragma once

namespace mainSettings {
	extern const int terminalHight;
	extern const int terminalWidth;
}

namespace UISettings {
	//mainWindow:
	extern const int mainWindowYOffset;
	extern const int mainWindowXOffset;
	//menuWindow:
	extern const int menuWindowHeight;
	extern const int menuWindowWides;
	extern const int menuWindowYOffset;
	extern const int menuWindowXOffset;
	//fieldWindow:
	extern const int fieldWindowYOffset;
	extern const int fieldWindowXOffset;
	//infoWindow:
	extern const int infoWindowHeight;
	//actionWindow:
	extern const int actionWindowHeight;
	extern const int actionWindowWides;
	extern const int actionWindowYOffset;
	extern const int actionWindowXOffset;
	//msgWindow:
	extern const int msgWindowHeight;
	extern const int msgWindowWides;
	extern const int msgWindowYOffset;
	extern const int msgWindowXOffset;
	//mainMenu:
	extern const int menuTitleYOffset;
	extern const int menuTitleXOffset;
	extern const int spaceBeetwenTitleAndItems;
}

namespace gameplaySettings {
	//field seeting:
	extern const int fieldX;
	extern const int fieldY;
	//room settings:
	extern const int countOfRooms;
	extern const int minRoomWidth;
	extern const int maxRoomWidth;
	extern const int minRoomHight;
	extern const int maxRoomHight;
	extern const int passagesWidth;  //от 2 до 8
	//cost settings:
	extern const int spaceCost;
	extern const int wallCost;
	extern const int diagonalCost;
	extern const int orthogonalCost;
}