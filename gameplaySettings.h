#ifndef SETTINGS_H
#define SETTINGS_H

namespace gameplaySettings {
	//Field seeting:
	int fieldX = 30;
	int fieldY = 30;

	//Room settings:
	int countOfRooms = 15;
	int minRoomWidth = 5;
	int maxRoomWidth = 15;
	int minRoomHight = 5;
	int maxRoomHight = 15;
	int passagesWidth = 3;  //от 2 до 8

	//Cost settings:
	int spaceCost = 0;
	int wallCost = 100;
	int diagonalCost = 50;
	int orthogonalCost = 10;
}

#endif