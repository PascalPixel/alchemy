#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

void Effect_SetAlphaBlendForScene9(void);
void Scene_ApplyOffset0Neg5(void);
void Scene_ApplyOffset0Neg5Second(void);
void Scene_ApplyOffset0Neg6(void);
void Scene_ApplyOffset0Pos5(void);
void Scene_ApplyOffset0Pos5Second(void);
void Scene_ApplyOffset0Pos6(void);
void State_SetActorEightValue3d(void);
void ToretoEda_PlayCueAndSelectTouchedExit(void);

struct SceneEntrance ToretoEda_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 100, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 328, 0, 424, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 424, 0, 440, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 376, 0, 360, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 424, 0, 328, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 376, 0, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 328, 0, 248, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 328, 0, 152, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 ToretoEda_SceneTable1[] = {
	SCENE_EXITS(46),
	SCENE_EXIT(1, 45, 2),
	SCENE_EXIT(2, 45, 3),
	SCENE_EXIT(3, 45, 4),
	SCENE_EXIT(4, 45, 5),
	SCENE_EXIT(5, 45, 7),
	SCENE_EXIT(6, 45, 6),
	SCENE_EXIT(7, 45, 8),
	SCENE_EXIT(8, 45, 9),
	SCENE_EXITS_END,
};

struct ScenePlacement ToretoEda_SceneTable2[] = {
	{ 0x5d, 0x6d, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(88), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent ToretoEda_SceneTable3[] = {
	{ 0xa02, 1, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 2, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 3, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 4, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 5, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 6, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 7, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ 0xa02, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ToretoEda_PlayCueAndSelectTouchedExit) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_ApplyOffset0Pos5) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_ApplyOffset0Neg5) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_ApplyOffset0Pos5Second) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_ApplyOffset0Neg5Second) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_ApplyOffset0Pos6) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_ApplyOffset0Neg6) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(State_SetActorEightValue3d) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 2), 0, CONDITION_ALWAYS, EVENT_SCRIPT(Effect_SetAlphaBlendForScene9) },
	{ SCENE_EVENTS_END },
};

u16 ToretoEda_CellSteps[] = {
	MAP_CELL_STEP(0, 96, 2, 2, 2), MAP_CELL_STEP(2, 96, 2, 2, 2), MAP_CELL_STEPS_END,
};
