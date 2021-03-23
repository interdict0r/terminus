#include "../features.hpp"
#include "visuals.h"

visuals esp;

void visuals::watermark() {
	if (!variables::watermark)
		return;

	std::stringstream ss;
	int scrx, scry;
	int x, y;
	static int fps, old_tick_count;

	if ((interfaces::globals->tick_count - old_tick_count) > 50) {
		fps = static_cast<int>(1.f / interfaces::globals->frame_time);
		old_tick_count = interfaces::globals->tick_count;
	}

	char FPS;
	auto net_channel = interfaces::engine->GetNetChannelInfo();

	std::string outgoing = csgo::local_player ? std::to_string((int)(net_channel->GetLatency(FLOW_OUTGOING) * 1000 / 1.3)) : "0";
	std::string server = csgo::local_player ? server == "loopback" ? net_channel->GetAddress() : "local-server" : "main-menu";
	if (server == "loopback")
		server == "local-server";

	ss << "terminus" << " | dev" << " | ms: " << outgoing.c_str() << " | fps: " << fps << " | server: " << server;
	interfaces::engine->get_screen_size(scrx, scry);
	x = static_cast<int>(scrx);
	y = static_cast<int>(scry) / 5;
	int w = render::get_text_size(render::fonts::watermark_font, ss.str()).x;

	render::draw_filled_rect(x - 30 - w, y - 206, w + 10, 26, color(45, 48, 55, 255));
	render::draw_rect(x - 30 - w, y - 206, w + 10, 5, color(0, 0, 0, 255));
	// old clr too sexy to remove color(52, 134, 235, 255));
	render::gradientline(x - 30 - w, y - 206, w + 10, 4, color(222, 60, 60, 255), color(52, 134, 235, 255));
	render::text(x - 25 - w, y - 198, render::fonts::watermark_font, ss.str(), false, color::white(255));
}

void visuals::runesp() {
	if (!variables::enableesp)
		return;

	for (int i = 1; i <= interfaces::globals->max_clients; i++) {
		player_t* player = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!player || !csgo::local_player)
			continue;

		if (!player->is_alive() || player == csgo::local_player || player->dormant())
			continue;

		box bbox;

		if (!get_playerbox(player, bbox))
			continue;

		player_info_t info;
		interfaces::engine->get_player_info(player->Networkable()->EntIndex(), &info);

		if (variables::radar)
			radar(player);

		if (csgo::local_player->team() == player->team() && variables::team)
			continue;

		if (variables::health)
			health(player, bbox);

		if (variables::flags)
			flags(player, bbox);

		if (variables::name)
			name(bbox, info, player);

		if (variables::ammo)
			ammo(player, bbox);

		if (variables::weapon)
			weapon(player, bbox);

		if (variables::box)
			boxesp(bbox);
	}
	for (int i = 1; i <= interfaces::entity_list->get_highest_index(); i++) {
		player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));

		if (!entity)
			continue;

		weaponesp(entity);
	}
}

void visuals::boxesp(box bbox) {
	render::draw_outline(bbox.x, bbox.y, bbox.w, bbox.h, color::white(255));
	render::draw_outline(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, color::black(255));
	render::draw_outline(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, color::black(255));
}

void visuals::flags(player_t* player, box bbox) {

	std::vector<std::pair<std::string, color>> flags;

	if (player->has_helmet() && player->armor() < 0) {
		flags.push_back({ "H", color((81, 154, 237, 255)) });
	}
	if (player->armor() > 0 && player->has_helmet()) {
		flags.push_back({ "HK", color(81, 154, 237, 255) });
	}
	if (player->is_scoped()) {
		flags.push_back({ "SCOPED", color(255, 140, 0, 255) });
	}
	if (player->is_flashed()) {
		flags.push_back({ "FLASHED", color(245, 233, 66, 255) });
	}

	for (int i = 0; i < flags.size(); i++) {
		render::text(bbox.x + bbox.w + 3, bbox.y + (i * 8), render::fonts::pixel, flags[i].first, false, color(flags[i].second.r, flags[i].second.g, flags[i].second.b));
	}
}

void visuals::ammo(player_t* player, box bbox) {
	weapon_t* weapon = player->active_weapon();
	weapon_info_t* WeaponData = weapon->get_weapon_data();
	if (!weapon || !WeaponData || bbox.w > 400)
		return;

	int max_in_clip = WeaponData->weapon_max_clip, ammo_in_clip = weapon->clip1_count(), scaled_ammo = ammo_in_clip * bbox.w / max_in_clip, value = 0;

	if (scaled_ammo > 0) {
		int w = bbox.w;
		value = std::clamp(scaled_ammo, 0, w);
	}

	render::draw_filled_rect(bbox.x - 1, bbox.y + bbox.h + 3, bbox.w + 2, 4, color(0, 0, 0, 170));
	render::draw_filled_rect(bbox.x, bbox.y + bbox.h + 4, value, 2, color(0, 191, 255));

	if (ammo_in_clip == max_in_clip)
		return;

	render::text(bbox.x + 1 + value - 3, bbox.y + bbox.h, render::fonts::pixel, std::to_string(ammo_in_clip), true, color(240, 240, 240, 255));
}

void visuals::health(player_t* player, box bbox) {
	uint8_t g = (255 * player->health() / 100);
	uint8_t r = 255 - g;

	int bar_value = bbox.h * (player->health() / 100.f);

	render::draw_filled_rect(bbox.x - 7, bbox.y - 1, 4, bbox.h + 2, color(0, 0, 0, 170));
	render::draw_filled_rect(bbox.x - 6, bbox.y + bbox.h - bar_value, 2, bar_value, color(r, g, 0, 255));

	if (player->health() < 100)
		render::text(bbox.x - 13, bbox.y + bbox.h - bar_value - 5, render::fonts::pixel, std::to_string(player->health()), true, color::white(255));
}

void visuals::name(box bbox, player_info_t info, player_t* player) {
	std::string name = info.name;
	int width = render::get_text_size(render::fonts::pixel, name).x + 4;

	render::draw_filled_rect(bbox.x + (bbox.w / 2) - width / 2, bbox.y - 17, width, 15, color(0, 0, 0, 70));
	render::draw_filled_rect(bbox.x + (bbox.w / 2) - width / 2, bbox.y - 17, width, 2, color(222, 60, 60, 255));
	render::text(bbox.x + (bbox.w / 2), bbox.y - 15, render::fonts::pixel, name, true, color::white(255));
}

void visuals::radar(player_t* player) {
	if (!player->dormant() && player->is_alive())
		player->spotted() = true;
}

void visuals::flash() {
	if (!csgo::local_player || !variables::noflash)
		return;

	csgo::local_player->flash_alpha() = 0.f;
}

void visuals::bombtimer() {
	int scrx, scry;
	int x;
	interfaces::engine->get_screen_size(scrx, scry);
	x = static_cast<int>(scrx);

	if (variables::bomb) {
		for (int i = 0; i <= 2048; i++) { // fuck
			player_t* entity = reinterpret_cast<player_t*>(interfaces::entity_list->get_client_entity(i));
			if (!entity)
				continue;

			if (entity->client_class()->class_id == 128) {
				C_CSBomb* bomb = reinterpret_cast<C_CSBomb*>(entity);

				if (bomb) {
					if (bomb->m_bBombDefused())
						continue;

					std::string invalid = "Bomb Time: 0.0";

					float c4_clock = bomb->m_flC4Blow() - interfaces::globals->cur_time;
					char buf[24];
					sprintf_s(buf, sizeof(buf) - 1, "Bomb Time: %.1f", c4_clock);

					uint8_t g = (255 * c4_clock / 45);
					uint8_t r = 255 - g;
					render::draw_filled_rect(0, 0, x, 12, color(0, 0, 0, 180));
					render::draw_filled_rect(0, 1, c4_clock * 40, 10, color(r, g, 0, 255));
					render::text(c4_clock * 45, 2, render::fonts::watermark_font, c4_clock >= 0 ? buf : invalid, false, color(240, 240, 240));
				}
			}
		}
	}
}

void visuals::weapon(player_t* player, box bbox) {
	weapon_t* weapon = player->active_weapon();
	weapon_info_t* WeaponData = weapon->get_weapon_data();

	if (!weapon || !WeaponData)
		return;

	std::string weapon_name = WeaponData->weapon_name;

	int size = weapon_name.size();
	std::string substr = weapon_name.substr(7, weapon_name.size() - 7);

	auto WeaponIcon = player->weapon2icon(weapon);

	render::text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 24, render::fonts::pixel, substr, true, color(200, 200, 200, 180));
	render::text(bbox.x + (bbox.w / 2), bbox.y + bbox.h + 5, render::fonts::weapon, WeaponIcon, true, color(255, 255, 255, 200));
}

void visuals::weaponesp(player_t* player) {
	if (!variables::dropped)
		return;

	auto clean_item_name = [](const char* name) -> const char* {
		if (name[0] == 'C')
			name++;

		auto start = strstr(name, "Weapon");
		if (start != nullptr)
			name = start + 6;

		return name;
	};

	if (player->is_weapon()) {
		vec3_t w2s;
		if (!interfaces::debug_overlay->world_to_screen(player->get_absolute_origin(), w2s))
			return;

		render::text(w2s.x, w2s.y, render::fonts::pixel, clean_item_name(player->client_class()->network_name), false, color::white(255));
	}
}