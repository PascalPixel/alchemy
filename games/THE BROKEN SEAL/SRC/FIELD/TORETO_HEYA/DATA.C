#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"

void FieldScene_RunStep200(void);
void FieldScene_RunStep201(void);
void FieldScene_RunStep202(void);
void FieldScene_RunStep203(void);
void FieldScene_RunStep204(void);
void FieldScene_RunStep205(void);
void FieldScene_RunStep206(void);
void FieldScene_RunStep207(void);
void FieldScene_RunStep208(void);
void FieldScene_RunStep209(void);
void FieldScene_RunStep20a(void);
void FieldScene_RunStep20b(void);
void FieldScene_RunStep20c(void);
void FieldScene_RunStep20d(void);
void FieldScene_RunStep20e(void);
void FieldScene_RunStep20f(void);
void FieldScene_RunStep210(void);
void FieldScene_RunStep211(void);
void FieldScene_RunStep212(void);
void FieldScene_RunStep213(void);
void FieldScene_RunStep214(void);
void SceneEffect_RegisterPaletteFadeCallback(void);
void SceneState_ClearStoryVariantWhenIdle(void);
void ToretoHeya_RunTableScene(void);

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

u32 ToretoHeya_MapPatches[] = {
	0x10200, 0x230040, 0x60071, 513, 0x230041, 0x60073, 0x10202, 0x230042,
	0x8006e, 0x10203, 0x230043, 0xa006e, 0x10204, 0x230044, 0xc006f, 0x10205,
	0x230045, 0xc0071, 0x10206, 0x230046, 0xe0070, 0x10207, 0x230047, 0xe0072,
	0x10208, 0x230048, 0xe0074, 0x10209, 0x230049, 0x170047, 0x1020a, 0x23004a,
	0x18004d, 0x1020c, 0x23004b, 0x180070, 525, 0x23004c, 0x180072, 0x1020e,
	0x23004d, 0x180074, 527, 0x23004e, 0x1a0070, 0x1020b, 0x23004f, 0x1a0072,
	0x10210, 0x230050, 0x1a0074, 0x10211, 0x230051, 0x1c0070, 0x10212, 0x230052,
	0x1c0072, 531, 0x230053, 0x1c0074, 0x10214, 0x230054, 0x1c0076, 0xffff,
};

s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 ToretoHeya_TableActions0[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(166), 0, PIXELS(80)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 ToretoHeya_TableActions1[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(148), 0, PIXELS(90)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 ToretoHeya_TableActions2[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(182), 0, PIXELS(90)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 ToretoHeya_TableActions3[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(166), 0, PIXELS(104)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 ToretoHeya_ActionTable1[] = {
	SC_SET(SW_MOTION, 0),
	SC_ADD(SW_Y, PIXELS(80)),
	SC_WAIT(4),
	SC_ADD(SW_Y, PIXELS(-78)),
	SC_ADD(SW_SCALE_X, -0x1000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(4),
	SC_LOOP(8, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 ToretoHeya_ActionTable2[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(166), 0, PIXELS(80)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

u32 ToretoHeya_TintSteps[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	1, 0, 0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 1, 0, 0, 1,
	0, 0, 1, 0, 0, 1, 1, 0,
	1, 1, 0, 1, 1, 0, 1, 1,
	0, 1, 1, 0, 1, 1, 0, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 2, 1,
	1, 2, 1, 1, 2, 1, 1, 2,
	1, 1, 2, 2, 1, 2, 2, 1,
	2, 2, 2, 3, 2, 2, 3, 2,
	2, 3, 3, 2, 3, 3, 2, 3,
	3, 2, 3, 3, 2, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 4, 3, 3, 4,
	3, 3, 4, 3, 3, 4, 3, 3,
	4, 3, 3, 4, 3, 3, 4, 4,
	3, 4, 4, 3, 4, 4, 3, 4,
	4, 3, 4, 4, 3, 4, 4, 3,
	4, 4, 3, 4, 4, 3, 4, 4,
	3, 4, 4, 3, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 4, 4, 4, 4, 4, 4,
	4, 4, 3, 4, 4, 3, 4, 4,
	3, 4, 4, 3, 4, 4, 3, 4,
	4, 3, 4, 4, 3, 4, 4, 3,
	4, 4, 3, 4, 4, 3, 4, 3,
	3, 4, 3, 3, 4, 3, 3, 4,
	3, 3, 4, 3, 3, 4, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 3, 3, 3, 3, 3, 3, 3,
	3, 2, 3, 3, 2, 3, 3, 2,
	3, 3, 2, 3, 2, 2, 3, 2,
	2, 2, 2, 2, 2, 2, 1, 2,
	2, 1, 2, 1, 1, 2, 1, 1,
	2, 1, 1, 2, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0,
	1, 1, 0, 1, 1, 0, 1, 1,
	0, 1, 1, 0, 1, 1, 0, 1,
	0, 0, 1, 0, 0, 1, 0, 0,
	1, 0, 0, 1, 0, 0, 1, 0,
	0, 1, 0, 0, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99, 99, 99, 99, 99, 99,
	99, 99, 99,
};

struct SceneEntrance gToretoHeyaEntrances[] = {
	{ 0, CONDITION_ALWAYS, 136, 0, 136, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 488, 0, 248, FACING_NORTH, 0, 352, 0, 640, 272, 0 },
	{ 2, CONDITION_ALWAYS, 712, 0, 152, FACING_EAST, 0, 672, 0, 944, 272, 0 },
	{ 3, CONDITION_ALWAYS, 920, 0, 152, FACING_WEST, 0, 672, 0, 944, 272, 0 },
	{ 4, CONDITION_ALWAYS, 168, 0, 520, FACING_NORTH, 0, 32, 272, 304, 544, 0 },
	{ 5, CONDITION_ALWAYS, 296, 0, 424, FACING_WEST, 0, 32, 272, 304, 544, 0 },
	{ 6, CONDITION_ALWAYS, 376, 0, 424, FACING_EAST, 0, 352, 272, 640, 544, 0 },
	{ 7, CONDITION_ALWAYS, 488, 0, 520, FACING_NORTH, 0, 352, 272, 640, 544, 0 },
	{ 8, CONDITION_ALWAYS, 712, 0, 424, FACING_EAST, 0, 672, 272, 960, 544, 0 },
	{ 9, CONDITION_ALWAYS, 920, 0, 424, FACING_WEST, 0, 672, 272, 960, 544, 0 },
	{ 10, CONDITION_ALWAYS, 168, 0, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
	{ 11, CONDITION_ALWAYS, 424, 24, 72, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 12, CONDITION_ALWAYS, 744, 0, 88, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
	{ 13, CONDITION_ALWAYS, 232, 24, 344, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
	{ 14, CONDITION_ALWAYS, 552, 0, 360, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 15, CONDITION_ALWAYS, 168, 24, 152, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
	{ 16, CONDITION_ALWAYS, 488, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 21, CONDITION_ALWAYS, 472, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 22, CONDITION_ALWAYS, 504, 0, 104, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 23, CONDITION_ALWAYS, 424, 0, 136, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 24, CONDITION_ALWAYS, 424, 0, 168, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 25, CONDITION_ALWAYS, 440, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 26, CONDITION_ALWAYS, 488, 0, 200, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 27, CONDITION_ALWAYS, 456, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 28, CONDITION_ALWAYS, 488, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 29, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 31, CONDITION_ALWAYS, 760, 0, 104, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
	{ 32, CONDITION_ALWAYS, 856, 0, 120, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
	{ 51, CONDITION_ALWAYS, 456, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 52, CONDITION_ALWAYS, 488, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 53, CONDITION_ALWAYS, 520, 0, 392, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 54, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 55, CONDITION_ALWAYS, 520, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 56, CONDITION_ALWAYS, 456, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 57, CONDITION_ALWAYS, 488, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 58, CONDITION_ALWAYS, 520, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 59, CONDITION_ALWAYS, 552, 0, 456, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 50, CONDITION_ALWAYS, 488, 0, 424, FACING_SOUTH, 0, 352, 272, 640, 544, 0 },
	{ 40, CONDITION_ALWAYS, 168, 0, 424, FACING_SOUTH, 0, 32, 272, 304, 544, 0 },
	{ 30, CONDITION_ALWAYS, 808, 0, 152, FACING_SOUTH, 0, 672, 0, 944, 272, 0 },
	{ 20, CONDITION_ALWAYS, 488, 0, 152, FACING_SOUTH, 0, 352, 0, 640, 272, 0 },
	{ 19, CONDITION_ALWAYS, 168, 0, 72, FACING_SOUTH, 0, 32, 0, 304, 272, 0 },
	{ SCENE_TABLE_END },
};

s16 gToretoHeyaRegions[] = {
	420, 28, 68, 428, 36, 76, -1, 10,
	228, 28, 338, 236, 36, 346, -1, 12,
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	163, 28, 148, 171, 36, 156, -1, 14,
#else
	159, 24, 144, 175, 40, 160, -1, 14,
#endif
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gToretoHeyaExits[] = {
	SCENE_EXITS(45),
	SCENE_EXIT(1, 44, 2),
	SCENE_EXIT(2, 46, 1),
	SCENE_EXIT(3, 46, 2),
	SCENE_EXIT(4, 46, 3),
	SCENE_EXIT(5, 46, 4),
	SCENE_EXIT(6, 46, 6),
	SCENE_EXIT(7, 46, 5),
	SCENE_EXIT(8, 46, 7),
	SCENE_EXIT(9, 46, 8),
	SCENE_EXIT(10, 45, 12),
	SCENE_EXIT(11, 45, 11),
	SCENE_EXIT(12, 45, 14),
	SCENE_EXIT(13, 45, 13),
	SCENE_EXIT(14, 45, 16),
	SCENE_EXIT(15, 45, 15),
	SCENE_EXITS_END,
};

struct ScenePlacement gToretoHeyaPlacements[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x37, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x47, CONDITION_ALWAYS, 7,
		PIXELS(168), 0, PIXELS(48), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gToretoHeyaEvents[] = {
	{ 0x4401, 1, CONDITION_ALWAYS, 1 },
	{ 0x8401, 2, CONDITION_ALWAYS, 2 },
	{ 0x401, 3, CONDITION_ALWAYS, 3 },
	{ 0x4401, 4, CONDITION_ALWAYS, 4 },
	{ 0x401, 5, CONDITION_ALWAYS, 5 },
	{ 0x8401, 6, CONDITION_ALWAYS, 6 },
	{ 0x4401, 7, CONDITION_ALWAYS, 7 },
	{ 0x8401, 8, CONDITION_ALWAYS, 8 },
	{ 0x401, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ 0x202, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 23, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 25, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 26, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 27, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 28, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 29, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 31, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 32, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 50, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 51, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 52, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 53, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 54, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 55, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 56, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 57, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 58, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ 0x202, 59, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ClearStoryVariantWhenIdle) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep200) },
	{ EVENT_TOUCH, 22, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep201) },
	{ EVENT_TOUCH, 23, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep202) },
	{ EVENT_TOUCH, 24, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep203) },
	{ EVENT_TOUCH, 25, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep204) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep205) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep206) },
	{ EVENT_TOUCH, 28, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep207) },
	{ EVENT_TOUCH, 29, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep208) },
	{ EVENT_TOUCH, 31, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep209) },
	{ EVENT_TOUCH, 32, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep20a) },
	{ EVENT_TOUCH, 50, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep20b) },
	{ EVENT_TOUCH, 51, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep20c) },
	{ EVENT_TOUCH, 52, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep20d) },
	{ EVENT_TOUCH, 53, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep20e) },
	{ EVENT_TOUCH, 54, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep20f) },
	{ EVENT_TOUCH, 55, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep210) },
	{ EVENT_TOUCH, 56, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep211) },
	{ EVENT_TOUCH, 57, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep212) },
	{ EVENT_TOUCH, 58, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep213) },
	{ EVENT_TOUCH, 59, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStep214) },
	{ EVENT_TOUCH, 40, 0x844, EVENT_SCRIPT(ToretoHeya_RunTableScene) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(SceneEffect_RegisterPaletteFadeCallback) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf5b, FIND_ITEM(0x109) },
	{ SEARCH(SEARCH_CHEST), 101, 0xf5c, FIND_ITEM(ITEM_NUT) },
	{ SEARCH(SEARCH_UNNAMED), 102, 0x350, FIND_PSYNERGY_STONE },
	{ SCENE_EVENTS_END },
};

s32 ToretoHeya_TintIndex = 0;
