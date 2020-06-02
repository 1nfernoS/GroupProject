#include<iostream>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include<time.h>
#include<allegro5/allegro_native_dialog.h>
#include "common.h"

void map_write(short map[64][64], short size, const char* filename)
{
	std::ofstream mapfile;
	mapfile.open(filename);
	if (mapfile.is_open())
	{
		for (short i = 0; i < size; i++)
		{
			for (short j = 0; j < size; j++)
			{
				mapfile << map[i][j];
				if (j < size - 1)
				{
					mapfile << ' ';
				}
			}
			if (i < size - 1)
			{
				mapfile << "\n";
			}
		}
		mapfile.close();
		std::cout << "Created \"" << filename << "\"\n";
	}
}

void map_load(short map[64][64], const char* filename)
{
	short map_width = 0, map_x = 0, map_y = 0;
	std::ifstream mapfile(filename);
	if (mapfile.is_open())
	{
		std::string line;
		getline(mapfile, line); // gets first line from the map file
		for (unsigned short i = 0; i < line.length(); i++)
		{
			if (line[i] == ' ') // gets map width in tiles
			{
				map_width++;
			}
		}
		map_width++;
		mapfile.seekg(0, std::ios::beg); // returns to the beginning of the file
		while (!mapfile.eof())
		{
			mapfile >> map[map_x][map_y];
			map_x++;

			if (map_x >= map_width)
			{
				map_x = 0;
				map_y++;
			}
		}
		std::cout << "Loaded \"" << filename << "\" " << map_width << "x" << map_y << "\n";
	}
	else
	{
		std::stringstream sstream;
		sstream << "Failed to open map file " << filename;
		al_show_native_message_box(NULL, "Error", sstream.str().c_str(), NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		sstream.str(std::string());
	}
}

void map_generate_overworld(short size, const char* filename)
{
	srand(time(NULL));
	short map[64][64] = { 0 };
	short x, y, width, height;

	// fill the map with grass
	for (short i = 0; i < size; i++) // rows
	{
		for (short j = 0; j < size; j++) //columns
		{
			map[i][j] = 1;
		}
	}
	// find and set spots without grass
	short num_dirt_tiles = rand() % 8 + 8;
	short dirt_tile_x, dirt_tile_y;
	for (short i = 0; i < num_dirt_tiles; i++)
	{
		dirt_tile_x = rand() % 64;
		dirt_tile_y = rand() % 64;
		if (map[dirt_tile_x - 1][dirt_tile_y] != 0 || map[dirt_tile_x + 1][dirt_tile_y] || map[dirt_tile_x][dirt_tile_y - 1] || map[dirt_tile_x][dirt_tile_y + 1])
		{
			map[dirt_tile_x][dirt_tile_y] = 3;
		}
	}
	// find upperleft corner coordinates, width and height of abandoned campsite
	do
	{
		width = rand() % 4 + 2;
		x = rand() % (size / 2) + (size / 4 - 1);
		height = rand() % 4 + 2;
		y = rand() % (size / 2) + (size / 4 - 1);
	} while ((x + width > 21 && x < 42) && (y + height > 18 && y < 45));
	//std::cout << "Campsite upperleft corner generated\n";
	std::cout << "X: " << x << " Y: " << y << "\n";
	std::cout << "Width: " << width << " Height: " << height << "\n";
	// set corners of campsite
	if (x + width < size && y + height < size)
	{
		map[x][y] = 10;
		map[x + width][y] = 12;
		map[x][y + height] = 11;
		map[x + width][y + height] = 13;
	}
	// set the rest of campsite
	for (short i = x; i < x + width + 1; i++) // rows
	{
		for (short j = y; j < y + height + 1; j++) // columns
		{
			if (i < 64 && j < 64)
			{
				if (i == x && (j > y && j < y + height))
				{
					map[i][j] = 15;
				}
				else if (i == x + width && (j > y && j < y + height))
				{
					map[i][j] = 17;
				}
				else if ((i > x && i < x + width) && j == y)
				{
					map[i][j] = 16;
				}
				else if ((i > x && i < x + width) && j == y + height)
				{
					map[i][j] = 14;
				}
				else if (map[i][j] == 1)
				{
					map[i][j] = 2;
				}
			}
		}
	}
	map_write(map, size, filename); // Write map to the file
}

void map_draw(short tile_size, short map[64][64], short map_size, ALLEGRO_BITMAP* tileset, short tilemap_width, short location_offset_x, short location_offset_y)
{
	short tile_id, tilemap_x = 0, tilemap_y = 0, tile_pos_x, tile_pos_y;
	float id_to_row_number_ratio;
	for (short i = 0; i < map_size; i++)
	{
		for (short j = 0; j < map_size; j++)
		{
			tile_id = map[i][j];
			if (tile_id == -1)
			{
				continue;
			}
			if (tile_id >= tilemap_width)
			{
				tilemap_x = tile_id - tilemap_width;
				id_to_row_number_ratio = tile_id / tilemap_width;
				tilemap_y = (short)id_to_row_number_ratio;
			}
			else
			{
				tilemap_x = tile_id;
				tilemap_y = 0;
			}
			tile_pos_x = i * tile_size - (map_size * tile_size - display_w) / 2 + location_offset_x;
			tile_pos_y = j * tile_size - (map_size * tile_size - display_h) / 2 + location_offset_y;
			if (tile_pos_x + tile_size > 0 && tile_pos_x < display_w && tile_pos_y + tile_size > 0 && tile_pos_y < display_h)
			{
				al_draw_bitmap_region(tileset, tilemap_x * tile_size, tilemap_y * tile_size, tile_size, tile_size, tile_pos_x, tile_pos_y, NULL);
			}
		}
	}
}