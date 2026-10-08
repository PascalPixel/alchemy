#include "types.h"
#include "field_scene.h"

void ConfigureAlternateSceneAndCheckActors(void);
void ConfigureSceneAndCheckActors(void);
void FieldScene_ApplyRect10_14_7_13_7(void);
void FieldScene_ApplyRect12_21_7_22_7(void);
void FieldScene_ApplyRect13_31_12_30_12(void);
void FieldScene_CallWhenCheck9_31_9(void);
void FieldScene_RunClosingSequence(void);
void FieldScene_RunFiveValueStep11(void);
void FieldScene_RunFiveValueStep15(void);
void FieldScene_RunFiveValueStep9(void);
void FieldScene_RunGuardedStep11(void);
void FieldScene_RunGuardedStep13(void);
void FieldScene_RunGuardedStep15(void);
void FieldScene_RunScene37bSequenceA(void);
void SceneActor_ApplyActorTwelveZCellPair(void);
void SceneActor_MoveActor10ByRow(void);
void SceneActor_RunSlot12ColumnStep(void);
void SceneActor_UseActorTenCellAndNext(void);
void SceneState_RunWhenActorZeroFacing4000(void);
void SceneState_RunWhenSlotZeroFacingC000(void);
void SoruSekizo_BlockLeftTriggerCell(void);
void SoruSekizo_BlockRightTriggerCell(void);
void SoruSekizo_RunStatueDropScene(void);

s32 SoruSekizo_SpriteRowsGfx[] = { 0 };

struct SceneEntrance SceneEventRuntime_ScriptData[] = {
	{ 0, CONDITION_ALWAYS, 287, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 646, 0, 329, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 647, 0, 470, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 99, CONDITION_ALWAYS, 576, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SceneEventRuntime_MessageData[] = {
	SCENE_EXITS(12),
	SCENE_EXIT(1, 11, 2),
	SCENE_EXIT(2, 13, 1),
	SCENE_EXIT(3, 11, 4),
	SCENE_EXIT(4, 11, 5),
	SCENE_EXIT(5, 11, 6),
	SCENE_EXIT(6, 11, 7),
	SCENE_EXIT(10, 510, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement SceneEventRuntime_ActorData[] = {
	{ 0x1e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(488), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(152), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(488), 0, PIXELS(200), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(216), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(200), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xc9, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(216), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xca, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(288), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcc, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(1608), 0, PIXELS(1608), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ JASMINE, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(560), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(592), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent SceneEventRuntime_EffectData[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_RunWhenSlotZeroFacingC000) },
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_RunWhenActorZeroFacing4000) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x80b, EVENT_SCRIPT(FieldScene_CallWhenCheck9_31_9) },
	{ PSYNERGY_ON_ACTOR(0x8c), 11, 0x80c, EVENT_SCRIPT(FieldScene_RunGuardedStep11) },
	{ PSYNERGY_ON_ACTOR(0x8c), 13, 0x80d, EVENT_SCRIPT(FieldScene_RunGuardedStep13) },
	{ PSYNERGY_ON_ACTOR(0x8c), 15, 0x80e, EVENT_SCRIPT(FieldScene_RunGuardedStep15) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 5, 0x80b, EVENT_SCRIPT(FieldScene_RunFiveValueStep9) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 6, 0x80c, EVENT_SCRIPT(FieldScene_RunFiveValueStep11) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 7, 0x80d, EVENT_SCRIPT(FieldScene_ApplyRect13_31_12_30_12) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 8, 0x80e, EVENT_SCRIPT(FieldScene_RunFiveValueStep15) },
	{ 0x10008c15, 10, 0x816, EVENT_SCRIPT(SoruSekizo_BlockLeftTriggerCell) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x816, EVENT_SCRIPT(ConfigureSceneAndCheckActors) },
	{ 0x10008c15, 12, 0x817, EVENT_SCRIPT(SoruSekizo_BlockRightTriggerCell) },
	{ PSYNERGY_ON_ACTOR(0x8c), 12, 0x817, EVENT_SCRIPT(ConfigureAlternateSceneAndCheckActors) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 20, 0x816, EVENT_SCRIPT(FieldScene_ApplyRect10_14_7_13_7) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 21, 0x816, EVENT_SCRIPT(FieldScene_ApplyRect10_14_7_13_7) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, 0x816, EVENT_SCRIPT(SceneActor_UseActorTenCellAndNext) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 23, 0x816, EVENT_SCRIPT(SceneActor_MoveActor10ByRow) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 24, 0x816, EVENT_SCRIPT(SceneActor_MoveActor10ByRow) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 25, 0x817, EVENT_SCRIPT(FieldScene_ApplyRect12_21_7_22_7) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 26, 0x817, EVENT_SCRIPT(FieldScene_ApplyRect12_21_7_22_7) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 26, 0x817, EVENT_SCRIPT(SceneActor_ApplyActorTwelveZCellPair) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 28, 0x817, EVENT_SCRIPT(SceneActor_RunSlot12ColumnStep) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 29, 0x817, EVENT_SCRIPT(SceneActor_RunSlot12ColumnStep) },
	{ 0x10008c15, 17, 0x818, EVENT_SCRIPT(FieldScene_RunClosingSequence) },
	{ PSYNERGY_ON_ACTOR(0x8c), 17, 0x818, EVENT_SCRIPT(SoruSekizo_RunStatueDropScene) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 32, 0x818, EVENT_SCRIPT(FieldScene_RunScene37bSequenceA) },
	{ SCENE_EVENTS_END },
};

s32 SoruSekizo_ScrollPhase = 0;
