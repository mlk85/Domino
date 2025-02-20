#include <iostream>
#include "klasy.h"

using namespace std;

void WholeGame();

int main()
{
	WholeGame();
}

void WholeGame()
{
	int choice;
	cout << "Select game mode:" << endl;
	cout << "1. with other player" << endl;
	cout << "2. with computer" << endl;
	cout << "3. computer vs computer" << endl << endl;
	cin >> choice;
	if (choice == 1)
	{
		GameVsPlayer newgame;
		newgame.setPlayers();
		newgame.StartGame();
	}
	else if (choice == 2)
	{
		GameVsComputer ng;
		ng.setPlayers();
		ng.StartGame();
	}
	else if (choice == 3)
	{
		ComputerVsComputer ng1;
		ng1.setPlayers();
		ng1.StartGame();
	}
	else
	{
		cout << "Unknown command." << endl;

		if (!isdigit(choice))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}

		WholeGame();
	}
}