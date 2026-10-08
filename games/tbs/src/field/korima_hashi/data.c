#include "types.h"
#include "field_scene.h"
#include "objcmd.h"

void FieldScene_RunTile10x20Transition(void);
void OverlayObject_ClearPendingAndRestoreMode(void);
void OverlayObject_SelectValueByFrameBit1(void);
void SceneActor_CheckRegionTrigger(void);
void SceneActor_PassSubjectOffsetPosition(void);
void SceneActor_UpdateRandomCounterMode(void);
void SceneEffect_AdvanceAngleAndFinishWhenParked(void);

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

s32 KorimaHashi_GeraldAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(234), 0, PIXELS(576)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_ARG(1),
	SC_END,
};

s32 KorimaHashi_IvanAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(234), 0, PIXELS(623)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_ARG(1),
	SC_END,
};

s32 KorimaHashi_MiaAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(211), 0, PIXELS(602)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH),
	SC_ARG(1),
	SC_END,
};

s32 KorimaHashi_PartyAction[] = {
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
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaHashi_ResetAction[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(1),
	SC_END,
};

s32 KorimaHashi_FormationAction[] = {
	SC_SET(SW_SCALE_X, 0x1999),
	SC_SET(SW_SCALE_Y, 0x1999),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 1310),
	SC_ADD(SW_SCALE_Y, 1310),
	SC_WAIT(1),
	SC_LOOP(70, SC_TO(1)),
	SC_CALL(OverlayObject_ClearPendingAndRestoreMode),
	SC_END,
};

s32 KorimaHashi_FinishAction[] = {
	SC_ARG(1),
	SC_ADD(SW_Z, -0x1999),
	SC_ADD(SW_SCALE_X, -983),
	SC_ADD(SW_SCALE_Y, -983),
	SC_WAIT(1),
	SC_LOOP(90, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 KorimaHashi_EntryAction[] = {
	SC_SET(SW_SCALE_X, 0xc000),
	SC_SET(SW_SCALE_Y, 0xc000),
	SC_SET(SW_COLLISION, 0),
	SC_SET(0x23, 0),
	SC_LABEL(1),
	SC_CALL(SceneActor_UpdateRandomCounterMode),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance KorimaHashi_Entrances[] = {
	{ 0, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 172, 0, 602, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance KorimaHashi_Exits[] = {
	{ 40, 0x0, 4137, 16, 4136, 0x20, 4136, 48, -32766, 64, 511, 0 },
};

struct SceneEntrance KorimaHashi_Placements[] = {
	{ 1, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0 },
	{ 2, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0 },
	{ 3, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0 },
	{ 205, CONDITION_ALWAYS, 7, 0, 0, 0xe8, 0, 0, 0, 128, 16384, 2 },
	{ 207, CONDITION_ALWAYS, 7, 0, 0, 0xd0, 0, 0, 0, 296, 16384, 2 },
	{ 205, CONDITION_ALWAYS, 7, 0, 0, 0x168, 0, 0, 0, 304, 16384, 2 },
	{ 55, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0 },
	{ 56, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0xe8, 0, 0, 0, 655, 16384, 0 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0xa6, 0, 0, 0, 576, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0xbd, 0, 0, 0, 532, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0x120, 0, 0, 0, 570, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0x14b, 0, 0, 0, 659, 16384, 256 },
	{ SCENE_TABLE_END },
};

struct SceneEvent KorimaHashi_Events[] = {
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTile10x20Transition) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_PassSubjectOffsetPosition) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SCENE_EVENTS_END },
};

s32 KorimaHashi_Object26Script[] = {
	SC_CALL(SceneEffect_AdvanceAngleAndFinishWhenParked),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaHashi_TriggerScript[] = {
	SC_SET(SW_SCALE_X, 0x20000),
	SC_SET(SW_SCALE_Y, 0x20000),
	SC_WAIT(10),
	SC_FREE,
};

s32 gFallingEffectScript[] = {
	SC_ADD(SW_Y, PIXELS(-2)),
	SC_CALL(OverlayObject_SelectValueByFrameBit1),
	SC_WAIT(1),
	SC_ADD(SW_Y, PIXELS(-2)),
	SC_CALL(OverlayObject_SelectValueByFrameBit1),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_Y, PIXELS(-1)),
	SC_CALL(OverlayObject_SelectValueByFrameBit1),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(1)),
	SC_CALL(SceneActor_CheckRegionTrigger),
	SC_FREE,
};
