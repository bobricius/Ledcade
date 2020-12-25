
void SpaceInvaders::play()
{
	// ship size
	controller.size = 3;

	// ship position
	controller.position.y = 7;

	// set the game speed
	gameDelay = 50;

	invaderXDirection = 1;
	invaderBounced = 0;

	// list containing the invaders
	enemies = LinkedList<point*>();

	// populate the levels invaders
	setupLevel(level);

	invaderStep = millis();
}

void SpaceInvaders::update()
{
	// firing
	if (controller.ButtonPressed ) //|| controller.leftButtonPressed
	{
 //   playJumpTone = true;
		fire();
	}

	// when it's time to move the invaders
	if (millis() - invaderStep > invaderStepDelay)
	{
		invaderStep = millis();

		moveInvaders();
	}

	// updating fired shot
	if (myBullet)
	{
		if (millis() - myBulletStep > myBulletStepDelay)
		{
 playTone( 1, 9000);

			myBulletStep = millis();

			myBullet->y--;

			// bullet out of the playing field
			if (myBullet->y < 0)
			{
				free(myBullet);

				myBullet = NULL;
			}
		}
	}

	// updating invader bullet (there's only one at the time...)
	if(invaderBullet)
	{
		if (millis() - invaderBulletStep > enemyBulletStepDelay)
		{

			invaderBulletStep = millis();

			invaderBullet->y++;

			if (invaderBullet->y > 7)
			{
				free(invaderBullet);

				invaderBullet = NULL;
			}
		}
	}
	else
	{
		// When there's no bullet let there be one...
		invaderShoot();
	}

	detectCollisions();

	draw();
}

void SpaceInvaders::moveInvaders()
{
	// Invaders move in formation, from left to right and vice versa
	// When bounced off the screen sides 6 times they step forward
	// and repeat. Here we set the x and y directions
	for (int i = 0; i < enemies.size(); i++)
	{
		point *enemy = enemies.get(i);

		if (enemy->x == 0 || enemy->x == 7)
		{
			invaderBounced++;

			if (invaderBounced % 6 == 0)
			{
				invaderYDirection = 1;

				break;
			}

			invaderXDirection *= -1;

			break;
		}
	}

	// Move each invader in the determined direction
	for (int i = 0; i < enemies.size(); i++)
	{
		point *enemy = enemies.get(i);

		stepInvader(enemy);
	}

	invaderYDirection = 0;
}

void SpaceInvaders::stepInvader(point *position)
{
	if (invaderYDirection == 0)
	{
		position->x += invaderXDirection;
	}

	position->y += invaderYDirection;
}


void SpaceInvaders::draw()
{
	display.clear();

	drawShip();

	for (int i = 0; i < enemies.size(); i++)
	{
		point *enemy = enemies.get(i);

		display.drawPixel(enemy->x, enemy->y);
	}

	drawBullets();
}

void SpaceInvaders::drawShip()
{
	// Ship is made up of a three point bar and
	// a single point as the turrot on top
	display.draw(&controller);
	display.drawPixel(controller.position.x + 1, controller.position.y - 1);
}

void SpaceInvaders::drawBullets()
{
	if (myBullet)
	{
		display.drawPixel(myBullet->x, myBullet->y);
	}

	if (invaderBullet)
	{
		display.drawPixel(invaderBullet->x, invaderBullet->y);
	}
}

void SpaceInvaders::setupLevel(int lvl)
{
	// first level, two rows of invaders
	spawnInvader(0, 1);
	spawnInvader(0, 3);
	spawnInvader(0, 5);
	spawnInvader(1, 2);
	spawnInvader(1, 4);
	spawnInvader(1, 6);

	invaderStepDelay = 750;

	// second level, add a third row
	if (lvl > 1)
	{
		spawnInvader(2, 1);
		spawnInvader(2, 3);
		spawnInvader(2, 5);

		invaderStepDelay = 625;
	}

	// third level, add even another row
	if (lvl > 2)
	{
		spawnInvader(3, 2);
		spawnInvader(3, 4);
		spawnInvader(3, 6);

		invaderStepDelay = 500;
	}	

	// here might be a boss level...
}

void SpaceInvaders::spawnInvader(int y, int x)
{
	point *position;
	position = (struct point*)malloc(sizeof(struct point));
	position->x = x;
	position->y = y;

	enemies.add(position);
}

void SpaceInvaders::detectCollisions()
{
	for (int i = 0; i < enemies.size(); i++)
	{
		point *enemyPosition = enemies.get(i);

		detectShipCollision(enemyPosition);		
	}

	detectShipCollision(invaderBullet);

	detectEnemyShot(myBullet);

	detectBulletsCollision();
}

void SpaceInvaders::detectBulletsCollision()
{
	if (myBullet->x == invaderBullet->x && myBullet->y == invaderBullet->y)
	{
		free(invaderBullet);
		invaderBullet = NULL;

		free(myBullet);
		myBullet = NULL;
	}
}

void SpaceInvaders::detectEnemyShot(point *position)
{
	// If an invaders gets shot the invader vanishes and the bullet is freed
	// so the ship may fire again.
	if (myBullet)
	{
		for (int i = 0; i < enemies.size(); i++)
		{
			point *enemy = enemies.get(i);

			if (myBullet->x == enemy->x && myBullet->y == enemy->y)
			{
				enemies.remove(i);
        score++;
          explode(50);
				free(myBullet);
				myBullet = NULL;

				if (enemies.size() == 0)
				{
					levelUp();
				}
			}
		}
	}
}

void SpaceInvaders::detectShipCollision(point *position)
{
	if (position->x == controller.position.x + 1 && position->y == controller.position.y - 1)
	{
		gameOver();
	}

	if (position->y == controller.position.y &&
		(position->x >= controller.position.x &&
		position->x <= controller.position.x + controller.size - 1))
	{
		gameOver();
	}
}

void SpaceInvaders::gameOver()
{
explode(300);
     
   talkieSay(spGAMES);
   talkieSay(spEND);
  
//  enemies.clear();
  resetGame();
  display.flashSprite(lost, 4, 500);   	
  
	level = 1;
  asm volatile ("  jmp 0");  
	menu.initialize();

}

void SpaceInvaders::fire()
{
	if (!myBullet)
	{
		myBullet = (struct point*)malloc(sizeof(struct point));
		myBullet->x = controller.position.x + 1;
		myBullet->y = controller.position.y - 1;
		myBulletStep = millis();
        playJumpTone = true;
	}
}

void SpaceInvaders::invaderShoot()
{
	// Get random enemy
	int invaderIndex = random(enemies.size());

	// Find lowest enemy in column to avoid shooting teamies in the back...
	point *lowestEnemy = enemies.get(invaderIndex);

	for (int i = 0; i < enemies.size(); i++)
	{
		point *tempEnemy = enemies.get(i);

		if (tempEnemy->x == lowestEnemy->x && tempEnemy->y > lowestEnemy->y)
		{
			lowestEnemy = tempEnemy;
		}
	}

	// Fire
   playTone( 10, 9000);
	invaderBullet = (struct point*)malloc(sizeof(struct point));
	invaderBullet->x = lowestEnemy->x;
	invaderBullet->y = lowestEnemy->y;

	invaderBulletStep = millis();
}

void SpaceInvaders::levelUp()
{
   talkieSay(spYOU_WIN);
	resetGame();

	level++;
//  enemies.clear();
	display.flashSprite(up, 3, 500); 
	play();	

}

void SpaceInvaders::resetGame()
{
	// clean up after ourselves

	free(myBullet);
	myBullet = NULL;

	free(invaderBullet);
	invaderBullet = NULL;

	for (int i; i < enemies.size(); i++)
	{
	   point *enemy = enemies.get(i);

	   free(enemy);
	   enemy = NULL;
	}
   enemies.clear();
   enemies.~LinkedList();
}
