#ifndef ENTITY_H
#define ENTITY_H

#include<allegro5/allegro.h>

class Entity
{
protected:
	ALLEGRO_BITMAP* Sprite;
public:
	unsigned short Width, Height;
	float X, Y;

	void SetSprite(ALLEGRO_BITMAP* sprite);

	virtual void Draw(short offset_x, short offset_y);
};

#endif