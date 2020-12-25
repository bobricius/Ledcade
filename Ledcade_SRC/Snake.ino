
void Snake::play()
{
	snakeStepElapsed = millis();
	snakeStepDelay = 750;

	// The segments that make up the snake
	segments = LinkedList<point*>();
	segments.add(spawnSegment(4,4));
	segments.add(spawnSegment(5,4));
	segments.add(spawnSegment(6,4));
	segments.add(spawnSegment(7,4));

	direction.x = -1;
	direction.y = 0;


	// set the bait
	setApple();

	addSegment = false;

	// depart in a straight line
	steer = stCentre;
}

void Snake::update()
{
	// Read controller buttons
	if (controller.leftButtonPressed)
	{
		steer = stLeft;
	}

	if (controller.rightButtonPressed)
	{
		steer = stRight;
	}

	detectCollisions();

	// when it's time to step
	if (millis() - snakeStepElapsed > snakeStepDelay)
	{
		snakeStepElapsed = millis();

		move();

		// Reset steering
		steer = stCentre;

		draw();		
	}	
}

void Snake::detectCollisions()
{
	point* head = segments.get(0);

	// check display bounds
	if (head->x < 0 || head->x > 7 || head->y < 0 || head->y > 7)
	{
		gameOver();
	}

	// check biting itself
	for (int i = 1; i < segments.size(); i++)
	{
		point* segment = segments.get(i);

		if (segment->x == head->x && segment->y == head->y)
		{
			gameOver();
		}
	}

	// check apple
	if (head->x == apple.x && head->y == apple.y)
	{
		addSegment = true;
    playTone( 35, 2000);
		// A little bit faster now
		snakeStepDelay -= 15;

		// spawn new apple
		setApple();
	}

	// Would be great to put some walls up at some point in the game...
}

void Snake::draw()
{
	display.clear();

	// draw snake
	for (int i = 0; i < segments.size(); i++)
	{
		point *segment = segments.get(i);

		display.drawPixel(segment->x, segment->y);
	}

	// draw apple
	display.drawPixel(apple.x, apple.y);
}

void Snake::move()
{
	// set new direction if left or right button was pressed
	int dX = direction.x;
	int dY = direction.y;

	if (steer == stLeft)
	{
		direction.x = dY;
		direction.y = -dX;
	}

	if (steer == stRight)
	{
		direction.x = -dY;
		direction.y = dX;
	}


	// keep last segment in memory if apple has been eaten
	int lastX;
	int lastY;

	if (addSegment)
	{
		point* last = segments.get(segments.size() -1);
		lastX = last->x;
		lastY = last->y;

	}

	// Move each segment one step forward
	for (int i = segments.size() - 1; i > 0; i--)
	{
		point *old = segments.get(i);
		free(old);	

		point *next = segments.get(i-1);


		point *segment = spawnSegment(next->x, next->y);
    
		segments.set(i, segment);
	}

	// If apple was eaten append retained segment
	if (addSegment)
	{
		segments.add(spawnSegment(lastX, lastY));
    score++;
    playTone( 100, 3000);
		addSegment = false;
	}

	// move head segment according to current direction
	point *head = segments.get(0);

	segments.set(0, spawnSegment(head->x + direction.x, head->y + direction.y));
}

point* Snake::spawnSegment(int x, int y)
{
	point *position;
	position = (struct point*)malloc(sizeof(struct point));
	position->x = x;
	position->y = y;

	return position;
}

void Snake::setApple()
{
	int x;
	int y;

	bool ok = true;

	// Find an unoccupied spot 
	do
	{
		x = random(0, 8);
		y = random(0, 8);

		ok = true;

		for (int i = 0; i < segments.size()-1; i++)
		{
			point* segment = segments.get(i);

			if (segment->x == x && segment->y == y)
			{
				ok = false;
				break;
			}
		}

	} while (!ok);

	apple.x = x;
	apple.y = y;
}

void Snake::gameOver()
{
	// Clean up after ourselves
	segments.~LinkedList();
  segments.clear();
//segments::~segments();
//~LinkedList<point*>();
  explode(100);
	// game over animation
	display.flashSprite(lost, 4, 500);
asm volatile ("  jmp 0");  
	// And back to menu
	menu.initialize();
}
