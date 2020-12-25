class Ball
{
  private:    
    int previousDirection;
    bool between(int val, int min, int max);
    hitKind isHitPaddle(Controller *controller);    
    int getNewDirection();
  public:
    point position;
    point direction;
	ballState move(Controller *controller);
    void initialise();
};
