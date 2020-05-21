#include "entity.h"

void Entity::Draw()
{
	al_draw_bitmap(Sprite,X, Y, NULL);
}