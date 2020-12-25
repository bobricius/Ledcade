
void BreakOut::play()
{
	gameDelay = 200;

	controller.size = 3;

	controller.position.y = 7;

	level = 1;

	bricks = LinkedList<point*>();

	setupLevel(level);

	ball.initialise();  
}

void BreakOut::update()
{
	switch(ball.move(&controller))
	{
	case bsOut:
		gameOver();
		break;
	}

	display.clear();
	display.draw(&breakout.ball);
	display.draw(&controller);  

	updateBricks(&breakout.ball);

	for (int i = 0; i < bricks.size(); i++)
	{
		point *brick = bricks.get(i);

		display.drawPixel(brick->x, brick->y);
	}

}

void BreakOut::gameOver()
{ 
	display.flashSprite(lost, 4, 500); 
  bricks.clear();
	clearBricks();
asm volatile ("  jmp 0"); 
	menu.initialize();  
}

void BreakOut::levelUp()
{
	level++;

	display.flashSprite(up, 3, 500); 

	// A little bit faster now
	gameDelay -= 30;

	clearBricks();
bricks.clear();
	setupLevel(level);

	ball.initialise();


}

void BreakOut::spawnBrick(int y, int x)
{
	point *position;
	position = (struct point*)malloc(sizeof(struct point));
	position->x = x;
	position->y = y;

	bricks.add(position);
}

void BreakOut::setupLevel(int lvl)
{
	// first level, two rows of bricks
	for (int i = 0; i <= lvl; i++)
	{
		spawnBrick(i, 0);
		spawnBrick(i, 1);
		spawnBrick(i, 2);
		spawnBrick(i, 3);
		spawnBrick(i, 4);
		spawnBrick(i, 5);
		spawnBrick(i, 6);
		spawnBrick(i, 7);
	}
}

void BreakOut::updateBricks(Ball *ball)
{
	for (int i = 0; i < bricks.size(); i++)
	{
		point *brick = bricks.get(i);

		if (ball->position.x == brick->x && ball->position.y == brick->y)
		{
			bricks.remove(i);
      score++;
    playTone( 15, 8000);
			int newX = -1 * ball->direction.x;

			if (ball->direction.x == 0 && ball->direction.y == 1)
			{
				newX = random(2) == 0 ? -1 : 1;
			}

			ball->direction.x = newX;
			ball->direction.y *= -1;
		}
	}

	if (bricks.size() == 0)
	{
		levelUp();
	}
}

void BreakOut::clearBricks()
{
	for (int i; i < bricks.size(); i++)
	{
		point *brick = bricks.get(i);

		free(brick);
		brick = NULL;
	}
}
