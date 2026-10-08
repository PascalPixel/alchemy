#include "types.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"

void ActorPresentation_AdvanceActorEightStates(void);
void ActorPresentation_AdvanceActorTenStates(void);
void ActorPresentation_SetupActorEightForFlag301(void);
void ActorPresentation_SetupActorEightForFlag313(void);
void ActorPresentation_SetupActorEightForFlags308And309(void);
void ActorPresentation_SetupActorEightForFlags308And309Guarded(void);
void ActorPresentation_SetupActorElevenAt0_112(void);
void ActorPresentation_SetupActorElevenAt0_64(void);
void ActorPresentation_SetupActorElevenAt0_80(void);
void ActorPresentation_SetupActorNineForFlag302(void);
void ActorPresentation_SetupActorNineForFlag306(void);
void ActorPresentation_SetupActorNineForFlag30a(void);
void ActorPresentation_SetupActorNineForFlag314(void);
void ActorPresentation_SetupActorTenForFlag303(void);
void ActorPresentation_SetupActorTenForFlag307(void);
void ActorPresentation_SetupActorTenForFlag315(void);
void ActorPresentation_SetupActorTenForFlags30bAnd30d(void);
void FieldScene_ApplyOffset0Neg32(void);
void FieldScene_PlaceActorEightByFlags(void);
void FieldScene_PlaceActorTenByFlags(void);
void FieldScene_RunActor10Flag315Sequence(void);
void FieldScene_RunActor10Flags30bTo30eSequenceA(void);
void FieldScene_RunActor10Flags30bTo30eSequenceB(void);
void FieldScene_RunActor10Flags30bTo30eSequenceC(void);
void FieldScene_RunActor10Transition303(void);
void FieldScene_RunActor10Transition30BTo30E(void);
void FieldScene_RunActor11Flags30fTo312Sequence(void);
void FieldScene_RunActor11Offset128Sequence(void);
void FieldScene_RunActor11Transition(void);
void FieldScene_RunActor8FlaggedSequence(void);
void FieldScene_RunActor8Transition304(void);
void FieldScene_RunActor8Transition304And305(void);
void FieldScene_RunActor8Transition305(void);
void FieldScene_RunActor8Transition308And309(void);
void FieldScene_RunActor8Transition313(void);
void FieldScene_RunActor9Flag306Sequence(void);
void FieldScene_RunActor9Flag314Sequence(void);
void FieldScene_RunActor9Transition302(void);
void FieldScene_RunActor9Transition30A(void);
void FieldScene_RunActorElevenFlaggedSteps(void);
void FieldScene_RunActorTenDepthSequence(void);
void FieldScene_RunFlag308DialogueBranch(void);
void FieldScene_RunFlag311DialogueBranch(void);
void FieldScene_RunFourPassCallbackSequence(void);
void FieldScene_RunScriptedStep17E3(void);
void FieldScene_RunSingleStep(void);
void FieldScene_SetActor9Values1And2(void);
void ImiruFuchin_HopOnTrigger(void);
void ImiruFuchin_StartFadeIn(void);
void ImiruFuchin_StartTrackingLeader(void);
void ImiruFuchin_StopTrackingLeader(void);
void ImiruFuchin_TakeDragonsEye(void);
void ImiruFuchin_TurnLeaderAndExit(void);
void OverlayObject_ApplyValue15(void);
void SceneActor_SetActor11Values1And2(void);
void SceneDialogue_RunLine17e2(void);
void SceneState_ApplyOffsetMinus32(void);
void SceneState_ApplyValues11And62(void);
void SceneState_ApplyValues8And2And1(void);
void SceneState_RunSlot8OffsetStep(void);
void SceneState_SetValue17e1(void);
void SceneState_SetWorkspace370ByFlag820(void);

extern s32 gEffectScript0[];
extern s32 gEffectScript1[];
extern s32 gEffectScript2[];

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

u16 gImiruFuchinKeyHeadings[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_NORTHEAST, FACING_NORTHWEST, FACING_NORTH,
	FACING_SOUTH, FACING_SOUTHEAST, FACING_SOUTHWEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, 0xffff, 0xffff, FACING_NORTH,
	FACING_SOUTH, 0xffff, 0xffff, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
};

u16 gImiruFuchinHeadings[] = {
	0xffff, FACING_EAST, FACING_WEST, 0xffff, FACING_NORTH, FACING_EAST, FACING_WEST, FACING_NORTH,
	FACING_SOUTH, FACING_EAST, FACING_WEST, FACING_SOUTH, 0xffff, FACING_EAST, FACING_WEST, 0xffff,
	FACING_EAST, FACING_EAST,
};

s32 gImiruFuchinDragonsEye = 0;

struct SceneEntrance gImiruFuchinEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 256, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 264, 0, 289, FACING_NORTH, 0, 135, 62, 376, 328, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 248, 0, 600, FACING_SOUTH, 0, 64, 64, 416, 656, 0 },
	{ 1, CONDITION_ALWAYS, 248, 0, 632, FACING_NORTH, 0, 64, 368, 416, 656, 0 },
	{ 2, CONDITION_ALWAYS, 376, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 488, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ 4, CONDITION_ALWAYS, 232, 0, 408, FACING_SOUTH, 0, 64, 368, 416, 656, 0 },
	{ 5, CONDITION_ALWAYS, 232, 0, 344, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
	{ 6, CONDITION_ALWAYS, 376, 0, 152, FACING_NORTH, 0, 64, 48, 416, 376, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 200, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
	{ 8, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 64, 48, 416, 376, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 248, 0, 120, FACING_SOUTH, 0, 96, 64, 416, 480, 0 },
	{ 2, CONDITION_ALWAYS, 152, 0, 200, FACING_NORTH, 0, 96, 64, 416, 480, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances4[] = {
	{ 1, CONDITION_ALWAYS, 248, 0, 104, FACING_SOUTH, 0, 128, 64, 368, 256, 0 },
	{ 2, CONDITION_ALWAYS, 184, 0, 328, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ 3, CONDITION_ALWAYS, 152, 0, 432, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ 4, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 96, 256, 416, 656, 0 },
	{ 5, CONDITION_ALWAYS, 280, 0, 616, FACING_NORTH, 0, 96, 256, 416, 656, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances5[] = {
	{ 1, CONDITION_ALWAYS, 328, 0, 72, FACING_SOUTH, 0, 64, 16, 416, 416, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 392, FACING_NORTH, 0, 64, 16, 416, 416, 0 },
	{ 3, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 416, 416, 800, 0 },
	{ 4, CONDITION_ALWAYS, 344, 0, 632, FACING_NORTH, 0, 64, 416, 416, 800, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances6[] = {
	{ 1, CONDITION_ALWAYS, 264, 0, 232, FACING_NORTH, 0, 128, 16, 384, 256, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 472, FACING_SOUTH, 0, 64, 256, 416, 720, 0 },
	{ 3, CONDITION_ALWAYS, 216, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
	{ 4, CONDITION_ALWAYS, 280, 0, 664, FACING_NORTH, 0, 64, 256, 416, 720, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gImiruFuchinEntrances7[] = {
	{ 1, CONDITION_ALWAYS, 280, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ 2, CONDITION_ALWAYS, 216, 0, 120, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ 3, CONDITION_ALWAYS, 104, 0, 280, FACING_SOUTH, 0, 64, 0, 416, 344, 0 },
	{ 4, CONDITION_ALWAYS, 200, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ 5, CONDITION_ALWAYS, 248, 0, 312, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ 6, CONDITION_ALWAYS, 296, 0, 344, FACING_NORTH, 0, 64, 16, 416, 344, 0 },
	{ 7, CONDITION_ALWAYS, 200, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 8, CONDITION_ALWAYS, 248, 0, 440, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 9, CONDITION_ALWAYS, 296, 0, 424, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 10, CONDITION_ALWAYS, 376, 0, 472, FACING_SOUTH, 0, 32, 344, 432, 736, 0 },
	{ 11, CONDITION_ALWAYS, 248, 0, 536, FACING_NORTH, 0, 32, 344, 432, 736, 0 },
	{ 12, CONDITION_ALWAYS, 120, 0, 696, FACING_SOUTH, 0, 32, 344, 432, 752, 0 },
	{ SCENE_TABLE_END },
};

u32 gImiruFuchinExits[] = {
	SCENE_EXITS(52),
	SCENE_EXIT(1, 50, 9),
	SCENE_EXIT(0, 62, 0),
	SCENE_EXIT(1, 60, 3),
	SCENE_EXIT(2, 63, 2),
	SCENE_EXIT(3, 65, 4),
	SCENE_EXIT(4, 62, 5),
	SCENE_EXIT(5, 62, 4),
	SCENE_EXIT(6, 64, 4),
	SCENE_EXIT(7, 67, 12),
	SCENE_EXIT(8, 67, 11),
	SCENE_EXIT(0, 63, 0),
	SCENE_EXIT(1, 64, 5),
	SCENE_EXIT(2, 62, 2),
	SCENE_EXIT(0, 64, 0),
	SCENE_EXIT(1, 64, 3),
	SCENE_EXIT(2, 67, 10),
	SCENE_EXIT(3, 64, 1),
	SCENE_EXIT(4, 62, 6),
	SCENE_EXIT(5, 63, 1),
	SCENE_EXIT(0, 65, 0),
	SCENE_EXIT(1, 67, 3),
	SCENE_EXIT(2, 65, 3),
	SCENE_EXIT(3, 65, 2),
	SCENE_EXIT(4, 62, 3),
	SCENE_EXIT(0, 66, 0),
	SCENE_EXIT(1, 66, 2),
	SCENE_EXIT(2, 66, 1),
	SCENE_EXIT(3, 67, 2),
	SCENE_EXIT(4, 67, 1),
	SCENE_EXIT(0, 67, 0),
	SCENE_EXIT(1, 66, 3),
	SCENE_EXIT(2, 66, 4),
	SCENE_EXIT(3, 65, 1),
	SCENE_EXIT(4, 67, 7),
	SCENE_EXIT(5, 67, 8),
	SCENE_EXIT(6, 67, 9),
	SCENE_EXIT(7, 67, 4),
	SCENE_EXIT(8, 67, 5),
	SCENE_EXIT(9, 67, 6),
	SCENE_EXIT(10, 64, 2),
	SCENE_EXIT(11, 62, 8),
	SCENE_EXIT(12, 62, 7),
	SCENE_EXITS_END,
};

struct ScenePlacement gImiruFuchinPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements1[] = {
	{ 0x5c, 0x59, ACTOR_STAND,
		PIXELS(263), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements2[] = {
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(200), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(296), 0, PIXELS(536), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements3[] = {
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(328), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x5d, 0x6e, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements4[] = {
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(328), 0, PIXELS(408), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(568), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(504), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements5[] = {
	{ 0xec, CONDITION_ALWAYS, 7,
		PIXELS(328), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(360), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(120), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gImiruFuchinPlacements7[] = {
	{ 0xdf, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(128), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gImiruFuchinEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents1[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_TurnLeaderAndExit) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunSingleStep) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyValues8And2And1) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents2[] = {
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEightForFlag301) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_RunSlot8OffsetStep) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor9Transition302) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorNineForFlag302) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorTenForFlag303) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Transition303) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_EXIT, 21, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 22, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 23, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 24, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 25, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 26, CONDITION_ALWAYS, 6 },
	{ 0x21, 27, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 28, CONDITION_ALWAYS, 8 },
	{ SEARCH(SEARCH_UNNAMED), 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetValue17e1) },
	{ SEARCH(SEARCH_CHEST), 100, 0xef1, 0x500001 },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents3[] = {
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, 0x306, EVENT_SCRIPT(FieldScene_RunActor8Transition304) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8Transition305) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8FlaggedSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8Transition304And305) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 14, 0x304, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorNineForFlag306) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor9Flag306Sequence) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorTenForFlag307) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorTenDepthSequence) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyValues11And62) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents4[] = {
	{ TOUCH_WALKING_INTO(FACING_WEST), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceActorEightByFlags) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8Transition308And309) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEightForFlags308And309Guarded) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ApplyOffset0Neg32) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEightForFlags308And309) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, IF_FLAG_SET(0x30b), EVENT_SCRIPT(FieldScene_ApplyOffset0Neg32) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorNineForFlag30a) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor9Transition30A) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceActorTenByFlags) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlag308DialogueBranch) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Transition30BTo30E) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 17, 0x308, EVENT_SCRIPT(FieldScene_RunFlag311DialogueBranch) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 18, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorTenForFlags30bAnd30d) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 18, 0x310, EVENT_SCRIPT(FieldScene_RunActor10Flags30bTo30eSequenceA) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Flags30bTo30eSequenceB) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Flags30bTo30eSequenceC) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 17, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 18, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 19, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorElevenFlaggedSteps) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, 0x308, EVENT_SCRIPT(FieldScene_RunActor11Offset128Sequence) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor11Flags30fTo312Sequence) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 22, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorElevenAt0_112) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyOffsetMinus32) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorElevenAt0_64) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorElevenAt0_80) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 24, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor11Transition) },
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_SetActor11Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_HopOnTrigger) },
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ 0x21, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ SEARCH(SEARCH_UNNAMED), 30, 0xf13, EVENT_SCRIPT(ImiruFuchin_TakeDragonsEye) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents5[] = {
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEightForFlag313) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8Transition313) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorEightStates) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 13, 0x313, EVENT_SCRIPT(FieldScene_RunActor9Flag314Sequence) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 14, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorNineForFlag314) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor9Values1And2) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorTenForFlag315) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor10Flag315Sequence) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorTenStates) },
	{ 0x21, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf6b, FIND_ITEM(0x8) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents6[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_UNNAMED), 7, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunLine17e2) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf14, FIND_ITEM(0xc8) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gImiruFuchinEvents7[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x31, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ 0x31, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ 0x31, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_StartTrackingLeader) },
	{ EVENT_TOUCH, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ImiruFuchin_StopTrackingLeader) },
	{ SEARCH(SEARCH_UNNAMED), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScriptedStep17E3) },
	{ 0xe604, 20, 0x820, EVENT_SCRIPT(FieldScene_RunFourPassCallbackSequence) },
	{ SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetWorkspace370ByFlag820) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, 0x820, EVENT_SCRIPT(ImiruFuchin_StartFadeIn) },
	{ SCENE_EVENTS_END },
};

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};
