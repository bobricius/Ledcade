void Racing::play()
{
	trackDelay = 200;

	distanceCovered = 0;

	trackLength = sizeof(track) / sizeof(byte);

	position = 4;

	laps = 0;

	isCarOn = true;

}

void Racing::update()
{
	if (millis() - flashTimeElapsed > 100)
	{
		flashTimeElapsed = millis();

		isCarOn  = !isCarOn;
	}

  controller.size=3;
int position=1+controller.position.x;

	trackDelayElapsed += gameDelay;;

	if(trackDelayElapsed > trackDelay)
	{
		distanceCovered--;
    score++;
		if (distanceCovered < 0)
		{
			distanceCovered = trackLength;

			laps++;

			if (laps > 1)
			{
				display.flashSprite(up, 2, 250);
			}

			trackDelay -= 10;
		}

		trackDelayElapsed = 0;
	}

	for (int i = 0; i < 8; i++)
	{
		int lOffset = (i + distanceCovered) % trackLength;

		byte lValue = pgm_read_byte(track+lOffset); // track[lOffset]; //pgm_read_byte(sprite+b);

		if (i > 5 && isCarOn)
		{
			if ((lValue & (1 << position)) > 0) //8 - 
			{
				gameOver();
			}

			lValue = lValue + (1 << position); //8 - 
		}                

		display.drawRow(i, lValue);
	}

}

void Racing::gameOver()
{
explode(900);
  score=score/50;
	display.flashSprite(lost, 4, 500);
	menu.initialize();
}
