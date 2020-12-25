class Menu
{
private:
	const int height = 8;
	games selectedGame;
	long elpased;
	games currentGame = gmMenu;
	void updateMenu();
public:
	void initialize();
	void update();
	bool between(int val, int min, int max);
	void startGame(int offset);
};
