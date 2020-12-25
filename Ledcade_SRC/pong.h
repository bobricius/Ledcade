
class Pong
{
  private:
    int level;
    Ball ball;
    int nofBumpers;
    point bumpers[2];
    void gameOver();
    void levelUp();
    void updateBumpers(point bumper, Ball *ball);
  public:
    void update();
    void play();
};
