#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void FieldScene_RunIndexedStep17(void);
void FieldScene_RunIndexedStep18(void);
void FieldScene_RunIndexedStep19(void);
void FieldScene_RunIndexedStep63(void);
void FieldScene_RunSingleStep(void);
void FieldScene_RunStepWithValue1632(void);
void FieldScene_RunSupplementalSequenceTwo(void);
void MakyuriIriguchi_ClearCurrentRecordAndReleaseTarget(void);
void MakyuriIriguchi_InstallActorEightPath(void);
void MakyuriIriguchi_OpenEntrance(void);
void MakyuriIriguchi_SendActor8ByLeaderColumn(void);
void MakyuriIriguchi_TickSpawnTimer(void);
void MakyuriIriguchi_TriggerPillarRise(void);
void Makyuri_FollowLeader(void);
void OverlayObject_AdvanceScaleCounter(void);
void OverlayObject_RampWords24And28Over16Frames(void);
void OverlayObject_ReleasePublishedAttachment(void);
void SceneAudio_PlayCue118AndReturnZero(void);
void SceneEffect_AdvanceAnchoredRiseFrame(void);

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

s32 MakyuriIriguchi_RiseScript[] = {
	SC_WAIT(30),
	SC_FREE,
};

s32 MakyuriHeya_SparkScript[] = {
	SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
	SC_FREE,
};

s32 Makyuri_RampScript[] = {
	SC_CALL(OverlayObject_RampWords24And28Over16Frames),
	SC_END,
};

s32 Makyuri_ScaleCounterScript[] = {
	SC_CALL(OverlayObject_AdvanceScaleCounter),
	SC_FREE,
};

s32 Makyuri_PillarScript[] = {
	SC_CALL(Makyuri_FollowLeader),
	SC_END,
};

u16 Makyuri_MoveAngles[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
	FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 MakyuriIriguchi_Actor8Path1[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_SET(SW_STEERING, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(136), 0, PIXELS(152)),
	SC_CALL(SceneAudio_PlayCue118AndReturnZero),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 MakyuriIriguchi_Actor8Path2[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_SET(SW_STEERING, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(120), 0, PIXELS(152)),
	SC_CALL(SceneAudio_PlayCue118AndReturnZero),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 MakyuriIriguchi_Actor8Path3[] = {
	SC_SET(SW_SPEED, 0x20000),
	SC_SET(SW_ACCEL, 0x20000),
	SC_SET(SW_STEERING, 0),
	SC_ARG(2),
	SC_MOVE(PIXELS(152), 0, PIXELS(152)),
	SC_CALL(SceneAudio_PlayCue118AndReturnZero),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

struct SceneEntrance MakyuriIriguchi_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 136, 0, 280, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 120, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
	{ 3, CONDITION_ALWAYS, 200, 0, 648, FACING_NORTH, 0, 80, 432, 320, 688, 0 },
	{ 4, CONDITION_ALWAYS, 120, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ 5, CONDITION_ALWAYS, 280, 0, 520, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ 6, CONDITION_ALWAYS, 200, 0, 552, FACING_SOUTH, 0, 80, 432, 320, 688, 0 },
	{ 7, CONDITION_ALWAYS, 360, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 8, CONDITION_ALWAYS, 456, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 9, CONDITION_ALWAYS, 616, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 10, CONDITION_ALWAYS, 760, 0, 184, FACING_NORTH, 0, 304, 32, 800, 208, 0 },
	{ 11, CONDITION_ALWAYS, 344, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
	{ 12, CONDITION_ALWAYS, 536, 0, 88, FACING_SOUTH, 0, 304, 32, 800, 208, 0 },
	{ 13, 0x876, 744, 0, 680, FACING_NORTH, 0, 544, 512, 784, 720, 0 },
	{ 14, 0x876, 584, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ 15, 0x876, 744, 0, 568, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ 13, CONDITION_ALWAYS, 584, 0, 424, FACING_NORTH, 0, 384, 256, 624, 448, 0 },
	{ 14, CONDITION_ALWAYS, 424, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
	{ 15, CONDITION_ALWAYS, 584, 0, 312, FACING_SOUTH, 0, 384, 256, 624, 448, 0 },
	{ 16, CONDITION_ALWAYS, 744, 0, 424, FACING_NORTH, 0, 704, 256, 944, 448, 0 },
	{ 17, CONDITION_ALWAYS, 344, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
	{ 18, CONDITION_ALWAYS, 232, 0, 872, FACING_SOUTH, 0, 48, 768, 400, 960, 0 },
	{ 19, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
	{ 20, CONDITION_ALWAYS, 696, 0, 872, FACING_SOUTH, 0, 464, 768, 816, 960, 0 },
	{ 25, CONDITION_ALWAYS, 744, 0, 600, FACING_SOUTH, 0, 544, 512, 784, 720, 0 },
	{ 30, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ 31, CONDITION_ALWAYS, 136, 0, 168, FACING_NORTH, 0, 16, 32, 256, 320, 0 },
	{ 60, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, 16, 32, 256, 320, 0 },
	{ 99, CONDITION_ALWAYS, 136, 0, 232, FACING_EAST, 0, 16, 32, 256, 320, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance MakyuriIriguchi_SceneTableB[] = {
	{ 53, 0x0, -16327, 16, 12341, 0x20, 8245, 48, -28619, 64, -32715, 80 },
	{ 4149, 0x61, 16437, 112, 20533, 0x80, 12341, 145, 4150, 160, 8246, 176 },
	{ 28726, 0xc0, 16438, 208, 8245, 0xe1, 16437, 241, 28725, 256, -12235, 272 },
	{ -24523, 0x120, 53, 305, -8139, 0x141, -4042, 800, -4043, 1009, 511, 0 },
};

struct SceneEntrance MakyuriIriguchi_SceneTableC[] = {
	{ 3, CONDITION_ALWAYS, 1, 0, 0, 0x8, 0, 0, 0, 8, -16384, 2 },
	{ 192, CONDITION_ALWAYS, 1, 0, 0, 0x8, 0, 0, 0, 8, 16384, 2 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0x58, 0, 0, 0, 136, 16384, 2 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0x58, 0, 0, 0, 200, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0x58, 0, 0, 0, 264, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0xb8, 0, 0, 0, 136, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0xb8, 0, 0, 0, 200, 16384, 258 },
	{ 224, CONDITION_ALWAYS, 7, 0, 0, 0xb8, 0, 0, 0, 264, 16384, 258 },
	{ 22, CONDITION_ALWAYS, 1, 0, 0, 0xc8, 0, 0, 0, 616, 16384, 2 },
	{ 22, CONDITION_ALWAYS, 7, 0, 0, 0xc8, 0, 0, 0, 512, 16384, 2 },
	{ SCENE_TABLE_END },
};

struct SceneEvent MakyuriIriguchi_SceneTableD[] = {
	{ 0x202, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriIriguchi_ClearCurrentRecordAndReleaseTarget) },
	{ 0xa02, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriIriguchi_TickSpawnTimer) },
	{ 0x202, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriIriguchi_TriggerPillarRise) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep63) },
	{ EVENT_TOUCH, 40, IF_FLAG_SET(0x205), EVENT_SCRIPT(MakyuriIriguchi_InstallActorEightPath) },
	{ EVENT_TOUCH, 42, IF_FLAG_SET(0x205), EVENT_SCRIPT(MakyuriIriguchi_InstallActorEightPath) },
	{ EVENT_TOUCH, 41, 0x205, EVENT_SCRIPT(MakyuriIriguchi_SendActor8ByLeaderColumn) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 40, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 41, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 67, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep17) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 68, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep18) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 69, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunIndexedStep19) },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 53, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 58, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
	{ 0x21, 62, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 63, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 64, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 65, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 66, CONDITION_ALWAYS, 16 },
	{ EVENT_TALK, 3, CONDITION_ALWAYS, MSGNO(MsgMakyuriMonsterInsideTower) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 3, CONDITION_ALWAYS, MSGNO(MsgMakyuriLetMePass) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriIriguchi_OpenEntrance) },
	{ SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStepWithValue1632) },
	{ PSYNERGY_ON_ACTOR(0x5d), 16, 0x875, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceTwo) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachment) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf65, FIND_ITEM(0x4c) },
	{ SEARCH(SEARCH_CHEST), 101, 0xef0, 0x500000 },
	{ SCENE_EVENTS_END },
};

u32 MakyuriIriguchi_SparkBurstScript[] = {
	21, 10, 0x10000, 21, 9, 0x10000, 0, 6,
	21, 10, 0xa000, 21, 9, 0xa000, 0, 6,
	21, 10, 0x4000, 21, 9, 0x4000, 0, 6,
	21, 10, 0xa000, 21, 9, 0xa000, 0, 6,
	12, 4, 0, 0, 1, 2, 0, 0,
	0, 0, 1, 27,
};
