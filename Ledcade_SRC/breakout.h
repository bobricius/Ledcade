class BreakOut
{
  private:
	int level = 1;
	LinkedList<point*> bricks;
    Ball ball;
	void spawnBrick(int x, int y);
	void setupLevel(int lvl);
	void updateBricks(Ball *ball);
	void clearBricks();
    void gameOver();
    void levelUp();
  public:
    void update();
    void play();
};
