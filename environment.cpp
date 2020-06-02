#include<iostream>
#include<fstream>
#include<sstream>
#include<allegro5/allegro_native_dialog.h>
#include "common.h"
#include "environment.h"

Environment::Environment()
{
	Id = 0;
	Walkable = true;
}

Environment::Environment(short id, float x, float y, ALLEGRO_BITMAP* sprite, bool walkable)
{
	Id = id;
	X = x;
	Y = y;
	Sprite = sprite;
	Width = al_get_bitmap_width(Sprite);
	Height = al_get_bitmap_height(Sprite);
	Walkable = walkable;
}

Environment::~Environment()
{
	std::cout << "Environment destructed\n";
}

void Environment::Draw(short offset_x, short offset_y)
{
	al_draw_bitmap(Sprite, X + offset_x, Y + offset_y, NULL);
}

void environment_write(Environment props[1], short num_props, const char* filename)
{
	std::ofstream propsfile;
	propsfile.open(filename);
	if (propsfile.is_open())
	{
		propsfile << num_props << "\n";
		for (short i = 0; i < num_props; i++)
		{
			propsfile << props[i].Id << ' ' << props[i].X << ' ' << props[i].Y << ' ' << props[i].Walkable;
			if (i < num_props - 1)
			{
				propsfile << "\n";
			}
		}
		propsfile.close();
		std::cout << "Created \"" << filename << "\"\n";
	}
}

void environment_load(Environment props[1], ALLEGRO_BITMAP* sprites[8], const char* filename)
{
	short num_props;
	std::ifstream propsfile(filename);
	if (propsfile.is_open())
	{
		propsfile >> num_props;
		for (short i = 0; i < num_props; i++)
		{
			propsfile >> props[i].Id;
			props[i].SetSprite(sprites[props[i].Id]);
			propsfile >> props[i].X;
			propsfile >> props[i].Y;
			propsfile >> props[i].Walkable;
		}
		std::cout << "Loaded \"" << filename << "\"\n";
	}
	else
	{
		std::stringstream sstream;
		sstream << "Failed to open props file " << filename;
		al_show_native_message_box(NULL, "Error", sstream.str().c_str(), NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		sstream.str(std::string());
	}
}

void environment_generate_overworld_props(short map[64][64], short map_size, Environment props[1], ALLEGRO_BITMAP* sprites[8], const char* filename)
{
	float env_x, env_y;
	char num_props = 0;
	bool campfire_placed = false;
	for (short i = 0; i < map_size; i++)
	{
		for (short j = 0; j < map_size; j++)
		{
			if (map[i][j] == 2)
			{
				env_x = i * 32;
				env_y = j * 32;
				std::cout << env_x << " | " << env_y << "\n";
				Environment environment_element(0, env_x, env_x, sprites[0], true);
				props[num_props] = environment_element;
				campfire_placed = true;
			}
			if (campfire_placed)
			{
				break;
			}
		}
		if (campfire_placed)
		{
			break;
		}
	}
	environment_write(props, 1, filename);
}

void environment_draw(Environment props[1], short location_offset_x, short location_offset_y, float player_y, float player_height, bool behind_player)
{
	for (short i = 0; i < 1; i++)
	{
		if (behind_player)
		{
			if (props[i].Y + location_offset_y + props[i].Height < player_y + player_height || props[i].Walkable)
			{
				props[i].Draw(location_offset_x, location_offset_y);
			}
		}
		else
		{
			if (props[i].Y + location_offset_y + props[i].Height > player_y + player_height && !props[i].Walkable)
			{
				props[i].Draw(location_offset_x, location_offset_y);
			}
		}
	}
}