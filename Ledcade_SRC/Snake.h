#pragma once
class Snake
{
private:
	LinkedList<point*> segments;
	point* spawnSegment(int x, int y);
	void setApple();
	point direction;
	point apple;
	bool addSegment;
	steering steer;
	long snakeStepElapsed;
	long snakeStepDelay;
	void gameOver();
	void draw();
	void detectCollisions();
public:
	void update();
	void play();
	void move();
};
