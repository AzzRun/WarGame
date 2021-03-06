#ifndef FT_ITEM_H
#define FT_ITEM_H
#include <SDL.h>

typedef enum
{
	BLANK = 0,
	WALL = 1,
	PLAYER = 2,
} ItemType;

typedef struct Item {
	ItemType type;
	void *data;
	SDL_Rect *rect;
	struct Item *next;
}Item;

uint32_t countItemWall(Item* head);
Item* pushItem(Item* head, Item *new_node);
int8_t checkWallColision(Item *head, int32_t direction);
bool browseItemWall(Item* head);
#endif // FT_ITEM_H
