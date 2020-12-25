typedef struct point
{
  int x;
  int y;
};

enum games { gmMenu = 0, gmPong = 1, gmTedshow = 2, gmSpaceInvaders = 3, gmSnake = 4, gmBreakOut = 5, gmRacing = 6};

enum steering { stLeft, stCentre, stRight };

enum tedShowState { tsLights, tsFalling };

enum ballState { bsBounce, bsOut, bsIn };

enum hitKind { hkTop, hkSide, hkNone };
