#ifndef MAP_H
#define MAP_H

#include<allegro5/allegro.h>

void map_write(short map[64][64], short size, const char* filename);

void map_load(short map[64][64], const char* filename);

void map_generate_overworld(short size, const char* filename);

void map_draw(short tile_size, short map[64][64], short map_size, ALLEGRO_BITMAP* tileset, short tilemap_width, short location_offset_x, short location_offset_y);

#endif