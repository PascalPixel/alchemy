#include "types.h"
#include "ability_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

void FieldScene_RunActor11Step(void);
void FieldScene_RunActor12Step(void);
void FieldScene_RunFlaggedDisplayScene(void);
void FieldScene_RunSingleStep(void);
void FieldScene_RunTransitionOrFallback(void);
void SceneActor_ApplyCounterLowBitsAsMode(void);
void SceneState_ApplyPlacementResult(void);
void StagedActor_PlaceAtObjectTenCell(void);
void TakaraHashira_AttachPillarRider(void);
void TakaraHashira_CheckLeaderOnPillar(void);
void TakaraHashira_DropActorTen(void);
void TakaraHashira_PushPillar(void);
void TakaraHashira_RaiseActorFourteen(void);
void TakaraHashira_ReleasePillarRider(void);
void TakaraHashira_RunMapShiftScene(void);
void TakaraHashira_RunPushScene(void);
void TakaraHashira_RunStagedCellScene(void);
void TakaraHashira_UpdatePillarActors(void);

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

s16 StagedActor_DirectionSteps[] = {
	0, 16,
	0, 16,
	0, 16,
	16, 0,
	16, 0,
	16, 0,
	16, 0,
	0, -16,
	0, -16,
	0, -16,
	0, -16,
	-16, 0,
	-16, 0,
	-16, 0,
	-16, 0,
	0, 16,
};

u32 StagedActor_FootprintKinds[] = {
	207, 205, 228, 229, 298, 297,
};

s32 StagedActor_FootprintBounds[] = {
	-32, -8, 32, 8,
	-8, -32, 8, 32,
	-32, -16, 32, 0,
	-8, -32, 8, 32,
	-32, -8, 32, 8,
	-8, -32, 8, 32,
};

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 TakaraHashira_ActionTable[] = {
	SC_SET(0x23, 2),
	SC_CALL(SceneActor_ApplyCounterLowBitsAsMode),
	SC_WAIT(2),
	SC_SET(0x23, 0),
	SC_CALL(SceneActor_ApplyCounterLowBitsAsMode),
	SC_WAIT(6),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 TakaraHashira_ShiftSteps1[] = {
	MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps2[] = {
	MAP_CELL_STEP(79, 34, 1, 2, 4), MAP_CELL_STEP(78, 34, 1, 2, 4), MAP_CELL_STEP(77, 34, 1, 2, 4), MAP_CELL_STEP(76, 34, 1, 2, 4), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps3[] = {
	MAP_CELL_STEP(67, 34, 2, 5, 6), MAP_CELL_STEP(69, 34, 2, 5, 6), MAP_CELL_STEP(71, 34, 2, 5, 6), MAP_CELL_STEP(73, 34, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps4[] = {
	MAP_CELL_STEP(71, 50, 2, 5, 6), MAP_CELL_STEP(69, 50, 2, 5, 6), MAP_CELL_STEP(67, 50, 2, 5, 6), MAP_CELL_STEP(65, 50, 2, 5, 6), MAP_CELL_STEPS_END,
};

u16 TakaraHashira_ShiftSteps5[] = {
	MAP_CELL_STEP(75, 38, 2, 1, 4), MAP_CELL_STEP(77, 38, 2, 1, 6), MAP_CELL_STEP(79, 38, 2, 1, 8), MAP_CELL_STEP(65, 53, 2, 1, 1), MAP_CELL_STEPS_END,
};

struct SceneEntrance gTakaraHashiraEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 712, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
	{ 2, CONDITION_ALWAYS, 600, 0, 760, FACING_NORTH, 0, 560, 368, 992, 792, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 216, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
	{ 2, CONDITION_ALWAYS, 104, 0, 360, FACING_NORTH, 0, 32, 32, 384, 400, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 184, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 64, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 744, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
	{ 2, CONDITION_ALWAYS, 632, 0, 520, FACING_NORTH, 0, 440, 16, 960, 544, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraHashiraEntrances5[] = {
	{ 1, CONDITION_ALWAYS, 280, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 488, FACING_NORTH, 0, 16, 32, 464, 528, 0 },
	{ SCENE_TABLE_END },
};

u32 gTakaraHashiraExits[] = {
	SCENE_EXITS(115),
	SCENE_EXIT(1, 126, 3),
	SCENE_EXIT(2, 126, 4),
	SCENE_EXIT(0, 116, 0),
	SCENE_EXIT(1, 127, 3),
	SCENE_EXIT(2, 127, 4),
	SCENE_EXIT(0, 119, 0),
	SCENE_EXIT(1, 130, 3),
	SCENE_EXIT(2, 130, 4),
	SCENE_EXIT(0, 121, 0),
	SCENE_EXIT(1, 132, 3),
	SCENE_EXIT(2, 132, 4),
	SCENE_EXIT(0, 122, 0),
	SCENE_EXIT(1, 133, 3),
	SCENE_EXIT(2, 133, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement gTakaraHashiraNoPlacements[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements1[] = {
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(744), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(552), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(696), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(776), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements2[] = {
	{ 0x12a, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x12a, CONDITION_ALWAYS, 7,
		PIXELS(256), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x12a, CONDITION_ALWAYS, 7,
		PIXELS(224), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x129, CONDITION_ALWAYS, 7,
		PIXELS(280), 0, PIXELS(160), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x129, CONDITION_ALWAYS, 7,
		PIXELS(184), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements3[] = {
	{ 0xcd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(208), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(272), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(352), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd6, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacementsOther[] = {
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(184), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(680), 0, PIXELS(392), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x100, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraHashiraPlacements5[] = {
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(208), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x125, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(296), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(296), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x122, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gTakaraHashiraEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTransitionOrFallback) },
	{ SEARCH(SEARCH_CHEST), 100, 0xec2, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_CHEST), 101, 0xec3, FIND_ITEM(ITEM_NUT) },
	{ SEARCH(SEARCH_CHEST), 102, 0xec4, FIND_COINS(111) },
	{ SEARCH(SEARCH_CHEST), 103, 0xec5, FIND_ITEM(0xc2) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS, 0 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyPlacementResult) },
	{ SEARCH(SEARCH_CHEST), 100, 0xec6, FIND_ITEM(0xc4) },
	{ SEARCH(SEARCH_CHEST), 101, 0xec7, FIND_ITEM(0x18) },
	{ SEARCH(SEARCH_CHEST), 102, 0xec8, FIND_COINS(222) },
	{ SEARCH(SEARCH_CHEST), 103, 0xec9, FIND_ITEM(ITEM_ELIXIR) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyPlacementResult) },
	{ 0x413, 100, 0xed2, FIND_COINS(555) },
	{ 0xc413, 100, 0xed2, FIND_COINS(555) },
	{ 0x413, 101, 0xed3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ 0xe413, 101, 0xed3, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 102, 0xed4, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 103, 0xed5, FIND_ITEM(0x62) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlaggedDisplayScene) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents4[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 30, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_PushPillar) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 31, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_RunStagedCellScene) },
	{ 0x202, 31, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_PushPillar) },
	{ EVENT_TOUCH, 40, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_AttachPillarRider) },
	{ EVENT_TOUCH, 41, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_ReleasePillarRider) },
	{ SEARCH(SEARCH_CHEST), 100, 0xeda, FIND_COINS(777) },
	{ SEARCH(SEARCH_CHEST), 101, 0xedb, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ 0x413, 102, 0xedc, FIND_ITEM(0xba) },
	{ SEARCH(SEARCH_CHEST), 103, 0xedd, FIND_ITEM(0x32) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x200, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x201, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x202, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x203, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 8, 0x200, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 9, 0x201, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 10, 0x202, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ PSYNERGY_ON_ACTOR(0x93), 11, 0x203, EVENT_SCRIPT(TakaraHashira_UpdatePillarActors) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraHashiraEvents5[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraHashira_RunPushScene) },
	{ SEARCH(SEARCH_CHEST), 100, 0xede, FIND_COINS(888) },
	{ SEARCH(SEARCH_CHEST), 101, 0xedf, FIND_ITEM(0xb7) },
	{ SEARCH(SEARCH_CHEST), 102, 0xee0, FIND_ITEM(0x10c) },
	{ SEARCH(SEARCH_CHEST), 103, 0xee1, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ 0x10008c15, 10, 0x204, EVENT_SCRIPT(StagedActor_PlaceAtObjectTenCell) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x204, EVENT_SCRIPT(TakaraHashira_DropActorTen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, 0x200, EVENT_SCRIPT(FieldScene_RunActor11Step) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, 0x201, EVENT_SCRIPT(FieldScene_RunActor12Step) },
	{ 0x10001815, 13, 0x202, EVENT_SCRIPT(TakaraHashira_CheckLeaderOnPillar) },
	{ 0x50001815, 13, 0x202, EVENT_SCRIPT(TakaraHashira_RaiseActorFourteen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x202, EVENT_SCRIPT(TakaraHashira_RunMapShiftScene) },
	{ SCENE_EVENTS_END },
};
