#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void Dialogue_ShowMessages8fbAnd8fc(void);
void Scene_RunActorNinePromptDialogue(void);
void Scene_RunActorTenRepeatedMotion(void);
void Scene_RunPrimarySequence(void);
void Scene_RunScene3c3SequenceA(void);
void Scene_RunScene3c3SequenceC(void);
void Scene_SetActor13Value1A(void);

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
s32 SuharaGate_PatrolStartup[] = {
	SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuharaGatePlacements1Flagged1[] = {
	SC_ARG(5),
	SC_END,
};

#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
s32 SuharaGate_FirstGuardStartup[] = {
	SC_SET(SW_COLLISION, 0),
};
#endif

s32 gSuharaGatePlacements1Flagged0[] = {
	SC_SET(SW_SCALE_X, -0x10000),
	SC_ARG(5),
	SC_END,
};

struct SceneEntrance gSuharaGateEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 152, 0, 504, FACING_SOUTH, 0, 32, 32, 272, 544, 0 },
	{ 1, CONDITION_ALWAYS, 152, 0, 520, FACING_NORTH, 0, 32, 32, 272, 544, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 280, FACING_WEST, 0, 32, 32, 272, 544, 0 },
	{ 3, CONDITION_ALWAYS, 136, 0, 304, FACING_SOUTH, 0, 32, 32, 272, 544, 0 },
	{ 4, CONDITION_ALWAYS, 360, 0, 296, FACING_EAST, 0, 336, 32, 736, 496, 0 },
	{ 5, CONDITION_ALWAYS, 664, 0, 424, FACING_WEST, 0, 336, 32, 736, 496, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaGateEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 40, 0, 304, FACING_EAST, 0, 16, 32, 416, 480, 0 },
	{ 2, CONDITION_ALWAYS, 392, 0, 368, FACING_WEST, 0, 16, 32, 416, 480, 0 },
	{ 3, CONDITION_ALWAYS, 40, 0, 912, FACING_EAST, 0, 16, 608, 720, 1008, 0 },
	{ 4, CONDITION_ALWAYS, 680, 0, 904, FACING_WEST, 0, 16, 608, 720, 1008, 0 },
	{ 5, CONDITION_ALWAYS, 616, 0, 920, FACING_SOUTH, 0, 16, 608, 720, 1008, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gSuharaGateEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 72, 0, 264, FACING_NORTH, 0, 24, 24, 280, 256, 0 },
	{ 2, CONDITION_ALWAYS, 184, 0, 168, FACING_SOUTH, 0, 24, 24, 280, 256, 0 },
	{ 3, CONDITION_ALWAYS, 280, 0, 344, FACING_SOUTH, 0, 208, 288, 704, 640, 0 },
	{ 4, CONDITION_ALWAYS, 664, 0, 616, FACING_NORTH, 0, 208, 288, 704, 640, 0 },
	{ SCENE_TABLE_END },
};

u32 SuharaGate_SceneTable[] = {
	SCENE_EXITS(169),
	SCENE_EXIT(1, 2, 29),
	SCENE_EXIT(2, 170, 1),
	SCENE_EXIT(3, 105, 10),
	SCENE_EXIT(4, 170, 4),
	SCENE_EXIT(5, 2, 42),
	SCENE_EXIT(0, 170, 0),
	SCENE_EXIT(1, 169, 2),
	SCENE_EXIT(2, 170, 3),
	SCENE_EXIT(3, 170, 2),
	SCENE_EXIT(4, 169, 4),
	SCENE_EXIT(5, 171, 1),
	SCENE_EXIT(0, 171, 0),
	SCENE_EXIT(1, 170, 5),
	SCENE_EXIT(2, 171, 3),
	SCENE_EXIT(3, 171, 2),
	SCENE_EXIT(4, 2, 33),
	SCENE_EXITS_END,
};

struct ScenePlacement gSuharaGatePlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaGatePlacements2[] = {
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(264), 0, PIXELS(400), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(280), 0, PIXELS(368), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(472), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(536), 0, PIXELS(912), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xee, CONDITION_ALWAYS, 7,
		PIXELS(552), 0, PIXELS(880), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x5b, 0x4a, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(888), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaGatePlacements1[] = {
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gSuharaGatePlacements1Flagged[] = {
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{ 0x98, CONDITION_ALWAYS, (s32)gSuharaGatePlacements1Flagged0,
		PIXELS(136), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)gSuharaGatePlacements1Flagged1,
		PIXELS(168), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)gSuharaGatePlacements1Flagged1,
		PIXELS(472), 0, PIXELS(304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#else
	{ 0x98, CONDITION_ALWAYS, (s32)SuharaGate_FirstGuardStartup,
		PIXELS(140), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)SuharaGate_PatrolStartup,
		PIXELS(164), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x98, CONDITION_ALWAYS, (s32)SuharaGate_PatrolStartup,
		PIXELS(476), 0, PIXELS(300), FACING_SOUTH, TALK_FACE_PARTY, 0 },
#endif
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gSuharaGateEventsOther[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(Dialogue_ShowMessages8fbAnd8fc) },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_TOUCH, 5, CONDITION_ALWAYS, EVENT_SCRIPT(Dialogue_ShowMessages8fbAnd8fc) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunScene3c3SequenceC) },
	{ EVENT_TOUCH, 10, 0x96f, EVENT_SCRIPT(Scene_RunPrimarySequence) },
	{ EVENT_TOUCH, 10, 0x89f, EVENT_SCRIPT(Scene_RunScene3c3SequenceA) },
	{ EVENT_TALK, 8, 0x89f, MSGNO(MsgSuharaTravelPapersRequired) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgSuharaSendHelpFromTower) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunActorNinePromptDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x89f, MSGNO(MsgSuharaObeyBabiOrders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgSuharaAshamedBeforeYodemu) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x89f, MSGNO(MsgSuharaSeeKorosseoNextYear) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgSuharaSurvivedAttack) },
	{ EVENT_TALK, 10, 0x897, EVENT_SCRIPT(Scene_RunActorTenRepeatedMotion) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40a, 0x897, EVENT_SCRIPT(Scene_RunActorTenRepeatedMotion) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaGateEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_SetActor13Value1A) },
	{ 0xc413, 100, 0xfb4, FIND_ITEM(0xc3) },
	{ 0x413, 100, 0xfb4, FIND_ITEM(0xc3) },
	{ 0x8413, 100, 0xfb4, FIND_ITEM(0xc3) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gSuharaGateEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ 0x31, 2, CONDITION_ALWAYS, 2 },
	{ 0x21, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE },
	{ SCENE_EVENTS_END },
};
