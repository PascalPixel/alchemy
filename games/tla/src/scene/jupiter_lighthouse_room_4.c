/// @file
/// The room of grey blocks and locked doors in Jupiter Lighthouse, between
/// rooms 3 and 5. The Red Key and the Blue Key open its two doors
/// (JupiterLighthouse_UseRedKey, JupiterLighthouse_UseBlueKey), Reveal shows a
/// hidden passage and its block (JupiterLighthouse_RevealThePassage), and once
/// the lighthouse is activated its blocks hover, Move setting the fifth on its
/// cell to bridge the gap (JupiterLighthouse_FinishMovingBlock). Two Cyclone
/// teleporters lead on (JupiterLighthouse_LeaveByFirstCyclone,
/// JupiterLighthouse_LeaveBySecondCyclone), and motes of light circle the room
/// (JupiterLighthouse_SpawnCirclingMotes).
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_4/, in two parts the shared field effect objects
/// link between.

#include "shared/whirlwind.h"
#include "shared/tables.h"
#include "flags.h"
#include "shared/jupiter_lighthouse.h"
#include "shared/actors.h"
#include "shared/effects.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/ram_buffer.h"
#include "field_event.h"
#include "field/event_inline.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "field_sprite.h"
#include "macros.h"
#include "system/main_routine_work.h"
#include "event_work.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "call.h"
#include "system/field_effect.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "item_icon.h"
#include "field/cell_target.h"
#include "objcmd.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "shared/cracked_tile.h"
#include "fixed_math.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"
#include "text/msg_ids.h"

// Set as each locked door opens to its key.
#define FLAG_RED_DOOR_OPENED 0xa07
#define FLAG_BLUE_DOOR_OPENED 0xa08
// Set once Move has set the fifth block on its cell, where it rises to bridge
// the gap.
#define FLAG_BRIDGE_BLOCK_RAISED 0xa06

/// The grey blocks, numbering the rows of gActorPlacements.
/// ★ Named from what the player sees: grey stone blocks (balance_platform.png)
enum JupiterLighthouseBlockId {
  /// The first of the six blocks that rise once the lighthouse is activated.
  ACTOR_FIRST_BLOCK = ACTOR_FIRST_PLACED,
  /// The block Reveal shows on the hidden passage.
  ACTOR_HIDDEN_BLOCK = ACTOR_FIRST_PLACED + 6,
  /// The block Move sets on its cell to bridge the gap.
  ACTOR_BRIDGE_BLOCK,
  /// The first of three more that rise once the lighthouse is activated.
  ACTOR_LAST_BLOCKS,
  /// An actor with no picture, raised with the blocks and given gravity.
  // XXX: what it stands for; a render of the activated room would settle it.
  ACTOR_UNSEEN = ACTOR_LAST_BLOCKS + 3
};

void Actor_RaiseFromCell(s32 id);
extern s16 gLightSpriteTile;
extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOam[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
void LightSprites_InitA(void);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void MapGrid_SetCellHeight();
extern s32 gWhirlwindRiding;

void Actor_ArriveInRing(s32 id);
void MapGrid_SetAttributeBAt();

extern u16 gPaletteCycleColors[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

static inline void SetCollisionCode(s32 x, s32 z, s32 layer, s32 value) {
  /* FAKEMATCH(wrapper): the call forwarded through this inline, as in ☀️'s
   * scene layer, so each call keeps its own constants; as a direct call, 12
   * of 47 instructions of JupiterLighthouse_RevealThePassage differ in tla-en.
   */
  ((u8)MapGrid_SetCellBehavior(x, z, layer, value));
}

void JupiterLighthouse_RevealThePassage(void) {
  MapGrid_CopyCellsLayered(17, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(17, 10, 1, 1, 19, 10);
  SetCollisionCode(0x1300000, 0xa00000, 0, 254);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, PIXELS(504), PIXELS(168));
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_HIDDEN_BLOCK);
    ((u8)FieldMapGrid_SetCellBehavior(0x1f00000, 0xa00000, 0, 254));
  }
}

void JupiterLighthouse_HideThePassage(void) {
  MapGrid_CopyCellsLayered(16, 10, 1, 4, 19, 10);
  MapGrid_CopyCellAttributes(16, 10, 1, 1, 19, 10);
  Actor_SetPosition(ACTOR_HIDDEN_BLOCK, 0, 0);
  MapGrid_CopyCellAttributes(30, 10, 1, 1, 31, 10);
}

void JupiterLighthouse_StartMovingBlock(s32 unused, s32 id) {
  Actor_SetSpritePriority(id, 2);
}

void JupiterLighthouse_FinishMovingBlock(s32 unused, s32 id) {
  struct Actor *object;
  s32 i;

  object = Actor_Get(id);
  if (object->x >> 20 == 10 && object->z >> 20 == 32) {
    object->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      object->y += 0x20000;
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_BRIDGE_BLOCK_RAISED);
  }
}

void JupiterLighthouse_LeaveByFirstCyclone(void) {
  Import_EventSetExitRequest(12);
}

void JupiterLighthouse_LeaveBySecondCyclone(void) {
  Import_EventSetExitRequest(13);
}

void JupiterLighthouse_OpenRedDoor(void) {
  MapGrid_CopyCellsLayered(73, 18, 1, 4, 74, 18);
  MapGrid_CopyCellAttributes(10, 9, 1, 1, 10, 20);
}

void JupiterLighthouse_OpenBlueDoor(void) {
  MapGrid_CopyCellsLayered(103, 18, 1, 4, 104, 18);
  MapGrid_CopyCellAttributes(40, 9, 1, 1, 40, 20);
}

void JupiterLighthouse_UseRedKey(s32 item) {
  Event_SetFlag(FLAG_RED_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenRedDoor();
  PartyInventory_Remove(item);
}

void JupiterLighthouse_UseBlueKey(s32 item) {
  Event_SetFlag(FLAG_BLUE_DOOR_OPENED);
  Event_PlaySound(SE_HEAVY_IMPACT);
  JupiterLighthouse_OpenBlueDoor();
  PartyInventory_Remove(item);
}

void JupiterLighthouse_TryLockedDoor(void) {
  ((struct EventWorkLock *)Ram_HeapSlots->eventWork)->locked = 1;
  Message_ShowCentered(MsgFieldDoorLocked, 1);
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

/// A particle's step, Jupiter Lighthouse room 2's Func_02000720 with other
/// numbers: it circles at its angle (action) around x 232 and rises with its
/// phase (unk66) from 120 pixels down, taking mode 7 at phase 1 and mode 2 at
/// phase 38 and starting over once above the ground; drawn in front while the
/// sine is negative and behind otherwise.
void JupiterLighthouse_CircleTheMote(struct Actor *object) {
  s16 angle = object->timer;
  s16 height = object->unk66;

  SET_POSITION(object,
               Iwram_MulQ16(Trig_Cos(angle), object->speedLimit) + (232 << 16),
               (height * 3 << 16) - (120 << 16), 130 << 18);
  if ((s16)object->unk66 == 1)
    Actor_SelectAnimation(object, 7);
  if ((s16)object->unk66 == 38)
    Actor_SelectAnimation(object, 2);
  if (object->y > 0)
    object->unk66 = 0xffff;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)object->sprite)->priority = 1;
  else
    ((struct FieldSprite *)object->sprite)->priority = 3;
  object->timer += object->acceleration;
  object->unk66++;
}

void JupiterLighthouse_SpawnCirclingMotes(void) {
  struct Actor *object;
  s32 i;
  s32 phase;

  i = 0;
  for (phase = 0; i != 7; i++) {
    object = (FieldActor_Create(672, 0, 0, 0));
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->speedLimit = (Random16() << 2) + 0x60000;
    object->acceleration = ((Random16() << 11) >> 16) + 0x800;
    object->update = (void (*)(void))JupiterLighthouse_CircleTheMote;
    object->timer = Random16();
    object->unk66 = phase;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 2);
    ((struct FieldSprite *)object->sprite)->priority = 1;
    phase += 5;
  }
}

s32 Scene_Initialize(void) {
  struct Actor *leader;
  s32 flag;
  s32 i;

  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  JupiterLighthouse_HideThePassage();
  if (Event_GetFlag(FLAG_RED_DOOR_OPENED))
    JupiterLighthouse_OpenRedDoor();
  if (Event_GetFlag(FLAG_BLUE_DOOR_OPENED))
    JupiterLighthouse_OpenBlueDoor();
  JupiterLighthouse_SpawnCirclingMotes();
  flag = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (flag == 0 && gMainRoutineWork.entrance == 11) {
    leader = Actor_Get(gMainRoutineWork.currentOwner);
    leader->priorityFlags &= ~64;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 1);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
    Actor_SetAnimationSpeed(leader, 64);
    leader->motionFlags = flag;
    leader->y = leader->terrainHeight = -0x780000;
    Event_StartScreenTransition();
    FieldEvent_Wait(10);
    for (i = 0; i <= 89 && leader->y < 0; i++) {
      leader->terrainHeight = leader->y = leader->y + 0x30000;
      leader->facing += 0x1800;
      System_Wait(2);
    }
    leader->terrainHeight = leader->y = 0;
    leader->facing = 0x4000;
    leader->motionFlags = 3;
    leader->velocityY = 0x30000;
    leader->gravity = 0x8000;
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
    FieldEvent_Wait(40);
    leader->gravity = 0x10000;
    leader->priorityFlags |= 64;
    Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    Event_End();
  }
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance == 12)
    Actor_ArriveInRing(gMainRoutineWork.currentOwner);
  return 0;
}

void JupiterLighthouse_LoadPalettes(s32 value);

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 3);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 4);
    Actor_RaiseFromCell(ACTOR_FIRST_BLOCK + 5);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS + 1);
    Actor_RaiseFromCell(ACTOR_LAST_BLOCKS + 2);
    Actor_RaiseFromCell(ACTOR_UNSEEN);
    Actor_Get(ACTOR_UNSEEN)->gravity = 0x30000;
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_UNSEEN), 0);
  if (Event_GetFlag(FLAG_BRIDGE_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_BRIDGE_BLOCK, PIXELS(168), PIXELS(520));
    Actor_RaiseFromCell(ACTOR_BRIDGE_BLOCK);
  }
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

DEFINE_PALETTE_CYCLE_STEP(PaletteCycle_Step, gPaletteCycleFrame,
                          gPaletteCyclePausesInEvents, gPaletteCycleColors)

DEFINE_JUPITER_LIGHTHOUSE_LOAD_PALETTES(JupiterLighthouse_LoadPalettes,
                                        gPaletteCyclePausesInEvents,
                                        gPaletteCycleColors,
                                        gPaletteCycleUnlitPalette,
                                        PaletteCycle_Step,
                                        gPaletteCycleUnlitColors)

DEFINE_ACTOR_RAISE_FROM_CELL(Actor_RaiseFromCell, MapGrid_SetCellHeight,
                             MapGrid_SetAttributeBAt)

DEFINE_WHIRLWIND_CARRY_LEADER(Whirlwind_CarryLeader)

DEFINE_POSITION_FLAT_DISTANCE(Position_FlatDistance)

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE(Whirlwind_Update, MapGrid_GetCellBehavior,
                        Position_FlatDistance, gWhirlwindEndScript,
                        gWhirlwindRiding)

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Actor_SelectAnimation, Whirlwind_Update,
                       FieldEffect_Spawn)

/// Unused: every eighth frame, sends a spark up from a random spot about the
/// actor.
UNUSED s32 Actor_EmitRisingSparks(struct Actor *object) {
  struct EffectOptions options;
  s32 x;
  s32 y;
  s32 velocityY;

  if ((gFrameCount & 7) == 0) {
    options.priority = 3;
    options.startScaleX = 0xb333;
    options.startScaleY = 0xb333;
    options.palette = 14;
    x = object->x + ((((Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - ((Random16() * 20) >> 16)) << 16);
    velocityY = (s32)(((((Random16() * 5) >> 16) << 16) + 0x50000)) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, velocityY, 0,
                      EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_START_SCALE,
                      &options);
  }
  return 0;
}

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

/// Unused here: places the aerie's two angels and the lid between them, each
/// angel the archers have energized raised with its half of the lid, and all
/// three lifted clear once both are. The aerie's beacon floor keeps the same
/// routine.
UNUSED void JupiterLighthouse_PlaceAngelsAndLid(void) {
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  struct FieldSprite *sprite = rider->sprite;

  Actor_SetPosition(24, 0x2000000, 0xf80000);
  Actor_SetPosition(23, 0x2000000, 0xf80000);
  Actor_SetPosition(9, 0x1d80000, 0x1000000);
  Actor_SetPosition(10, 0x2280000, 0x1000000);
  Actor_Get(24)->scaleX = 0;
  rider->motionFlags = 0;
  rider->y = rider->terrainHeight;
  left->motionFlags = 0;
  left->y = left->terrainHeight;
  right->motionFlags = 0;
  right->y = right->terrainHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    left->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += -0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_Get(9), 4);
    Actor_SetSpriteColorMode(Actor_Get(11), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    right->y += 0x66640;
    rider->y += 0x1eb80;
    rider->x += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_Get(10), 4);
    Actor_SetSpriteColorMode(Actor_Get(12), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    left->y += 0x199900;
    right->y += 0x199900;
    rider->y += 0x1b8480;
    left->motionFlags = 4;
    right->motionFlags = 4;
    rider->motionFlags = 4;
  }
}

extern struct Actor *gLightSpriteActors[];

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_LIGHT_SPRITES_DRAW(LightSprites_Draw, gLightSpriteTile, gLightSpriteOam,
                          gLightSpriteCount, gLightSpriteActors,
                          gLightSpritesLarge, gLightSpritePriority,
                          BattleEffect_GetObjectPriority,
                          Oam_AddSpriteAtDepthClamped)

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

DEFINE_LIGHT_SPRITES_ADD(LightSprites_Add, gLightSpriteCount,
                         gLightSpriteActors)

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

// clang-format off
DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_ShortScript, 22)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_MediumScript, 44)

DEFINE_FIELD_EFFECT_DURATION_SCRIPT(gFieldEffect_LongScript, 126)

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(257),
    SCENE_EXIT(1, 258, 1),
    SCENE_EXIT(2, 256, 2),
    SCENE_EXIT(3, 257, 4),
    SCENE_EXIT(4, 257, 3),
    SCENE_EXIT(5, 257, 6),
    SCENE_EXIT(6, 257, 5),
    SCENE_EXIT(7, 257, 8),
    SCENE_EXIT(8, 257, 7),
    SCENE_EXIT(9, 263, 9),
    SCENE_EXIT(10, 263, 10),
    SCENE_EXIT(11, 257, 11),
    SCENE_EXIT(12, 256, 12),
    SCENE_EXIT(13, 256, 13),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    {316, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(136), FACING_SOUTHEAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(136), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(168), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(168), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(488), 0, PIXELS(200), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(520), 0, PIXELS(200), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, -0x80000, 0, -0x80000, FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(520), FACING_EAST, 2, 0},
    {316, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(520), FACING_SOUTHEAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(520), FACING_EAST, 2, 1},
    {316, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(520), FACING_EAST, 2, 1},
    {489, CONDITION_ALWAYS, 1, PIXELS(232), 0, PIXELS(520), FACING_EAST, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {33, 1, CONDITION_ALWAYS, 1},
    {49, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXAMINE, 20, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {TRIGGER_EXAMINE, 21, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_TryLockedDoor)},
    {USE_ITEM(243), 20, FLAG_RED_DOOR_OPENED, EVENT(JupiterLighthouse_UseRedKey)}, // the Red Key
    {USE_ITEM(244), 21, FLAG_BLUE_DOOR_OPENED, EVENT(JupiterLighthouse_UseBlueKey)}, // the Blue Key
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RevealThePassage)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_HideThePassage)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingBlock)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByFirstCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 41, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveBySecondCyclone)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "../field/field_effect/move.c"

#undef FLAG_RED_DOOR_OPENED
#undef FLAG_BLUE_DOOR_OPENED
#undef FLAG_BRIDGE_BLOCK_RAISED
// @file
// The room of grey blocks and locked doors in Jupiter Lighthouse: its copy of
// the effect spawner, and the cracked tiles, found, cracked and fallen
// through.

struct CellTarget;
void Leader_CenterInCell(void);
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SelectAnimation(struct Actor *object, s32 mode);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      ((struct FieldEffect *)Actor_GetById(gMainRoutineWork.currentOwner));
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect = (struct FieldEffect *)Import_ActorCreate(285, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - Q16_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - Q16_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

// The cells CrackedTiles_Step watches, and the cell it copies over the one the
// leader steps on.
#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

DEFINE_CRACKED_TILES_FIND(CrackedTiles_Find, gCrackedTiles, CrackedTiles_Update)

void Leader_CenterInCell(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x;
  s32 z = object->z;

  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x40000, 0x20000);

  x >>= 20;
  z >>= 20;
  Actor_MoveToAndWaitById(gMainRoutineWork.currentOwner, (x << 4) + 8,
                          (z << 4) + 8);
  System_Wait(1);
  FieldActor_SetPositionAndResetMotion(object, (x << 20) + PIXELS(8), object->y,
                                       (z << 20) + PIXELS(8));
  FieldEvent_Wait(4);
  ((struct Actor *)FieldCamera_GetActor())->y += 0xfff80000;
}

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
DEFINE_CRACKED_TILE_CRACK(CrackedTile_Crack, Leader_CenterInCell)

/* VIEW(s1888): tag struct CellTarget as struct CellTarget__s1888; cell_target.h
 * has another definition; tried with the definition the rest of the object has,
 * CrackedTile_Fall moves: 2 of 131 instructions in tla-ja. */
struct CellTarget__s1888 {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

DEFINE_CRACKED_TILE_FALL(CrackedTile_Fall, CellTarget__s1888,
                         Leader_CenterInCell)

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// The room of grey blocks and locked doors in Jupiter Lighthouse: the cracked
// tiles' steps and the fall to the floor below.

struct CellTarget;
struct CellTargetEntry;
extern struct CellTarget gCrackedTiles;
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
void JupiterLighthouse_HideThePassage(void);
void JupiterLighthouse_LeaveByFirstCyclone(void);
void JupiterLighthouse_LeaveBySecondCyclone(void);
void JupiterLighthouse_TryLockedDoor(void);
extern u32 gFieldEffect_LongScript[];

DEFINE_CRACKED_TILE_DROP_IN(CrackedTile_DropIn, FieldEffect_Spawn)

void GameFlag_SetByte(s32 flag, s32 value);

s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;
  struct CellTarget *puzzle = &gCrackedTiles;
  struct CellTargetEntry *tile = puzzle->entries;
  s32 i;

  for (i = 0; i < puzzle->count; i++, tile++) {
    if (tile->x == x && tile->z == z) {
      if (!Event_GetFlag(tile->flag)) {
        CrackedTile_Crack(puzzle, tile);
        Event_SetFlag(tile->flag);
        puzzle->current = i;
        break;
      }
      if (i == puzzle->current)
        break;
      puzzle->current = i;
      CrackedTile_Fall(puzzle, tile);
      GameFlag_SetByte(puzzle->flagByte, x);
      GameFlag_SetByte(puzzle->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void CrackedTile_PlaceLanding(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  u8 *event = Ram_HeapSlots->eventWork;
  struct CellTarget *work = &gCrackedTiles;
  s32 x = ((s32)FieldGameFlag_GetByte(work->flagByte));
  s32 z = ((s32)FieldGameFlag_GetByte(work->flagByte + 8));

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x = ((x + dx) << 20) + 0x80000;
    object->z = ((z + dz) << 20) + 0x80000;
    shadow = *(struct Actor **)(event + 460);
    shadow->x = object->x;
    shadow->z = object->z;
    Map_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = value;
  }
}
