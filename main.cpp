#include<cmath>
#include<fstream>
#include<iostream>
#include<sstream>
#include<string>
#include<allegro5/allegro.h>
#include<allegro5/allegro_native_dialog.h>
#include<allegro5/allegro_font.h>
#include<allegro5/allegro_ttf.h>
#include<allegro5/allegro_image.h>

using namespace std;

stringstream ss;

// General properties
bool gameloop = true;

// Display properties
int display_w = 800;
int display_h = 600;

float fps = 60.0;

// Keyboard controls properties
int key_up = ALLEGRO_KEY_UP, key_down = ALLEGRO_KEY_DOWN, key_right = ALLEGRO_KEY_RIGHT, key_left = ALLEGRO_KEY_LEFT;
int key_sprint = ALLEGRO_KEY_LSHIFT;
const int key_back = ALLEGRO_KEY_ESCAPE;

enum directions { LEFT, DOWN, RIGHT, UP };

// Player properties
float player_x = display_w / 2 - 16, player_y = display_h / 2 - 32;
int player_dir = DOWN;
const float player_walk_speed = 2, player_sprint_speed = 3;
float player_speed = 2;
int player_dx = 0, player_dy = 0;
bool moving = false, diagonal = false, sprinting = false;

// World properties
const int tile_size = 32;
int map_index_x = 0, map_index_y = 0;
int map[25][19] = { 0 };

void load_map(const char* filename)
{
	int map_width, map_x = 0, map_y = 0;
	ss << "Game/" << filename;
	ifstream mapfile(ss.str());
	ss.str(string());
	if (mapfile.is_open())
	{
		string line;
		getline(mapfile, line);
		line.erase(remove(line.begin(), line.end(), ' '), line.end());
		map_width = line.length();
		mapfile.seekg(0, ios::beg);
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
	}
	else
	{
		al_show_native_message_box(NULL, "Error", NULL, "Failed to open map file", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
}

void draw_map(int map[25][19], ALLEGRO_BITMAP* tileset);

int main()
{
	ALLEGRO_DISPLAY* display;

	ALLEGRO_EVENT_QUEUE* queue;

	ALLEGRO_TIMER* timer;

	bool draw = true;

	if (!al_init())
	{
		al_show_native_message_box(NULL, "Error", NULL, "Failed to initialize Allegro 5", NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}

	// Colors
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR magenta = al_map_rgb(255, 0, 255);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

	// Fonts
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT* advpix = al_load_font("Data/Fonts/advanced_pixel-7.ttf", 20, NULL);

	// Sprites
	al_init_image_addon();

	ALLEGRO_BITMAP* tiles = al_load_bitmap("Data/Sprites/tiles.png");
	ALLEGRO_BITMAP* player = al_load_bitmap("Data/Sprites/pc_base.png");

	load_map("world.map");
	/*
	// FIXME
	for (short int i = 0; i < 25; i++) // until we will learn how to load values into array from file, this is our map array initialization
	{
		for (short int j = 0; j < 19; j++)
		{
			map[i][j] = 1;
		}
	}
	*/

	// Keyboard
	al_install_keyboard();

	ALLEGRO_KEYBOARD_STATE key_state;

	// Display creation
	display = al_create_display(display_w, display_h);
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_window_title(display, "Pointless Expedition");

	// Event queue creation
	queue = al_create_event_queue();

	// Timer creation
	timer = al_create_timer(1.0 / fps);

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_start_timer(timer);

	while (gameloop)
	{
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);

		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			gameloop = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (event.keyboard.keycode)
			{
			case key_back:
				gameloop = false;
			}
		}

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			al_get_keyboard_state(&key_state);

			if (al_key_down(&key_state, key_left) && !al_key_down(&key_state, key_right))
			{
				player_dir = LEFT;
				player_dx = -1;
			}
			else if (al_key_down(&key_state, key_right) && !al_key_down(&key_state, key_left))
			{
				player_dir = RIGHT;
				player_dx = 1;
			}
			else
			{
				player_dx = 0;
			}

			if (al_key_down(&key_state, key_up) && !al_key_down(&key_state, key_down))
			{
				player_dir = UP;
				player_dy = -1;
			}
			else if (al_key_down(&key_state, key_down) && !al_key_down(&key_state, key_up))
			{
				player_dir = DOWN;
				player_dy = 1;
			}
			else
			{
				player_dy = 0;
			}

			if (player_dx != 0 && (abs(player_dx) == abs(player_dy)))
			{
				diagonal = true;
			}
			else
			{
				diagonal = false;
			}

			if (al_key_down(&key_state, key_sprint))
			{
				sprinting = true;
			}
			else
			{
				sprinting = false;
			}

			if (player_dx != 0 || player_dy != 0)
			{
				moving = true;
			}
			else
			{
				moving = false;
				sprinting = false;
			}

			if (sprinting)
			{
				player_speed = player_sprint_speed;
			}
			else
			{
				player_speed = player_walk_speed;
			}

			if (!diagonal)
			{
				player_x += player_dx * player_speed;
				player_y += player_dy * player_speed;
			}
			else
			{
				player_x += player_dx * sin(45) * player_speed;
				player_y += player_dy * sin(45) * player_speed;
			}

			// Window borders limits for movement
			if (player_x < -5)
			{
				player_x = -5;
			}
			else if (player_x > display_w - 32 + 5)
			{
				player_x = display_w - 32 + 5;
			}

			if (player_y < -3)
			{
				player_y = -3;
			}
			else if (player_y > display_h - 64 + 3)
			{
				player_y = display_h - 64 + 3;
			}

			draw = true;
		}

		// Drawing graphics on display
		if (draw)
		{
			draw = false;
			al_clear_to_color(black);

			draw_map(map, tiles);

			al_draw_bitmap_region(player, player_dir * 32, 0, 32, 64, player_x, player_y, NULL);

			// if (debug_mode)
			ss << "X = " << player_x;
			al_draw_text(advpix, magenta, 5, 5, NULL, ss.str().c_str());
			ss.str(string());
			ss << "Y = " << player_y;
			al_draw_text(advpix, magenta, 5, 15, NULL, ss.str().c_str());
			ss.str(string());
			ss << "Moving = " << moving;
			al_draw_text(advpix, magenta, 5, 25, NULL, ss.str().c_str());
			ss.str(string());
			ss << "Sprinting = " << sprinting;
			al_draw_text(advpix, magenta, 5, 35, NULL, ss.str().c_str());
			ss.str(string());

			al_flip_display();
		}
	}

	// Freeing memory
	al_destroy_bitmap(player);
	al_destroy_bitmap(tiles);
	al_destroy_font(advpix);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_uninstall_keyboard();
	al_destroy_display(display);

	return 0;
}

void draw_map(int map[25][19], ALLEGRO_BITMAP* tileset)
{
	int tileset_row_tiles_number = al_get_bitmap_width(tileset) / tile_size;
	int tile_id, tile_x = 0, tile_y = 0;
	float id_to_row_number_ratio;
	for (short int i = 0; i < 25; i++)
	{
		for (short int j = 0; j < 19; j++)
		{
			//cout << map[i][j];
			tile_id = map[i][j];
			if (tile_id >= tileset_row_tiles_number)
			{
				tile_x = tile_id - tileset_row_tiles_number;
				id_to_row_number_ratio = tile_id / tileset_row_tiles_number;
				tile_y = (int)id_to_row_number_ratio;
			}
			else
			{
				tile_x = tile_id;
			}
			al_draw_bitmap_region(tileset, tile_x*tile_size, tile_y*tile_size, tile_size, tile_size, i*tile_size, j*tile_size, NULL);
		}
		//cout << endl;
	}
}