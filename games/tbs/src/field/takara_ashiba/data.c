#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

void ConfigureAndPlaceActorTwelve(void);
void FieldScene_CopyActorPosition(void);
void FieldScene_DispatchByActorZeroFacing(void);
void FieldScene_DispatchBySlotZeroFacing(void);
void FieldScene_RunActor15ZeroStep(void);
void FieldScene_RunIndexedStep0(void);
void FieldScene_RunIndexedStep1(void);
void FieldScene_RunLateIndexedStep0(void);
void FieldScene_RunLateIndexedStep1(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunScene3b4SequenceA(void);
void FieldScene_RunScene3b4SequenceB(void);
void FieldScene_RunScene3b4SequenceC(void);
void FieldScene_RunSingleStep(void);
void FieldScene_RunStep10ValueEe9(void);
void FieldScene_RunStep11ValueEea(void);
void FieldScene_RunStep12ValueEeb(void);
void FieldScene_RunStep8ValueEe7(void);
void FieldScene_RunStep9ValueEe8(void);
void FieldScene_RunSupplementalSequenceOne(void);
void FieldScene_RunThreeCallSequence(void);
void FieldScene_RunThreeCallSequenceB(void);
void FieldScene_RunTwoStepSequence(void);
void Resource3b4_EmptyHookB(void);
void SceneActor_BranchOnSlotZeroAtTile38(void);
void SceneActor_MarkSlot13AndSetFlag200(void);
void SceneActor_RunWhenActor9AtTile45x43(void);
void SceneActor_SetActor15ModeZero(void);
void SceneEffect_SetBlendAlpha0607(void);
void SceneState_ApplyRectAndPlaceSlot12(void);
void SceneState_BranchOnActorZeroFacing(void);
void SetBlendAlphaCoefficients(void);
void TakaraAshiba_CheckLeaderAhead(void);
void TakaraAshiba_DispatchByActorEightColumn(void);
void TakaraAshiba_RepaintActorCells(void);
void TakaraAshiba_ResetBlockedStepsForSelectorThree(void);
void TakaraAshiba_ResetBlockedStepsForSelectorTwo(void);
void TakaraAshiba_ReturnActorFifteen(void);
void TakaraAshiba_RunActorElevenFollowScene(void);
void TakaraAshiba_RunActorFifteenSprint(void);
void TakaraAshiba_StartActorFifteenJump(void);
void TakaraAshiba_UpdateBlockRects(void);

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

s32 TakaraAshiba_ActorFifteenMotionScript[] = {
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(64),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 TakaraAshiba_ActorFifteenWalkTargets[] = {
	0x378, 0x358,
	0x368, 0x358,
	0x358, 0x358,
	0x348, 0x358,
	0x338, 0x358,
	0x338, 0x348,
	0x338, 0x338,
	0x348, 0x338,
	0x358, 0x338,
	0x368, 0x338,
	0x378, 0x338,
	0x378, 0x348,
};

struct SceneEntrance gTakaraAshibaEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraAshibaEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 200, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
	{ 2, CONDITION_ALWAYS, 88, 0, 968, FACING_NORTH, 0, 24, 560, 504, 1008, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraAshibaEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 200, 0, 408, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
	{ 2, CONDITION_ALWAYS, 88, 0, 392, FACING_NORTH, 0, 40, 0, 464, 440, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gTakaraAshibaEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 776, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
	{ 2, CONDITION_ALWAYS, 664, 0, 952, FACING_NORTH, 0, 512, 544, 928, 992, 0 },
	{ SCENE_TABLE_END },
};

u32 TakaraAshiba_SceneTable[] = {
	SCENE_EXITS(117),
	SCENE_EXIT(1, 128, 3),
	SCENE_EXIT(2, 128, 4),
	SCENE_EXIT(0, 118, 0),
	SCENE_EXIT(1, 129, 3),
	SCENE_EXIT(2, 129, 4),
	SCENE_EXIT(0, 120, 0),
	SCENE_EXIT(1, 131, 3),
	SCENE_EXIT(2, 131, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement gTakaraAshibaPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraAshibaPlacements1[] = {
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(1016), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraAshibaPlacements2[] = {
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(152), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(312), 0, PIXELS(232), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gTakaraAshibaPlacements3[] = {
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(616), 0, PIXELS(680), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(728), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(600), 0, PIXELS(856), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(824), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(888), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x14, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(728), 0, PIXELS(760), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5a, 0x36, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gTakaraAshibaEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraAshibaEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectAndPlaceSlot12) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureAndPlaceActorTwelve) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep8ValueEe7) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep9ValueEe8) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep10ValueEe9) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep11ValueEea) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep12ValueEeb) },
	{ EVENT_TOUCH, 50, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3b4SequenceC) },
	{ EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep0) },
	{ EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep1) },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraAshiba_ResetBlockedStepsForSelectorTwo) },
	{ EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraAshiba_ResetBlockedStepsForSelectorThree) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraAshiba_CheckLeaderAhead) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 54, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraAshiba_CheckLeaderAhead) },
	{ SEARCH(SEARCH_CHEST), 100, 0xeca, FIND_COINS(333) },
	{ SEARCH(SEARCH_CHEST), 101, 0xecb, FIND_ITEM(0x10b) },
	{ SEARCH(SEARCH_CHEST), 102, 0xecc, FIND_ITEM(0xc0) },
	{ SEARCH(SEARCH_CHEST), 103, 0xecd, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraAshibaEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateIndexedStep0) },
	{ EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunLateIndexedStep1) },
	{ EVENT_TOUCH, 61, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_TOUCH, 62, CONDITION_ALWAYS, EVENT_SCRIPT(TakaraAshiba_RunActorElevenFollowScene) },
	{ EVENT_RAISED, 96, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3b4SequenceB) },
	{ EVENT_RAISED, 92, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3b4SequenceA) },
	{ EVENT_RAISED, 91, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceOne) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SetBlendAlphaCoefficients) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_SetBlendAlpha0607) },
	{ PSYNERGY_ON_ACTOR(0x91), 8, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 10, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 11, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ PSYNERGY_ON_ACTOR(0x91), 12, CONDITION_ALWAYS, EVENT_SCRIPT(Resource3b4_EmptyHookB) },
	{ SEARCH(SEARCH_CHEST), 100, 0xece, FIND_COINS(444) },
	{ SEARCH(SEARCH_CHEST), 101, 0xecf, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_CHEST), 102, 0xed0, FIND_ITEM(0x60) },
	{ SEARCH(SEARCH_CHEST), 103, 0xed1, FIND_ITEM(0xba) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gTakaraAshibaEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeCallSequenceB) },
	{ 0x202, 51, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_BranchOnSlotZeroAtTile38) },
	{ 0x202, 52, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTwoStepSequence) },
	{ 0x202, 53, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeCallSequence) },
#if EDITION_INTERNATIONAL
	{ 0x202, 61, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_BranchOnSlotZeroAtTile38) },
#endif
	{ 0x202, 54, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchByActorZeroFacing) },
	{ 0x202, 60, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DispatchBySlotZeroFacing) },
	{ 0x202, 70, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_BranchOnActorZeroFacing) },
	{ EVENT_TOUCH, 20, 0x36, EVENT_SCRIPT(TakaraAshiba_StartActorFifteenJump) },
	{ EVENT_TOUCH, 21, 0x36, EVENT_SCRIPT(TakaraAshiba_RunActorFifteenSprint) },
	{ EVENT_TOUCH, 22, 0x36, EVENT_SCRIPT(TakaraAshiba_ReturnActorFifteen) },
	{ EVENT_TOUCH, 30, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 31, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 32, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 33, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 34, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 35, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 36, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 37, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 38, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 39, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 40, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ EVENT_TOUCH, 41, 0x36, EVENT_SCRIPT(FieldScene_CopyActorPosition) },
	{ 0x7, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor15ModeZero) },
	{ PSYNERGY_ON_ACTOR(0x91), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor15ZeroStep) },
	{ PSYNERGY_ON_ACTOR(ABILITY_FROST), 13, 0x200, EVENT_SCRIPT(SceneActor_MarkSlot13AndSetFlag200) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x201, EVENT_SCRIPT(TakaraAshiba_DispatchByActorEightColumn) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x206, EVENT_SCRIPT(SceneActor_RunWhenActor9AtTile45x43) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x203, EVENT_SCRIPT(TakaraAshiba_UpdateBlockRects) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x204, EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, 0x205, EVENT_SCRIPT(TakaraAshiba_RepaintActorCells) },
	{ SEARCH(SEARCH_CHEST), 104, 0xed6, FIND_COINS(666) },
	{ SEARCH(SEARCH_CHEST), 105, 0xed7, FIND_ITEM(ITEM_WATER_OF_LIFE) },
	{ SEARCH(SEARCH_CHEST), 106, 0xed8, FIND_ITEM(ITEM_ELIXIR) },
	{ SEARCH(SEARCH_CHEST), 107, 0xed9, FIND_ITEM(0x26) },
	{ SCENE_EVENTS_END },
};

u32 TakaraAshiba_SlotColumns[] = {
	16, 21, 0,
};

s32 TakaraAshiba_IconTimer = 0;
