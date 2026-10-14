// alchemy camelotize constants: a scene with every kind of C id position,
// and literals of the same values where no id is written.
#include "event.h"

#define GATE_OPENED 0x240 // the alias aliases.tsv gives 0x240 here

static void EV08_TALK(s32 a, s32 group, struct EventRuntime *ev)
{
	if (GameFlag_IsSet(0x909) != 0)
	{
		GameFlag_Set(0x240);
		Engine_GameFlagClear((u16)0x182);
		FLAG_SET(0x400);
	}
	if (FieldEvent_GameFlagIsSet(0x909))
		Engine_AudioPlayCue(74);
	GameFlag_SetBitFar(0x16a);
	Actor_SetPosition(8, 0x909, 0);
	group = 0x909;
	WAIT(74);
	// GameFlag_Set(0x909);
	Engine_EventSetMessage("GameFlag_Set(0x909)");
	BtlEvPush(BEV_NUM, 74);
	BtlEvPush(BEV_SE, 113);
	SE(0x71);
	Event_PlayCue(-1);
	GameFlag_Clear(GATE_OPENED);
	GameFlag_Clear(group);
	GameFlag_Set(0x180 + group);
	GameFlag_Clear(0x123);
	GameFlag_Set(0x16e);
	Engine_GameFlagSet(181 << 1);
	GameFlag_IsConditionActive(-1);
	GameFlag_IsConditionActive(0x909);
	ev->pending_cue = 113;
	gGameState.return_cue = -1;
	Engine_AudioPlayCue(a + 90);
	SetModeAndPlayCue(ev, 3, 0x4a);
	if (ev->pending_cue == 74)
		x = 113;
}

struct ScenePlacement gScenePlacements[] = {
	{0x6c, CONDITION_ALWAYS, ACTOR_STAND, PIXELS(104), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY,
		0},
	{0x6a, 0x0, ACTOR_STAND, PIXELS(669), 0, PIXELS(128), FACING_EAST, TALK_FACE_PARTY, 0},
	{0x6a, 0x909, ACTOR_STAND, PIXELS(0x909), 0, PIXELS(128), FACING_EAST, TALK_FACE_PARTY, 0},
	{0x67, IF_FLAG_SET(0x1), ACTOR_WANDER, PIXELS(350), 0, PIXELS(286), FACING_NORTH,
		TALK_FACE_PARTY, 0},
	{SCENE_TABLE_END},
};

const struct ScenePlacement *Scene_GetPlacements(void);

struct SceneEvent gSceneEvents[] = {
	{EVENT_TALK, 8, 0x909, EVENT_SCRIPT(EV08_TALK)},
	{EVENT_TALK, 9, 0x1909, MSGNO(MsgSceneThankYou)},
	{EVENT_TALK, 0x909, -1, 0x909},
	{SEARCH(SEARCH_BARREL), 100, 0x240, FIND_ITEM(ITEM_NUT)},
	{0x69c05860, 0x7f20, 0x69c0, 0x48005860},
	{SCENE_EVENTS_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(75),
	SCENE_EXIT_IF(1, 4, 2, 0x814),
	SCENE_EXIT_IF(1, 4, 2, 0xffffffff),
	SCENE_EXITS_END,
};

// 0x8000 | 曲, 続いてその曲を選ぶ組
const u16 gBattleCueTable[5] = {
	0x8036,
	0x804a,
	0x0620,
	0x0000,
};

static const s8 Shop_PartyActionCues[4] =
{
	0x52, 0x54, 0x58, 0x57,
};
