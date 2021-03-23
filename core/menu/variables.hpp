#pragma once
namespace variables {
	inline bool enableesp = false;
	inline bool aimbot_isvisiblecheck = false;
	inline bool silent = false;
	inline bool autoshoot = false;
	inline bool norecoil = true;
	inline bool autoscope = false;
	inline bool hitchance = false;
	inline bool teamcheck = false;
	inline bool team = false;
	inline bool bhop = false;
	inline bool watermark = true;
	inline bool aimbot = false;
	inline bool onkey = false;
	inline bool box = false;
	inline bool health = false;
	inline bool ammo = false;
	inline bool noflash = false;
	inline bool weapon = false;
	inline bool name = false;
	inline bool flags = false;
	inline bool grenade = false;
	inline bool bomb = false;
	inline bool clantag = false;
	inline bool radar = false;
	inline bool grenadetraj = false;
	inline bool fastduck = false;
	inline bool thirdperson = false;
	inline bool dropped = false;

	namespace menu {
		inline bool opened = true;
		inline int x = 140, y = 140;
		inline int w = 550, h = 700;
	}
}