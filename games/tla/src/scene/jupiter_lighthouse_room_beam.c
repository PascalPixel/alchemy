/// @file
/// The great central room of Jupiter Lighthouse, up from the ground floor and
/// open to room 3 by its stairs. Cyclone lifts the leader up to its
/// ledges or drops him down (JupiterLighthouse_CycloneLiftsLeader,
/// JupiterLighthouse_CycloneDropsLeader); once the lighthouse is activated four
/// of its five grey blocks hover, and the fifth, Moved to the end of the row,
/// rises to bridge the gap (JupiterLighthouse_RaiseBridgeBlock). The energy
/// path block pushed down the ledge into the gap lights the path below it
/// (JupiterLighthouse_DropEnergyBlock), and the beam of light in the middle,
/// circled by motes, carries the leader up to room 4
/// (JupiterLighthouse_RideTheBeam).
#include "flags.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "field/event_commands.h"
#include "field/cell_target.h"
#include "scene/scene.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "field/field_sprite.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/iwram_call.h"
#include "macros.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "field/call.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/irq.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "system/heap_release.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "fixed_math.h"

/// The energy path block lies in its gap, lighting the path.
#define FLAG_ENERGY_BLOCK_IN_GAP 0xa02
/// The fifth grey block, Moved to the end of the row, has risen into the
/// bridge.
#define FLAG_BRIDGE_BLOCK_RAISED 0xa03

enum JupiterLighthouseRoomBeamActorId {
  /// The four grey blocks that hover once the lighthouse is activated.
  ACTOR_FIRST_HOVERING_BLOCK = ACTOR_FIRST_PLACED,
  ACTOR_LAST_HOVERING_BLOCK = ACTOR_FIRST_HOVERING_BLOCK + 3,
  ACTOR_BRIDGE_BLOCK,
  /// The four pillars round the beam.
  ACTOR_FIRST_BEAM_PILLAR,
  ACTOR_SECOND_BEAM_PILLAR,
  ACTOR_ENERGY_BLOCK = ACTOR_FIRST_BEAM_PILLAR + 4
};

void Actor_Release(struct Actor *actor);

void Actor_RaiseFromCell(s32 id);

void LightSprites_InitB(void);

void LightSprites_Add(s32 id, s32 value);

void BattleEffect_FillOverlayTiles(s32 a);

void JupiterLighthouse_LoadPalettes(s32 mode);

struct EffectSprite gJupiterLighthouse_GlowSprite BSS_DATA ALIGNED(16);
s32 gJupiterLighthouse_GlowPosition[3] BSS_DATA;
s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

s32 Position_FlatDistance(FIXED *a, FIXED *b);

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void Leader_CenterInCell(void);

/// The step triggers the Cyclone pads raise (FieldWork.raisedTrigger): up onto
/// the ledge from the west or east pad, or down from it.
enum {
  TRIGGER_CYCLONE_UP_WEST = 30,
  TRIGGER_CYCLONE_UP_EAST,
  TRIGGER_CYCLONE_DOWN_WEST,
  TRIGGER_CYCLONE_DOWN_EAST
};

void JupiterLighthouse_CycloneUpOnWestPad(void);
void JupiterLighthouse_CycloneUpOnEastPad(void);

void JupiterLighthouse_CycloneDownOnEastPad(void);

void JupiterLighthouse_RideTheBeam(void);

void JupiterLighthouse_StartHover(void);

void JupiterLighthouse_EndHover(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

u8 MapGrid_SetAttributeBAt(s32 x, s32 z, s32 layer, s32 value);
void EffectSprite_InitWithFadeOverlayBlended(const void *script, s32 a, s32 b,
                                             s32 c);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Sets the cell behavior of the two Cyclone pads' cells by the layer the
/// leader is on: 234 on the ledge (layer 1), 235 below it.
void JupiterLighthouse_SetLayerCellsByLeader(void) {
  struct Actor *actor;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  if (actor->layer == 1) {
    FieldMapGrid_SetCellBehavior(toFIXED(144), toFIXED(576), 2, 234);
    FieldMapGrid_SetCellBehavior(toFIXED(256), toFIXED(560), 2, 234);
  } else {
    FieldMapGrid_SetCellBehavior(toFIXED(144), toFIXED(576), 2, 235);
    FieldMapGrid_SetCellBehavior(toFIXED(256), toFIXED(560), 2, 235);
  }
}

/// A task: draws the glow over the Cyclone pad in use.
void JupiterLighthouse_DrawGlow(void) {
  BattleEffect_DrawSpriteAtWorldPosition(
      (s32)&gJupiterLighthouse_GlowSprite, gJupiterLighthouse_GlowPosition[0],
      gJupiterLighthouse_GlowPosition[1], gJupiterLighthouse_GlowPosition[2]);
}

/// Cyclone on a pad on a ledge (the trigger, 32 west or 33 east) whirls the
/// leader down to the floor below, the pad glowing.
void JupiterLighthouse_CycloneDropsLeader(s32 kind) {
  s32 owner = gMainRoutineWork.leaderActorId;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 i;
  s32 *target;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target = gJupiterLighthouse_GlowPosition;
  if (kind == TRIGGER_CYCLONE_DOWN_WEST) {
    target[0] = toFIXED(136);
    target[1] = toFIXED(144);
    target[2] = toFIXED(592);
  } else {
    target[0] = toFIXED(248);
    target[1] = toFIXED(144);
    target[2] = toFIXED(576);
  }
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_AddFrame(JupiterLighthouse_DrawGlow, priority);
  }
  EventCamera_FollowActor(owner, 1);
  for (i = 35; i >= 0; i--) {
    leader->y.fixed += -toFIXED(1);
    System_Wait(1);
  }
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  effect = Actor_Spawn(SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed,
                       leader->z.fixed);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y.fixed > toFIXED(32)) {
    leader->y.fixed += -toFIXED(1);
    leader->facing += DEGtoANG(45);
    effect->x.fixed = leader->x.fixed;
    effect->y.fixed = leader->y.fixed + -toFIXED(6);
    effect->z.fixed = leader->z.fixed;
    System_Wait(1);
  }
  Actor_SelectAnimation(effect, 6);
  i = 14;
  do {
    leader->facing += DEGtoANG(45);
    System_Wait(1);
    i--;
  } while (i >= 0);
  Actor_Release(effect);
  Actor_SetSpriteFlags(Actor_Get(owner), 1);
  leader->facing = FACING_SOUTH;
  leader->layer = 0;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  FieldTask_Remove(JupiterLighthouse_DrawGlow);
  JupiterLighthouse_SetLayerCellsByLeader();
  Event_End();
}

/// Cyclone on a pad below a ledge (the trigger, 30 west or 31 east) whirls the
/// leader up onto the ledge, 144 pixels up, the pad glowing.
void JupiterLighthouse_CycloneLiftsLeader(s32 kind) {
  s32 owner = gMainRoutineWork.leaderActorId;
  struct Actor *leader = Actor_Get(owner);
  struct Actor *effect;
  s32 *target;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  target = gJupiterLighthouse_GlowPosition;
  if (kind == TRIGGER_CYCLONE_UP_WEST) {
    target[0] = toFIXED(136);
    target[1] = toFIXED(144);
    target[2] = toFIXED(592);
  } else {
    target[0] = toFIXED(248);
    target[1] = toFIXED(144);
    target[2] = toFIXED(576);
  }
  {
    s32 priority = TASK_PASS_VBLANK;

    Task_AddFrame(JupiterLighthouse_DrawGlow, priority);
  }
  EventCamera_FollowActor(owner, 1);
  leader->scaleX = FIXED_ONE;
  leader->scaleY = FIXED_ONE;
  effect = Actor_Spawn(SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed,
                       leader->z.fixed);
  Actor_SelectAnimation(effect, 2);
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE;
  ((struct FieldSprite *)effect->sprite)->priority = 1;
  while (leader->y.fixed < toFIXED(108)) {
    leader->y.fixed += toFIXED(1);
    leader->facing += DEGtoANG(45);
    effect->x.fixed = leader->x.fixed;
    effect->y.fixed = leader->y.fixed + -toFIXED(6);
    effect->z.fixed = leader->z.fixed;
    System_Wait(1);
  }
  leader->scaleX = 0;
  effect->scaleX = 0;
  while (leader->y.fixed < toFIXED(144)) {
    leader->y.fixed += toFIXED(1);
    effect->y.fixed += toFIXED(1);
    System_Wait(1);
  }
  effect->scaleX = FIXED_ONE / 2;
  effect->scaleY = FIXED_ONE * 3 / 4;
  for (i = 31; i >= 0; i--) {
    effect->scaleX += FIXED_ONE / 32;
    effect->scaleY += FIXED_ONE / 128;
    System_Wait(1);
  }
  effect->scaleX = FIXED_ONE * 3 / 2;
  effect->scaleY = FIXED_ONE;
  Actor_SelectAnimation(effect, 6);
  leader->scaleX = FIXED_ONE * 3 / 8;
  leader->scaleY = FIXED_ONE * 3 / 8;
  i = 19;
  do {
    leader->facing += DEGtoANG(45);
    leader->scaleX += FIXED_ONE / 32;
    leader->scaleY = leader->scaleX;
    System_Wait(1);
    i--;
  } while (i >= 0);
  Actor_Release(effect);
  Actor_SetSpriteFlags(Actor_Get(owner), 1);
  leader->facing = FACING_SOUTH;
  leader->layer = 1;
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  leader->y.fixed = toFIXED(144);
  leader->groundHeight = toFIXED(144);
  FieldTask_Remove(JupiterLighthouse_DrawGlow);
  JupiterLighthouse_SetLayerCellsByLeader();
  Event_End();
}

/// Cyclone on the west pad below the ledge: the leader is whirled up
/// (JupiterLighthouse_CycloneLiftsLeader).
void JupiterLighthouse_CycloneUpOnWestPad(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;

  event->raisedTrigger = TRIGGER_CYCLONE_UP_WEST;
  effect->childOption = 1;
}

/// Cyclone on the east pad below the ledge: the leader is whirled up.
void JupiterLighthouse_CycloneUpOnEastPad(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;

  event->raisedTrigger = TRIGGER_CYCLONE_UP_EAST;
  effect->childOption = 1;
}

/// Cyclone on the west pad on the ledge: the leader is whirled down
/// (JupiterLighthouse_CycloneDropsLeader).
void JupiterLighthouse_CycloneDownOnWestPad(void) {
  gFieldWork->raisedTrigger = TRIGGER_CYCLONE_DOWN_WEST;
}

/// Cyclone on the east pad on the ledge: the leader is whirled down.
void JupiterLighthouse_CycloneDownOnEastPad(void) {
  gFieldWork->raisedTrigger = TRIGGER_CYCLONE_DOWN_EAST;
}

/// A leader who reaches the beam hovering, 32 pixels up or more, is drawn into
/// it and carried up, spinning, to room 4.
void JupiterLighthouse_RideTheBeam(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 i;
  s32 angle;
  FIXED y;

  y = actor->y.fixed;
  if (y >= toFIXED(32)) {
    Actor_SetSpeed(gMainRoutineWork.leaderActorId, 2);
    FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
    Actor_WalkTo(gMainRoutineWork.leaderActorId, 216, 680);
    Actor_SetAnimationSpeed(actor, 64);
    actor->motionFlags = 0;
    for (i = 109; i >= 0; i--) {
      actor->y.fixed += toFIXED(3);
      angle = actor->facing;
      angle += TRIG_TURN * 3 / 32;
      actor->facing = angle;
      System_Wait(2);
    }
    FieldEvent_SetExitRequest(12);
  }
}

/// Move takes hold of the energy path block: it stops falling and it and the
/// leader are drawn at priority 1.
void JupiterLighthouse_PushEnergyBlock(s32 unused, s32 id) {
  Actor_Get(id)->motionFlags = 0;
  FieldActor_SetSpritePriority(id, 1);
  FieldActor_SetSpritePriority(gMainRoutineWork.leaderActorId, 1);
}

/// The energy path block lands with a thump once pushed off its ledge; landed
/// in the gap at cell (20, 39), it stays there and the path beyond it lights
/// up in three steps.
void JupiterLighthouse_DropEnergyBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  struct MainRoutineWork *party;
  s32 i;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Actor_Get(id)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  party = &gMainRoutineWork;
  Actor_Get(party->leaderActorId)->priorityFlags |= ACTOR_PRIORITY_AUTOMATIC;
  if (FIELD_CELL(actor->x.fixed) == 20 && FIELD_CELL(actor->z.fixed) == 39)
    FieldActor_SetSpritePriority(id, 3);
  if (actor->groundHeight !=
      Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
    System_Wait(2);
    for (i = 0; i <= 29 &&
                (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
         i++)
      System_Wait(1);
    Event_PlaySound(SE_THUMP);
  }
  if (FIELD_CELL(actor->x.fixed) == 20 && FIELD_CELL(actor->z.fixed) == 39) {
    Event_SetFlag(FLAG_ENERGY_BLOCK_IN_GAP);
    Actor_SetSpriteFlags(Actor_Get(id), 0);
    FieldActor_SetSpritePriority(id, 3);
    Actor_SetPosition(id, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 1, 1, 84, 39);
    FieldEvent_Wait(30);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(92, 40, 1, 1, 84, 40);
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(92, 41, 1, 1, 84, 41);
    FieldEvent_Wait(20);
    Event_PlaySound(SE_MAP_TRANSITION);
    MapGrid_CopyCellsLayered(93, 41, 1, 1, 85, 41);
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(656), 2, 236);
  }
}

/// Move takes hold of the fifth grey block: it is drawn at priority 2.
void JupiterLighthouse_MoveBridgeBlock(s32 unused, s32 id) {
  Actor_SetSpritePriority(id, 2);
}

/// The fifth grey block, Moved to the end of the row at cell (18, 6) once the
/// lighthouse is activated, rises to hover with the others.
void JupiterLighthouse_RaiseBridgeBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 i;

  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) &&
      FIELD_CELL(actor->x.fixed) == 18 && FIELD_CELL(actor->z.fixed) == 6) {
    actor->motionFlags = 0;
    for (i = 15; i >= 0; i--) {
      actor->y.fixed += toFIXED(2);
      System_Wait(1);
    }
    Actor_RaiseFromCell(id);
    Event_SetFlag(FLAG_BRIDGE_BLOCK_RAISED);
  }
}

/// Hover starts: while the leader hovers, the cell at (21, 37) before the beam
/// takes cell behavior 255.
void JupiterLighthouse_StartHover(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->movementMode == LEADER_MOVEMENT_HOVER)
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(592), 0, 255);
}

/// Hover ends: the cell at (21, 37) takes cell behavior 4 again.
void JupiterLighthouse_EndHover(void) {
  MapGrid_SetCellBehavior(toFIXED(336), toFIXED(592), 0, 4);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// A mote's update: it circles the beam at its own speed, rising until it
/// reaches the top and starts again from the floor, in front of the beam on
/// the near half of the circle and behind it on the far half.
void JupiterLighthouse_CircleTheBeam(struct Actor *actor) {
  s16 angle = actor->timer;
  s16 height = actor->userValue;

  SET_ACTOR_POSITION(
      actor, Iwram_MulFX(Trig_Cos(angle), actor->speedLimit) + toFIXED(214),
      (height * toFIXED(3)) + toFIXED(8), toFIXED(680));
  // Read signed, as the phase is above; the field is declared unsigned.
  if ((s16)actor->userValue == 0)
    Actor_SelectAnimation(actor, 7);
  if (actor->y.fixed > toFIXED(340))
    actor->userValue = 0xffff;
  if (Math_Sin(angle) < 0)
    ((struct FieldSprite *)actor->sprite)->priority = 1;
  else
    ((struct FieldSprite *)actor->sprite)->priority = 2;
  actor->timer += actor->acceleration;
  actor->userValue++;
}

/// Places the beam's 22 motes, each circling it at a random speed and starting
/// five frames after the one before.
void JupiterLighthouse_SpawnBeamMotes(void) {
  s32 i;
  s32 phase;
  struct Actor *actor;

  for (i = 0, phase = 0; i != 22; i++, phase += 5) {
    actor = (FieldActor_Create(SPRITE_PARTICLE, 0, 0, 0));
    actor->motionFlags = 0;
    actor->priorityFlags = 0;
    actor->speedLimit = (Random16() << 2) + toFIXED(6);
    actor->acceleration = ((Random16() << 11) >> 16) + DEGtoANG(45) / 4;
    actor->update = JupiterLighthouse_CircleTheBeam;
    actor->timer = Random16();
    actor->userValue = phase;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RBG);
    ((struct FieldSprite *)actor->sprite)->priority = 1;
  }
}

/// Sets up the room: the beam and its motes, the energy path block (lit until
/// it lies in its gap) and the grey blocks, hovering once the lighthouse is
/// activated.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldActor_SetSpritePriority(ACTOR_FIRST_BEAM_PILLAR, 3);
  FieldActor_SetSpritePriority(ACTOR_SECOND_BEAM_PILLAR, 3);
  Io_Write16(BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
  BattleEffect_FillOverlayTiles(0);
  Call4((void (*)())EffectSprite_InitWithFadeOverlayBlended,
        (s32)&gJupiterLighthouse_GlowSprite, 0, 0, 0x80000000);
  JupiterLighthouse_SetLayerCellsByLeader();
  JupiterLighthouse_SpawnBeamMotes();
  if (Event_GetFlag(FLAG_ENERGY_BLOCK_IN_GAP)) {
    Actor_SetPosition(ACTOR_ENERGY_BLOCK, 0, 0);
    MapGrid_CopyCellAttributes(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(28, 39, 1, 1, 20, 39);
    MapGrid_CopyCellsLayered(92, 39, 2, 3, 84, 39);
    FieldMapGrid_SetCellBehavior(toFIXED(336), toFIXED(656), 2, 236);
  } else {
    LightSprites_InitB();
    LightSprites_Add(ACTOR_ENERGY_BLOCK, 0);
  }
  if (gMainRoutineWork.entrance == 3 || gMainRoutineWork.entrance == 6 ||
      gMainRoutineWork.entrance == 7)
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
    Actor_RaiseFromCell(ACTOR_FIRST_HOVERING_BLOCK);
    Actor_RaiseFromCell(ACTOR_FIRST_HOVERING_BLOCK + 1);
    Actor_RaiseFromCell(ACTOR_FIRST_HOVERING_BLOCK + 2);
    Actor_RaiseFromCell(ACTOR_LAST_HOVERING_BLOCK);
  }
  if (Event_GetFlag(FLAG_BRIDGE_BLOCK_RAISED)) {
    Actor_SetPosition(ACTOR_BRIDGE_BLOCK, 296, 104);
    Actor_RaiseFromCell(ACTOR_BRIDGE_BLOCK);
  }
  return 0;
}

/// Sets the scene's arrival flags and loads the lighthouse's palettes before
/// the map is built. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step_at.h"

#define PALETTE_CYCLE_TASK PaletteCycle_StepAt
#include "shared/jupiter_lighthouse_load_palettes.h"

void MapGrid_SetCellHeight();

/// Lifts an actor 32 pixels over its ground as a platform others may stand on,
/// and raises the map cell under it six height steps to match.
void Actor_RaiseFromCell(s32 id) {
  struct Actor *actor;
  s32 height;

  actor = Actor_Get(id);
  actor->motionFlags = ACTOR_FLOATS;
  height = actor->groundHeight;
  actor->bounce = 0;
  actor->y.fixed = height + toFIXED(32);
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, (height >> 19) + 6);
  MapGrid_SetAttributeBAt(actor->x.fixed, actor->z.fixed, 0, 128);
}

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

/// Lights an actor, drawn at picture `value` of the light sprites.
void LightSprites_Add(s32 id, s32 value) {
  struct Actor *actor = Actor_Get(id);

  if (actor) {
    actor->timer = value;
    {
      s16 *count;

      count = &gLightSpriteCount;
      gLightSpriteActors[*count] = actor;
      (*count)++;
    }
  }
}

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(s32 priority) { gLightSpritePriority = priority; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_JUPITER_LIGHTHOUSE_ROOM_2_SET_STATE
extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *actor, u32 value);

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

/// The cells CrackedTile_Step watches, and the cell it copies over the one the
/// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find_lower.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTile_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

void JupiterLighthouse_CycloneDownOnWestPad(void);

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseRoomBeam),         SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),   SCENE_EXIT(2, JupiterLighthouseRoomBeam, 3),   
      SCENE_EXIT(3, JupiterLighthouseRoomBeam, 2),    SCENE_EXIT(4, JupiterLighthouseRoomBeam, 5),   SCENE_EXIT(5, JupiterLighthouseRoomBeam, 4),   
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A),   SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B),  SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST),   
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A),   SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B), SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH), 
      SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM), SCENE_EXITS_END,                                
  };

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 104, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 600, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 600, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 728, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 728, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RideTheBeam)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneUpOnWestPad)},
    {TRIGGER_RAISED, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneLiftsLeader)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneUpOnEastPad)},
    {TRIGGER_RAISED, 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneLiftsLeader)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDownOnWestPad)},
    {TRIGGER_RAISED, 32, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDropsLeader)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDownOnEastPad)},
    {TRIGGER_RAISED, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneDropsLeader)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushEnergyBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_DropEnergyBlock)},
    {TRIGGER_PUSH, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushEnergyBlock)},
    {TRIGGER_PUSHED, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_DropEnergyBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_MoveBridgeBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BRIDGE_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseBridgeBlock)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHover)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_EndHover)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
