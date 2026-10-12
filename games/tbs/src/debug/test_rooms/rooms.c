#include "unit/debug/test_rooms/rooms.h"
#include "unit/battle/effect/object.h"
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
#include "scene.h"
#include "field_event.h"
#include "field_scene.h"

u8 *TestRooms_GetEntrances(void)
{
	return (u8 *)TestRooms_Entrances;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *TestRooms_GetExits(void)
{
	return (u8 *)TestRooms_Exits;
}

u8 *TestRooms_GetPlacements(void)
{
	return (u8 *)TestRooms_Placements;
}

void State_ApplyValues8And0And0Then30(void)
{
	BattleFx_RunPageEffectForSlot(8, 0, 0);
	Engine_GameFlagClear(0x30);
}

void State_ApplyValues9And1And0Then44(void)
{
	BattleFx_RunPageEffectForSlot(9, 1, 0);
	Engine_GameFlagClear(0x44);
}

void State_ApplyValues10And2And0Then58(void)
{
	BattleFx_RunPageEffectForSlot(0xA, 2, 0);
	Engine_GameFlagClear(0x58);
}

void State_ApplyValues11And3And0Then6c(void)
{
	BattleFx_RunPageEffectForSlot(0xB, 3, 0);
	Engine_GameFlagClear(0x6C);
}

u8 *TestRooms_GetEvents(void)
{
	return (u8 *)TestRooms_Events;
}

s32 Scene_RunEmptyScene(void)
{
	return 0;
}

u32 TestRooms_EntrancesHead[] = {
	0x500050,
	0,
	80,
	0x500000,
	0x10003,
	2,
	0,
	0x780078,
	0,
	120,
	0x780000,
	0x10003,
	2,
	0,
	0x28003c,
	0,
	128,
	0x800000,
	0x10002,
	1,
	0,
	0x28003c,
	0,
	128,
	0x800000,
	0x10002,
	3,
	0,
	0x800080,
	0,
	128,
	0x800000,
	0x20003,
	1,
	0,
	0x800080,
	0,
	128,
	0x800000,
	0x20003,
	1,
	0,
	0x200020,
	0,
	0,
	0,
	0,
	0,
	0,
	0x800080,
	0,
	128,
	0x800000,
	0x20003,
	1,
	0,
};

struct SceneEntrance TestRooms_Entrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 TestRooms_Exits[] = {
	SCENE_EXITS(191),
	SCENE_EXIT(0, 192, 0),
	SCENE_EXIT(0, 193, 0),
	SCENE_EXIT(0, 194, 0),
	SCENE_EXITS_END,
};

struct ScenePlacement TestRooms_Placements[] = {
	{0x5a, 0x30, ACTOR_STAND, PIXELS(496), 0, PIXELS(176), FACING_EAST, TALK_FACE_PARTY, 0},
	{0x5b, 0x44, ACTOR_STAND, PIXELS(496), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0},
	{0x5c, 0x58, ACTOR_STAND, PIXELS(528), 0, PIXELS(176), FACING_WEST, TALK_FACE_PARTY, 0},
	{0x5d, 0x6c, ACTOR_STAND, PIXELS(528), 0, PIXELS(200), FACING_WEST, TALK_FACE_PARTY, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent TestRooms_Events[] = {
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(State_ApplyValues8And0And0Then30)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(State_ApplyValues9And1And0Then44)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(State_ApplyValues10And2And0Then58)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(State_ApplyValues11And3And0Then6c)},
	{SCENE_EVENTS_END},
};
