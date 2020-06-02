#include<algorithm>
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
#include "common.h"
#include "map.h"
#include "environment.h"

// Display properties
short display_w = 800, display_h = 600;

float fps = 60.0f;

// Keyboard controls properties
int key_up = ALLEGRO_KEY_UP, key_down = ALLEGRO_KEY_DOWN, key_right = ALLEGRO_KEY_RIGHT, key_left = ALLEGRO_KEY_LEFT, key_sprint = ALLEGRO_KEY_LSHIFT;
const int key_back = ALLEGRO_KEY_ESCAPE;

enum directions : char
{
	LEFT = 0, DOWN, RIGHT, UP
};

// Player properties
const short player_w = 32, player_h = 64;
float player_x = display_w / 2 - 16, player_y = display_h / 2 - 32;
char player_dir = DOWN;
const float player_walk_speed = 2, player_sprint_speed = 3;
float player_speed = 2;
char player_dx = 0, player_dy = 0;
bool moving = false, sprinting = false;

// World properties
const char tile_size = 32;
const short overworld_size = 64;
short tilemap_w;
short location_offset_x = 0, location_offset_y = 0;
const char num_props_types = 8;
const char num_props = 1;

enum props_sprites : char
{
	CAMPFIRE = 0, TREE_STUMP, TREE_A, TREE_B, TREE_C, ROCK_A, ROCK_B, ROCK_C
};

const char* overworld_map_filename = "world.map";
const char* overworld_props_filename = "overworld_props.epf";

bool file_exists(const char* filename)
{
	std::ifstream file(filename);
	if (file.good())
	{
		return true;
	}
	else
	{
		return false;
	}
}

int main()
{
	std::stringstream ss;
	
	bool gameloop = true;
	bool debug_info = true;

	bool draw = true;

	if (!al_init())
	{
		al_show_native_message_box(NULL, "Error", "Failed to initialize Allegro 5", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	// Display creation
	ALLEGRO_DISPLAY* display;
	display = al_create_display(display_w, display_h);
	if (!display)
	{
		al_show_native_message_box(NULL, "Error", "Failed to create a display", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
	}
	al_set_new_display_flags(ALLEGRO_WINDOWED);
	al_set_window_title(display, "Pointless Expedition");

	// Colors
	ALLEGRO_COLOR black = al_map_rgb(0, 0, 0);
	ALLEGRO_COLOR white = al_map_rgb(255, 255, 255);

	// Fonts
	al_init_font_addon();
	al_init_ttf_addon();

	ALLEGRO_FONT* advpix = al_load_font("Data/Fonts/advanced_pixel-7.ttf", 20, NULL);

	// Sprites
	al_init_image_addon();

	ALLEGRO_BITMAP* tilemap = al_load_bitmap("Data/Sprites/tilemap_overworld.png");
	tilemap_w = al_get_bitmap_width(tilemap) / tile_size;

	ALLEGRO_BITMAP* player = al_load_bitmap("Data/Sprites/pc_male_pants.png");

	short map[64][64] = { 0 };
	ss << "Game/" << overworld_map_filename;
	if (!file_exists(ss.str().c_str()))
	{
		map_generate_overworld(overworld_size, ss.str().c_str());
	}
	map_load(map, ss.str().c_str());
	ss.str(std::string());

	ALLEGRO_BITMAP* props_sprites[8] =
	{
		al_load_bitmap("Data/Sprites/campfire.png"),
		al_load_bitmap("Data/Sprites/tree_stump.png"),
		al_load_bitmap("Data/Sprites/tree_a.png"),
		al_load_bitmap("Data/Sprites/tree_b.png"),
		al_load_bitmap("Data/Sprites/tree_c.png"),
		al_load_bitmap("Data/Sprites/rock_a.png"),
		al_load_bitmap("Data/Sprites/rock_b.png"),
		al_load_bitmap("Data/Sprites/rock_c.png")
	};

	extern ALLEGRO_BITMAP* campfire;

	Environment props[num_props];
	ss << "Game/" << overworld_props_filename;
	if (!file_exists(ss.str().c_str()))
	{
		environment_generate_overworld_props(map, overworld_size, props, props_sprites, ss.str().c_str());
	}
	ss.str(std::string());

	for (int i = 0; i < num_props; i++)
	{
		Environment props;
	}

	// Keyboard
	al_install_keyboard();

	ALLEGRO_KEYBOARD_STATE key_state;

	// Joystick
	al_install_joystick();

	ALLEGRO_JOYSTICK* joy;

	bool joystick_connected;

	if (al_get_num_joysticks() != 0)
	{
		joy = al_get_joystick(0);
	}
	else
	{
		joy = NULL;
	}

	ALLEGRO_JOYSTICK_STATE joy_state;

	// Event queue creation
	ALLEGRO_EVENT_QUEUE* queue;
	queue = al_create_event_queue();

	// Timer creation
	ALLEGRO_TIMER* timer;
	timer = al_create_timer(1.0 / fps);

	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_joystick_event_source());
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
		if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{
			gameloop = false;
		}
		if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_F1)
		{
			debug_info = !debug_info;
		}

		if (al_get_num_joysticks() != 0)
		{
			joystick_connected = true;
		}
		else
		{
			joystick_connected = false;
		}

		if (event.type == ALLEGRO_EVENT_JOYSTICK_CONFIGURATION)
		{
			al_reconfigure_joysticks();
			joy = al_get_joystick(0);
		}

		if (event.type == ALLEGRO_EVENT_TIMER)
		{
			if (joystick_connected)
			{
				al_get_joystick_state(joy, &joy_state);
			}
			else
			{
				al_get_keyboard_state(&key_state);
			}

			// Gamepad input
			if (joystick_connected)
			{
				if ((joy_state.stick[0].axis[0] < 0) || (joy_state.stick[2].axis[0] < 0))
				{
					player_dir = LEFT;
					player_dx = -1;
				}
				else if ((joy_state.stick[0].axis[0] > 0) || (joy_state.stick[2].axis[0] > 0))
				{
					player_dir = RIGHT;
					player_dx = 1;
				}
				else
				{
					player_dx = 0;
				}

				if ((joy_state.stick[0].axis[1] < 0) || (joy_state.stick[2].axis[1] < 0))
				{
					player_dir = UP;
					player_dy = -1;
				}
				else if ((joy_state.stick[0].axis[1] > 0) || (joy_state.stick[2].axis[1] > 0))
				{
					player_dir = DOWN;
					player_dy = 1;
				}
				else
				{
					player_dy = 0;
				}

				if (joy_state.button[5])
				{
					sprinting = true;
				}
				else
				{
					sprinting = false;
				}
			}
			// Keyboard input
			else
			{
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

				if (al_key_down(&key_state, key_sprint))
				{
					sprinting = true;
				}
				else
				{
					sprinting = false;
				}
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

			if (!(abs(player_dx) != 0 && abs(player_dy) != 0))
			{
				player_x += player_dx * player_speed;
				player_y += player_dy * player_speed;
			}
			else
			{
				player_x += player_dx * sin(45) * player_speed;
				player_y += player_dy * sin(45) * player_speed;
			}

			// Player sprite screen position limits and map scrolling
			if (player_x < display_w / 4)
			{
				location_offset_x += player_speed;
				player_x = display_w / 4;
			}
			else if (player_x > display_w - display_w / 4 - player_w)
			{
				location_offset_x -= player_speed;
				player_x = display_w - display_w / 4 - player_w;
			}

			if (player_y < display_h / 4)
			{
				location_offset_y += player_speed;
				player_y = display_h / 4;
			}
			else if (player_y > display_h - display_w / 4 - player_h)
			{
				location_offset_y -= player_speed;
				player_y = display_h - display_w / 4 - player_h;
			}

			draw = true;
		}

		// Drawing graphics on display
		if (draw)
		{
			draw = false;
			al_clear_to_color(black);

			map_draw(tile_size, map, overworld_size, tilemap, tilemap_w, location_offset_x, location_offset_y);

			environment_draw(props, location_offset_x, location_offset_y, player_y, player_h, true);

			al_draw_bitmap_region(player, player_dir * 32, 0, 32, 64, player_x, player_y, NULL);

			environment_draw(props, location_offset_x, location_offset_y, player_y, player_h, false);

			if (debug_info)
			{
				ss << "X = " << player_x;
				al_draw_text(advpix, white, 5, 5, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "DX = " << player_dx;
				al_draw_text(advpix, white, 100, 5, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "Location Offset X = " << location_offset_x;
				al_draw_text(advpix, white, 170, 5, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "Y = " << player_y;
				al_draw_text(advpix, white, 5, 15, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "DY = " << player_dy;
				al_draw_text(advpix, white, 100, 15, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "Location Offset Y = " << location_offset_y;
				al_draw_text(advpix, white, 170, 15, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "Moving = " << moving;
				al_draw_text(advpix, white, 5, 25, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "Sprinting = " << sprinting;
				al_draw_text(advpix, white, 5, 35, NULL, ss.str().c_str());
				ss.str(std::string());
				ss << "Joysticks detected: " << al_get_num_joysticks();
				al_draw_text(advpix, white, 5, 55, NULL, ss.str().c_str());
				ss.str(std::string());
			}

			al_flip_display();
		}
	}

	// Freeing memory
	al_destroy_bitmap(player);
	for (short i = 0; i < num_props_types; i++)
	{
		al_destroy_bitmap(props_sprites[i]);
	}
	al_destroy_bitmap(tilemap);
	al_destroy_font(advpix);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_uninstall_joystick();
	al_uninstall_keyboard();
	al_destroy_display(display);

	return 0;
}