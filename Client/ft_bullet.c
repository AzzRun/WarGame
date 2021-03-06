#include "include/ft_bullet.h"
SDL_Rect RECT = { .x = 0,.y = 0,.h = 6,.w = 6 };
void browserBullets(BulletElm* head, callback f)
{
	BulletElm* cursor = head;
	while (cursor != NULL)
	{
		f(cursor);
		cursor = cursor->next;
	}
}


void drawBullet(BulletElm *bullet) {

	bullet->pos.h = bullet->pos.w = 6;
	SDL_Rect obj = { .x = 0,.y = 0,.h = 6,.w = 6 };
	obj.x = bullet->pos.x - _engine.camera.x;
	obj.y = bullet->pos.y - _engine.camera.y;
	SDL_RenderCopy(_engine.screenRenderer, _engine.bulletSurface, &RECT, &obj);
}