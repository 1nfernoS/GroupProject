#ifndef MAP_GENERATION_H
#define MAP_GENERATION_H

void write_map(short map[64][64], short size, const char* filename);

void generate_map_overworld(short size);

#endif