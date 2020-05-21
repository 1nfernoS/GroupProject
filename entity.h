#ifndef ENTITY_H
#define ENTITY_H

#include<allegro5/allegro.h>

class Entity
{
protected:
	ALLEGRO_BITMAP *Sprite;
public:
	int Width, Height;
	float X, Y;

	virtual void Draw();
};

#endif