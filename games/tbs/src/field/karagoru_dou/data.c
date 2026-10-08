#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void ActorPresentation_RunActorEightThresholdScene(void);
void ActorPresentation_RunActorElevenRecoveryScene(void);
void ActorPresentation_RunActorNineThresholdScene(void);
void ActorPresentation_SelectActorNineScript(void);
void FieldScene_RunScene3beSequenceB(void);
void KaragoruDou_AskToCross(void);
void KaragoruDou_LeaveArena(void);
void StagedActorPairScene_RunActorTwelveCommand(void);
void StagedActorPairScene_RunStep(void);
void StagedActorPairScene_RunUpdate(void);

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

struct SceneEntrance gKaragoruDouEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKaragoruDouEntrances1[] = {
	{ 1, CONDITION_ALWAYS, 152, 0, 120, FACING_SOUTH, 0, 32, 32, 272, 480, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 440, FACING_NORTH, 0, 32, 32, 272, 480, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKaragoruDouEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 728, 0, 312, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 408, FACING_NORTH, 0, 96, 48, 768, 432, 0 },
	{ 3, CONDITION_ALWAYS, 168, 0, 520, FACING_SOUTH, 0, 112, 464, 576, 704, 0 },
	{ 4, CONDITION_ALWAYS, 504, 0, 664, FACING_NORTH, 0, 112, 464, 576, 704, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKaragoruDouEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 792, 0, 456, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
	{ 2, CONDITION_ALWAYS, 360, 0, 152, FACING_SOUTH, 0, 240, 48, 864, 464, 0 },
	{ 3, CONDITION_ALWAYS, 344, 0, 440, FACING_NORTH, 0, 240, 48, 864, 464, 0 },
	{ 4, CONDITION_ALWAYS, 344, 0, 584, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
	{ 5, CONDITION_ALWAYS, 136, 0, 504, FACING_SOUTH, 0, 48, 432, 408, 896, 0 },
	{ 6, CONDITION_ALWAYS, 216, 0, 888, FACING_NORTH, 0, 48, 432, 408, 896, 0 },
	{ SCENE_TABLE_END },
};

u32 gKaragoruDouExits[] = {
	SCENE_EXITS(152),
	SCENE_EXIT(1, 2, 25),
	SCENE_EXIT(2, 2, 54),
	SCENE_EXIT(0, 157, 0),
	SCENE_EXIT(1, 158, 2),
	SCENE_EXIT(2, 157, 3),
	SCENE_EXIT(3, 157, 2),
	SCENE_EXIT(4, 158, 5),
	SCENE_EXIT(0, 158, 0),
	SCENE_EXIT(1, 2, 26),
	SCENE_EXIT(2, 157, 1),
	SCENE_EXIT(3, 158, 4),
	SCENE_EXIT(4, 158, 3),
	SCENE_EXIT(5, 157, 4),
	SCENE_EXIT(6, 2, 41),
	SCENE_EXITS_END,
};

struct ScenePlacement gKaragoruDouPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements1[] = {
	{ 0x96, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(144), 0, PIXELS(168), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements1Flag96f[] = {
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(120), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(184), 0, PIXELS(120), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x39, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(232), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements2[] = {
	{ 0x127, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(624), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(440), 0, PIXELS(104), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x14, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKaragoruDouPlacements3[] = {
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(304), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x127, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(636), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(640), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKaragoruDouEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(KaragoruDou_AskToCross) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouUsWereTheSoldiersWhoGuard) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouEvenIfYouHeadEastYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouThoseWhoGoToGondowanRarely) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SelectActorNineScript) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouYoudThinkYouCouldGetThrough) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents1Flag96f[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3beSequenceB) },
	{ EVENT_TOUCH, 11, 0x9a0, EVENT_SCRIPT(ActorPresentation_RunActorElevenRecoveryScene) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouWhyWouldIodemChooseToWait) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouWeHaventSeenAnybodyUnusualCrossing) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouEvenIfYouGoEastFrom) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorElevenRecoveryScene) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouIodemIsGoingToBabiLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouItsALotOfStressBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouEveryoneComesDownThisWayWhen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKaragoruDouWeHaveToHeadForThe) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ PSYNERGY_ON_ACTOR(0x8f), 8, CONDITION_ALWAYS, 0 },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKaragoruDouEvents3[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(KaragoruDou_LeaveArena) },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ PSYNERGY_ON_ACTOR(0x8f), 11, CONDITION_ALWAYS, EVENT_SCRIPT(StagedActorPairScene_RunActorTwelveCommand) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x9a2, EVENT_SCRIPT(ActorPresentation_RunActorEightThresholdScene) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x205, EVENT_SCRIPT(ActorPresentation_RunActorNineThresholdScene) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(StagedActorPairScene_RunStep) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(StagedActorPairScene_RunStep) },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(StagedActorPairScene_RunUpdate) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfa0, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfa1, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SCENE_EVENTS_END },
};
