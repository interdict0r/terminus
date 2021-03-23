#pragma once
#include "../../../source-sdk/structs/rect.h"
#include "../features.hpp"
#include "../../../source-sdk/math/vector3d.hpp"
#include "../../../source-sdk/sdk.hpp"
#include "../../../source-sdk/misc/color.hpp"

class visuals {
public:
	void runesp();
	void watermark();
	void bombtimer();
	void flash();

	// box math
	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

private:
	void flags(player_t* player, box bbox);
	void ammo(player_t* player, box bbox);
	void health(player_t* player, box bbox);
	void name(box bbox, player_info_t info, player_t* player);
	void weapon(player_t* player, box bbox);
	void weaponesp(player_t* player);
	void boxesp(box bbox);
	void radar(player_t* player);

	bool get_playerbox(player_t* ent, box& in) {
		vec3_t origin, min, max, flb, brt, blb, frt, frb, brb, blt, flt;
		int left, top, right, bottom;

		origin = ent->get_absolute_origin();
		min = ent->collideable()->mins() + origin;
		max = ent->collideable()->maxs() + origin;

		vec3_t points[] = {
			vec3_t(min.x, min.y, min.z),
			vec3_t(min.x, max.y, min.z),
			vec3_t(max.x, max.y, min.z),
			vec3_t(max.x, min.y, min.z),
			vec3_t(max.x, max.y, max.z),
			vec3_t(min.x, max.y, max.z),
			vec3_t(min.x, min.y, max.z),
			vec3_t(max.x, min.y, max.z)
		};
		if (!interfaces::debug_overlay->world_to_screen(points[3], flb) || !interfaces::debug_overlay->world_to_screen(points[5], brt)
			|| !interfaces::debug_overlay->world_to_screen(points[0], blb) || !interfaces::debug_overlay->world_to_screen(points[4], frt)
			|| !interfaces::debug_overlay->world_to_screen(points[2], frb) || !interfaces::debug_overlay->world_to_screen(points[1], brb)
			|| !interfaces::debug_overlay->world_to_screen(points[6], blt) || !interfaces::debug_overlay->world_to_screen(points[7], flt))
			return false;

		vec3_t arr[] = { flb, brt, blb, frt, frb, brb, blt, flt };

		left = flb.x;
		top = flb.y;
		right = flb.x;
		bottom = flb.y;

		for (int i = 1; i < 8; i++) {
			if (left > arr[i].x)
				left = arr[i].x;
			if (bottom < arr[i].y)
				bottom = arr[i].y;
			if (right < arr[i].x)
				right = arr[i].x;
			if (top > arr[i].y)
				top = arr[i].y;
		}
		in.x = (int)left;
		in.y = (int)top;
		in.w = int(right - left);
		in.h = int(bottom - top);

		return true;
	}
};
class player_info {
public:
	std::string info;
	color fcolor;

	player_info(std::string info, color fcolor) {
		this->info = info;
		this->fcolor = fcolor;
	}
};
extern visuals esp;