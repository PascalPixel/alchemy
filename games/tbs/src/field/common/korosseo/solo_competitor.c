#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "game_state.h"
#include "battle_unit.h"

void Engine_CameraFollowActor(s32 id, s32 target);

void Korosseo_SelectSoloCompetitor(s32 id)
{
	struct BtlUnit *unit;
	s32				v;
	s32				t;

	Party_RemoveActiveOwner(0);
	Party_RemoveActiveOwner(1);
	Party_RemoveActiveOwner(2);
	Party_RemoveActiveOwner(3);
	Party_RemoveActiveOwner(5);
	Party_AddActiveOwner(id);
	gGameState.selected_actor = id;
	Engine_CameraFollowActor(id, 0);
	unit = UnitGet(id);
	unit->hp = unit->max_hp;
	unit->ep = unit->max_ep;
	unit->doku = 0;
	v = (unit->hp << 14) / unit->max_hp;
	t = 0x4000;
	if (v <= 0x4000)
	{
		t = 0;
		if (v >= 0)
			t = v;
	}
	unit->hp_gauge = t;
	if ((u16)t == 0 && unit->hp != 0)
	{
		unit->hp_gauge = 1;
	}
	v = (unit->ep << 14) / unit->max_ep;
	t = 0x4000;
	if (v <= 0x4000)
	{
		t = 0;
		if (v >= 0)
			t = v;
	}
	unit->ep_gauge = t;
	if ((u16)t == 0 && unit->ep != 0)
	{
		unit->ep_gauge = 1;
	}
	Engine_EventClearInvalidPackedValues();
}
