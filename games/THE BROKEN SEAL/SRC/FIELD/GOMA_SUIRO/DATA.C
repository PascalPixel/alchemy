#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "OBJCMD.H"

void FieldScene_RunActor8AtCell24Sequence(void);

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

struct SceneEntrance gGomaSuiroEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 376, 0, 776, FACING_NORTH, 0, 88, 8, 664, 832, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 552, FACING_SOUTH, 0, 88, 8, 664, 832, 0 },
	{ 3, CONDITION_ALWAYS, 600, 0, 520, FACING_SOUTH, 0, 88, 8, 664, 832, 0 },
	{ 4, CONDITION_ALWAYS, 376, 0, 168, FACING_SOUTH, 0, 88, 8, 664, 832, 0 },
	{ 5, CONDITION_ALWAYS, 614, 0, 848, FACING_NORTH, 0, 480, 605, 720, 900, 0 },
	{ 99, CONDITION_ALWAYS, 424, 0, 280, FACING_WEST, 0, 88, 8, 664, 832, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gGomaSuiroEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 154, 0, 350, FACING_NORTH, 0, 16, 48, 632, 360, 0 },
	{ 2, CONDITION_ALWAYS, 552, 0, 312, FACING_NORTH, 0, 16, 48, 632, 360, 0 },
	{ SCENE_TABLE_END },
};

u32 GomaSuiro_Exits[] = {
	SCENE_EXITS(28),
	SCENE_EXIT(1, 25, 2),
	SCENE_EXIT(2, 27, 1),
	SCENE_EXIT(3, 27, 2),
	SCENE_EXIT(4, 29, 1),
	SCENE_EXIT(5, 5, 11),
	SCENE_EXIT(0, 29, 0),
	SCENE_EXIT(1, 28, 4),
	SCENE_EXIT(2, 2, 40),
	SCENE_EXITS_END,
};

struct ScenePlacement gGomaSuiroPlacementsOther[] = {
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(400), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gGomaSuiroPlacements2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent gGomaSuiroEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 1 },
	{ 0x31, 6, CONDITION_ALWAYS, 2 },
	{ 0x31, 7, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf28, FIND_ITEM(0xbf) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x864, EVENT_SCRIPT(FieldScene_RunActor8AtCell24Sequence) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gGomaSuiroEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};
