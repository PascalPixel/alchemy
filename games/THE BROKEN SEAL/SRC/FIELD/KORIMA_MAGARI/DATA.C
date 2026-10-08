#include "TYPES.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"

void KorimaMagari_RedrawBoard(void);
void KorimaMagari_RunDepartSequence(void);
void KorimaMagari_RunReturnSequence(void);
void KorimaMagari_TryJumpForward(void);
void Scene_RunKorimaMagariSequence(void);

u32 KorimaMagari_DefaultRecords[] = {
	0xb00cd, 0x10009, 0, 0xc00cf, 12, 0, 0x1100cf, 12,
	0, 0xd00cd, 0x1000d, 0, 0xa00cf, 18, 0, 0xffff,
	0, 0,
};

s32 KorimaMagari_PushAnimations = 0x3030202;

s32 KorimaMagari_PushStepX = 0xf80008;

s32 KorimaMagari_PushStepZ = -0x7fff800;

struct SceneEntrance gKorimaMagariEntrances[] = {
	{ 0, CONDITION_ALWAYS, 407, 0, 276, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 50, 32, 369, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 162, 32, 84, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gKorimaMagariExits[] = {
	SCENE_EXITS(43),
	SCENE_EXIT(1, 42, 2),
	SCENE_EXIT(2, 44, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gKorimaMagariPlacements[] = {
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(104), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd6, CONDITION_ALWAYS, 7,
		PIXELS(256), 0, PIXELS(464), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x112, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(348), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKorimaMagariEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMagari_TryJumpForward) },
	{ 0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMagari_RedrawBoard) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMagari_RunReturnSequence) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMagari_RunDepartSequence) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKorimaDoNotTouchFloodgate) },
	{ SEARCH(SEARCH_UNNAMED), 9, CONDITION_ALWAYS, EVENT_SCRIPT(Scene_RunKorimaMagariSequence) },
	{ SEARCH(SEARCH_CHEST), 100, 0xf00, FIND_ITEM(0x5b) },
	{ SCENE_EVENTS_END },
};
