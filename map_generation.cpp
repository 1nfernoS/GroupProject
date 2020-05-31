#include<iostream>
#include<fstream>
#include<stdlib.h>
#include<time.h>

void write_map(short map[64][64], short size, const char* filename)
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
				if (j < 63)
				{
					mapfile << ' ';
				}
			}
			if (i < 63)
			{
				mapfile << "\n";
			}
		}
		mapfile.close();
		std::cout << "Successfully created map file " << filename << "\n";
	}
}

void generate_map_overworld(short size)
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
	write_map(map, size, "Game/world.map"); // Write map to the file
}