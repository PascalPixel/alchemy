#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"

void ActorPresentation_AdvanceActorElevenStates(void);
void FieldScene_ConfigureRegionAtRow15(void);
void FieldScene_ConfigureRegionAtRow17(void);
void FieldScene_RunActor11Transition301(void);
void FieldScene_RunActor8ZeroStep(void);
void FieldScene_RunInitBracketThenSequence(void);
void FieldScene_RunPrimarySequence(void);
void FieldScene_RunScene3a6SequenceA(void);
void FieldScene_RunScene3a6SequenceB(void);
void FieldScene_RunScene3a6SequenceC(void);
void FieldScene_RunShiftAndSetFlag301(void);
void HaidiaDou_PlayCueAndRequestTouchedExit(void);
void HaidiaDou_RunLoweredActorScene(void);
void HaidiaDou_RunProbedColumnScene(void);
void HaidiaDou_SinkPillarColumn23(void);
void HaidiaDou_SinkPillarColumn27(void);
void HaidiaDou_ToppleActorNine(void);
void SceneState_SetValues8_3_4(void);

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

s32 HaidiaDou_WalkTargets[] = {
	0x2f8, 0x128,
	0x2f8, 0x118,
	0x308, 0x118,
	0x318, 0x118,
	0x328, 0x118,
	0x338, 0x118,
	0x338, 0x128,
	0x338, 0x138,
	0x328, 0x138,
	0x318, 0x138,
	0x308, 0x138,
	0x2f8, 0x138,
};

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

struct SceneEntrance gHaidiaDouEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gHaidiaDouEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
	{ 1, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
	{ 3, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gHaidiaDouEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 328, 0, 392, FACING_NORTH, 0, 48, 32, 544, 416, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 160, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 120, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ 4, CONDITION_ALWAYS, 504, 0, 136, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ 5, CONDITION_ALWAYS, 792, 0, 360, FACING_NORTH, 0, 624, 16, 944, 384, 0 },
	{ 6, CONDITION_ALWAYS, 680, 0, 248, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
	{ 7, CONDITION_ALWAYS, 904, 0, 232, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
	{ 8, CONDITION_ALWAYS, 408, 0, 872, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
	{ 9, CONDITION_ALWAYS, 488, 0, 744, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
	{ 10, CONDITION_ALWAYS, 632, 0, 552, FACING_SOUTH, 0, 312, 472, 704, 896, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gHaidiaDouEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
	{ 2, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
	{ 4, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
	{ 5, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
	{ SCENE_TABLE_END },
};

u32 HaidiaDou_Exits[] = {
	SCENE_EXITS(93),
	SCENE_EXIT(1, 6, 4),
	SCENE_EXIT(2, 94, 1),
	SCENE_EXIT(3, 6, 6),
	SCENE_EXIT(0, 94, 0),
	SCENE_EXIT(1, 93, 2),
	SCENE_EXIT(2, 94, 5),
	SCENE_EXIT(3, 94, 8),
	SCENE_EXIT(4, 95, 1),
	SCENE_EXIT(5, 94, 2),
	SCENE_EXIT(6, 94, 9),
	SCENE_EXIT(7, 95, 3),
	SCENE_EXIT(8, 94, 3),
	SCENE_EXIT(9, 94, 6),
	SCENE_EXIT(10, 95, 4),
	SCENE_EXIT(0, 95, 0),
	SCENE_EXIT(1, 94, 4),
	SCENE_EXIT(2, 95, 5),
	SCENE_EXIT(3, 94, 7),
	SCENE_EXIT(4, 94, 10),
	SCENE_EXIT(5, 95, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement gHaidiaDouPlacements1[] = {
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaDouPlacements2[] = {
	{ 0x5d, 0x70, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(344), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(456), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(376), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(422), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(440), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(384), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(400), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaDouPlacements3[] = {
	{ 0x5d, 0x70, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(200), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(240), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gHaidiaDouEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaDouEvents1[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_PlayCueAndRequestTouchedExit) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaDouEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ TOUCH_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunShiftAndSetFlag301) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor11Transition301) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorElevenStates) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorElevenStates) },
	{ 0x202, 24, 0x201, EVENT_SCRIPT(FieldScene_RunInitBracketThenSequence) },
	{ EVENT_TOUCH, 40, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceA) },
	{ EVENT_TOUCH, 41, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceB) },
	{ EVENT_TOUCH, 45, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 46, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 47, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 48, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 49, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 50, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 51, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 52, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 53, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 54, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 55, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 56, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ 0x7, 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetValues8_3_4) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfc2, FIND_ITEM(0xcc) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfc3, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ConfigureRegionAtRow17) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ConfigureRegionAtRow15) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x200, EVENT_SCRIPT(HaidiaDou_SinkPillarColumn23) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x201, EVENT_SCRIPT(HaidiaDou_SinkPillarColumn27) },
	{ PSYNERGY_ON_ACTOR(0x91), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor8ZeroStep) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaDouEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_RunProbedColumnScene) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_ToppleActorNine) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_RunLoweredActorScene) },
	{ SCENE_EVENTS_END },
};
