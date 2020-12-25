Controller::Controller()
{
  size = 3;
  position.x = 1;
  position.y = 7;
  
  pinMode(ControllerRightButtonPin, INPUT_PULLUP);
  pinMode(ControllerLeftButtonPin, INPUT_PULLUP);
   pinMode(ControllerPin, INPUT_PULLUP);
  pinMode(3, OUTPUT);

  pinMode(A4,INPUT_PULLUP); // white led lamp
  pinMode(A5,INPUT_PULLUP); // white led lamp
}

void Controller::update()
{
int x=0;

//  position.x = map(analogRead(ControllerPotPin) / 10, 0, 102, 8 - size, 0);
x=position.x;

  if ((digitalRead(A5)==0)&&((millis() - leftTick) >= 200)){

   x++;
    if (x > 8 - size) {
      x = 8 - size;
    }
    
//delay(100); 
//  playTone( 1, 10000);
    lastSleep = millis();
    leftTick = millis();
  }
 
 if((digitalRead(A4)==0)&&((millis() - rightTick) >= 200)){

    x--;
    if (x < 1) {
      x = 0;
    }


//delay(100);
//  playTone( 1, 10000);
    lastSleep = millis();
    rightTick = millis();
  }
position.x=x;

  rightButtonPressed = !digitalRead(ControllerRightButtonPin);
  leftButtonPressed = !digitalRead(ControllerLeftButtonPin);
  ButtonPressed = !digitalRead(ControllerPin);
    
  rightButtonWasPressed = (previousRightButtonPressed == true && rightButtonPressed == false);
  leftButtonWasPressed = (previousLeftButtonPressed == true && leftButtonPressed == false);
  ButtonWasPressed = (previousButtonPressed == true && ButtonPressed == false);
  
//  digitalWrite(13, rightButtonPressed);
  
  previousRightButtonPressed = rightButtonPressed;
  previousLeftButtonPressed = leftButtonPressed;
  previousButtonPressed = ButtonPressed;
}
