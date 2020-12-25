void Menu::initialize()
{
	gameDelay = 10;

	controller.size = 1;

	currentGame = gmMenu;

	elpased = millis();
}

void Menu::update()
{
	switch (currentGame)
	{
	case gmMenu:
		updateMenu();
		break;
	case gmPong:
		pong.update();
		break;
	case gmTedshow:
		tedShow.update();
		break;
	case gmSpaceInvaders:
		spaceInvaders.update();
		break;
	case gmSnake:
		snake.update();
		break;
	case gmBreakOut:
		breakout.update();
		break;
	case gmRacing:
		racing.update();
		break;
	}
}

void Menu::updateMenu()
{
  controller.size=1;
int game=controller.position.x;
 //   delay(175);
  switch (game)
  {
  case 0:
    display.drawSprite(pong_ico);
    break;
  case 1:
    display.drawSprite(ted_ico);
    break;
  case 2:
    display.drawSprite(invaders_ico);
    break;
  case 3:
    display.drawSprite(snake_ico);
    break;
  case 4:
    display.drawSprite(breakout_ico);
    break;
  case 5:
    display.drawSprite(racing_ico);
    break;
  case 6:
    display.drawSprite(tetris_ico);
    break;
  case 7:
    display.drawSprite(jump_ico);
    break;
  }
  
	if (controller.ButtonPressed)
	{
		startGame(game);
	}
}

void Menu::startGame(int game)
{
score=0;
  switch (game)
  {
  case 0:
    currentGame = gmPong;
    display.newGame();
    pong.play();
    break;
  case 1:
    currentGame = gmTedshow;    
      display.newGame();  
    tedShow.play();
    break;
  case 2:
    currentGame = gmSpaceInvaders;
      display.newGame();
    spaceInvaders.play();
    break;
  case 3:
    currentGame = gmSnake;
      display.newGame();
    snake.play();
    break;
  case 4:
    currentGame = gmBreakOut;
    display.newGame(); 
    breakout.play();
    break;
  case 5:
    currentGame = gmRacing;
    display.newGame(); 
    racing.play();
    break;
  case 6:
setupTetris();
loopTetris(); 
    break;
  case 7:
//void loop() {
//    playTone( 300, 8000);
  clearMatrix();
  setupInitialVariables();
  renderMatrix();
     waitForSwitch();
    delay(75);
     doodlerAlive = true;
  while ( doodlerAlive ) {
    updateXPosition();
    updateYPosition();
    renderMatrix();
    checkForDeath();
    delay(75);
  }
//  } else {
//    waitForSwitch();
//  }
//  
//  delay(75);
//}
    
    break;
  }

}
