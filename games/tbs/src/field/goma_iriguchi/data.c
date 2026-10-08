#include "types.h"
#include "field_scene.h"
#include "objcmd.h"

void FieldScene_RunOpeningAuxiliarySequence(void);
void FieldScene_RunScene387SequenceA(void);
void FieldScene_RunScene387SequenceC(void);
void FieldScene_RunScene387SequenceD(void);
void FieldScene_RunStepWithValue866(void);
void Overlay387_ConfigureActorEightAtDepth(void);

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

s32 GomaIriguchi_SceneTableCData[] = {
	SC_SET(SW_SCALE_Y, 0x19999),
	SC_END,
};

struct SceneEntrance GomaIriguchi_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 192, 0, 197, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 96, 0, 440, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 200, 0, 184, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 GomaIriguchi_SceneTableB[] = {
	SCENE_EXITS(25),
	SCENE_EXIT(1, 28, 1),
	SCENE_EXIT(2, 2, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement GomaIriguchi_SceneTableC[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(312), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd3, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd4, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd7, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0xd7, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf8, 0x865, 7,
		PIXELS(191), 0, PIXELS(194), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0xf8, 0x866, (s32)GomaIriguchi_SceneTableCData,
		PIXELS(200), 0, PIXELS(168), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent GomaIriguchi_SceneTableD[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_EAST), 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene387SequenceD) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 6, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene387SequenceD) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x865, EVENT_SCRIPT(FieldScene_RunScene387SequenceA) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x860, EVENT_SCRIPT(Overlay387_ConfigureActorEightAtDepth) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningAuxiliarySequence) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x863, EVENT_SCRIPT(FieldScene_RunScene387SequenceC) },
	{ PSYNERGY_ON_ACTOR(0x4e), 16, 0x866, EVENT_SCRIPT(FieldScene_RunStepWithValue866) },
	{ SCENE_EVENTS_END },
};
