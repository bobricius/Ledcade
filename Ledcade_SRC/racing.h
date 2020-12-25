class Racing
{
private:
	int distanceCovered;
	int trackLength;
	int position;
	int laps;
	long flashTimeElapsed;
	bool isCarOn;
	int trackDelay;
	int trackDelayElapsed;
    void gameOver();
public:
    void update();
    void play();
};
