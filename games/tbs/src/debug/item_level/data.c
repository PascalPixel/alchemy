#include "types.h"
#include "edition.h"
#include "field_scene.h"

void FieldScene_DrawThreeCaptionWindow(void);
void FieldScene_RunActor13Mode102Step(void);
void FieldScene_RunActor13Mode105Step(void);
void FieldScene_RunCountAdjustPanel(void);
void ItemLevel_RunMotionTest(void);
void ItemLevel_SelectAbility(void);
void ItemLevel_SelectItem(void);
void SceneState_GetFarResult100c(void);
void SceneState_GetFarResult1020(void);
void SceneState_SetRecordFlag53(void);

char gItemLevelLvLabel[4] = "Lv";

char gItemLevelItemPrompt[12] = "Item No:";

char gItemLevelItemHelp[24] = "A:Get Item  B:Return";

char gItemLevelItemFull[20] = "ITEM FULL........";

#if EDITION_INTERNATIONAL
char gItemLevelPsyPrompt[8] = "Psy No:";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelPsyHelp[12] = "B:Return";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelGlyphsUpper[28] = "A B C D E F G H J K L M N O";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelGlyphsLower[28] = "P Q R S T U V W X Y Z a b c";
#endif

#if EDITION_INTERNATIONAL
char gItemLevelGlyphsMarks[12] = "! ? # $ %";
#endif

struct SceneEntrance gItemLevelEntrances[] = {
	{ 0, CONDITION_ALWAYS, 824, 0, 864, FACING_NORTH, 0, 280, 504, 912, 896, 0 },
	{ SCENE_TABLE_END },
};

u32 gItemLevelExits[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement gItemLevelPlacements[] = {
	{ 0x1f4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(864), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x20, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(480), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x21, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(504), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
#if !EDITION_INTERNATIONAL
	{ 0x88, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(800), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
#elif defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{ 0x88, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(864), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x15, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x15, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
#else
	{ 0x88, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(800), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x8f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(80), 0, PIXELS(864), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x15, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(848), 0, PIXELS(688), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x15, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(816), 0, PIXELS(688), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
#endif
	{ SCENE_TABLE_END },
};

struct SceneEvent gItemLevelEvents[] = {
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_SelectItem) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_GetFarResult100c) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_GetFarResult1020) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunCountAdjustPanel) },
#if !EDITION_INTERNATIONAL
	{ PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_RunMotionTest) },
#elif defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ItemLevel_SelectAbility) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode102Step) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode105Step) },
	{ PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DrawThreeCaptionWindow) },
#else
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode102Step) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Mode105Step) },
	{ PSYNERGY_ON_ACTOR(0x8e), 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_DrawThreeCaptionWindow) },
#endif
	{ 0x10008e15, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetRecordFlag53) },
	{ SCENE_EVENTS_END },
};
