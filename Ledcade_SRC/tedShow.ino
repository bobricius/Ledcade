
void TedShow::play()
{
	// Set the catch thingy
	controller.size = ControllerSize;
	controller.position.y = 7;

	gameSpeedDelay = 200;

	newGame();
}

void TedShow::newGame()
{
	// Start with blinking lights
	state = tsLights;

	gameDelay = 200;
	
	stepDelay = random(250, 1000);

	// Random stick x position 
	stick.x = random(1, 7);
	
	stick.y = 0;
}

void TedShow::update()
{
	switch (state)
	{
	case tsLights:
		updateLights();
		break;
	case tsFalling:
		updateStick();
		break;
	}

	draw();
}

void TedShow::updateLights()
{
	// We wait, wait, wait until the stick falls
	if (millis() - elapsed > stepDelay)
	{
		elapsed = millis();

		gameDelay -= 25;
		stepDelay = random(250, 450 + gameDelay);

		// And then it is falling and the player needs to catch it
		if (gameDelay < 15)
		{
			gameDelay = gameSpeedDelay;
			state = tsFalling;
		}
	}
}

void TedShow::updateStick()
{
	// Stick falling
	stick.y += 1;

	// game over if not caught
	if (stick.y > 7)
	{
		delay(500);

		gameOver();
	}

	// Cheers, praise and levelup when caught
	if (stick.y == 7 && stick.x == controller.position.x + 1)
	{
		delay(500);

		levelUp();
	}
}

// Draw stuff...
void TedShow::draw()
{
	display.clear();

	drawCatcher();

	switch (state)
	{
	case tsLights:
// drawLights();
		break;
	case tsFalling:
		drawStick();
		break;
	}
}

void TedShow::drawCatcher()
{
	display.draw(&controller);

	point leftEdge;
	leftEdge.x = controller.position.x;
	leftEdge.y = controller.position.y - 1;
	display.drawObject(1, leftEdge);

	point rightEdge;
	rightEdge.x = controller.position.x + controller.size - 1;
	rightEdge.y = controller.position.y - 1;
	display.drawObject(1, rightEdge);
}

void TedShow::drawLights()
{
	// Lights switch on and off to build up tension
	toggle = !toggle;

	if (toggle)
	{
		display.drawRow(0, lights[0]);
		display.drawRow(1, lights[1]);
	}
	else
	{
		display.drawRow(0, lights[1]);
		display.drawRow(1, lights[0]);
	}
}

void TedShow::drawStick()
{

		display.drawPixel(stick.x, stick.y);
}

void TedShow::gameOver()
{
	display.flashSprite(lost, 4, 500);

	menu.initialize();
}

void TedShow::levelUp()
{
//	display.flashSprite(up, 3, 50);
  playTone( 100, 6000);
  playTone( 100, 7000);
  playTone( 100, 8000);  
  score++;
	// A little bit faster now
	gameSpeedDelay -= 3;

	// The game is to finish the game...
	if (gameSpeedDelay < 20)
	{
		menu.initialize();
	}

	newGame();
}
