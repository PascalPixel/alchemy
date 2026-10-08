#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"

void FieldScene_RunActor10Step(void);
void FieldScene_RunActor11Step(void);
void FieldScene_RunActor12Step(void);
void FieldScene_RunActor8Step(void);
void FieldScene_RunActor9Step(void);
void FieldScene_RunActorThirteenRestoration(void);
void FieldScene_RunLateActor10Step(void);
void FieldScene_RunLateActor11Step(void);
void FieldScene_RunLateActor12Step(void);
void FieldScene_RunLateActor8Step(void);
void FieldScene_RunLateActor9Step(void);
void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene3c0SequenceA(void);
void PlaceActorTwelveWhenFlagClear(void);
void SceneState_SendRequest15With45(void);
void SuharaSabaku_ClearActorSpriteAndCollisionFlags(void);
void SuharaSabaku_DropAndLeave(void);
void SuharaSabaku_EnableEffectChildOption(void);
void SuharaSabaku_FollowLeaderWithSparks(void);
void SuharaSabaku_MeetActor(void);
void SuharaSabaku_ToggleActorActive(void);

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

s32 gSuharaSabakuPlacements1_Data[] = {
	SC_CALL(SuharaSabaku_ClearActorSpriteAndCollisionFlags),
	SC_CALL(SuharaSabaku_ToggleActorActive),
	SC_END,
};

s32 gSuharaSabakuPlacements2_1[] = {
	SC_CALL(SuharaSabaku_ClearActorSpriteAndCollisionFlags),
	SC_CALL(SuharaSabaku_ToggleActorActive),
	SC_END,
};

struct SceneEntrance gSuharaSabakuEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaSabakuEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 264, 0, 56, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 936, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaSabakuEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 488, 0, 456, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 488, 0, 1208, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 440, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 56, 0, 624, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaSabakuEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 72, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 440, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 632, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 648, 0, 280, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gSuharaSabakuExits[] = {
	SCENE_EXITS(164),
	SCENE_EXIT(1, 2, 28),
	SCENE_EXIT(2, 165, 1),
	SCENE_EXIT(0, 165, 0),
	SCENE_EXIT(1, 164, 2),
	SCENE_EXIT(2, 2, 55),
	SCENE_EXIT(3, 166, 4),
	SCENE_EXIT(4, 166, 1),
	SCENE_EXIT(0, 166, 0),
	SCENE_EXIT(1, 165, 4),
	SCENE_EXIT(2, 166, 3),
	SCENE_EXIT(3, 166, 2),
	SCENE_EXIT(4, 165, 3),
	SCENE_EXITS_END,
};

struct ScenePlacement gSuharaSabakuPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaSabakuPlacements1[] = {
	{ 0x121, 0x301, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(56), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x302, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(456), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x303, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(120), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x304, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(264), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x305, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(440), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaSabakuPlacements2[] = {
	{ 0x121, 0x311, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x312, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(296), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x313, (s32)gSuharaSabakuPlacements1_Data,
		PIXELS(408), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x121, 0x314, (s32)gSuharaSabakuPlacements2_1,
		PIXELS(376), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x121, 0x315, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5c, 0x5d, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaSabakuPlacements3[] = {
	{ SCENE_TABLE_END },
};

s32 gSuharaSabakuActor12Action[] = {
	SC_CALL(SuharaSabaku_FollowLeaderWithSparks),
	SC_END,
};

struct SceneEvent gSuharaSabakuEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
#if !EDITION_INTERNATIONAL
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
#else
	{ 0xc401, 4, CONDITION_ALWAYS, 4 },
#endif
	{ EVENT_TOUCH, 9, 0x9b5, EVENT_SCRIPT(FieldScene_RunActorThirteenRestoration) },
	{ EVENT_TOUCH, 41, 0x301, EVENT_SCRIPT(FieldScene_RunActor8Step) },
	{ EVENT_TOUCH, 42, 0x302, EVENT_SCRIPT(FieldScene_RunActor9Step) },
	{ EVENT_TOUCH, 43, 0x303, EVENT_SCRIPT(FieldScene_RunActor10Step) },
	{ EVENT_TOUCH, 44, 0x304, EVENT_SCRIPT(FieldScene_RunActor11Step) },
	{ EVENT_TOUCH, 45, 0x305, EVENT_SCRIPT(FieldScene_RunActor12Step) },
	{ EVENT_TOUCH, 51, 0x311, EVENT_SCRIPT(FieldScene_RunActor8Step) },
	{ EVENT_TOUCH, 52, 0x312, EVENT_SCRIPT(FieldScene_RunActor9Step) },
	{ EVENT_TOUCH, 53, 0x313, EVENT_SCRIPT(FieldScene_RunActor10Step) },
	{ EVENT_TOUCH, 61, 0x206, EVENT_SCRIPT(FieldScene_RunLateActor8Step) },
	{ EVENT_TOUCH, 62, 0x207, EVENT_SCRIPT(FieldScene_RunLateActor9Step) },
	{ EVENT_TOUCH, 63, 0x208, EVENT_SCRIPT(FieldScene_RunLateActor10Step) },
	{ EVENT_TOUCH, 64, 0x209, EVENT_SCRIPT(FieldScene_RunLateActor11Step) },
	{ EVENT_TOUCH, 65, 0x20a, EVENT_SCRIPT(FieldScene_RunLateActor12Step) },
	{ EVENT_TOUCH, 71, 0x206, EVENT_SCRIPT(FieldScene_RunLateActor8Step) },
	{ EVENT_TOUCH, 72, 0x207, EVENT_SCRIPT(FieldScene_RunLateActor9Step) },
	{ EVENT_TOUCH, 73, 0x208, EVENT_SCRIPT(FieldScene_RunLateActor10Step) },
	{ EVENT_TOUCH, 54, 0x314, EVENT_SCRIPT(FieldScene_RunActor11Step) },
	{ EVENT_TOUCH, 81, 0x20e, EVENT_SCRIPT(PlaceActorTwelveWhenFlagClear) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SendRequest15With45) },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x351, FIND_PSYNERGY_STONE },
	{ SEARCH(SEARCH_CHEST), 103, 0xfb1, FIND_ITEM(0x8d) },
	{ SEARCH(SEARCH_CHEST), 104, 0xfb2, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfb3, FIND_ITEM(0xc0) },
	{ SEARCH(SEARCH_CHEST), 102, 0xef6, 0x500006 },
	{ 0x10002115, 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ 0x10002115, 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_EnableEffectChildOption) },
	{ PSYNERGY_ON_ACTOR(0x21), 8, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 9, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 10, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 11, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_ON_ACTOR(0x21), 12, IF_FLAG_SET(0x20f), EVENT_SCRIPT(SuharaSabaku_MeetActor) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c0SequenceA) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningAuxiliarySequence) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(SuharaSabaku_DropAndLeave) },
	{ EVENT_RAISED, 55, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor12Step) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaSabakuEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SCENE_EVENTS_END },
};
