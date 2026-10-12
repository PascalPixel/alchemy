#include "unit/field/toreto_eda/eda.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "global_cells.h"
#include "types.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "resource_397.h"
#include "io_reg.h"
#include "irq.h"
#include "game_state.h"
#include "ability_ids.h"
#include "field_event.h"
#include "field_scene.h"

// 前方宣言
static void ToretoEda_StartBg3Split(void);

struct SceneObject
{
	u8	unknown_00[8];
	s32 x;
	s32 y;
	s32 z;
	s32 settled_y;
	u8	unknown_18[10];
	u8	layer;
};

union SceneCell
{
	s32 w;
	s16 h[2];
};

static u16 sTransferHeader[16];
static u8  sTransferData[0x60];
static s32 sSplitLine;
static u16 sHofsAbove;
static u16 sHofsBelow;

static void EV08_TALK(void)
{
	BattleFx_SetPhaseRequest(8, 0x3D);
}

u8 *ToretoEda_GetEntrances(void)
{
	return (u8 *)ToretoEda_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *ToretoEda_GetExits(void)
{
	return (u8 *)ToretoEda_SceneTable1;
}

u8 *ToretoEda_GetPlacements(void)
{
	return (u8 *)ToretoEda_SceneTable2;
}

static void Actor_ShiftObjectsByBlock(s32 bx, s32 bz)
{
	u8				   *work = *(u8 **)&gEventWork;
	struct SceneObject *obj;
	s32					dx = bx;
	s32					dz = bz;
	s32					h;

	obj = (struct SceneObject *)Object_GetById(gGameState.selected_actor);
	dx <<= 20;
	dz <<= 20;

	if (obj != 0)
	{
		obj->x += dx;
		obj->z += dz;
		h = Map_GetTerrainHeight(obj->layer, obj->x, obj->z);
		obj->y = h;
		obj->settled_y = h;
	}

	obj = *(struct SceneObject **)(work + 480);
	if (obj != 0)
	{
		obj->x += dx;
		obj->z += dz;
		h = Map_GetTerrainHeight(obj->layer, obj->x, obj->z);
		obj->y = h;
		obj->settled_y = h;
	}
}

static void EV10_TOUCH(void)
{
	Actor_ShiftObjectsByBlock(0, 5);
}

static void EV11_TOUCH(void)
{
	Actor_ShiftObjectsByBlock(0, -5);
}

static void EV12_TOUCH(void)
{
	Actor_ShiftObjectsByBlock(0, 5);
}

static void EV13_TOUCH(void)
{
	Actor_ShiftObjectsByBlock(0, -5);
}

static void EV14_TOUCH(void)
{
	Actor_ShiftObjectsByBlock(0, 6);
}

static void EV15_TOUCH(void)
{
	Actor_ShiftObjectsByBlock(0, -6);
}

static void ToretoEda_PlayCueAndSelectTouchedExit(void)
{
	struct EventWork *state = gEventWork;
	s16				 *cnt;

	Engine_AudioPlayCue(0x7B);
	cnt = &state->touched_trigger;
	Engine_EventRequestExit(*cnt);
}

static void EV00_PSYNERGY(void)
{
	u8 *disp;

	DisplayTransition_InitializeBattleEffectState(9);

	REG_BLDCNT = 0x3f42;
	REG_BLDALPHA = 0x0c04;

	disp = (u8 *)gDisplayTransition;
	{
		u16 *slot = (u16 *)(disp + 0x534);
		int	 value = 0x3f3f;
		*slot = value;
	}
	{
		u16 *slot = (u16 *)(disp + 0x536);
		int	 value = 31;
		*slot = value;
	}
	{
		u16 *slot = (u16 *)(disp + 0x52a);
		int	 value = 10;
		*slot = value;
	}
}

u8 *ToretoEda_GetEvents(void)
{
	return (u8 *)ToretoEda_SceneTable3;
}

static void Scene_RunTwoCallSequence(void)
{
	Engine_EventBegin();
	Engine_EventEnd();
}

static void State_RunEmptyHookFirst(void)
{
}

static void State_RunEmptyHook(void)
{
}

static void ToretoEda_ExchangeInputPacket(void)
{
	u16 *hdr = sTransferHeader;

	hdr[0] = gKeysHeld;
	hdr[1] = gKeysRepeat;
	SerialRuntime_PollAndTransfer(hdr, sTransferData);
}

s32 ToretoEda_InitializeBackdropTransition(void)
{
	u8 **base = (u8 **)&gEventWork;
	u8	*work;
	u8	*disp;
	s32	 off = 224;
	s32 *scene;

	off <<= 1;
	work = *base;
	scene = (s32 *)(work + off);
	off -= 192;
	*scene = off;

	DisplayTransition_InitializeBattleEffectState(9);

	REG_BLDCNT = 0x3f42;
	REG_BLDALPHA = 0x0c04;

	disp = base[4];
	{
		u16 *slot = (u16 *)(disp + 0x534);
		int	 value = 0x3f3f;
		*slot = value;
	}
	{
		u16 *slot = (u16 *)(disp + 0x536);
		int	 value = 31;
		*slot = value;
	}
	{
		u16 *slot = (u16 *)(disp + 0x52a);
		int	 value = 10;
		*slot = value;
	}

	ToretoEda_StartBg3Split();
	return 0;
}

static void Effect_UpdateBg3HofsByVcount(void)
{
	u16 *src;
	u32	 value;

	if (REG_VCOUNT >= sSplitLine)
	{
		src = &sHofsAbove;
	}
	else
	{
		src = &sHofsBelow;
	}
	value = *src;
	REG_BG3HOFS = value;
}

static void Effect_SetBg3HofsSplit(void)
{
	union SceneCell *work = (union SceneCell *)(((u8 *)gMapWork[0]) + 260);
	s32				 hofs;

	sSplitLine = 192 - work[1].h[1];
	sHofsAbove = hofs = work[0].h[1];
	sHofsBelow = hofs - (gFrameCount >> 2);
}

static void ToretoEda_StartBg3Split(void)
{
	Runtime_SetIrqHandler(1, 0, Effect_UpdateBg3HofsByVcount);
	Engine_TaskAddCallback((s32)Effect_SetBg3HofsSplit, 0xc80);
}

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
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ EVENT_TOUCH, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH) },
	{ EVENT_TOUCH, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TOUCH) },
	{ EVENT_TOUCH, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TOUCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 2), 0, CONDITION_ALWAYS, EVENT_SCRIPT(EV00_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

u16 ToretoEda_CellSteps[] = {
	MAP_CELL_STEP(0, 96, 2, 2, 2), MAP_CELL_STEP(2, 96, 2, 2, 2), MAP_CELL_STEPS_END,
};
