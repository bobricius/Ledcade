class Controller
{
  private:
    bool previousRightButtonPressed;
	bool previousLeftButtonPressed;
  bool previousButtonPressed;
  public:
    point position;
    bool rightButtonPressed;
	bool leftButtonPressed;
  bool ButtonPressed;
    int size;
    Controller();
    void update();
    bool rightButtonWasPressed;
	bool leftButtonWasPressed;
  bool ButtonWasPressed;
};
