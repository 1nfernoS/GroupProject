#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "entity.h"

class Environment : public Entity
{
public:
	short Id;
	bool Walkable;

	Environment();
	Environment(short id, float x, float y, ALLEGRO_BITMAP* sprite, bool walkable);
	~Environment();

	void Draw(short offset_x, short offset_y);
};

void environment_write(Environment props[2], short num_props, const char* filename);

void environment_load(Environment props[2], ALLEGRO_BITMAP* sprites[8], const char* filename);

void environment_generate_overworld_props(short map[64][64], short map_size, Environment props[2], ALLEGRO_BITMAP* props_sprites[8], const char* filename);

void environment_draw(Environment props[2], short num_props, short location_offset_x, short location_offset_y, float player_y, float player_height, bool behind_player);

#endif