/// @file
/// A hall of Jupiter Lighthouse just below its aerie. Its stairs lead up to
/// the aerie (later to the beacon's floor) and down to rooms 5 and 8. Standing
/// on the button by its door holds the door open; the pillar pushed onto it
/// (JupiterLighthouse_CheckBlockOnCell) keeps it open, a shortcut through the
/// tower. Cracked tiles on the floor and on the upper ledge give way under a
/// second step (JupiterLighthouse_StepOnLowerTeleporter,
/// JupiterLighthouse_StepOnUpperTeleporter), dropping the party to the floor
/// below or to room 8.
///
/// The cracked tiles' code, the Teleporter routines, is in
/// jupiter_lighthouse_room_6/, in two parts the shared field effect objects
/// link between.

#include "shared/whirlwind.h"
#include "shared/tables.h"
#include "flags.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "module/field/field.h"

// Set while the leader stands on the button by the door, which holds it open.
#define FLAG_DOOR_BUTTON_HELD 0x200

/// The pillar the party pushes onto the button by the door, after the four
/// angel statues in gActorPlacements.
/// ★ Named from what the player sees: a grey stone pillar (spr_312.png)
enum { ACTOR_SHORTCUT_PILLAR = ACTOR_FIRST_PLACED + 4 };
#include "module/scene/scene.h"
#include "module/tla.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "map.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "objcmd.h"

void JupiterLighthouse_OpenDoor(s32 value);
void JupiterLighthouse_CheckBlockOnCell(void);
void Teleporter_PlaceArrival(s32 x, s32 z, s32 effect);
extern s16 gLightSpriteTile;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void JupiterLighthouse_StepOffButton(void);
void JupiterLighthouse_FinishMovingBlock(void);
extern s16 gLightSpriteCount;
extern s16 gLightSpritePriority;
extern u16 gPaletteCycleColors[];
void Func_02001be4();
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
extern s32 gWhirlwindRiding;
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

void Func_02001b34();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

extern s32 gPaletteCyclePausesInEvents;
s32 Teleporters_Step(s32 value);
void JupiterLighthouse_CloseDoor(void);
void JupiterLighthouse_LoadPalettes(s32 value);

void JupiterLighthouse_StepOnLowerTeleporter(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      Teleporters_Step(value))
    FieldEvent_SetExitRequest(13);
}

void JupiterLighthouse_StepOnUpperTeleporter(s32 value) {
  if (Actor_Get(gMainRoutineWork.currentOwner)->y == 0 &&
      Teleporters_Step(value))
    FieldEvent_SetExitRequest(14);
}

DEFINE_JUPITER_LIGHTHOUSE_OPEN_DOOR(JupiterLighthouse_OpenDoor, 84, 36, 2, 78,
                                    40, 84, 39, 20, 38, 14, 42)

DEFINE_JUPITER_LIGHTHOUSE_CLOSE_DOOR(JupiterLighthouse_CloseDoor, 84, 36, 2, 78,
                                     40, 84, 20, 44, 14, 42)

DEFINE_JUPITER_LIGHTHOUSE_CHECK_BLOCK_ON_CELL(
    JupiterLighthouse_CheckBlockOnCell, ACTOR_SHORTCUT_PILLAR,
    FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, 4, 37,
    JupiterLighthouse_OpenDoor)

DEFINE_BUTTON_STEP_ON(JupiterLighthouse_StepOnButton, FLAG_DOOR_BUTTON_HELD,
                      JupiterLighthouse_OpenDoor)

DEFINE_BUTTON_STEP_OFF(JupiterLighthouse_StepOffButton, FLAG_DOOR_BUTTON_HELD,
                       JupiterLighthouse_CloseDoor,
                       JupiterLighthouse_CheckBlockOnCell)

void JupiterLighthouse_FinishMovingBlock(void) {
  System_Wait(10);
  JupiterLighthouse_CheckBlockOnCell();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

#include "dma.h"
#include "io_reg.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

s32 __modsi3();
void Teleporters_Find(s32 a, s32 b, s32 c, s32 d, s32 e);

s32 Scene_Initialize(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 0x204;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_SHORTCUT_PILLAR, PIXELS(72), PIXELS(600));
    JupiterLighthouse_OpenDoor(0);
  }
  if (Event_GetFlag(FLAG_DOOR_BUTTON_HELD))
    JupiterLighthouse_OpenDoor(0);
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    Teleporters_Find(33, 0x280, 0x380, 20, 31);
  } else {
    Teleporters_Find(43, 0x280, 0x380, 21, 31);
    Teleporter_PlaceArrival(21, -3, 0x204);
  }
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  if (!Event_GetFlag(265))
    Event_ClearFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
  return 0;
}

#include "shared/scene_set_arrival_flags.h"

#include "shared/jupiter_lighthouse_aerie.h"
#include "ram_buffer.h"
#include "field_event.h"

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_Step,
                                        gPaletteCycleUnlitColors)

/// Unused: holds an actor 32 pixels above its ground, without gravity.
UNUSED void JupiterLighthouse_FloatAboveGround(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->terrainHeight;
  object->bounce = 0;
  object->y = height + 0x200000;
  object->priorityFlags |= 128;
  height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);
  Func_02001b34(object->x, object->z, 0, (height >> 19) + 6);
  Func_02001be4(object->x, object->z, 0, 128);
}

#include "frame.h"
#include "field_sprite.h"
#include "event_work.h"

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, Func_02001bdc, Position_FlatDistance,
                        gWhirlwindEndScript, gWhirlwindRiding)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Import_ActorSelectAnimation,
                       Whirlwind_Update, FieldEffect_Spawn)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, FieldEffect_Spawn)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(
    JupiterLighthouse_PlaceRaisedActors)

extern struct Actor *gLightSpriteActors[];

void Import_OamAddSpriteAtDepthClamped(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOams,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Import_OamAddSpriteAtDepthClamped)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_LIGHT_SPRITES_INIT_A(LightSprites_InitA, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_B(LightSprites_InitB, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_INIT_C(LightSprites_InitC, gLightSpriteActors,
                            gLightSpriteTile, LightSprites_Draw,
                            gLightSpriteCount, gLightSpritesLarge,
                            gLightSpritePriority)

DEFINE_LIGHT_SPRITES_ADD_COUNTED(LightSprites_AddCounted, gLightSpriteActors,
                                 gLightSpriteCount)

void LightSprites_SetPriority(u16 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

void JupiterLighthouse_StepOnButton(void);

// clang-format off
DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_ShortLifeScript, 22)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_MediumLifeScript, 44)

DEFINE_FIELD_EFFECT_LIFE_SCRIPT(gFieldEffect_LongLifeScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(259),
    SCENE_EXIT(1, 264, 1),
    SCENE_EXIT(2, 264, 2),
    SCENE_EXIT(3, 264, 3),
    SCENE_EXIT(4, 259, 5),
    SCENE_EXIT(5, 259, 4),
    SCENE_EXIT(6, 259, 7),
    SCENE_EXIT(7, 259, 6),
    SCENE_EXIT(8, 261, 8),
    SCENE_EXIT(9, 258, 9),
    SCENE_EXIT(10, 258, 10),
    SCENE_EXIT(11, 259, 12),
    SCENE_EXIT(12, 259, 11),
    SCENE_EXIT(13, 259, 13),
    SCENE_EXIT(14, 261, 14),
    SCENE_EXIT(21, 265, 1),
    SCENE_EXIT(22, 265, 2),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {341, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(456), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(456), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(632), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(632), FACING_SOUTHEAST, 2, 1},
    {312, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(600), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_SOUTHWEST, 2, 0},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(184), FACING_SOUTHEAST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(264), FACING_SOUTHWEST, 2, 1},
    {341, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(264), FACING_SOUTHEAST, 2, 1},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, IF_FLAG_SET(0xa23), 21},
    {TRIGGER_EXIT, 2, IF_FLAG_SET(0xa23), 22},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {33, 4, CONDITION_ALWAYS, 4},
    {49, 5, CONDITION_ALWAYS, 5},
    {33, 6, CONDITION_ALWAYS, 6},
    {49, 7, CONDITION_ALWAYS, 7},
    {49, 8, CONDITION_ALWAYS, 8},
    {49, 9, CONDITION_ALWAYS, 9},
    {49, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 20, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, 21, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerTeleporter)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerTeleporter)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerTeleporter)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperTeleporter)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperTeleporter)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperTeleporter)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SHORTCUT_PILLAR, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

s32 gFieldEffect_LifeScripts[] = {
    (s32)gFieldEffect_ShortLifeScript,
    (s32)gFieldEffect_MediumLifeScript,
    (s32)gFieldEffect_LongLifeScript,
};
