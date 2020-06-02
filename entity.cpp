#include "entity.h"

void Entity::SetSprite(ALLEGRO_BITMAP* sprite)
{
	Sprite = sprite;
}

void Entity::Draw(short offset_x, short offset_y)
{
	al_draw_bitmap(Sprite, X + offset_x, Y + offset_y, NULL);
}