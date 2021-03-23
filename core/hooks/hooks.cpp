#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"
#include "../features/visuals/visuals.h"
#include "../../dependencies/interfaces/GlobalVars.h"

std::vector<grenade_info> grenadeinfo;
hooks::create_move::fn create_move_original = nullptr;
hooks::paint_traverse::fn paint_traverse_original = nullptr;
hooks::scene_end::fn scene_end_original = nullptr;
hooks::do_post_screen_effects::fn do_post_screen_effects_original = nullptr;

bool hooks::initialize() {
	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 24));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, 41));
	auto scene_end_target = reinterpret_cast<void*>(get_virtual(interfaces::render_view, 9));
	auto do_post_screen_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::clientmode, 44));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error("failed to initialize MH_Initialize.");

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move_original)) != MH_OK)
		throw std::runtime_error("failed to initialize create_move. (outdated index?)");

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse_original)) != MH_OK)
		throw std::runtime_error("failed to initialize paint_traverse. (outdated index?)");

	if (MH_CreateHook(scene_end_target, &scene_end::hook, reinterpret_cast<void**>(&scene_end_original)) != MH_OK)
		throw std::runtime_error("failed to initialize scene_end. (outdated index?)");

	if (MH_CreateHook(do_post_screen_effects_target, &do_post_screen_effects::hook, reinterpret_cast<void**>(&do_post_screen_effects_original)) != MH_OK)
		throw std::runtime_error("failed to initialize do_post_screen_effects. (outdated index?)");

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error("failed to enable hooks.");

	console::log("[terminus] hooks initialized!\n");
	return true;
}

void hooks::release() {
	MH_Uninitialize();

	MH_DisableHook(MH_ALL_HOOKS);
}

bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) {
	create_move_original(input_sample_frametime, cmd);

	if (!cmd || !cmd->command_number)
		return false;

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x1C);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;

	misc::movement::bunny_hop(cmd);
	misc::movement::fastduck(cmd);
	misc::clan_tag();
	nade_pred::tick(cmd->buttons);

	prediction::start(cmd); {
		misc::movement::edge_jump(cmd);
		ragebot::aimbot(cmd);
	} 
	prediction::end();


	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

int __stdcall hooks::do_post_screen_effects::hook(int value) {
	glow::run();

	return do_post_screen_effects_original(interfaces::clientmode, value);
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		menu::toggle();
		menu::render();
		esp.watermark();
		esp.bombtimer();
		esp.flash();
		esp.runesp();
		nade_pred::paint();
		break;

	case fnv::hash("FocusOverlayPanel"):
		interfaces::panel->set_keyboard_input_enabled(panel, false);
		interfaces::panel->set_mouse_input_enabled(panel, variables::menu::opened);
		break;
	}

	paint_traverse_original(interfaces::panel, panel, force_repaint, allow_force);
}

void __stdcall hooks::scene_end::hook() {
	nade_pred::view();

	scene_end_original(interfaces::render_view);
}
