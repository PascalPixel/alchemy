#include "battle_effect_runtime.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/korima_hiroba/hiroba.h"
#include "unit/field/common/object/staged_actor.h"
#include "unit/field/common/korima/palette.h"
#include "unit/battle/effect/slots.h"
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

typedef struct
{
	s32	 w0, w1, w2, w3;
	Pair tail;
} Query;

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
	u8 pal;
	u8 padding_1d[10];
	u8 state;
} OrbitingSceneObjectSprite;

typedef struct OrbitingSceneObject
{
	u8						   padding_00[8];
	s32						   x;
	s32						   y;
	u8						   padding_10[19];
	u8						   flags_23;
	u8						   padding_24[12];
	s32						   orbit_angle;
	u8						   padding_34[4];
	s32						   orbit_center_x;
	s32						   orbit_center_y;
	u8						   padding_40[16];
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

void SceneActor_RunPlacementQuery(void)
{
	struct StagedActorProbe result;
	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&result))
		SceneActor_MoveAndRedraw(result);
	Engine_EventEnd();
}

void KorimaHiroba_AwardHiddenNut(void)
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
		SceneEffect_InitOrbitingParticle(11);
	}
	FieldScene_RedrawActorFootprint(8);
	FieldScene_RedrawActorFootprint(9);
	FieldScene_RedrawActorFootprint(10);
	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		SceneEffect_AdjustPaletteWindow(11);
	}
	return 0;
}

void SceneEffect_AdjustPaletteWindow(s32 adj)
{
	volatile u16 *pal = BG_PLTT;
	u32			  phase;
	u32			  next;
	KorimaPalette_SaveFirst();
	phase = 0;
	do
	{
		u32 idx = phase >> 16;
		u32 win;

		if ((phase + 0xffef0000) > 0x60000)
		{
			win = (idx + 0xff3f) << 16;
			if (win > 0x70000)
				pal[idx] = SceneEffect_AdjustColorChannels(pal[idx], adj);
		}
		next = phase + 0x10000;
		phase = next;
	} while (next <= 0x00df0000);
	KorimaPalette_Capture();
	KorimaPalette_SaveSecond();
	Engine_ColorBufferApplyTarget(0x10000, 0);
}

u16 SceneEffect_AdjustColorChannels(u16 color, s32 adj)
{
	s16 green = ((color >> 5) & 31);
	s16 red = (color & 31);
	s16 blue = ((color >> 10) & 31);
	u32 packed;

	red = (red + Math_Divide(red, (adj << 2)));
	green = (green - Math_Divide(green, adj));
	blue = (blue - Math_Divide(blue, adj));

	if (red > 31)
		red = 31;

	packed = (s32)red;
	packed |= ((s32)blue << 10) | ((s32)green << 5);
	return packed;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korima/palette.c"

s32 SceneEffect_UpdateOrbitingParticle(struct Particle_02000c4c *record)
{
	u16 *sprite = record->sprite;
	s32	 lift;
	s32	 tilt;
	s32	 jitter;

	lift = Engine_MathSin(record->angle) * 2;
	if (lift > 0)
		lift = -lift;

	record->x = record->base_x + Engine_MathCos(record->angle) * 2;
	record->y = record->base_y + lift;

	tilt = Engine_MathCos(record->angle + 0x8000);
	if (tilt < 0)
		tilt += 7;
	sprite[15] = (tilt >> 3);

	jitter = (((u32)Engine_RandomNext() << 9) >> 16);
	jitter += (((u32)Engine_RandomNext() << 9) >> 16);
	record->angle += jitter + 1024;

	return 0;
}

void SceneEffect_InitOrbitingParticle(s32 id)
{
	OrbitingSceneObject		  *actor;
	OrbitingSceneObjectSprite *sprite;
	u8						  *transfer;
	s32						   zero;

	actor = (void *)Object_GetById(id);
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
	Engine_VramLoad(sprite->pal, 128, transfer);
	Engine_HeapRelease(17);

	actor->orbit_center_x = actor->x;
	actor->orbit_angle = zero;
	actor->orbit_center_y = actor->y;
	actor->active = 1;
	actor->callback = (u32)SceneEffect_UpdateOrbitingParticle;
	actor->state = zero;
}

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
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_RunPlacementQuery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, 0xfd3, EVENT_SCRIPT(KorimaHiroba_AwardHiddenNut) },
	{ SCENE_EVENTS_END },
};

// .bss 内の並びは最初に宣言した順 (field.h の extern)
u8 KorimaPalette_First[1792] __attribute__((section(".bss")));
u8 KorimaPalette_Second[896] __attribute__((section(".bss")));
