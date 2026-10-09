#include "types.h"
#include "ability_ids.h"
#include "edition.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void CommandTable_ConfigureCommandGroups(void);
void CommandTable_NoOpCallback(void);
void DebugMenu_GiveItemToParty(void);
void DebugMenu_RunMessageTest(void);
void DebugMenu_SelectCharacter(void);
void FieldScene_ApplySlotOffsetsAndFlags(void);
void FieldScene_AssignCodeSetAToSlots(void);
void FieldScene_AssignCodeSetBToSlots(void);
void FieldScene_GrantItemListToSlots(void);
void MenuTest_RaisePartyLevelTarget(void);
void SceneState_ApplyBlockC9b(void);
void SceneState_ApplyBlockCc6(void);
void SceneState_ApplyBlockCf1(void);
void SceneState_ApplyBlockD21(void);
void SceneState_ApplyBlockD4c(void);
void SceneState_ApplyBlockD77(void);
void SceneState_ApplyBlockDa2(void);
void SceneState_ApplyOne(void);
void SceneState_ApplyZero(void);
void SceneState_GetFarResult2384(void);
void SceneState_GetFarResult2418(void);
void SceneState_QueryTwoValues(void);
void SceneState_RunCall1c00(void);
void SceneState_SetRecordFlag53(void);

u32 gDebugItemPromptHead[] = {
	0x500050,
	0,
	80,
	0x500000,
	0x10003,
	2,
	0,
	0x780078,
	0,
	120,
	0x780000,
	0x10003,
	2,
	0,
	0x28003c,
	0,
	128,
	0x800000,
	0x10002,
	1,
	0,
	0x28003c,
	0,
	128,
	0x800000,
	0x10002,
	3,
	0,
	0x800080,
	0,
	128,
	0x800000,
	0x20003,
	1,
	0,
	0x800080,
	0,
	128,
	0x800000,
	0x20003,
	1,
	0,
	0x200020,
	0,
	0,
	0,
	0,
	0,
	0,
	0x800080,
	0,
	128,
	0x800000,
	0x20003,
	1,
	0,
};

char gDebugItemPrompt[12] = "Item No:";

char gDebugItemCapacityLabel[24] = "A:Get Item  B:Return";

char gDebugItemFullLabel[20] = "ITEM FULL........";

struct SceneEntrance MenuTest_Entrances[] = {
	{0, CONDITION_ALWAYS, 60, 0, 94, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 MenuTest_Exits[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement MenuTest_Placements[] = {
	{GERALD, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(48), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{JASMINE, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(80), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{IWAN, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(112), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{MEARI, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(40), 0, PIXELS(144), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x1e, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(48), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x1f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(80), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x20, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(112), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x21, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(80), 0, PIXELS(144), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x7d, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(48), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x1f, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(80), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x20, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(112), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0xf8, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(120), 0, PIXELS(144), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x23, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(160), 0, PIXELS(48), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{0x23, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(160), 0, PIXELS(80), FACING_SOUTHEAST,
		TALK_FACE_PARTY, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent MenuTest_Events[] = {
	{EVENT_TALK, 1, CONDITION_ALWAYS, EVENT_SCRIPT(MenuTest_RaisePartyLevelTarget)},
	{EVENT_TALK, 5, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_AssignCodeSetAToSlots)},
	{EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_GrantItemListToSlots)},
	{EVENT_TALK, 3, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyZero)},
	{EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_QueryTwoValues)},
	{EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(CommandTable_ConfigureCommandGroups)},
	{EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ApplySlotOffsetsAndFlags)},
	{EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_AssignCodeSetBToSlots)},
	{EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyOne)},
#if defined(TBS_EDITION_FR)
	{EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_GiveItemToParty)},
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
	{EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTorebiFuneThenTheyAreDangerous)},
#else
	{EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_GetFarResult2418)},
#endif
	{EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgHaidiaKyleAbleStop)},
	{EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_GetFarResult2384)},
#if !EDITION_INTERNATIONAL
	{EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_SelectCharacter)},
	{EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_RunCall1c00)},
#elif defined(TBS_EDITION_EN)
	{0xc400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockC9b)},
	{0x8400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockCc6)},
	{0x400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockCf1)},
	{0x4400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockD21)},
	{0xc400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockD4c)},
	{0x8400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockD77)},
	{0x400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockDa2)},
#else
	{0xc400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockC9b)},
	{0x8400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockCc6)},
	{0x400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockCf1)},
	{0x4400, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockD21)},
	{0xc400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockD4c)},
	{0x8400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockD77)},
	{0x400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_ApplyBlockDa2)},
	{0x4400, 17, CONDITION_ALWAYS, EVENT_SCRIPT(DebugMenu_RunMessageTest)},
#endif
	{PSYNERGY_ON_ACTOR(0x4e), 15, CONDITION_ALWAYS, EVENT_SCRIPT(CommandTable_NoOpCallback)},
	{PSYNERGY_ON_ACTOR(0x8e), 15, CONDITION_ALWAYS, EVENT_SCRIPT(CommandTable_NoOpCallback)},
	{PSYNERGY_ON_ACTOR(ABILITY_CATCH), 15, CONDITION_ALWAYS,
		EVENT_SCRIPT(CommandTable_NoOpCallback)},
	{0x10008e15, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneState_SetRecordFlag53)},
	{SCENE_EVENTS_END},
};

s32 MenuTest_SlotOffsets = 10;

s32 MenuTest_BenchmarkCharacter = 65;
