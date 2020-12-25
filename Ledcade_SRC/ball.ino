
void Ball::initialise()
{
	position.x = random(0, 7);
	position.y = 0;

	direction.x = random(-1, 2);
	direction.y = 1;
}

ballState Ball::move(Controller *controller)
{
	// Move ball
	position.x += direction.x;
	position.y += direction.y;

	// Bounce against sides
	if (between(position.x, 0, 6) == false)
	{
		position.x = constrain(position.x, 1, 7);
		direction.x *= -1;
  playTone( 5, 2500);
	}

	// Bounce against top
	if (position.y < 0)
	{
		position.y = 1; 
		direction.y *= -1;
  playTone( 5, 1500);
	}

	// Bounce against paddle
	hitKind hit = isHitPaddle(controller);

	if (hit == hkTop || hit == hkSide)
	{
		position.y = 6;
		direction.y *= -1;

		if (hit == hkTop)
		{
			// Get random outward direction
			direction.x = getNewDirection();
  playTone( 5, 7000);
		}

		if (hit == hkSide)
		{
			// reverse x direction
			direction.x *= -1;
  playTone( 5, 2000);
		}

		return bsBounce;
		//pong.levelUp();
	}

	// Missed, game over
	if (position.y == 7)
	{
		//pong.gameOver();
		return bsOut;
	}

	return bsIn;
}

hitKind Ball::isHitPaddle(Controller *controller)
{
	// The ball hits the paddle when its y-position is 6 and it is either directly above the paddle or
	// in the next step will be at the same postition as any point of the paddle
	// ...    or \   /
	// XXX        XXX
	if (position.y == 6)
	{
      score++;
      playJumpTone = true;
		if (direction.y == 1)
		{
			if (between(position.x, controller->position.x, (controller->position.x + controller->size) - 1))
			{        
				return hkTop;
			}

			if (between(position.x + direction.x, controller->position.x, (controller->position.x + controller->size) - 1))
			{
				return hkSide;
			}
		}
	}

	return hkNone;
}

bool Ball::between(int val, int min, int max)
{
	return val >= min && val <= max;
}

int Ball::getNewDirection()
{
	// Get a somewhat random bouncing direction to
	// keep the game at least a little bit interesting
	int newDirection;

	do 
	{
		newDirection = random(-1, 2);
	} while (newDirection == previousDirection);

	previousDirection = newDirection;

	return newDirection;
}
