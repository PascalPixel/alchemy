#include "types.h"
#include "ability_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "item_ids.h"
#include "objcmd.h"
#include "text/msg_ids.h"

void FieldScene_RunActor184Sequence(void);
void FieldScene_RunActorElevenAtTile5And13(void);
void FieldScene_RunActorThreeBranchSequence(void);
void FieldScene_RunColumnChoreography(void);
void FieldScene_RunFlag881Dialogue(void);
void FieldScene_RunFourCallSequence(void);
void FieldScene_RunFourStepSequence(void);
void FieldScene_RunFourSteps(void);
void FieldScene_RunRandomEffectActorSequence(void);
void FieldScene_RunScene39cSequenceA(void);
void FieldScene_RunScene39cSequenceB(void);
void FieldScene_RunScriptedSteps0And1576(void);
void FieldScene_RunScriptedSteps0And953(void);
void FieldScene_RunSingleStep(void);
void MakyuriHeya_ClearSceneRecordAndTarget(void);
void MakyuriHeya_CloseFloorSwitch(void);
void MakyuriHeya_RunColumnProbeScene(void);
void MakyuriHeya_RunExitFiveScene(void);
void MakyuriHeya_RunExitThirteenScene(void);
void MakyuriHeya_RunExitThreeScene(void);
void MakyuriHeya_RunPartyScene(void);
void MakyuriHeya_RunPushedBlockScene(void);
void MakyuriHeya_RunSequenceE(void);
void MakyuriHeya_StartActorNineParticles(void);
void MakyuriHeya_StopActorNinePositionCallback(void);
void MakyuriHeya_SyncBlockFlags(void);
void MakyuriHeya_TickSpawnTimer(void);
void MakyuriHeya_TriggerFloorSwitch(void);
void MakyuriHeya_TriggerPillarRise(void);
void MakyuriHeya_TriggerSmallFloorSwitch(void);
void MakyuriHeya_WalkLeaderIn(void);
void Makyuri_FollowLeader(void);
void OverlayObject_ReleasePublishedAttachmentB(void);
void OverlayObject_StepScaleUpSixteenFrames(void);
void SceneDialogue_RunActor3TimedLine(void);
void SceneDialogue_RunLine1637(void);
void SceneEffect_AdvanceAnchoredRiseFrame(void);
void SceneEffect_AdvanceScaleOverSixteenFrames(void);
void SceneState_ApplyPair12And21(void);
void SceneState_ApplyRectWhenActor20AtColumn28(void);
void SceneState_RunWhenActor8AtTile10x23(void);

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

s32 MakyuriHeya_RiseScript[] = {
	SC_WAIT(30),
	SC_FREE,
};

s32 MakyuriHeya_SparkScript[] = {
	SC_CALL(SceneEffect_AdvanceAnchoredRiseFrame),
	SC_FREE,
};

s32 Makyuri_RampScript[] = {
	SC_CALL(SceneEffect_AdvanceScaleOverSixteenFrames),
	SC_END,
};

s32 Makyuri_ScaleCounterScript[] = {
	SC_CALL(OverlayObject_StepScaleUpSixteenFrames),
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

s32 MakyuriHeya_SparkBurstScript[] = {
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0xa000),
	SC_SET(SW_SCALE_X, 0xa000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0x4000),
	SC_SET(SW_SCALE_X, 0x4000),
	SC_WAIT(6),
	SC_SET(SW_SCALE_Y, 0xa000),
	SC_SET(SW_SCALE_X, 0xa000),
	SC_WAIT(6),
	SC_LOOP(4, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 MakyuriHeya_PushScriptA[] = {
	SC_WAIT(45),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_CALL(MakyuriHeya_StartActorNineParticles),
	SC_MOVE(PIXELS(792), 0, PIXELS(712)),
	SC_SYNC,
	SC_END,
};

s32 MakyuriHeya_PushScriptB[] = {
	SC_WAIT(60),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_CALL(MakyuriHeya_StartActorNineParticles),
	SC_MOVE(PIXELS(792), 0, PIXELS(744)),
	SC_SYNC,
	SC_CALL(MakyuriHeya_StopActorNinePositionCallback),
	SC_END,
};

s32 MakyuriHeya_PushScriptD[] = {
	SC_WAIT(60),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_CALL(MakyuriHeya_StartActorNineParticles),
	SC_MOVE(PIXELS(824), 0, PIXELS(744)),
	SC_SYNC,
	SC_CALL(MakyuriHeya_StopActorNinePositionCallback),
	SC_END,
};

s32 MakyuriHeya_PushScriptC[] = {
	SC_WAIT(45),
	SC_CALL(MakyuriHeya_StartActorNineParticles),
	SC_END,
};

s32 gPaletteCycleRed = 0;

s32 gPaletteCycleGreen = 0;

s32 gPaletteCycleBlue = 0;

u16 MakyuriHeya_GateCells[] = {
	MAP_CELL_STEP(15, 1, 1, 2, 10), MAP_CELL_STEP(13, 1, 1, 2, 6), MAP_CELL_STEP(11, 1, 1, 2, 6), MAP_CELL_STEP(9, 1, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MakyuriHeya_GateCloseCells[] = {
	MAP_CELL_STEP(9, 1, 1, 2, 10), MAP_CELL_STEP(11, 1, 1, 2, 6), MAP_CELL_STEP(13, 1, 1, 2, 6), MAP_CELL_STEP(15, 1, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MakyuriHeya_FloorSwitchCells[] = {
	MAP_CELL_STEP(16, 28, 1, 2, 10), MAP_CELL_STEP(14, 28, 1, 2, 6), MAP_CELL_STEP(12, 28, 1, 2, 6), MAP_CELL_STEP(10, 28, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MakyuriHeya_FloorSwitchCloseCells[] = {
	MAP_CELL_STEP(10, 28, 1, 2, 10), MAP_CELL_STEP(12, 28, 1, 2, 6), MAP_CELL_STEP(14, 28, 1, 2, 6), MAP_CELL_STEP(16, 28, 1, 2, 6), MAP_CELL_STEPS_END,
};

u16 MakyuriHeya_ColumnCells[] = {
	MAP_CELL_STEP(126, 35, 1, 2, 2), MAP_CELL_STEP(126, 38, 1, 2, 2), MAP_CELL_STEP(126, 41, 1, 2, 2), MAP_CELL_STEP(126, 44, 1, 2, 2), MAP_CELL_STEP(126, 47, 1, 2, 2), MAP_CELL_STEPS_END,
};

struct SceneEntrance gMakyuriHeyaEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 56, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 440, FACING_NORTH, 0, 16, 16, 496, 448, 0 },
	{ 3, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, 16, 16, 496, 448, 0 },
	{ 4, CONDITION_ALWAYS, 760, 0, 344, FACING_NORTH, 0, 544, 16, 816, 352, 0 },
	{ 5, CONDITION_ALWAYS, 120, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
	{ 6, CONDITION_ALWAYS, 344, 0, 936, FACING_NORTH, 0, 64, 784, 384, 944, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
	{ 8, CONDITION_ALWAYS, 280, 0, 584, FACING_SOUTH, 0, 80, 528, 320, 688, 0 },
	{ 9, CONDITION_ALWAYS, 776, 0, 472, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
	{ 10, CONDITION_ALWAYS, 856, 0, 520, FACING_SOUTH, 0, 640, 416, 896, 704, 0 },
	{ 11, CONDITION_ALWAYS, 472, 0, 696, FACING_NORTH, 0, 352, 512, 592, 720, 0 },
	{ 15, CONDITION_ALWAYS, 712, 0, 264, FACING_SOUTH, 0, 544, 16, 816, 352, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMakyuriHeyaEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 120, 0, 472, FACING_SOUTH, 0, 32, 400, 480, 848, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 824, FACING_NORTH, 0, 32, 400, 480, 848, 0 },
	{ 3, CONDITION_ALWAYS, 152, 0, 184, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
	{ 4, CONDITION_ALWAYS, 488, 0, 136, FACING_SOUTH, 0, 48, 16, 592, 416, 0 },
	{ 5, CONDITION_ALWAYS, 728, 0, 88, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
	{ 6, CONDITION_ALWAYS, 728, 0, 216, FACING_SOUTH, 0, 656, 16, 896, 272, 0 },
	{ 7, CONDITION_ALWAYS, 632, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
	{ 8, CONDITION_ALWAYS, 792, 0, 712, FACING_NORTH, 0, 576, 480, 832, 736, 0 },
	{ 9, CONDITION_ALWAYS, 712, 0, 568, FACING_SOUTH, 0, 576, 480, 832, 736, 0 },
	{ 10, CONDITION_ALWAYS, 888, 0, 440, FACING_NORTH, 0, 760, 304, 1040, 464, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMakyuriHeyaEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 88, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
	{ 2, CONDITION_ALWAYS, 504, 0, 184, FACING_NORTH, 0, 32, 32, 544, 208, 0 },
	{ 3, CONDITION_ALWAYS, 264, 0, 120, FACING_SOUTH, 0, 32, 32, 544, 208, 0 },
	{ 4, CONDITION_ALWAYS, 648, 0, 168, FACING_NORTH, 0, 592, 16, 992, 320, 0 },
	{ 5, CONDITION_ALWAYS, 776, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
	{ 6, CONDITION_ALWAYS, 936, 0, 88, FACING_SOUTH, 0, 592, 16, 992, 320, 0 },
	{ 7, CONDITION_ALWAYS, 88, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ 8, CONDITION_ALWAYS, 168, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ 9, CONDITION_ALWAYS, 248, 0, 328, FACING_SOUTH, 0, 48, 272, 288, 432, 0 },
	{ 10, CONDITION_ALWAYS, 472, 0, 328, FACING_SOUTH, 0, 352, 272, 592, 432, 0 },
	{ 11, CONDITION_ALWAYS, 424, 0, 408, FACING_NORTH, 0, 352, 272, 592, 432, 0 },
	{ 12, CONDITION_ALWAYS, 88, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
	{ 13, CONDITION_ALWAYS, 456, 0, 600, FACING_SOUTH, 0, 48, 512, 544, 672, 0 },
	{ 14, CONDITION_ALWAYS, 888, 0, 712, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
	{ 15, CONDITION_ALWAYS, 168, 0, 368, FACING_SOUTH, 0, 40, 256, 296, 448, 0 },
	{ 16, CONDITION_ALWAYS, 760, 0, 584, FACING_SOUTH, 0, 688, 384, 928, 816, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gMakyuriHeyaEntrances4[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 232, 0, 104, FACING_SOUTH, 0, 64, 32, 288, 504, 0 },
	{ 2, CONDITION_ALWAYS, 232, 0, 488, FACING_NORTH, 0, 64, 32, 288, 504, 0 },
	{ 3, CONDITION_ALWAYS, 408, 0, 104, FACING_SOUTH, 0, 352, 32, 592, 328, 0 },
	{ 4, CONDITION_ALWAYS, 408, 0, 312, FACING_NORTH, 0, 352, 32, 592, 328, 0 },
	{ 10, CONDITION_ALWAYS, 792, 0, 216, FACING_SOUTH, 0, 664, 124, 920, 292, 0 },
	{ 11, CONDITION_ALWAYS, 840, 0, 856, FACING_NORTH, 0, 576, 328, 960, 912, 0 },
	{ 12, CONDITION_ALWAYS, 840, 0, 600, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
	{ 13, CONDITION_ALWAYS, 872, 0, 600, FACING_SOUTH, 0, 720, 544, 960, 912, 0 },
	{ 15, CONDITION_ALWAYS, 680, 0, 584, FACING_SOUTH, 0, 576, 328, 960, 912, 0 },
	{ SCENE_TABLE_END },
};

u32 MakyuriHeya_SceneTable[] = {
	SCENE_EXITS(54),
	SCENE_EXIT(1, 53, 11),
	SCENE_EXIT(2, 53, 12),
	SCENE_EXIT(3, 54, 9),
	SCENE_EXIT(4, 53, 15),
	SCENE_EXIT(5, 53, 14),
	SCENE_EXIT(6, 56, 7),
	SCENE_EXIT(7, 55, 3),
	SCENE_EXIT(8, 53, 6),
	SCENE_EXIT(9, 54, 3),
	SCENE_EXIT(10, 54, 5),
	SCENE_EXIT(11, 54, 10),
	SCENE_EXIT(12, 55, 1),
	SCENE_EXIT(13, 53, 25),
	SCENE_EXIT(0, 55, 0),
	SCENE_EXIT(1, 54, 11),
	SCENE_EXIT(2, 55, 2),
	SCENE_EXIT(3, 56, 1),
	SCENE_EXIT(4, 57, 1),
	SCENE_EXIT(5, 56, 3),
	SCENE_EXIT(6, 54, 6),
	SCENE_EXIT(7, 55, 4),
	SCENE_EXIT(8, 56, 5),
	SCENE_EXIT(9, 56, 6),
	SCENE_EXIT(10, 56, 2),
	SCENE_EXIT(0, 56, 0),
	SCENE_EXIT(1, 55, 5),
	SCENE_EXIT(2, 55, 9),
	SCENE_EXIT(3, 55, 10),
	SCENE_EXIT(4, 56, 8),
	SCENE_EXIT(5, 55, 7),
	SCENE_EXIT(6, 55, 8),
	SCENE_EXIT(7, 54, 8),
	SCENE_EXIT(8, 56, 14),
	SCENE_EXIT(9, 56, 4),
	SCENE_EXIT(10, 57, 3),
	SCENE_EXIT(11, 56, 13),
	SCENE_EXIT(12, 57, 2),
	SCENE_EXIT(13, 56, 11),
	SCENE_EXIT(14, 56, 9),
	SCENE_EXIT(15, 56, 15),
	SCENE_EXIT(16, 56, 16),
	SCENE_EXIT(0, 57, 0),
	SCENE_EXIT(1, 55, 6),
	SCENE_EXIT(2, 56, 12),
	SCENE_EXIT(3, 56, 10),
	SCENE_EXIT(4, 57, 10),
	SCENE_EXIT(5, 57, 4),
	SCENE_EXIT(6, 2, 10),
	SCENE_EXIT(7, 53, 1),
	SCENE_EXIT(8, 2, 90),
	SCENE_EXIT(9, 58, 5),
	SCENE_EXITS_END,
};

struct ScenePlacement gMakyuriHeyaPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMakyuriHeyaPlacements1[] = {
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(672), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(704), 0, PIXELS(272), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(744), 0, PIXELS(240), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(648), 0, PIXELS(299), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMakyuriHeyaPlacements2[] = {
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(152), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(184), 0, PIXELS(587), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(704), 0, PIXELS(672), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5b, 0x45, ACTOR_STAND,
		PIXELS(888), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMakyuriHeyaPlacements3[] = {
	{ 0xe0, IF_FLAG_SET(0x877), ACTOR_STAND,
		PIXELS(200), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, 0x877, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(712), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe4, CONDITION_ALWAYS, 7,
		PIXELS(816), 0, PIXELS(544), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe5, CONDITION_ALWAYS, 7,
		PIXELS(872), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(760), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(776), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe6, CONDITION_ALWAYS, 7,
		PIXELS(872), 0, PIXELS(635), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(712), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(776), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(936), 0, PIXELS(88), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(472), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMakyuriHeyaPlacements4[] = {
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(664), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(872), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(888), 0, PIXELS(872), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcb, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(840), 0, PIXELS(600), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xe0, IF_FLAG_SET(0x881), ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x6c, IF_FLAG_SET(0x881), ACTOR_STAND,
		PIXELS(824), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, IF_FLAG_SET(0x881), ACTOR_WANDER,
		PIXELS(760), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, IF_FLAG_SET(0x881), ACTOR_WANDER,
		PIXELS(920), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, IF_FLAG_SET(0x881), ACTOR_STAND,
		PIXELS(856), 0, PIXELS(792), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gMakyuriHeyaEvents1[] = {
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ 0x21, 53, CONDITION_ALWAYS, 3 },
	{ 0x31, 59, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 62, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
	{ 0x31, 55, CONDITION_ALWAYS, 5 },
	{ 0x21, 56, CONDITION_ALWAYS, 6 },
	{ EVENT_TOUCH, 32, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriHeya_ClearSceneRecordAndTarget) },
	{ 0x202, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 31, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriHeya_ClearSceneRecordAndTarget) },
	{ 0xa02, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 21, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriHeya_TickSpawnTimer) },
	{ 0x202, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TOUCH, 11, IF_FLAG_SET(0x875), EVENT_SCRIPT(MakyuriHeya_TriggerPillarRise) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene39cSequenceB) },
	{ 0x202, 25, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunPushedBlockScene) },
	{ EVENT_TALK, 3, 0x873, MSGNO(MsgMakyuriHeyaThePathIsBlockedAgainWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 3, 0x873, MSGNO(MsgMakyuriHeyaThisStatueWasntHereBeforeI) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachmentB) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x873, EVENT_SCRIPT(MakyuriHeya_RunPartyScene) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf64, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMakyuriHeyaEvents2[] = {
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 57, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 53, CONDITION_ALWAYS, 3 },
	{ 0x21, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 58, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 60, CONDITION_ALWAYS, 10 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene39cSequenceB) },
	{ 0x202, 1, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunSequenceE) },
	{ EVENT_TOUCH, 2, IF_FLAG_SET(0x302), EVENT_SCRIPT(MakyuriHeya_WalkLeaderIn) },
	{ EVENT_TOUCH, 3, IF_FLAG_SET(0x302), EVENT_SCRIPT(MakyuriHeya_WalkLeaderIn) },
	{ EVENT_TOUCH, 4, IF_FLAG_SET(0x302), EVENT_SCRIPT(MakyuriHeya_WalkLeaderIn) },
	{ 0x202, 10, 0x874, EVENT_SCRIPT(FieldScene_RunFourCallSequence) },
	{ EVENT_TOUCH, 12, 0x874, EVENT_SCRIPT(MakyuriHeya_TriggerSmallFloorSwitch) },
	{ EVENT_TOUCH, 10, 0x874, EVENT_SCRIPT(MakyuriHeya_CloseFloorSwitch) },
	{ EVENT_TOUCH, 13, 0x874, EVENT_SCRIPT(MakyuriHeya_CloseFloorSwitch) },
	{ 0x202, 45, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunSequenceE) },
	{ EVENT_TOUCH, 46, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_WalkLeaderIn) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyPair12And21) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedSteps0And953) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachmentB) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x874, EVENT_SCRIPT(FieldScene_RunActorElevenAtTile5And13) },
	{ SEARCH(SEARCH_CHEST), 101, 0xf66, FIND_ITEM(0xba) },
	{ SEARCH(SEARCH_CHEST), 102, 0xf67, FIND_ITEM(ITEM_NUT) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMakyuriHeyaEvents3[] = {
	{ EVENT_EXIT, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 53, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunExitThreeScene) },
	{ 0x31, 57, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 59, CONDITION_ALWAYS, 9 },
	{ 0x21, 58, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 61, CONDITION_ALWAYS, 11 },
	{ 0x21, 60, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 62, CONDITION_ALWAYS, 12 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 63, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunExitThirteenScene) },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 55, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 56, CONDITION_ALWAYS, 6 },
	{ 0x31, 64, CONDITION_ALWAYS, 14 },
	{ 0x202, 10, 0x878, EVENT_SCRIPT(FieldScene_RunFourSteps) },
	{ 0x202, 12, 0x878, EVENT_SCRIPT(FieldScene_RunFourSteps) },
	{ EVENT_TOUCH, 12, 0x878, EVENT_SCRIPT(MakyuriHeya_TriggerFloorSwitch) },
	{ EVENT_TOUCH, 10, 0x878, EVENT_SCRIPT(FieldScene_RunScene39cSequenceA) },
	{ 0x202, 40, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunColumnProbeScene) },
	{ 0x202, 47, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFourStepSequence) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedSteps0And953) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachmentB) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x878, EVENT_SCRIPT(SceneState_RunWhenActor8AtTile10x23) },
	{ PSYNERGY_ON_ACTOR(0x8c), 15, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 16, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 17, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 18, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_SyncBlockFlags) },
	{ PSYNERGY_ON_ACTOR(0x8c), 20, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyRectWhenActor20AtColumn28) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMakyuriHeyaEventsOther[] = {
	{ EVENT_EXIT, 44, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 43, CONDITION_ALWAYS, 6 },
	{ 0x31, 51, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 52, CONDITION_ALWAYS, 2 },
	{ 0x31, 53, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 54, CONDITION_ALWAYS, 4 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 55, CONDITION_ALWAYS, EVENT_SCRIPT(MakyuriHeya_RunExitFiveScene) },
	{ EVENT_TALK, 3, 0x870, EVENT_SCRIPT(FieldScene_RunActorThreeBranchSequence) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x403, 0x870, EVENT_SCRIPT(FieldScene_RunActorThreeBranchSequence) },
	{ EVENT_TALK, 3, 0x871, EVENT_SCRIPT(SceneDialogue_RunActor3TimedLine) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 3, 0x871, MSGNO(MsgMakyuriHeyaSomeoneDoesntWantMeToGet) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaTheLegendsSaidThatIfThe) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaOurFountainIsBackTheySay) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaIDrankTheHealingWaterAnd) },
	{ EVENT_TALK, 17, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaWeDontHaveToFearAny) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaHowDidTheFountainKnowThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaAllThatLivesThatMeansIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaIKnewItWouldWorkBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, IF_FLAG_SET(0x881), MSGNO(MsgMakyuriHeyaINeverBelievedThoseStoriesBut) },
	{ SEARCH(SEARCH_UNNAMED), 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunLine1637) },
	{ SEARCH(SEARCH_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedSteps0And1576) },
	{ SEARCH(SEARCH_UNNAMED), 5, 0x200, EVENT_SCRIPT(FieldScene_RunFlag881Dialogue) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x82b, 0 },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x871, EVENT_SCRIPT(FieldScene_RunColumnChoreography) },
	{ PSYNERGY_ON_ACTOR(0x5d), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunRandomEffectActorSequence) },
	{ PSYNERGY_IN_AREA(0x95, 1), 0, CONDITION_ALWAYS, EVENT_SCRIPT(OverlayObject_ReleasePublishedAttachmentB) },
	{ 0xb904, 5, IF_FLAG_SET(0x881), EVENT_SCRIPT(FieldScene_RunActor184Sequence) },
	{ SCENE_EVENTS_END },
};
