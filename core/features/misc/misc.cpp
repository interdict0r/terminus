#include "../features.hpp"
#include "../../../dependencies/interfaces/GlobalVars.h"
#include "../../../dependencies/utilities/utilities.hpp"
#include "engine_prediction.hpp"

template<class T, class U>
inline T clamp_value(T in, U low, U high)
{
	if (in <= low)
		return low;
	else if (in >= high)
		return high;
	else
		return in;
}

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::bhop)
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	static bool lastjump = false;
	static bool shouldfake = false;

	if (!lastjump && shouldfake) {
		shouldfake = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP) {
		if (csgo::local_player->flags() & fl_onground)
			shouldfake = lastjump = true;
		else {
			cmd->buttons &= ~IN_JUMP;
			lastjump = false;
		}
	}
	else
		shouldfake = lastjump = false;
};

void misc::clan_tag() {
	auto SetClanTag = [](const char* name) -> void {
		using Fn = int(__fastcall*)(const char*, const char*);
		static auto fn = reinterpret_cast<Fn>(utilities::pattern_scan("engine.dll", "53 56 57 8B DA 8B F9 FF 15"));
		fn(name, name);
	};

	std::string szClanTag = ("terminus.xyz ");
	static bool bReset = false;
	static float last_time = 0;
	static int iPrevFrame = 0;
	int iCurFrame = ((int)(interfaces::globals->cur_time * 2.f)) % (szClanTag.size() * 2);

	if (variables::clantag) {
		if (iPrevFrame != iCurFrame) {
			for (int i = 0; i < iCurFrame; i++) {
				std::rotate(szClanTag.begin(), szClanTag.begin() + 1, szClanTag.end());
			}
			SetClanTag(szClanTag.data());
			iPrevFrame = iCurFrame;
		}
		bReset = true;
	}
	else {
		// reset our clantag
		if (bReset) {
			SetClanTag("");
			bReset = false;
		}
	}
}

void misc::movement::edge_jump(c_usercmd* cmd) {
	if (!GetAsyncKeyState(VK_XBUTTON1))
		return;

	const int MoveType = csgo::local_player->move_type();

	if (!csgo::local_player || !csgo::local_player->is_alive())
		return;

	if (const auto mt = csgo::local_player->move_type(); mt == movetype_ladder || mt == movetype_noclip)
		return;

	if ((prediction::getFlags() & 1) && !(csgo::local_player->flags() & 1))
		cmd->buttons |= cmd->buttons = IN_JUMP;
}

void misc::movement::fastduck(c_usercmd* cmd) {
	if (variables::fastduck)
		cmd->buttons |= IN_BULLRUSH;
}