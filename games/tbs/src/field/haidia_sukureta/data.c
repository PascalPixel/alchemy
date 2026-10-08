#include "types.h"
#include "ability_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear(void);
void HaidiaSukureta_RunActorSequence(void);
void HaidiaSukureta_RunArrivalScene(void);
void OverlayObject_UpdateFacingTowardTarget(void);
void SceneDialogue_RunActorSixteenDialogue(void);
void SceneState_SetWorkAndFlag87d(void);
void SceneState_SetWorkAndFlag87e(void);
void Scene_OverhearSaturosAndMenardi(void);
void Scene_PlanSanctumVisit(void);
void Scene_RunActorTwelveDialogue(void);

s32 Sukureta_Actor11RockslideActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(294)),
	SC_SYNC,
	SC_MOVE(PIXELS(77), 0, PIXELS(329)),
	SC_SYNC,
	SC_MOVE(PIXELS(70), 0, PIXELS(415)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

s32 Sukureta_Actor11Actions[] = {
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, 2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 2048),
	SC_ADD(SW_SCALE_X, -1024),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 1024),
	SC_ADD(SW_SCALE_X, -512),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, 1024),
	SC_ADD(SW_SCALE_X, -512),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(60),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 Sukureta_StrangerActions[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance Placement_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 228, 0, 172, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 454, 0, 290, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 454, 0, 443, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 229, 0, 233, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 72, 0, 80, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 104, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 232, 0, 264, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(6),
	SCENE_EXIT(1, 4, 3),
	SCENE_EXIT(2, 4, 4),
	SCENE_EXIT(3, 8, 2),
	SCENE_EXIT(4, 93, 1),
	SCENE_EXIT(6, 93, 3),
	SCENE_EXIT(11, 3, 3),
	SCENE_EXIT(12, 3, 4),
	SCENE_EXIT(13, 8, 22),
	SCENE_EXITS_END,
};

struct ScenePlacement Placement_Actors[] = {
	{ 0x127, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(256), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(240), 0, PIXELS(353), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(384), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, CONDITION_ALWAYS, (s32)Sukureta_Actor11Actions,
		PIXELS(83), 0, PIXELS(273), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(268), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(1608), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x20, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(344), FACING_SOUTH + FACING_STEP, 3, 0 },
	{ 0x21, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(344), FACING_SOUTHEAST + FACING_STEP, 3, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Placement_Effects[] = {
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_PlanSanctumVisit) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunActorTwelveDialogue) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaDoNotKeepSukuretaWaiting) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgHaidiaLeaveBeforeWeChangeMinds) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaCuriousBeyondFence) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear) },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaSukureta_RunArrivalScene) },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_OverhearSaturosAndMenardi) },
	{ PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0 },
	{ SEARCH(SEARCH_JAR), 100, 0xf41, FIND_ITEM(ITEM_ELIXIR) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Placement_Effects834[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 12 },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgHaidiaRockCannotBeMoved) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaSukureta_RunActorSequence) },
	{ EVENT_TOUCH, 20, 0x82f, EVENT_SCRIPT(HaidiaSukureta_RunActorSequence) },
	{ EVENT_TOUCH, 10, 0x87d, EVENT_SCRIPT(SceneState_SetWorkAndFlag87d) },
	{ EVENT_TOUCH, 11, 0x87e, EVENT_SCRIPT(SceneState_SetWorkAndFlag87e) },
	{ SEARCH(SEARCH_JAR), 100, CONDITION_ALWAYS, 0 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent Placement_Effects87a[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupWithDescriptorA0ACWhenFlag242Clear) },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaWonderBeyondFence) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorSixteenDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgHaidiaAdventuresBeyondFence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgHaidiaLonelyWithoutSukureta) },
	{ PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0 },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	{ SEARCH(SEARCH_JAR), 100, 0xf41, FIND_ITEM(ITEM_ELIXIR) },
#endif
	{ SCENE_EVENTS_END },
};

u16 Sukureta_GateCells[] = {
	MAP_CELL_STEP(2, 36, 2, 2, 1), MAP_CELL_STEP(4, 36, 2, 2, 1), MAP_CELL_STEPS_END,
};
