#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>

using namespace std;

//Klasa odpowiedzialna za pojedynczy kafelek domina, przechowuje jego warosci z prawej i lewej strony
class Domino
{
protected:

	int left;	//wartosc lewej strony kafelka
	int right;	//wartosc prawej strony kafelka

public:

	Domino();	//konstruktor domyslny
	Domino(int l, int r);	//konstruktor z podanymi wartosciami pol

	int getLeft() const;	//pobiera wartosc lewej strony kafelka
	int getRight() const;	//pobiera wartosc prawej strony kafelka
	void printDomino() const;	//wypisuje caly kafelek domino
	bool matches(int value) const;	//sprawdza czy kafelek pasuje do jednego z konca "weza" na stole
	Domino flip() const;	//odwraca kafelek
};

//Klasa odpowiedzialna za talie domina. Pozwala generowac talie, operowac jej elementami, losowac kafelki, wypisac ja 
class DominoDeck : public Domino
{
protected:

	vector<Domino> deck;	//"talia" domino

public:

	DominoDeck();	//konstruktor domyslny

	vector<Domino> generateDeck();	//generuje "talie" domino, kazda wartosc z kazda od 0 do 6 (lacznie z dublami)
	int getSize() const;	//zwraca ilosc kafelkow pozostalych w talii
	void writeDomino(int index);	//wypisuje wartosc kafelka, ktore stoi na miejscu o okreslonym indeksie
	Domino getDomino(int index);	//zwraca wartosc kafelka, ktore stoi na miejscu o okreslonym indeksie
	Domino getRandomDomino();	//z talii wybiera i zwraca losowy kafelek, nastepnie usuwa go z talii
	void printDeck();	//wypisuje cala talie (funkcja do testowania dzialania)
};

//Klasa reprezentujaca gracza, pozwala przechowywac informacje o kafelkach w jego rece, jego nazwie, liczbie punktow
//oraz o tym czy gracz zwyciezyl rozgrywke. Pozwala graczowi wybierac kafelki oraz moze aktualizowac ilosc jego punktow
class Player
{
private:
	vector<Domino> hand;	//reka gracza, zawiera informacje o jego kafelkach domina
	string name;	//nazwa gracza
	int points;		//liczba punktow gracza
	bool winnerStatus;	//status zwyciezcy, pozwala okreslic kiedy zakonczyc rozgrywke

public:

	Player(const string& playername);		//konstruktor tworzacy gracza o podanej nazwie

	string getName()const;	//zwraca nazwe gracza

	int getHandSize()const;	//zwraca wielkosc reki gracza

	int getPoints()const;	//zwraca ilosc punktow gracza

	void addDomino(const Domino& kostka);	//dodaje domino do reki gracza

	void updatePoints(int newPoints);	//aktualizuje ilosc punktow gracza o podana w argumencie

	Domino selectDomino(int index);	//pozwala wykonac ruch kafelkiem o podanym indeksie

	Domino showDomino(int index);	//pozwala uzyskac dostep do kafelka o podanym indeksie

	void showHand()const;	//pokazuje wszystkie kafelki w rece gracza

	void setWinnerStatus(bool status);	//ustawia status gracza na podany w argumencie (do konczenia rozgrywki)

	bool getWinnerStatus() const;	//zwraca status gracza

	void clearHand();	//usuwa wszystkie kafelki z reki gracza w ramach przygotowania do kolejnej rundy
};

//Klasa odpowiedzialna za rozgrywke. Przechowuje informacje o graczach biarocych w niej udzial, kafelkach na stole,
//stanie rozgrywki, pozwala korzystac z talii i wykonywac ruchy graczy.
class Game
{
protected:
	vector<Player> players;	//tabela graczy
	DominoDeck newdeck;	//talia domina
	vector<Domino> stock;	//"waz" domina ukladany na stole
	int turns;	//tury, na ich podstawie jest pobierana wartosc ostatniego kafelka z "weza"
	bool isRoundFinished;	//status konca rundy
	bool isGameFinished;	//status konca gry
	int rounds;	//wskaznik aktualnej rundy

public:

	Game();	//konstruktor domyslny

	void addPlayer();	//dodaje gracza do tabeli

	void addComputer();	//dodaje bota do tabeli (wykonuje ruchy samodzielnie)

	virtual void setPlayers() = 0;	//pobiera graczy/bota i wklada ich do tabeli

	void dealDominos(int dominos);	//rozdaje podana ilosc kafelkow dla wszystkich graczy znajdujacych sie w tabeli

	void writePlayers();	//wypisuje gracza oraz jego reke

	void writeStock();	//wypisuje kombinacje w jakiej aktualnie znajduje sie na stole "waz"

	bool isTurnAvailable(Player& player, int l, int r);	//sprawdza czy gracz ma w rece chociaz jeden kafelek umozliwiajacy ruch biorac pod uwage aktualna kombinacje "weza"

	int pickSide(); // jesli gracz ma mozliwosc polozyc kafelek z obu stron weza, pozwala mu zdecydowac z ktorej chce to zrobic

	int getRounds() const;	//zwraca aktualna runde

	int addPoints(int l, int p);	//jesli spelnione sa warunki (suma podzielna przez 5), zwraca ilosc punktow z dwoch koncow weza

	bool checkEOD(Player& player);	//sprawdza czy w talii nadal sa kafelki (czy nie znajduje sie tam kontrolna kostka [7|7])

	void SumPointsAtTheEndOfTheTurn(vector <Player>& pl);	//jesli runda zakonczy sie przez oproznienie reki, sumuje wartosci pol z kostek przeciwnika i dodaje zwyciezcy punkty rowne tej wartosci

	void playerPoints(Player& player);	//wypisuje aktualna ilosc punktow gracza

	void checkGameStatus(vector<Player>& pl);	//sprawdza w tabeli czy ktorys z graczy spelnil warunek zwyciestwa 

	void checkRoundStatus(Player& player);	//sprawdza czy dany gracz spelnil warunki zakonczenia rundy (pusta reka, wyczerpana talia)

	int checkIndex(Player& player);	//sprawdza czy domino o podanym przez gracza indeksie istnieje i moze byc wykorzystane, jesli tak to zwraca ten indeks

	void PlayersTurn(Player& player);	//przeprowadza szereg operacji pozwalajacych wykonac ruch graczowi

	void ComputerTurn(Player& player);	//automatycznie wykonuje ruch za komputer

	void setup();	//przygotowuje do rozpoczecia rundy (czysci rece graczy, ustawia status rundy, rozdaje kafelki, czysci stol)

	void SaveToFile(Player& player);	//zapisuje zwyciezce oraz ilosc jego punktow do pliku "dane.txt"

	virtual void StartGame() = 0;	//Przeprowadza rozgrywke na podstawie wybranej opcji z druim graczem lub botem
};

//Klasa odpowiedzialna za przeprowadzenie rozgdywki gracza z komputerem.
class GameVsComputer : public Game
{
public:

	GameVsComputer();	//konstruktor domyslny

	void setPlayers() override;	//pozwala wpisac nazwe po czym dodaje do tabeli podanego gracza i bota

	void StartGame() override;	//przeprowadza rozgrywke w trybie z botem
};

//Klasa odpowiedzialna za przeprowadzanie rozgrywki dla dwoch graczy. 
class GameVsPlayer : public Game
{
public:
	GameVsPlayer();	//konstruktor domyslny

	void setPlayers() override;	//pobiera nazwe dwoch graczy i wpisuje ich do tabeli

	void StartGame() override;	//przeprowadza rozgrywke w trybie dla dwoch graczy
};

//
class ComputerVsComputer : public Game
{
public:

	ComputerVsComputer();	//konstruktor domyslny

	void setPlayers() override;	//pozwala wpisac nazwe po czym dodaje do tabeli podanego gracza i bota

	void StartGame() override;	//przeprowadza rozgrywke w trybie z botem
};