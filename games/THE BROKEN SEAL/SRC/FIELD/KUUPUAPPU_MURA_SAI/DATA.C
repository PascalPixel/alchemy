#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void ActorDraw_SetupActorSceneCells(void);
void ActorPresentation_RunActor13AcceptanceDialogue(void);
void ActorPresentation_SetAlternatePairedSceneCells(void);
void ActorPresentation_SetFlag200AndSceneCell23(void);
void ActorPresentation_SetPairedSceneCells(void);
void ActorPresentation_SetupActorEighteenAt312_304(void);
void ActorPresentation_SetupActorZeroForSceneEight(void);
void ActorPresentation_SetupActorZeroForSceneNine(void);
void ActorPresentation_SetupActorZeroForSceneTwelve(void);
void FieldScene_RunActor18MotionSequence(void);
void FieldScene_RunActor21SequenceOnce(void);
void FieldScene_RunActorEighteenDialogue(void);
void FieldScene_RunScene385SequenceA(void);
void FieldScene_SetActor21Values0And4(void);
void FieldScene_SetupScene10(void);
void FieldScene_SetupScene5(void);
void FieldScene_SetupScene7(void);
void KuupuappuMuraSai_InspectWell(void);
void KuupuappuMuraSai_RunActor14Talk(void);
void SceneDialogue_RunActor11AcceptanceDialogue(void);
void SceneDialogue_RunActor11Line(void);
void SceneDialogue_RunActor12TimedTwoFlagScene(void);
void SceneDialogue_RunActor12TwoFlagScene(void);
void SceneDialogue_RunActor16CountedDialogue(void);
void SceneDialogue_RunActor8FlagScene(void);
void SceneDialogue_RunActorEightTimedDialogue(void);
void SceneDialogue_RunActorFifteenDialogue(void);
void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void);
void SceneDialogue_RunActorFourteenFlagDialogue(void);
void SceneDialogue_ShowLine1CB0ForActor13(void);
void SceneState_SetFlag200AndConfigureRegion55_26(void);

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

s32 KuupuappuMuraSai_Actor0[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(136), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(392)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 KuupuappuMuraSai_Actor1[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(120), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(408)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

struct SceneEntrance KuupuappuMuraSai_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 408, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 312, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 216, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 296, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 120, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 217, 0, 409, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 420, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KuupuappuMuraSai_Messages[] = {
	SCENE_EXITS(23),
	SCENE_EXIT(1, 2, 3),
	SCENE_EXIT(2, 2, 44),
	SCENE_EXIT(5, 24, 5),
	SCENE_EXIT(6, 24, 6),
	SCENE_EXIT(7, 24, 7),
	SCENE_EXIT(8, 24, 8),
	SCENE_EXIT(9, 24, 9),
	SCENE_EXIT(10, 24, 10),
	SCENE_EXIT(11, 9, 11),
	SCENE_EXIT(12, 22, 10),
	SCENE_EXIT(13, 96, 10),
	SCENE_EXIT(14, 98, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement KuupuappuMuraSai_Actors[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, (s32)KuupuappuMuraSai_Actor0,
		PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x66, CONDITION_ALWAYS, (s32)KuupuappuMuraSai_Actor1,
		PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xf8, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x5a, 0x34, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

u16 KuupuappuMuraSai_Scene5Cells[] = {
	MAP_CELL_STEP(35, 52, 2, 2, 4), MAP_CELL_STEP(33, 52, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene6Cells[] = {
	MAP_CELL_STEP(35, 50, 2, 2, 4), MAP_CELL_STEP(33, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene7Cells[] = {
	MAP_CELL_STEP(35, 48, 2, 2, 4), MAP_CELL_STEP(33, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene8Cells[] = {
	MAP_CELL_STEP(42, 48, 2, 2, 4), MAP_CELL_STEP(40, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene9Cells[] = {
	MAP_CELL_STEP(42, 46, 2, 2, 4), MAP_CELL_STEP(40, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene10Cells[] = {
	MAP_CELL_STEP(35, 46, 2, 2, 4), MAP_CELL_STEP(33, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMuraSai_Scene12Cells[] = {
	MAP_CELL_STEP(35, 33, 2, 2, 4), MAP_CELL_STEP(33, 33, 2, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEvent KuupuappuMuraSai_Extras[] = {
	{ EVENT_EXIT, 20, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 21, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 40, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 41, CONDITION_ALWAYS, 14 },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetFlag200AndConfigureRegion55_26) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetFlag200AndSceneCell23) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupScene5) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorEighteenAt312_304) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupScene7) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorZeroForSceneEight) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorZeroForSceneNine) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetupScene10) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorDraw_SetupActorSceneCells) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetupActorZeroForSceneTwelve) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor8FlagScene) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYouveProbablyAlreadyHeardButThe) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWonderWhatHappenedToHammet) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11Line) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor12TwoFlagScene) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine1CB0ForActor13) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorFourteenFlagDialogue) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorFifteenFacingPreservedDialogue) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor16CountedDialogue) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheThievesEscapedButIThink) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorEighteenDialogue) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThereYouSeeTheRoofRepairs) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SetActor21Values0And4) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor21SequenceOnce) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, 0x305, EVENT_SCRIPT(SceneDialogue_RunActorEightTimedDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuICantBelieveTheyreHereWhat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhenThoseGuysFromKalayCame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYeahSomeWarriorsFromKalayCame) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11AcceptanceDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40c, 0x306, EVENT_SCRIPT(SceneDialogue_RunActor12TimedTwoFlagScene) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThoseStupidThievesEscapedAndWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActor13AcceptanceDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40e, 0x307, EVENT_SCRIPT(KuupuappuMuraSai_RunActor14Talk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMuraSai_RunActor14Talk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorFifteenDialogue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x410, 0x308, EVENT_SCRIPT(FieldScene_RunScene385SequenceA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene385SequenceA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIKeepWonderingWasOurStatue) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x858), MSGNO(MsgKuupuappuCarefulSearchWillRevealPassage) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIfYouBringMeSomeBones) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThatsTheLastOfTheRoof) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMuraSai_InspectWell) },
	{ SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
	{ 0xe714, 18, 0x858, EVENT_SCRIPT(FieldScene_RunActor18MotionSequence) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetAlternatePairedSceneCells) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_SetPairedSceneCells) },
	{ SCENE_EVENTS_END },
};
