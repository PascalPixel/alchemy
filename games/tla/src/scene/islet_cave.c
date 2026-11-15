/// @file
/// The Islet Cave, in the rooms of this overlay: logs the party rides across
/// its water (IsletCave_RideLog), the Venus Djinni Meld and the Mercury Djinni
/// Serac at the end of their rides (IsletCave_FightMeld, IsletCave_FightSerac),
/// and the cycling colours of its water (IsletCave_StartSlowPaletteCycle).
/// Teleport carries the party in by its circles
/// (IsletCave_TeleportToFirstCircle), each corridor is darker than the last
/// (IsletCave_DimPalette), Tremor shakes Serac out
/// (IsletCave_TremorRevealsSerac), and at the end the Sentinel waits
/// (IsletCave_SentinelAwaits).

#include "flags.h"
#include "djinn_ids.h"
#include "field/field.h"
#include "graphics/palette_buffers.h"
#include "system/main_routine_work.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "system/task.h"
#include "field/actor.h"
#include "field/scene_tables.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "scene_ids.h"
#include "stddef.h"
#include "shared/tables.h"
#include "field/actor_script.h"

void IsletCave_TeleportIn(void);

s32 List_CountEntries(u16 *list);

void PaletteCycle_Fade(void);

void IsletCave_PlaceLogs(void);

void IsletCave_FightMeld(void);

void IsletCave_FightSerac(void);

void IsletCave_TeleportToFirstCircle(void);

void IsletCave_TeleportToSecondCircle(void);

void IsletCave_OpenDoorAndEnter(void);

void IsletCave_SettleFirstLog(void);

void IsletCave_SettleSecondLog(void);

void IsletCave_SettleThirdLog(void);

void IsletCave_SettleFourthLog(void);

void IsletCave_SettleFifthLog(void);

void IsletCave_SettleSixthLog(void);

void IsletCave_SettleSeventhLog(void);
void IsletCave_RideFirstLogEast(void);
void IsletCave_RideSecondLogEast(void);
void IsletCave_RideSecondLogWest(void);

void IsletCave_RideThirdLogEast(void);

void IsletCave_RideFourthLogEast(void);
void IsletCave_RideFourthLogWest(void);
void IsletCave_RideFifthLogEast(void);
void IsletCave_RideFifthLogWest(void);
void IsletCave_RideSixthLogEast(void);
void IsletCave_RideSixthLogWest(void);
void IsletCave_RideSeventhLogNorth(void);
void IsletCave_RideSeventhLogSouth(void);
void IsletCave_SentinelAwaits(void);

void IsletCave_TremorRevealsSerac(void);

extern u32 gIsletCave_SentinelScript[];

extern s32 gIsletCave_ShrunkenSpriteScript[];
extern s16 gIsletCave_DoorFrames[];

extern u16 *gPaletteSlots;
extern u16 *gPaletteCycleColors;
extern u16 gPaletteCycleBright;
extern u16 gPaletteCycleDelay;
extern u16 gPaletteCycleLength;
extern u16 gPaletteCycleIndex;
void Teleport_PlaySparks(void);
void FieldEffect_StartBlankOverlay(s32 mode);
void PaletteFade_PackTarget(void);

extern u16 *gPaletteColors;
extern u16 *gPaletteCycleSlots;

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Palette_SetColors(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->paletteFade;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = a;
  gPaletteCycleSlots = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  if (gMainRoutineWork.teleportState == 2)
    IsletCave_TeleportIn();
}

extern u16 gPaletteCycleTimer;

void IsletCave_StartSlowPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

void IsletCave_StopPaletteCycle(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Remove(PaletteCycle_Fade);
}

void IsletCave_StartFastPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

void PaletteCycle_Fade(void) {
  s32 step;
  u32 i;
  u16 *src;
  volatile u16 *dst;
  int red, green, blue;

  step = 0;
  if (gPaletteCycleTimer != 0) {
    if (--gPaletteCycleTimer == 0) {
      step = 1;
      gPaletteCycleTimer = gPaletteCycleDelay;
      if (gPaletteCycleDelay > 2)
        gPaletteCycleDelay--;
    }
  }
  if (step == 0)
    return;
  src = gPaletteCycleColors;
  for (i = 0; i < gPaletteCycleLength; i++) {
    dst = &BG_PLTT[gPaletteCycleSlots[gPaletteCycleIndex * gPaletteCycleLength +
                                      i]];
    red = *src++;
    green = *src++;
    blue = *src++;
    SET_RGB(*dst, red, green, blue);
  }
  dst = &BG_PLTT[gPaletteSlots[gPaletteCycleLength]];
  if (gPaletteCycleBright != 0)
    src = gPaletteColors + gPaletteCycleLength * 3;
  else
    src = gPaletteCycleColors + gPaletteCycleLength * 3;
  red = *src++;
  green = *src++;
  blue = *src++;
  SET_RGB(*dst, red, green, blue);
  gPaletteCycleBright ^= 1;
  if (++gPaletteCycleIndex == gPaletteCycleLength)
    gPaletteCycleIndex = 0;
}

/// Arriving by Teleport: the water's colours spin fast while the whirl plays,
/// then settle.
void IsletCave_TeleportIn(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 saved;

  saved = work->transition;
  PaletteFade_SetTarget(PALETTE_TINT_CUSTOM | RGB(8, 8, 8), 1);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, 2);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  IsletCave_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  IsletCave_StopPaletteCycle();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->teleportState = 0;
  }
  work->transition = saved;
}

#include "text/msg_ids.h"

/// Meld, the Djinni waiting in this room.
enum { ACTOR_MELD = 15 };

void IsletCave_FightMeld(void) {
  Field_RequestDjinnBattle(ACTOR_MELD, DJINNI_MELD);
  return;
}

/// Serac, the Djinni waiting in this room, and the winged statue Tremor shakes
/// him out of.
enum { ACTOR_WINGED_STATUE = 8, ACTOR_SERAC = 9 };

/// The Sentinel and the first of the ten fires around it.
enum { ACTOR_SENTINEL = 8, ACTOR_FIRST_FIRE = 9 };

/// Set once the Sentinel is beaten, as the party comes back to its room.
#define FLAG_SENTINEL_DEFEATED 0x9cf

void IsletCave_FightSerac(void) {
  Field_RequestDjinnBattle(ACTOR_SERAC, DJINNI_SERAC);
  return;
}

s32 IsletCave_ClearScriptedActorSpriteFlags(s32 actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

s32 IsletCave_BobOnWater(struct Actor *object) {
  u8 *phase = (u8 *)object + 0x62;
  s32 step = *phase & 63;
  s16 *animation = object->sprite;

  object->motionFlags = 3;
  if (step < 32) {
    if (step == 0)
      object->velocityY = toFIXED(1);
    animation[9] = Trig_Cos(step << 12) / 128;
  }
  (*phase)++;
  return -1;
}

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gIsletCave_Placements[];
extern struct ActorPlacement gIsletCave_Room1Placements[];
extern struct ActorPlacement gIsletCave_Room2Placements[];
extern struct Trigger gIsletCave_Triggers[];
extern struct Trigger gIsletCave_Room2Triggers[];
extern struct Trigger gIsletCave_Room3Triggers[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ISLET_CAVE_MELD)
    return gIsletCave_Room1Placements;
  if (scene == SCENE_ISLET_CAVE_SERAC)
    return gIsletCave_Room2Placements;
  return gIsletCave_Placements;
}

const struct Trigger *Scene_GetTriggers(void) {
  s16 scene = gMainRoutineWork.next;

  if (scene == SCENE_ISLET_CAVE_SERAC)
    return gIsletCave_Room2Triggers;
  if (scene == SCENE_ISLET_CAVE_SENTINEL)
    return gIsletCave_Room3Triggers;
  return gIsletCave_Triggers;
}

void IsletCave_FinishTeleport(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  s32 effect = work->transition;

  IsletCave_StartFastPaletteCycle();
  Teleport_PlayWhirl();
  wait(40);
  IsletCave_StopPaletteCycle();
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 1);
  PaletteFade_StartInterpolation(16);
  wait(16);

  gMainRoutineWork.teleportState = 0;
  Actor_ConfigureSpriteColor(gMainRoutineWork.leaderActorId, 0);
  work->transition = effect;
}

#define DEFINE_ISLET_CAVE_ARRIVE_AT(name, n1, n2)                              \
  void name(void) {                                                            \
    IsletCave_StartSlowPaletteCycle();                                         \
    Teleport_PlaySparks();                                                     \
                                                                               \
    gMainRoutineWork.teleportState = 1;                                        \
    Event_Wait(40);                                                            \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.leaderActorId,     \
                                           toFIXED(n1), toFIXED(n2));          \
    IsletCave_FinishTeleport();                                                \
  }

DEFINE_ISLET_CAVE_ARRIVE_AT(IsletCave_TeleportToFirstCircle, 232, 136)

DEFINE_ISLET_CAVE_ARRIVE_AT(IsletCave_TeleportToSecondCircle, 240, 480)

/// Darkens the palette by the scale given: each corridor is darker than the
/// last.
void IsletCave_DimPalette(s32 scale) {
  u16 *source = Ram_HeapSlots->paletteFade;
  u16 *channel = (u16 *)((u8 *)source + 0xe00);
  u32 count = 224;
  u32 i = 0;
  u32 color, value, mask;

  do {
    color = *source++;
    mask = ~0x3ff;
    value = (color >> 10) & 31;
#ifdef BUGFIX
    value *= scale;
#else
    // BUG: blue is multiplied by the full scale, 0x400, not by scale, so it
    // never dims.
    value *= 0x400;
#endif
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = (color >> 5) & 31;
    value *= scale;
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    value = color & 31;
    value *= scale;
    value &= mask;
    if (value > 0x7c00)
      value = 0x7c00;
    *channel++ = value;
    i++;
  } while (i < count);
}

extern struct MapGridCellAnimation gIsletCave_Doors[];

void IsletCave_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 i;
  u32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  i = 8;
loop:
  {
    struct Actor *object = Actor_GetById(i);

    if (object != 0)
      object->motionFlags = 0;
  }
  i++;
  if (i <= 63)
    goto loop;
  index = work->stepTrigger - 1;
  Sound_Play(SE_DOOR_OPEN);
  index >>= 1;
  {
    s32 x = gIsletCave_Doors[index].x;
    s32 y = gIsletCave_Doors[index].y;

    MapGrid_AnimateCells(gIsletCave_Doors[index].steps, x, y);
  }
  Actor_SetAnimationById(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, 0, -4);
  Event_Wait(6);
  Import_EventSetExitRequest(work->stepTrigger);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

extern u16 gIsletCave_PaletteSlots[];
extern u16 gIsletCave_PaletteColors[];
extern u16 gIsletCave_DimColors[];
extern u16 gIsletCave_CycleSlots[];
extern s32 gIsletCave_TremorTargetScript[];

s32 Scene_Initialize(void) {
  s32 scene, dim;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  FieldEffect_StartBlankOverlay(0);
  scene = gMainRoutineWork.next;
  if (scene == SCENE_ISLET_CAVE_MELD) {
    IsletCave_PlaceLogs();
    Palette_SetColors(gIsletCave_PaletteSlots, gIsletCave_PaletteColors,
                      gIsletCave_DimColors, gIsletCave_CycleSlots);
  } else if (scene == SCENE_ISLET_CAVE_SENTINEL) {
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_FIRE), 0);
    if (gMainRoutineWork.entrance == 2) {
      Actor_SetPosition(ACTOR_SENTINEL, 0, 0);
      GameFlag_Set(FLAG_SENTINEL_DEFEATED);
    }
  } else if (scene == SCENE_ISLET_CAVE_SERAC) {
    if (GameFlag_Get(FLAG_DJINNI(DJINNI_SERAC)) ||
        GameFlag_Get(FLAG_DJINNI_HIDING(DJINNI_SERAC))) {
      GameFlag_Set(FLAG_SCENE_TEMP_30);
    } else if (!GameFlag_Get(FLAG_SCENE_TEMP_30)) {
      Actor_SetPosition(ACTOR_SERAC, 0, 0);
      Actor_SetMovementTypeById(ACTOR_WINGED_STATUE,
                                gIsletCave_TremorTargetScript);
    }
  } else {
    goto other;
  }
  if (gMainRoutineWork.next == SCENE_ISLET_CAVE_SERAC)
    goto fade;
other:
  if (gMainRoutineWork.next == SCENE_ISLET_CAVE_SENTINEL) {
  fade:
    if (gMainRoutineWork.next == SCENE_ISLET_CAVE_SERAC)
      dim = (gMainRoutineWork.entrance - 3) / 2;
    else
      dim = 3;
    IsletCave_DimPalette(0x400 - (dim << 7));
    PaletteFade_PackTarget();
    PaletteFade_StartInterpolation(1);
    Event_WaitTransitionFrames();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Readies object id: when the flag is set, places it at (x, z) in cells;
/// marks it, then calls Map_CopyCellAttributeRect with the cell to the left of
/// it.
void IsletCave_PlaceLogByCellLeft(s32 id, s32 a, s32 b, s32 x, s32 z,
                                  s32 unused, s32 flag) {
  struct Actor *object = Actor_GetById(id);

  if (object != NULL) {
    if (GameFlag_Get(flag))
      Actor_SetPosition(id, x, z);
    Actor_SetSpritePriority(id, 3);
    object->priorityFlags |= 2;
    MapGrid_CopyCellAttributes(a, b, 3, 1, FIELD_CELL(object->x.fixed) - 1,
                               FIELD_CELL(object->z.fixed));
  }
}

/// As IsletCave_PlaceLogByCellLeft, with the cell above the object.
void IsletCave_PlaceLogByCellAbove(s32 id, s32 a, s32 bArg, s32 x, s32 z,
                                   s32 unused, s32 flag) {
  struct Actor *object;

  if (Actor_GetById(id) != 0) {
    Actor_SetSpritePriority(id, 3);
    if (GameFlag_Get(flag))
      Actor_SetPosition(id, x, z);
    object = Actor_GetById(id);
    object->priorityFlags |= 2;
    MapGrid_CopyCellAttributes(a, bArg, 1, 3, FIELD_CELL(object->x.fixed),
                               FIELD_CELL(object->z.fixed) - 1);
  }
}

/// A log the leader steps onto rocks under him.
void IsletCave_SettleLog(s32 logId) {
  Actor_SetAnimationById(logId, 1);
  Actor_SetAnimationById(logId, 2);
  return;
}

/// Places the seven logs on the water where their flags left them.
void IsletCave_PlaceLogs(void) {
  ((void (*)())IsletCave_PlaceLogByCellAbove)(8, 27, 0, 136, 0x1f8, 20, 0x280);
  ((void (*)())IsletCave_PlaceLogByCellAbove)(9, 28, 0, 136, 232, 21, 0x281);
  ((void (*)())IsletCave_PlaceLogByCellAbove)(10, 29, 0, 0x358, 152, 22, 0x282);
  ((void (*)())IsletCave_PlaceLogByCellAbove)(11, 30, 0, 0x318, 152, 23, 0x283);
  ((void (*)())IsletCave_PlaceLogByCellAbove)(12, 31, 0, 0x358, 0x178, 24,
                                              0x284);
  ((void (*)())IsletCave_PlaceLogByCellAbove)(13, 32, 0, 0x318, 0x178, 25,
                                              0x285);
  ((void (*)())IsletCave_PlaceLogByCellLeft)(14, 33, 0, 0x338, 120, 26, 0x286);
  return;
}

void IsletCave_SettleFirstLog(void) {
  IsletCave_SettleLog(8);
  return;
}

void IsletCave_SettleSecondLog(void) {
  IsletCave_SettleLog(9);
  return;
}

void IsletCave_SettleThirdLog(void) {
  IsletCave_SettleLog(10);
  return;
}

void IsletCave_SettleFourthLog(void) {
  IsletCave_SettleLog(11);
  return;
}

void IsletCave_SettleFifthLog(void) {
  IsletCave_SettleLog(12);
  return;
}

void IsletCave_SettleSixthLog(void) {
  IsletCave_SettleLog(13);
  return;
}

void IsletCave_SettleSeventhLog(void) {
  IsletCave_SettleLog(14);
  return;
}

void IsletCave_RideLog(s32 actorId, s32 dx, s32 dz) {
  struct Actor *leader;
  struct Actor *actor;
  FIXED x;
  FIXED z;

  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  actor = Actor_GetById(actorId);
  Event_Start();
  Event_ResolvePendingActions(0);
  x = ((leader->x.fixed + toFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((leader->z.fixed + toFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);
  leader->speedLimit = toFIXED(1);
  leader->acceleration = toFIXED(1) / 2;
  Actor_SetMoveTarget(leader, x, leader->y.fixed, z);
  Actor_SelectAnimation(leader, ANIM_LEADER_WALK_BACKWARDS);
  x = ((actor->x.fixed + toFIXED(dx)) & FIELD_CELL_MASK) + toFIXED(8);
  z = ((actor->z.fixed + toFIXED(dz)) & FIELD_CELL_MASK) + toFIXED(8);
  actor->speedLimit = toFIXED(1);
  actor->acceleration = toFIXED(1) / 2;
  Actor_SetMoveTarget(actor, x, actor->y.fixed, z);
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(actor, 4);
  } else {
    Actor_SelectAnimation(actor, 3);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(leader);
  Actor_SelectAnimation(actor, 2);
  Sound_Play(SE_STOP_EXIT_SE);
  Event_End();
}

/// Rides log `logId` `dx` pixels across the water and moves its cells with it,
/// then sets or clears its flag (`flagCall`) to say where it lies. The
/// name is ours.
#define DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(name, logId, dx, flagCall, flag)     \
  void name(void) {                                                            \
    s32 fromX = Actor_GetById(logId)->x.fixed;                                 \
    s32 fromZ = Actor_GetById(logId)->z.fixed;                                 \
                                                                               \
    IsletCave_RideLog(logId, dx, 0);                                           \
    wait(2);                                                                   \
    Map_CopyCellAttributeRect(FIELD_CELL(fromX), FIELD_CELL(fromZ) - 1, 1, 3,  \
                              FIELD_CELL(Actor_GetById(logId)->x.fixed),       \
                              FIELD_CELL(fromZ) - 1);                          \
    Map_CopyCellAttributeRect(0, 0, 1, 3, FIELD_CELL(fromX),                   \
                              FIELD_CELL(fromZ) - 1);                          \
    flagCall(flag);                                                            \
  }

/// The logs floating in the cave's water, in the order the party rides them.
enum {
  ACTOR_FIRST_LOG = 8,
  ACTOR_SECOND_LOG = 9,
  ACTOR_THIRD_LOG = 10,
  ACTOR_FOURTH_LOG = 11,
  ACTOR_FIFTH_LOG = 12,
  ACTOR_SIXTH_LOG = 13,
  ACTOR_SEVENTH_LOG = 14
};

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogEast, ACTOR_FIRST_LOG,
                                  32, GameFlag_Clear, FLAG_SCENE_TEMP_80)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogWest, ACTOR_FIRST_LOG,
                                  -32, GameFlag_Set, FLAG_SCENE_TEMP_80)

void IsletCave_RideSecondLogEast(void) {
  FIXED fromX = Actor_GetById(ACTOR_SECOND_LOG)->x.fixed;
  FIXED fromZ = Actor_GetById(ACTOR_SECOND_LOG)->z.fixed;

  IsletCave_RideLog(ACTOR_SECOND_LOG, 112, 0);
  IsletCave_RideLog(ACTOR_SECOND_LOG, 96, 0);
  wait(2);
  Map_CopyCellAttributeRect(
      FIELD_CELL(fromX), FIELD_CELL(fromZ) - 1, 1, 3,
      FIELD_CELL(Actor_GetById(ACTOR_SECOND_LOG)->x.fixed),
      FIELD_CELL(fromZ) - 1);
  Map_CopyCellAttributeRect(0, 0, 1, 3, FIELD_CELL(fromX),
                            FIELD_CELL(fromZ) - 1);
  GameFlag_Clear(FLAG_SCENE_TEMP_81);
}

void IsletCave_RideSecondLogWest(void) {
  FIXED fromX = Actor_GetById(ACTOR_SECOND_LOG)->x.fixed;
  FIXED fromZ = Actor_GetById(ACTOR_SECOND_LOG)->z.fixed;

  IsletCave_RideLog(ACTOR_SECOND_LOG, -112, 0);
  IsletCave_RideLog(ACTOR_SECOND_LOG, -96, 0);
  wait(2);
  Map_CopyCellAttributeRect(
      FIELD_CELL(fromX), FIELD_CELL(fromZ) - 1, 1, 3,
      FIELD_CELL(Actor_GetById(ACTOR_SECOND_LOG)->x.fixed),
      FIELD_CELL(fromZ) - 1);
  Map_CopyCellAttributeRect(0, 0, 1, 3, FIELD_CELL(fromX),
                            FIELD_CELL(fromZ) - 1);
  GameFlag_Set(FLAG_SCENE_TEMP_81);
}

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogEast, ACTOR_THIRD_LOG,
                                  80, GameFlag_Set, FLAG_SCENE_TEMP_82)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogWest, ACTOR_THIRD_LOG,
                                  -80, GameFlag_Clear, FLAG_SCENE_TEMP_82)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFourthLogEast, ACTOR_FOURTH_LOG,
                                  80, GameFlag_Clear, FLAG_SCENE_TEMP_83)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFourthLogWest, ACTOR_FOURTH_LOG,
                                  -80, GameFlag_Clear, FLAG_SCENE_TEMP_83)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFifthLogEast, ACTOR_FIFTH_LOG,
                                  80, GameFlag_Set, FLAG_SCENE_TEMP_84)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFifthLogWest, ACTOR_FIFTH_LOG,
                                  -80, GameFlag_Clear, FLAG_SCENE_TEMP_84)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideSixthLogEast, ACTOR_SIXTH_LOG,
                                  80, GameFlag_Clear, FLAG_SCENE_TEMP_85)

DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideSixthLogWest, ACTOR_SIXTH_LOG,
                                  -80, GameFlag_Set, FLAG_SCENE_TEMP_85)

void IsletCave_RideSeventhLogNorth(void) {
  FIXED fromX = Actor_GetById(ACTOR_SEVENTH_LOG)->x.fixed;
  s32 column = fromX;
  s32 step = 112;
  s32 fromRow = Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed;

  step = -step;
  fromRow >>= 20;
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, step);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, step);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, -128);
  wait(2);
  column = FIELD_CELL(column) - 1;
  MapGrid_CopyCellAttributes(
      column, fromRow, 3, 1, column,
      FIELD_CELL(Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed));
  Map_CopyCellAttributeRect(0, 0, 3, 1, column, fromRow);
  GameFlag_Set(FLAG_SCENE_TEMP_86);
}

void IsletCave_RideSeventhLogSouth(void) {
  FIXED fromX = Actor_GetById(ACTOR_SEVENTH_LOG)->x.fixed;
  s32 column = fromX;
  FIXED fromZ = Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed;
  s32 fromRow = fromZ;

  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, 112);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, 112);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, 128);
  wait(2);
  column = FIELD_CELL(column) - 1;
  fromRow >>= 20;
  MapGrid_CopyCellAttributes(
      column, fromRow, 3, 1, column,
      FIELD_CELL(Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed));
  Map_CopyCellAttributeRect(0, 0, 3, 1, column, fromRow);
  GameFlag_Clear(FLAG_SCENE_TEMP_86);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// At the end of the corridors the guardian of wind is summoned and the
/// Sentinel fights.
void IsletCave_SentinelAwaits(void) {
  s32 cue;
  struct Actor *object;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_StartRepeatedMotion(ACTOR_SENTINEL, 2);
  Event_Wait(20);
  PaletteFade_ApplySource(PALETTE_TINT_NONE, 0);
  PaletteFade_SetTarget(PALETTE_TINT_WARM, 1);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  EventMessage_ShowCentered((s32)MsgSummonGuardianOfWind, 0, 2);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  PaletteFade_StartInterpolation(60);
  Event_Wait(60);
  object = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (object != 0) {
    Actor_SetDestinationById(ACTOR_SENTINEL, object->x.part[0].integer,
                             object->z.part[0].integer);
  }
  Event_Wait(20);
  FieldEvent_SetAfterBattleDestination(SCENE_ISLET_CAVE_SENTINEL, 2);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(0x67, 2);
  Event_End();
}

/// Tremor on the shaking spot: Serac drops out.
void IsletCave_TremorRevealsSerac(void) {
  if (!Event_GetFlag(FLAG_SCENE_TEMP_30)) {
    struct Actor *object = Actor_GetById(ACTOR_SERAC);
    struct Actor *owner = Actor_GetById(gMainRoutineWork.leaderActorId);

    Event_SetFlag(FLAG_SCENE_TEMP_30);
    Actor_SetPosition(ACTOR_SERAC, 296, 160);
    object->speedLimit = toFIXED(3) / 2;
    object->velocityY = toFIXED(15);
    Actor_MoveToAndWait(ACTOR_SERAC, 344, ((owner->z.fixed >> 19) << 3) + 24);
    Actor_Stop(ACTOR_WINGED_STATUE);
    object->speedLimit = toFIXED(1);
    Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, 9, 0);
  }
}

#undef DEFINE_ISLET_CAVE_ARRIVE_AT
#undef DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS
#undef FIELD_AT

void IsletCave_RideFirstLogWest(void);
void IsletCave_RideThirdLogWest(void);

// clang-format off
u16 gIsletCave_PaletteSlots[] = {9, 10, 11, 12, 13, 14, 15, 65535};

u16 gIsletCave_PaletteColors[] = {23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 23, 25, 22, 65535};

u16 gIsletCave_DimColors[] = {30, 27, 31, 22, 21, 31, 20, 18, 31, 13, 11, 27, 11, 8, 24, 8, 5, 21, 31, 31, 31, 65535};

u16 gIsletCave_CycleSlots[] = {                                                               
      14, 9, 10, 11, 12, 13, 13, 14, 9, 10, 11, 12, 12, 13, 14, 9, 10, 11,  
      11, 12, 13, 14, 9, 10, 10, 11, 12, 13, 14, 9, 9, 10, 11, 12, 13, 14,  
  };

u32 gIsletCave_SentinelScript[] = {
    SC_SET(SW_SCALE_Y, 0x14ccc),
    SC_SET(SW_SCALE_X, 0x14ccc),
    SC_END,
};

s32 gIsletCave_ShrunkenSpriteScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(-4)),
    SC_SET(SW_SCALE_X, 0xcccc),
    SC_SET(SW_SCALE_Y, 0xcccc),
    SC_CALL(IsletCave_ClearScriptedActorSpriteFlags),
    SC_END,
};

s32 gIsletCave_TremorTargetScript[] = {
    SC_CALL(IsletCave_BobOnWater),
    SC_END,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(IsletCaveMeld),
    SCENE_EXIT_UNLESS(1, IsletsTurtle, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IsletCaveMeld, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IsletCaveMeld, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IsletCaveMeld, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IsletCaveMeld, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IsletCaveMeld, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IsletCaveMeld, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IsletCaveSerac, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, IsletCaveMeld, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IsletCaveMeld, 9, EXIT_FLAG_NONE),
    SCENE_EXITS(IsletCaveSerac),
    SCENE_EXIT_UNLESS(1, IsletCaveMeld, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, IsletCaveSerac, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, IsletCaveSerac, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, IsletCaveSerac, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, IsletCaveSerac, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, IsletCaveSerac, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, IsletCaveSerac, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, IsletCaveSerac, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, IsletCaveSerac, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, IsletCaveSentinel, 1, EXIT_FLAG_NONE),
    SCENE_EXITS(IsletCaveSentinel),
    SCENE_EXIT_UNLESS(1, IsletCaveSerac, 10, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gIsletCave_Placements[] = {
    ACTOR_PLACEMENT(170, FLAG_SENTINEL_DEFEATED, (s32)gIsletCave_SentinelScript, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 152, 0, 144, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 216, 0, 144, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 136, 0, 176, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 232, 0, 176, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 152, 0, 224, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 216, 0, 224, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 136, 0, 256, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 232, 0, 256, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 136, 0, 304, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FIRE, CONDITION_ALWAYS, (s32)gIsletCave_ShrunkenSpriteScript, 232, 0, 304, FACING_NORTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIsletCave_Room1Placements[] = {
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 168, 0, 504, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 344, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 776, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 872, 0, 152, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 776, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7, 872, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, 7, 824, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MELD), MOVEMENT_TYPE_STAND, 824, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gIsletCave_Room2Placements[] = {
    ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SERAC), MOVEMENT_TYPE_STAND, 336, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct Trigger gIsletCave_Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT(IsletCave_RideFirstLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 20, CONDITION_ALWAYS, EVENT(IsletCave_RideFirstLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT(IsletCave_RideSecondLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(IsletCave_RideSecondLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 22, CONDITION_ALWAYS, EVENT(IsletCave_RideThirdLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 22, CONDITION_ALWAYS, EVENT(IsletCave_RideThirdLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 23, CONDITION_ALWAYS, EVENT(IsletCave_RideFourthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS, EVENT(IsletCave_RideFourthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 24, CONDITION_ALWAYS, EVENT(IsletCave_RideFifthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 24, CONDITION_ALWAYS, EVENT(IsletCave_RideFifthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 25, CONDITION_ALWAYS, EVENT(IsletCave_RideSixthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 25, CONDITION_ALWAYS, EVENT(IsletCave_RideSixthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 26, CONDITION_ALWAYS, EVENT(IsletCave_RideSeventhLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT(IsletCave_RideSeventhLogSouth)},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(IsletCave_SettleFirstLog)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(IsletCave_SettleSecondLog)},
    {TRIGGER_STEP, 22, CONDITION_ALWAYS, EVENT(IsletCave_SettleThirdLog)},
    {TRIGGER_STEP, 23, CONDITION_ALWAYS, EVENT(IsletCave_SettleFourthLog)},
    {TRIGGER_STEP, 24, CONDITION_ALWAYS, EVENT(IsletCave_SettleFifthLog)},
    {TRIGGER_STEP, 25, CONDITION_ALWAYS, EVENT(IsletCave_SettleSixthLog)},
    {TRIGGER_STEP, 26, CONDITION_ALWAYS, EVENT(IsletCave_SettleSeventhLog)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, EVENT(IsletCave_FightMeld)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 50, CONDITION_ALWAYS, EVENT(IsletCave_TeleportToFirstCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_TELEPORT, PSYNERGY_PHASE_AFTER_CAST), 51, CONDITION_ALWAYS, EVENT(IsletCave_TeleportToSecondCircle)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIsletCave_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 4, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT(IsletCave_OpenDoorAndEnter)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 8, FLAG_SCENE_TEMP_30, EVENT(IsletCave_TremorRevealsSerac)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(IsletCave_FightSerac)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIsletCave_Room3Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_STEP, 10, 0x9cf, EVENT(IsletCave_SentinelAwaits)},
    {TRIGGER_TABLE_END},
};

s16 gIsletCave_DoorFrames[] = {
    72, 4, 2, 2, 2, 73, 4, 2,
    2, 2, -1, 0,
};
// clang-format on

struct MapGridCellAnimation gIsletCave_Doors[] = {
    {gIsletCave_DoorFrames, 72, 4},  {gIsletCave_DoorFrames, 84, 4},
    {gIsletCave_DoorFrames, 96, 4},  {gIsletCave_DoorFrames, 107, 4},
    {gIsletCave_DoorFrames, 118, 4},
};
