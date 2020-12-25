
void Pong::play()
{
	gameDelay = 200;

	controller.size = ControllerSize; //ControllerSize

	controller.position.y = 7;

	level = 0;

	// Bumpers will bounce the ball. 
	point bumperLeft;
	bumperLeft.x = 2;
	bumperLeft.y = 3;

	bumpers[0] = bumperLeft;

	point bumperRight;
	bumperRight.x = 5;
	bumperRight.y = 3;

	bumpers[1] = bumperRight;

	// Bumpers will be activated in level 3
	nofBumpers = 0;

	ball.initialise();  
}

void Pong::update()
{
	for(int i = 0; i < nofBumpers; i++)
	{
		updateBumpers(bumpers[i], &ball);
	}

	switch(ball.move(&controller))
	{
	case bsBounce:
		levelUp();
		break;
	case bsOut:
		gameOver();
		break;
	}

	display.clear();
	display.draw(&pong.ball);
	display.draw(&controller);

	for(int i = 0; i < nofBumpers; i++)
	{
		display.drawPixel(bumpers[i].x, bumpers[i].y);
	}
}

void Pong::gameOver()
{ 
	display.flashSprite(lost, 4, 500); 

	menu.initialize();  
}

void Pong::levelUp()
{
	level++;

	if (level % 10 == 0)
	{
		display.flashSprite(up, 3, 500); 

		// A little bit faster now
		gameDelay -= 30;

		// Time to use smaller paddle
		if (gameDelay < 120)
		{
			controller.size--;

			if (controller.size == 0)
			{
				menu.initialize();
			}

			gameDelay = 200;
		}

		// At some point, add in the bumpers
		nofBumpers = gameDelay < 150 ? 2 : 0;

		ball.initialise();
	}
}

void Pong::updateBumpers(point bumper, Ball *ball)
{
	if (ball->position.x == bumper.x && ball->position.y == bumper.y)
	{
		int newX = -1 * ball->direction.x;

		if (ball->direction.x == 0 && ball->direction.y == 1)
		{
			newX = random(2) == 0 ? -1 : 1;
		}

		ball->direction.x = newX;
		ball->direction.y *= -1;
	}
}
