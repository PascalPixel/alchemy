#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"
#include "OBJCMD.H"
#include "text/MSG_IDS.H"

void ConfigureActorThirteenSceneParameters(void);
void ConfigureSceneForActorEightColumn(void);
void FieldScene_RunActor13Departure(void);
void FieldScene_RunFourStepSequence(void);
void FieldScene_RunPillarSequence(void);
void FieldScene_RunThreeCallSequence(void);
void FieldScene_TryJumpForward(void);
void OverlayObject_ApplyValue15(void);

extern s32 GomaHashira_PillarScript0[];
extern s32 GomaHashira_PillarScript1[];
extern s32 GomaHashira_PillarScript2[];

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

s32 *GomaHashira_PillarScripts[] = {
	GomaHashira_PillarScript0,
	GomaHashira_PillarScript1,
	GomaHashira_PillarScript2,
};

s32 GomaHashira_PillarScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_FREE,
};

s32 GomaHashira_PillarScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_FREE,
};

s32 GomaHashira_PillarScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_CALL(OverlayObject_ApplyValue15),
	SC_WAIT(1),
	SC_FREE,
};

struct SceneEntrance GomaHashira_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ 2, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ 3, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ 4, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ 5, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ 99, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ SCENE_TABLE_END },
};

u32 GomaHashira_Messages[] = {
	SCENE_EXITS(27),
	SCENE_EXIT(1, 28, 2),
	SCENE_EXIT(2, 26, 1),
	SCENE_EXIT(3, 26, 2),
	SCENE_EXIT(4, 26, 3),
	SCENE_EXIT(5, 28, 3),
	SCENE_EXITS_END,
};

struct SceneEntrance GomaHashira_Actors[] = {
	{ 211, CONDITION_ALWAYS, 1, 0, 0, 0x148, 0, 0, 0, 104, 16384, 2 },
	{ 212, CONDITION_ALWAYS, 1, 0, 0, 0x180, 0, 0, 0, 192, 16384, 2 },
	{ 211, CONDITION_ALWAYS, 1, 0, 0, 0x248, 0, 0, 0, 280, 16384, 2 },
	{ 223, CONDITION_ALWAYS, 1, 0, 0, 0x8, 0, 0, 0, 8, 16384, 2 },
	{ 92, 0x58, 1, 0, 0, 0x298, 0, 0, 0, 216, 16384, 0 },
	{ 134, 0x869, 1, 0, 0, 0x274, 0, 0, 0, 184, 8192, 1 },
	{ SCENE_TABLE_END },
};

struct SceneEvent GomaHashira_Extras[] = {
	{ 0x21, 5, CONDITION_ALWAYS, 1 },
	{ 0x31, 6, CONDITION_ALWAYS, 2 },
	{ 0x31, 7, CONDITION_ALWAYS, 3 },
	{ 0x31, 8, CONDITION_ALWAYS, 4 },
	{ 0x21, 9, CONDITION_ALWAYS, 5 },
	{ 0x202, 10, 0x301, EVENT_SCRIPT(FieldScene_RunFourStepSequence) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_TryJumpForward) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureActorThirteenSceneParameters) },
	{ EVENT_TALK, 13, 0xa8, MSGNO(MsgKuupuappuDidYouSeeSomethingStrangeOver) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuHmmmItMustBeHidingSomewhere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0xa8, MSGNO(MsgKuupuappuItJustWhackedMeInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIKnowitsKindItllBeBack) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(ConfigureSceneForActorEightColumn) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x300, EVENT_SCRIPT(FieldScene_RunPillarSequence) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x301, EVENT_SCRIPT(FieldScene_RunThreeCallSequence) },
	{ SEARCH(SEARCH_CHEST), 101, 0xf4f, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EVENT_RAISED, 99, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor13Departure) },
	{ SCENE_EVENTS_END },
};
