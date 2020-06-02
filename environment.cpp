#include<iostream>
#include<fstream>
#include<sstream>
#include<stdlib.h>
#include<time.h>
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

}

void Environment::Draw(short offset_x, short offset_y)
{
	al_draw_bitmap(Sprite, X + offset_x, Y + offset_y, NULL);
}

void environment_write(Environment props[2], short num_props, const char* filename)
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

void environment_load(Environment props[2], ALLEGRO_BITMAP* sprites[8], const char* filename)
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

void environment_generate_overworld_props(short map[64][64], short map_size, Environment props[2], ALLEGRO_BITMAP* sprites[8], const char* filename)
{
	srand(time(NULL));
	float env_x, env_y;
	char num_props = 0;
	bool campfire_placed = false, seat_placed = false;
	// camsite props
	for (short i = 0; i < map_size; i++)
	{
		for (short j = 0; j < map_size; j++)
		{
			if (map[i][j] == 2)
			{
				env_x = i * tile_size - (map_size * tile_size - display_w) / 2;
				env_y = j * tile_size - (map_size * tile_size - display_h) / 2;
				Environment campfire(0, env_x, env_y, sprites[0], true);
				props[0] = campfire;
				campfire_placed = true;
			}
			if (map[i][j] == 11)
			{
				short sprite = rand() % 2 + 4;
				env_x = (i - 1) * tile_size - (map_size * tile_size - display_w) / 2;
				env_y = j * tile_size - (map_size * tile_size - display_h) / 2;
				if (sprite == 4)
				{
					env_y -= 32;
				}
				Environment seat(sprite, env_x, env_y, sprites[sprite], false);
				props[1] = seat;
				seat_placed = true;
			}
			if (campfire_placed && seat_placed)
			{
				break;
			}
		}
		if (campfire_placed && seat_placed)
		{
			break;
		}
	}
	environment_write(props, 2, filename);
}

void environment_draw(Environment props[2], short num_props, short location_offset_x, short location_offset_y, float player_y, float player_height, bool behind_player)
{
	for (short i = 0; i < num_props; i++)
	{
		if ((props[i].X + props[i].Width > 0 && props[i].X < display_w) || (props[i].Y + props[i].Height > 0 && props[i].Y < display_h))
		{
			if (behind_player)
			{
				if (props[i].Y + location_offset_y + props[i].Height <= player_y + player_height || props[i].Walkable)
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
}