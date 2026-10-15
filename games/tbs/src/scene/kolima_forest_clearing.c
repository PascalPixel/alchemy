#include "battle_effect_runtime.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_clearing.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psyfx.h"
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

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_staged_actor.h"

u8 *SceneData_GetScriptTable(void)
{
	return KolimaForestClearing_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return KolimaForestClearing_Messages;
}

u8 *SceneData_GetActorTable(void)
{
	return KolimaForestClearing_Actors;
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
	return KolimaForestClearing_Extras;
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
#include "common/kolima_palette.h"
#include "common/object_orbit.h"

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

struct SceneEntrance KolimaForestClearing_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 480, 0, 376, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 40, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KolimaForestClearing_Messages[] = {
	SCENE_EXITS(41),
	SCENE_EXIT(1, 42, 1),
	SCENE_EXIT(2, 40, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement KolimaForestClearing_Actors[] = {
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

struct SceneEvent KolimaForestClearing_Extras[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 11, 0xfd3, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

// .bss 内の並びは最初に宣言した順 (field.h の extern)
u8 KolimaPalette_First[1792] __attribute__((section(".bss")));
u8 KolimaPalette_Second[896] __attribute__((section(".bss")));
