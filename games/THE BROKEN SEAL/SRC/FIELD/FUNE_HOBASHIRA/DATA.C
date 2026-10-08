#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void FieldScene_RunActor232SceneWhenFlag923Or922(void);
void FuneHobashira_SwayActor(void);
void FuneHobashira_UpdateGridPlacement(void);
void Object_PlaceFromCameraOffset(void);
void OverlayObject_ShrinkScaleThenStop(void);
void SceneActor_SetFacingFromSample(void);

s32 FuneHobashira_FollowCameraActions[] = {
	SC_ADD(SW_SCALE_X, 2048),
	SC_ADD(SW_SCALE_Y, 2048),
	SC_CALL(Object_PlaceFromCameraOffset),
	SC_WAIT(1),
	SC_LOOP(10, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -2048),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_CALL(Object_PlaceFromCameraOffset),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 FuneHobashira_LookoutActions[] = {
	SC_CALL(SceneActor_SetFacingFromSample),
};

s32 FuneHobashira_DriftActions[] = {
	SC_CALL(FuneHobashira_SwayActor),
};

s32 FuneHobashira_WaveActions[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_ADD(SW_SCALE_X, -0x5000),
	SC_ADD(SW_SCALE_Y, 0x1000),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x2800),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, 0x1400),
	SC_ADD(SW_SCALE_Y, -1024),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 FuneHobashira_EnsembleWalk9[] = {
	SC_MOVE(PIXELS(72), 0, PIXELS(216)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 FuneHobashira_EnsembleWalk10[] = {
	SC_MOVE(PIXELS(132), 0, PIXELS(242)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 FuneHobashira_EnsembleWalk11[] = {
	SC_MOVE(PIXELS(88), 0, PIXELS(166)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 FuneHobashira_EnsembleWalk12[] = {
	SC_MOVE(PIXELS(220), 0, PIXELS(134)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 FuneHobashira_EnsembleWalk13[] = {
	SC_MOVE(PIXELS(264), 0, PIXELS(228)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

s32 FuneHobashira_EnsembleWalk14[] = {
	SC_MOVE(PIXELS(282), 0, PIXELS(168)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(196)),
	SC_CALL(OverlayObject_ShrinkScaleThenStop),
	SC_END,
};

u32 FuneHobashira_EnsembleWalk15[] = {
	3, 0x9a0000, 0, 0x870000, 1, 3, 0x980000, 0,
	0xc40000, 34, (u32)OverlayObject_ShrinkScaleThenStop, 16, 0x9090000, 0xd70090d, 0xdb315cb, 0x15f5164c,
	0x1f0e1aad, 0x1f6f227a, 0x23f223b1, 0x2bf727f4,
};

struct SceneEntrance FuneHobashira_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 165, 0, 235, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 184, 56, 352, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 164, 96, 321, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

s16 FuneHobashira_SceneTableB[] = {
	176, 16, 348, 192, 32, 364, -1, 1,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 FuneHobashira_SceneTableC[] = {
	SCENE_EXITS(110),
	SCENE_EXIT(1, 109, 4),
	SCENE_EXIT(10, 111, 10),
	SCENE_EXIT(11, 111, 13),
	SCENE_EXIT(12, 109, 13),
	SCENE_EXIT(13, 109, 14),
	SCENE_EXIT(14, 109, 17),
	SCENE_EXIT(15, 109, 19),
	SCENE_EXITS_END,
};

struct ScenePlacement FuneHobashira_SceneTableD[] = {
	{ 0x91, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement FuneHobashira_StagingObjects[] = {
	{ 0x11e, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement FuneHobashira_EnsembleObjects[] = {
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(144), 0, PIXELS(169), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(134), 0, PIXELS(189), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(124), 0, PIXELS(209), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(129), 0, PIXELS(174), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(139), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(149), 0, PIXELS(204), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x9b, CONDITION_ALWAYS, 7,
		PIXELS(154), 0, PIXELS(194), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(182), 0, PIXELS(420), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement FuneHobashira_LandingObjects[] = {
	{ 0xc5, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent FuneHobashira_SceneTableE[] = {
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FuneHobashira_UpdateGridPlacement) },
	{ EVENT_TALK, 8, 0x928, MSGNO(MsgFuneWatchSeasHelpKaja) },
	{ EVENT_TALK, 8, 0x8a0, MSGNO(MsgFuneLookoutCannotRow) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFuneIslandMonstersUnknown) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x928, MSGNO(MsgFuneHiddenAnchorDiscovered) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8a0, MSGNO(MsgFuneSeaTooVast) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgFuneIslandTreasures) },
	{ SEARCH(SEARCH_UNNAMED), 11, 0x924, EVENT_SCRIPT(FieldScene_RunActor232SceneWhenFlag923Or922) },
	{ SCENE_EVENTS_END },
};

u32 FuneHobashira_FlagValues[] = {
	7, 5, 8, 4, 9, 12, 6, 3,
	0,
};
