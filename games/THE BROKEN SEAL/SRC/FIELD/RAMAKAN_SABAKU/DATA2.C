#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "OBJCMD.H"

extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];
void FieldScene_ApplyActor13Values3And3(void);
void FieldScene_RunFlags8B2And8B3Steps(void);
void RamakanSabaku_ApplyEntryState(void);
void RamakanSabaku_ConfigureAreaLayout(void);
void RamakanSabaku_FaceNearestActor(void);
void RamakanSabaku_PlayStartledRetreat(void);
void RamakanSabaku_ReturnToArea3(void);
void RamakanSabaku_ShowRandomPartyLine(void);
void RamakanSabaku_SweepBack(void);
void RamakanSabaku_UpdateDustAndProbe(void);
void RamakanSabaku_UpdateTravelDust(void);

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

struct SceneEntrance gRamakanSabakuEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, 0, 0, 0, 0, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gRamakanSabakuEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 984, 0, 952, FACING_WEST, 0, 0, 0, 1016, 1016, 0 },
	{ 2, CONDITION_ALWAYS, 400, 0, 24, FACING_SOUTH, 0, 0, 0, 1016, 1016, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gRamakanSabakuEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 912, 0, 1000, FACING_NORTH, 0, 0, 0, 1016, 1016, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 584, FACING_EAST, 0, 0, 0, 1016, 1016, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gRamakanSabakuEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 1511, 0, 104, FACING_WEST, 0, 0, 8, 1528, 504, 0 },
	{ 2, CONDITION_ALWAYS, 727, 0, 104, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ 3, CONDITION_ALWAYS, 599, 0, 296, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ 4, CONDITION_ALWAYS, 167, 0, 120, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ 5, CONDITION_ALWAYS, 167, 0, 136, FACING_NORTH, 0, 0, 8, 1528, 504, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gRamakanSabakuEntrances4[] = {
	{ 0, CONDITION_ALWAYS, 424, 0, 824, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 424, 0, 968, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
	{ 2, CONDITION_ALWAYS, 552, 0, 776, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
	{ 3, CONDITION_ALWAYS, 440, 0, 312, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
	{ 4, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
	{ SCENE_TABLE_END },
};

u32 RamakanSabaku_Exits[] = {
	SCENE_EXITS(89),
	SCENE_EXIT(1, 2, 18),
	SCENE_EXIT(2, 90, 1),
	SCENE_EXIT(0, 90, 0),
	SCENE_EXIT(1, 89, 2),
	SCENE_EXIT(2, 91, 1),
	SCENE_EXIT(0, 91, 0),
	SCENE_EXIT(1, 90, 2),
	SCENE_EXIT(2, 92, 2),
	SCENE_EXIT(3, 92, 1),
	SCENE_EXIT(4, 92, 3),
	SCENE_EXIT(0, 92, 0),
	SCENE_EXIT(1, 91, 2),
	SCENE_EXIT(2, 91, 3),
	SCENE_EXIT(3, 2, 53),
	SCENE_EXIT(4, 91, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement gRamakanSabakuPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gRamakanSabakuPlacements1[] = {
	{ 0xc3, 0x90a, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(400), 0, PIXELS(364), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gRamakanSabakuPlacements2[] = {
	{ 0xc3, 0x90a, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(352), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(736), 0, PIXELS(396), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(380), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(576), 0, PIXELS(716), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x5d, 0x6f, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gRamakanSabakuPlacements3[] = {
	{ 0xc3, 0x90a, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(104), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(316), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent RamakanSabaku_Events[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 4 },
	{ EVENT_TOUCH, 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlags8B2And8B3Steps) },
	{ EVENT_TOUCH, 20, 0x200, EVENT_SCRIPT(RamakanSabaku_SweepBack) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 22, 0x200, EVENT_SCRIPT(RamakanSabaku_FaceNearestActor) },
	{ EVENT_TOUCH, 30, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateDustAndProbe) },
	{ EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_PlayStartledRetreat) },
	{ EVENT_TALK, 8, 0x90a, EVENT_SCRIPT(RamakanSabaku_ReturnToArea3) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ApplyActor13Values3And3) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf7b, FIND_ITEM(0x24) },
	{ SEARCH(SEARCH_UNNAMED), 101, 0x350, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 102, 0xf7c, FIND_ITEM(0xc4) },
	{ SEARCH(SEARCH_UNNAMED), 103, 0x351, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 105, 0xf7d, FIND_COINS(777) },
	{ SEARCH(SEARCH_CHEST), 106, 0xf7e, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 107, 0xf7f, FIND_ITEM(0xc3) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_ConfigureAreaLayout) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_ApplyEntryState) },
	{ EVENT_RAISED, 1, 0x201, EVENT_SCRIPT(RamakanSabaku_ShowRandomPartyLine) },
	{ EVENT_RAISED, 2, 0x201, EVENT_SCRIPT(RamakanSabaku_ShowRandomPartyLine) },
	{ EVENT_RAISED, 3, 0x201, EVENT_SCRIPT(RamakanSabaku_ShowRandomPartyLine) },
	{ EVENT_TOUCH, 100, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 101, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 102, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 103, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 104, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 105, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 106, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ EVENT_TOUCH, 107, CONDITION_ALWAYS, EVENT_SCRIPT(RamakanSabaku_UpdateTravelDust) },
	{ SCENE_EVENTS_END },
};

s16 RamakanSabaku_SandLevel = 0;

s16 RamakanSabaku_SandCounter = 0;

s16 RamakanSabaku_SandFlash = 0;
