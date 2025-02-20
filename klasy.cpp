#include <fstream>
#include <string>
#include "klasy.h"

using namespace std;

//Metody klasy Domino

Domino::Domino() : left(0), right(0) {}
Domino::Domino(int l, int r) : left(l), right(r) {}

int Domino::getLeft() const { return left; }

int Domino::getRight() const { return right; }

void Domino::printDomino() const
{
	cout << "[" << left << "|" << right << "]";
}

bool Domino::matches(int value)const
{
	return left == value || right == value;
}

Domino Domino::flip() const
{
	return Domino(right, left);
}

//


//Metody klasy DominoDeck

DominoDeck::DominoDeck() : deck(generateDeck()) {}

vector<Domino> DominoDeck::generateDeck()
{

	deck.clear();	//Czysci kafelki z talii jesli jakies pozostaly
	vector<Domino> newDeck;

	//og 6
	for (int i = 0; i <= 6; ++i)
	{
		//og 6
		for (int j = 0; j <= 6; ++j)
		{
			Domino domino(i, j);	//tworzy kafelek o parametrach i oraz j
			newDeck.push_back(domino);	//wklada kafelek do talii
		}
	}

	deck = newDeck;
	return deck;
}

int DominoDeck::getSize() const { return deck.size(); }

void DominoDeck::writeDomino(int index) { deck[index].printDomino(); }

Domino DominoDeck::getDomino(int index) { return deck[index]; }

Domino DominoDeck::getRandomDomino()
{
	if (deck.size() > 0)
	{
		srand(time(NULL));

		int randomIndex = rand() % deck.size();	//losuje indeks kafelka z talii, ktory zostanie dodany do reki gracza
		Domino temp = deck[randomIndex];	//zapisuje wylosowany kafelek w zmiennej pomocniczej
		deck.erase(deck.begin() + randomIndex);	//usuwa kafelek z talii
		return temp;	//zwraca wylosowany kafelek
	}
	else
	{
		//zwraca kafelek symbolizujacy koniec talii, sygnal ze runda sie zakocznyla
		Domino endOfDeck(7, 7);
		return endOfDeck;
	}
}

void DominoDeck::printDeck()
{
	for (int i = 0; i < getSize(); ++i)
	{
		deck[i].printDomino();
	}
}

//


//Metody klasy Player

Player::Player(const string& playername) : name(playername), points(0), winnerStatus(false) {}

string Player::getName()const { return name; }

int Player::getHandSize()const { return hand.size(); }

int Player::getPoints()const { return points; }

void Player::addDomino(const Domino& kostka)
{
	hand.push_back(kostka);
}

void Player::updatePoints(int newPoints)
{
	points += newPoints;
}

Domino Player::selectDomino(int index)
{
	Domino temp = hand[index];
	hand.erase(hand.begin() + index);
	return temp;
}

Domino Player::showDomino(int index)
{
	Domino temp = hand[index];
	return temp;
}

void Player::showHand()const
{
	cout << name << "'s hand: " << endl;
	int size = hand.size();
	for (int i = 0; i < size; ++i)
	{
		hand[i].printDomino();
		cout << ", ";
	}
}

void Player::setWinnerStatus(bool status) { winnerStatus = status; }

bool Player::getWinnerStatus() const { return winnerStatus; }

void Player::clearHand()
{
	hand.clear();
}

//


//Metody klasy Game

Game::Game() : players(), newdeck(), stock(), turns(-1), isRoundFinished(false), isGameFinished(false), rounds(1) {}

void Game::addPlayer()
{
	string name;
	cout << "Your name: ";
	cin >> name;
	Player temp(name);
	players.push_back(temp);
}

void Game::addComputer()
{
	string name = "Computer";
	Player temp(name);
	players.push_back(temp);
}

void Game::setPlayers()
{
	addPlayer();
	addPlayer();
}

void Game::dealDominos(int dominos)
{
	for (int k = 0; k < players.size(); ++k)
	{
		for (int i = 0; i < dominos; ++i)
		{
			players[k].addDomino(newdeck.getRandomDomino());
		}
	}
}

void Game::writePlayers()
{
	for (int k = 0; k < players.size(); ++k)
	{
		cout << endl;
		cout << "Player " << k + 1 << ": " << players[k].getName() << endl;
		players[k].showHand();
		cout << endl << endl;
	}
}

void Game::writeStock()
{
	for (int i = 0; i < stock.size(); ++i)
	{
		stock[i].printDomino();
	}
	cout << endl << endl;
}

bool Game::isTurnAvailable(Player& player, int l, int r)
{
	bool isAvailable = false;
	for (int i = 0; i < player.getHandSize(); ++i)
	{
		Domino temp = player.showDomino(i);
		if (temp.getLeft() == l || temp.getRight() == l || temp.getLeft() == r || temp.getRight() == r)
		{
			isAvailable = true;
			break;
		}
	}
	return isAvailable;
}

int Game::pickSide()
{
	cout << "Select side" << endl;
	cout << "1 for left, 2 for right: ";

	int side;
	cin >> side;

	if (side == 1)
		return side;
	else if (side == 2)
		return side;
	else
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Wrong instructions." << endl;
		side = pickSide();
	}
}

int Game::getRounds() const { return rounds; }

int Game::addPoints(int l, int p)
{
	int points = 0;
	if ((l + p) % 5 == 0)
		points = l + p;
	return points;
}

bool Game::checkEOD(Player& player)
{
	for (int i = 0; i < player.getHandSize(); ++i)
	{
		if (player.showDomino(i).getLeft() == 7 || player.showDomino(i).getRight() == 7)
		{
			return true;
		}
	}
	return false;
}

void Game::SumPointsAtTheEndOfTheTurn(vector <Player>& pl)
{
	int sum = 0;
	for (int i = 0; i < pl.size(); ++i)
	{
		if (checkEOD(pl[i]))
		{
			for (int j = 0; j < pl[i].getHandSize() - 1; ++j)
			{
				sum += pl[i].showDomino(j).getLeft();
				sum += pl[i].showDomino(j).getRight();
			}
			break;
		}
		else if (pl[i].getHandSize() == 0)
		{
			for (int k = 0; k < pl.size(); ++k)
			{
				if (pl[k].getHandSize() != 0)
				{
					for (int j = 0; j < pl[k].getHandSize(); ++j)
					{
						{
							sum += pl[k].showDomino(j).getLeft();
							sum += pl[k].showDomino(j).getRight();
						}
					}
					break;
				}
			}
		}
	}

	sum = sum / 5;
	sum = sum * 5;

	for (int i = 0; i < pl.size(); ++i)
	{
		if (!checkEOD(pl[i]) && pl[i].getHandSize() == 0)
		{
			pl[i].updatePoints(sum);
			break;
		}
	}
}

void Game::playerPoints(Player& player)
{
	string name = player.getName();
	cout << name << "'s points: " << player.getPoints() << endl;
}

void Game::checkGameStatus(vector<Player>& pl)
{
	int size = pl.size();
	for (int i = 0; i < size; ++i)
	{
		//Ustawienie ilosci punktow do wygranej, bazowo 100
		if (pl[i].getPoints() >= 100)
		{
			pl[i].setWinnerStatus(true);
			isGameFinished = true;
			isRoundFinished = true;
		}
	}
}

void Game::checkRoundStatus(Player& player)
{
	int handSize = player.getHandSize();
	if (handSize == 0 && !isRoundFinished)
	{
		isRoundFinished = true;
		++rounds;
		return;
	}
	else if (!isRoundFinished)
	{
		for (int i = 0; i < handSize; ++i)
		{
			Domino temp = player.showDomino(i);
			if (temp.getLeft() == 7 || temp.getRight() == 7)
			{
				isRoundFinished = true;
				++rounds;
				return;
			}
		}
	}
}

int Game::checkIndex(Player& player)
{
	int index;
	cout << "Select domino: ";
	cin >> index;
	--index;

	while (index >= player.getHandSize() || index < 0)
	{
		if (!isdigit(index))
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Wrong piece." << endl;
			cout << "Select domino: ";
			cin >> index;
			--index;
			continue;
		}

		cout << "Wrong piece." << endl;
		cout << "Select domino: ";
		cin >> index;
		--index;
	}

	return index;
}

void Game::PlayersTurn(Player& player)
{
	int index;
	Domino picked;
	bool isAvailable;
	int handSize = player.getHandSize();

	if (stock.size() == 0)
	{
		index = checkIndex(player);
		picked = player.selectDomino(index);
		stock.push_back(picked);
	}
	else
	{
		int l = stock[0].getLeft();
		int r = stock[turns].getRight();

		isAvailable = isTurnAvailable(player, l, r);

		while (!isAvailable)
		{
			cout << player.getName() << ", you don't have any matching pieces." << endl;
			player.addDomino(newdeck.getRandomDomino());


			checkRoundStatus(player);
			if (isRoundFinished == true)
				break;


			Domino temp = player.showDomino(player.getHandSize() - 1);
			if (temp.getLeft() != 7 && temp.getRight() != 7)
			{
				cout << "You drew ";
				temp.printDomino();
				cout << endl;
			}

			isAvailable = isTurnAvailable(player, l, r);
		}

		if (!isRoundFinished)
		{
			player.showHand();

			index = checkIndex(player);

			picked = player.showDomino(index);

			while ((picked.getLeft() != l && picked.getRight() != l && picked.getLeft() != r && picked.getRight() != r) || index > player.getHandSize())
			{
				cout << "You can't select this piece." << endl;
				index = checkIndex(player);
				picked = player.showDomino(index);
			}

			if ((picked.getLeft() == l || picked.getRight() == l) && (picked.getLeft() == r || picked.getRight() == r))
			{
				player.selectDomino(index);
				int side = pickSide();
				if (side == 1)
				{
					if (l == picked.getLeft())
					{
						picked = picked.flip();
						stock.insert(stock.begin(), picked);
					}
					else if (l == picked.getRight())
						stock.insert(stock.begin(), picked);
				}
				else if (side == 2)
				{
					if (r == picked.getLeft())
						stock.push_back(picked);
					else if (r == picked.getRight())
					{
						picked = picked.flip();
						stock.push_back(picked);
					}
				}
			}
			else
			{
				player.selectDomino(index);

				if (l == picked.getLeft())
				{
					picked = picked.flip();
					stock.insert(stock.begin(), picked);
				}
				else if (l == picked.getRight())
					stock.insert(stock.begin(), picked);
				else if (r == picked.getLeft())
					stock.push_back(picked);
				else if (r == picked.getRight())
				{
					picked = picked.flip();
					stock.push_back(picked);
				}

				checkRoundStatus(player);
			}
		}
	}
	++turns;

	int l = stock[0].getLeft();
	int r = stock[turns].getRight();

	int points = addPoints(l, r);
	player.updatePoints(points);
	playerPoints(player);

	cout << endl;

	checkRoundStatus(player);
	checkGameStatus(players);
}

void Game::ComputerTurn(Player& player)
{
	int index;
	Domino picked;
	bool isAvailable;
	int handSize = player.getHandSize();

	if (stock.size() == 0)
	{
		stock.push_back(player.selectDomino(1));
	}
	else
	{
		int l = stock[0].getLeft();
		int r = stock[turns].getRight();

		isAvailable = isTurnAvailable(player, l, r);

		while (!isAvailable)
		{
			cout << player.getName() << ", you don't have any matching pieces." << endl;
			player.addDomino(newdeck.getRandomDomino());


			checkRoundStatus(player);
			if (isRoundFinished == true)
				break;


			Domino temp = player.showDomino(player.getHandSize() - 1);
			if (temp.getLeft() != 7 && temp.getRight() != 7)
			{
				cout << "You drew ";
				temp.printDomino();
				cout << endl;
			}

			isAvailable = isTurnAvailable(player, l, r);
		}

		if (!isRoundFinished)
		{
			for (int i = 0; i < player.getHandSize(); ++i)
			{
				if (player.showDomino(i).getLeft() == l || player.showDomino(i).getLeft() == r || player.showDomino(i).getRight() == l || player.showDomino(i).getRight() == r)
				{
					Domino picked = player.showDomino(i);
					if ((picked.getLeft() == l || picked.getRight() == l) && (picked.getLeft() == r || picked.getRight() == r))
					{
						player.selectDomino(i);

						if (l == picked.getLeft())
						{
							picked = picked.flip();
							stock.insert(stock.begin(), picked);
							break;
						}
						else if (l == picked.getRight())
						{
							stock.insert(stock.begin(), picked);
							break;
						}

					}
					else
					{
						player.selectDomino(i);

						if (l == picked.getLeft())
						{
							picked = picked.flip();
							stock.insert(stock.begin(), picked);
							break;
						}
						else if (l == picked.getRight())
						{
							stock.insert(stock.begin(), picked);
							break;
						}
						else if (r == picked.getLeft())
						{
							stock.push_back(picked);
							break;
						}
						else if (r == picked.getRight())
						{
							picked = picked.flip();
							stock.push_back(picked);
							break;
						}

						checkRoundStatus(player);
					}
				}

			}


		}
	}
	++turns;

	int l = stock[0].getLeft();
	int r = stock[turns].getRight();

	int points = addPoints(l, r);
	player.updatePoints(points);
	playerPoints(player);

	cout << endl;

	checkRoundStatus(player);
	checkGameStatus(players);
}

void Game::setup()
{
	for (int i = 0; i < players.size(); ++i)
	{
		players[i].clearHand();
	}

	isRoundFinished = false;
	newdeck.generateDeck();
	dealDominos(7);
	writePlayers();
	system("cls");
	cout << "Runda " << getRounds() << endl << endl;
	for (int i = 0; i < players.size(); ++i)
	{
		playerPoints(players[i]);
	}

	players[0].showHand();
	stock.clear();
	turns = -1;
}

void Game::SaveToFile(Player& player)
{
	string filename = "dane.txt";
	ofstream file;
	file.open(filename, ios::app);

	if (file.is_open())
	{
		string gracz = player.getName();
		int punkty = player.getPoints();
		file << "Winner: " << gracz << ", points: " << punkty << endl;
		file.close();
		cout << "Results saved to file." << endl;
	}
	else
	{
		cerr << "Can't open file to save." << endl;
	}
}

void Game::StartGame()
{
	while (!isGameFinished)
	{
		cout << "Runda " << getRounds() << endl << endl;
		setup();
		while (!isRoundFinished)
		{
			if (!isRoundFinished)
			{
				PlayersTurn(players[0]);
				writeStock();
			}
			if (!isRoundFinished)
			{
				PlayersTurn(players[1]);
				writeStock();
			}
		}
		SumPointsAtTheEndOfTheTurn(players);
		checkGameStatus(players);
	}

	checkGameStatus(players);
	cout << "Gra zakonczona." << endl;
	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i].getWinnerStatus() == true)
		{
			cout << "Winner: " << players[i].getName() << endl;
			cout << "Points: " << players[i].getPoints() << endl;
		}
	}
	cout << "Koniec." << endl;
}

//


//Metody klasy GameVsComputer

GameVsComputer::GameVsComputer() : Game() {}

void GameVsComputer::setPlayers()
{
	addPlayer();
	addComputer();
}

void GameVsComputer::StartGame()
{
	while (!isGameFinished)
	{
		setup();
		while (!isRoundFinished)
		{
			if (!isRoundFinished)
			{
				PlayersTurn(players[0]);
				system("cls");
				writeStock();
				cout << players[1].getName() << "s' points: " << players[1].getPoints() << endl;
			}
			if (!isRoundFinished)
			{
				ComputerTurn(players[1]);
				system("cls");
				writeStock();
				cout << players[0].getName() << "s' points: " << players[0].getPoints() << endl;
			}
		}
		SumPointsAtTheEndOfTheTurn(players);
		checkGameStatus(players);
	}

	checkGameStatus(players);
	cout << "Gra zakonczona." << endl;
	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i].getWinnerStatus() == true)
		{
			cout << "Winner: " << players[i].getName() << endl;
			cout << "Points: " << players[i].getPoints() << endl;
			SaveToFile(players[i]);
		}
	}
	cout << "Koniec." << endl;
}

//


//Metody klasy GameVsPlayer

GameVsPlayer::GameVsPlayer() : Game() {}

void GameVsPlayer::setPlayers()
{
	addPlayer();
	addPlayer();
}

void GameVsPlayer::StartGame()
{
	while (!isGameFinished)
	{
		cout << "Runda " << getRounds() << endl << endl;
		setup();
		while (!isRoundFinished)
		{
			if (!isRoundFinished)
			{
				PlayersTurn(players[0]);
				system("cls");
				writeStock();
				cout << players[1].getName() << "'s points: " << players[1].getPoints() << endl;
			}
			if (!isRoundFinished)
			{
				PlayersTurn(players[1]);
				system("cls");
				writeStock();
				cout << players[0].getName() << "'s points: " << players[0].getPoints() << endl;
			}
		}
		SumPointsAtTheEndOfTheTurn(players);
		checkGameStatus(players);
	}

	checkGameStatus(players);
	cout << "Gra zakonczona." << endl;
	for (int i = 0; i < players.size(); ++i)
	{
		if (players[i].getWinnerStatus() == true)
		{
			cout << "Winner: " << players[i].getName() << endl;
			cout << "Points: " << players[i].getPoints() << endl;
			SaveToFile(players[i]);
		}
	}
	cout << "Koniec." << endl;
}

//






ComputerVsComputer::ComputerVsComputer() : Game() {}

void ComputerVsComputer::setPlayers()
{
	addComputer();
	addComputer();
}

//
void ComputerVsComputer::StartGame()
{
	while (!isGameFinished)
	{
		setup();
		while (!isRoundFinished)
		{
			if (!isRoundFinished)
			{
				ComputerTurn(players[0]);
				system("cls");
				writeStock();
				cout << players[1].getName() << "1'st points: " << players[1].getPoints() << endl;
			}
			if (!isRoundFinished)
			{
				ComputerTurn(players[1]);
				system("cls");
				writeStock();
				cout << players[0].getName() << "2'nd points: " << players[0].getPoints() << endl;
			}
		}
		SumPointsAtTheEndOfTheTurn(players);
		checkGameStatus(players);
	}

	checkGameStatus(players);
	cout << "Gra zakonczona." << endl;
	
		if (players[0].getWinnerStatus() == true)
		{
			cout << "Winner: Computer 1st" << endl;
			cout << "Points: " << players[0].getPoints() << endl;
			SaveToFile(players[0]);
		}
		else if (players[1].getWinnerStatus() == true)
		{
			cout << "Winner: Computer 2nd" << endl;
			cout << "Points: " << players[1].getPoints() << endl;
			SaveToFile(players[1]);
		}
	
	cout << "Koniec." << endl;
}