
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

typedef enum
{
    UP = 0,
    UP_RIGHT = 1,
    RIGHT = 2,
    DOWN_RIGHT = 3,
    DOWN = 4,
    DOWN_LEFT = 5,
    LEFT = 6,
    UP_LEFT = 7,
} View_Orientation;

typedef enum
{
	IDLE=0,
	WALK=1,
	FIRE=2,
	DEATH=3
} Player_State;
#endif // SPRITE_H_INCLUDED
