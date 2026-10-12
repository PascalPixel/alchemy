#include "battle_effect_runtime.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/korima_hiroba/hiroba.h"
#include "unit/field/common/korima/palette.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "staged_actor.h"
#include "io_reg.h"
#include "ability_ids.h"
#include "field_scene.h"

// 前方宣言
static void InitializeStagedActorSceneOrbitingEffect(s32 id);

typedef struct OrbitingSceneObjectSprite
{
	u8 padding_00[5];
	u8 flags_05_low : 5;
	u8 flags_05_bit_5 : 1;
	u8 flags_05_high : 2;
	u8 padding_06[3];
	u8 flags_09_low : 2;
	u8 flags_09_mode : 2;
	u8 flags_09_high : 4;
	u8 padding_0a[18];
	u8 palette;
	u8 padding_1d[10];
	u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject
{
	u8						   padding_00[8];
	s32						   x;
	s32						   y;
	s32						   z;
	u8						   padding_14[15];
	u8						   flags_23;
	u8						   padding_24[12];
	s32						   orbit_angle;
	u8						   padding_34[4];
	s32						   orbit_center_x;
	s32						   orbit_center_y;
	s32						   orbit_center_z;
	u8						   padding_44[12];
	OrbitingSceneObjectSprite *sprite;
	u8						   padding_54;
	u8						   mode;
	u8						   state;
	u8						   padding_57[5];
	u8						   active;
	u8						   padding_5d[4];
	u8						   visible;
	u8						   padding_62[10];
	u32						   callback;
} OrbitingSceneObject;

#ifdef MODERN
// called before its definition below, which modern GCC refuses for an
// implicit int declaration of a u16 function
#endif

enum
{
	ITEM_NUT = 181
};

typedef struct
{
	s32 lo, hi;
} Pair;

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"

u8 *SceneData_GetScriptTable(void)
{
	return KorimaHiroba_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return KorimaHiroba_Messages;
}

u8 *SceneData_GetActorTable(void)
{
	return KorimaHiroba_Actors;
}

static void EV10_SCRIPT(void)
{
	struct StagedActorProbe result;
	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&result))
		SceneActor_MoveAndRedraw(result);
	Engine_EventEnd();
}

static void EV11_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorSetPosition(11, 0, 0);
	Engine_GameFlagSet(0xfd3);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

u8 *SceneData_GetExtraTable(void)
{
	return KorimaHiroba_Extras;
}

s32 FieldScene_SetupEntryActors8To11(void)
{

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (Engine_GameFlagIsSet(0xfd3) == 0)
	{
		InitializeStagedActorSceneOrbitingEffect(11);
	}
	FieldScene_RedrawActorFootprint(8);
	FieldScene_RedrawActorFootprint(9);
	FieldScene_RedrawActorFootprint(10);
	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		SceneEffect_AdjustPaletteColors(11);
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korima/palette.c"

static s32 SceneEffect_UpdateOrbitPosition(s32 *p)
{
	s16 *q = (s16 *)p[20];
	s32	 a, b;
	s32	 d = Engine_MathSin(p[12]) * 2;
	if (d > 0)
		d = -d;
	p[2] = p[14] + Engine_MathCos(p[12]) * 2;
	p[3] = p[15] + d;
	q[15] = Engine_MathCos(p[12] + 0x8000) / 8;
	a = Engine_RandomNext();
	b = Engine_RandomNext();
	p[12] = p[12] + ((((u32)a << 9) >> 16) + (((u32)b << 9) >> 16)) + 0x400;
	return 0;
}

static void InitializeStagedActorSceneOrbitingEffect(s32 id)
{
	OrbitingSceneObject		  *actor;
	OrbitingSceneObjectSprite *sprite;
	u8						  *transfer;
	s32						   zero;

	actor = (OrbitingSceneObject *)Object_GetById(id);
	sprite = actor->sprite;
	sprite->flags_09_mode = 1;
	sprite->flags_05_bit_5 = 0;
	sprite->flags_09_high = 0;

	zero = 0;
	sprite->state = zero;
	Engine_ActorSetSpriteFlags(actor, zero);
	actor->active = zero;
	actor->mode = zero;

	if (Engine_GameFlagIsSet(0x109) == 0)
		actor->y += 0x200000;

	actor->flags_23 &= 0xfe;
	actor->visible = 1;

	transfer = Engine_HeapAllocate(17, 0x608);
	Engine_ItemLoadIcon(ITEM_NUT);
	transfer += 0x400;
	Engine_VramLoad(sprite->palette, 128, transfer);
	Engine_HeapRelease(17);

	actor->orbit_center_x = actor->x;
	actor->orbit_angle = zero;
	actor->orbit_center_y = actor->y;
	actor->active = 1;
	actor->callback = (u32)SceneEffect_UpdateOrbitPosition;
	actor->state = zero;
}

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 StagedActor_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
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

struct SceneEntrance KorimaHiroba_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KorimaHiroba_Messages[] = {
	SCENE_EXITS(41),
	SCENE_EXIT(1, 42, 1),
	SCENE_EXIT(2, 40, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement KorimaHiroba_Actors[] = {
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(360), 0, PIXELS(368), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(160), 0, PIXELS(392), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(136), 0, PIXELS(336), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN) || defined(TBS_EDITION_DE)
	{ 0x16, 0xfd0, 7,
		PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#else
	{ 0x16, 0xfd3, 7,
		PIXELS(280), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
	{ SCENE_TABLE_END },
};

struct SceneEvent KorimaHiroba_Extras[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, 0xfd3, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

// .bss 内の並びは最初に宣言した順 (field.h の extern)
u8 KorimaPalette_First[1792] __attribute__((section(".bss")));
u8 KorimaPalette_Second[896] __attribute__((section(".bss")));
