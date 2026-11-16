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
#include "macros.h"
#include "system/ram_buffer.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "system/task.h"
#include "field/actor.h"
#include "field/field_sprite.h"
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

u16 gPaletteCycleBright BSS_DATA;
u16 *gPaletteCycleSlots BSS_DATA;
u16 gPaletteCycleTimer BSS_DATA;
u16 gPaletteCycleDelay BSS_DATA ALIGNED(4);
u16 *gPaletteSlots BSS_DATA;
u16 *gPaletteColors BSS_DATA;
u16 *gPaletteCycleColors BSS_DATA;
u16 gPaletteCycleLength BSS_DATA;
u16 gPaletteCycleIndex BSS_DATA ALIGNED(4);
void Teleport_PlaySparks(void);
void FieldEffect_StartBlankOverlay(s32 mode);
void PaletteFade_PackTarget(void);

/// Ends the lists of palette slots and colours the water's cycle reads.
#define PALETTE_LIST_END 0xffff

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies the BG and OBJ palettes into the palette
/// fade's source and starts the blend.
void Palette_SetColors(u16 *slots, u16 *colours, u16 *cycleColours,
                       u16 *cycleSlots) {
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;

  gPaletteSlots = slots;
  gPaletteColors = colours;
  gPaletteCycleColors = cycleColours;
  gPaletteCycleSlots = cycleSlots;
  if (*slots != PALETTE_LIST_END && *colours != PALETTE_LIST_END) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      SET_RGB(((u16 *)BG_PLTT)[index], red, green, blue);
    }
    if (*slots != PALETTE_LIST_END && *colours != PALETTE_LIST_END)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, fade->palette, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, &fade->palette[224], DMA_COPY_WORDS(112),
          REG_DMA3);
  PaletteFade_SetTarget(PALETTE_TINT_NONE, 0);
  if (gMainRoutineWork.teleportState == 2)
    IsletCave_TeleportIn();
}

/// Starts the water's colours cycling, slowly at first: the first step waits
/// 16 frames, and each step after waits a frame less, down to two.
void IsletCave_StartSlowPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 15;
  gPaletteCycleTimer = 16;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

/// Stops the water's colours cycling, and sets the next screen transition to a
/// fade of the backdrop.
void IsletCave_StopPaletteCycle(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Task_Remove(PaletteCycle_Fade);
}

/// Starts the water's colours cycling at full speed, a step every two frames.
void IsletCave_StartFastPaletteCycle(void) {
  gPaletteCycleIndex = 0;
  gPaletteCycleLength = List_CountEntries(gPaletteSlots);
  gPaletteCycleDelay = 2;
  gPaletteCycleTimer = 2;
  gPaletteCycleBright = 1;
  Task_Add(PaletteCycle_Fade, TASK_PASS_VBLANK);
}

#include "shared/list_count_entries.h"

#include "shared/palette_cycle_fade.h"

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

/// Talking to the Djinni at the end of the logs starts the battle with Meld, a
/// Venus Djinni.
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

/// The Sentinel's room's entrance the battle with it returns the party to.
enum { ENTRANCE_AFTER_BATTLE = 2 };

/// Talking to Serac once Tremor has shaken him out starts the battle with him,
/// a Mercury Djinni.
void IsletCave_FightSerac(void) {
  Field_RequestDjinnBattle(ACTOR_SERAC, DJINNI_SERAC);
  return;
}

/// The fires' script's last call: clears the actor's sprite flags.
s32 IsletCave_ClearScriptedActorSpriteFlags(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// The winged statue's script while Serac hides in it: every 64 frames it
/// hops, rocking its sprite through the first half of the cycle.
s32 IsletCave_HopWingedStatue(struct Actor *object) {
  s32 step = object->riseCounter & 63;
  struct FieldSprite *sprite = object->sprite;

  object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (step < 32) {
    if (step == 0)
      object->velocityY = toFIXED(1);
    sprite->rotation = Trig_Cos(step << 12) / 128;
  }
  object->riseCounter++;
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

/// The end of a Teleport onto one of the circles: the water spins fast while
/// the whirl plays, then the colours settle and the leader is shown again.
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

/// Teleport cast on a circle: the sparks rise, and after 40 frames the leader
/// stands at (x, z).
#define DEFINE_ISLET_CAVE_ARRIVE_AT(name, x, z)                                \
  void name(void) {                                                            \
    IsletCave_StartSlowPaletteCycle();                                         \
    Teleport_PlaySparks();                                                     \
                                                                               \
    gMainRoutineWork.teleportState = 1;                                        \
    Event_Wait(40);                                                            \
    Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.leaderActorId,     \
                                           toFIXED(x), toFIXED(z));            \
    IsletCave_FinishTeleport();                                                \
  }

/// Teleport cast where it answers carries the leader to the first circle.
DEFINE_ISLET_CAVE_ARRIVE_AT(IsletCave_TeleportToFirstCircle, 232, 136)

/// Teleport cast where it answers carries the leader to the second circle.
DEFINE_ISLET_CAVE_ARRIVE_AT(IsletCave_TeleportToSecondCircle, 240, 480)

/// One level of a palette fade's colour channel, which holds its 0 to 31
/// shifted up 10 bits: also the scale that leaves a colour as it is.
#define FADE_CHANNEL_LEVEL (1 << 10)

/// Darkens the palette by the scale given, out of FADE_CHANNEL_LEVEL: each
/// corridor is darker than the last.
void IsletCave_DimPalette(s32 scale) {
  struct PaletteFade *fade = Ram_HeapSlots->paletteFade;
  u16 *source = fade->palette;
  u16 *channel = fade->target;
  u32 count = ARRAY_COUNT(fade->palette) / 2;
  u32 i = 0;
  u32 color, value, mask;

  do {
    color = *source++;
    mask = ~(FADE_CHANNEL_LEVEL - 1);
    value = RGB_BLUE(color);
#ifdef BUGFIX
    value *= scale;
#else
    // BUG: blue is multiplied by the full scale, not by scale, so it never
    // dims.
    value *= FADE_CHANNEL_LEVEL;
#endif
    value &= mask;
    if (value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
      value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
    *channel++ = value;
    value = RGB_GREEN(color);
    value *= scale;
    value &= mask;
    if (value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
      value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
    *channel++ = value;
    value = RGB_RED(color);
    value *= scale;
    value &= mask;
    if (value > RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL)
      value = RGB_LEVEL_MAX * FADE_CHANNEL_LEVEL;
    *channel++ = value;
    i++;
  } while (i < count);
}

extern struct MapGridCellAnimation gIsletCave_Doors[];

/// Walking north into one of the Serac room's doors: everything stops, the
/// door opens and the leader walks through it to the next corridor.
void IsletCave_OpenDoorAndEnter(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  u32 i;
  u32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (i = ACTOR_FIRST_PLACED; i <= 63; i++) {
    struct Actor *object = Actor_GetById(i);

    if (object != 0)
      object->motionFlags = 0;
  }
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

/// Sets up the room being entered: the logs where their flags left them and the
/// water's colours, the Sentinel gone once beaten, or Serac hidden in the
/// winged statue until Tremor; the corridors darken a step a room.
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
    if (gMainRoutineWork.entrance == ENTRANCE_AFTER_BATTLE) {
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
    IsletCave_DimPalette(FADE_CHANNEL_LEVEL - dim * (FADE_CHANNEL_LEVEL / 8));
    PaletteFade_PackTarget();
    PaletteFade_StartInterpolation(1);
    Event_WaitTransitionFrames();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// The logs floating in the cave's water, in the order the party rides them.
/// The first six are three cells tall and drift east and west, the seventh is
/// three cells wide and drifts north and south.
enum {
  ACTOR_FIRST_LOG = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_LOG,
  ACTOR_THIRD_LOG,
  ACTOR_FOURTH_LOG,
  ACTOR_FIFTH_LOG,
  ACTOR_SIXTH_LOG,
  ACTOR_SEVENTH_LOG
};

/// The cell behaviour under each log, whose step triggers settle it and ride
/// it (gIsletCave_Triggers).
enum {
  CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG = 20,
  CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG,
  CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG
};

/// Set while a log lies across the water from where it is placed, until the
/// party leaves the cave's map: IsletCave_PlaceLogs then places it there.
///
/// XXX: both of the fourth log's rides clear its flag, so it is placed back at
/// its start; the game run past that log would show whether it jumps back.
enum {
  FLAG_ISLET_CAVE_FIRST_LOG_MOVED = FLAG_SCENE_TEMP_80,
  FLAG_ISLET_CAVE_SECOND_LOG_MOVED = FLAG_SCENE_TEMP_81,
  FLAG_ISLET_CAVE_THIRD_LOG_MOVED = FLAG_SCENE_TEMP_82,
  FLAG_ISLET_CAVE_FOURTH_LOG_MOVED = FLAG_SCENE_TEMP_83,
  FLAG_ISLET_CAVE_FIFTH_LOG_MOVED = FLAG_SCENE_TEMP_84,
  FLAG_ISLET_CAVE_SIXTH_LOG_MOVED = FLAG_SCENE_TEMP_85,
  FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED = FLAG_SCENE_TEMP_86
};

/// Places a log three cells wide that drifts north and south: at (x, z) if its
/// flag says it has drifted, under the party, then draws its footprint from
/// the stored cells at (sourceColumn, sourceRow); its cell behaviour is passed
/// and not used.
void IsletCave_PlaceWestEastLog(s32 actorId, s32 sourceColumn, s32 sourceRow,
                                s32 x, s32 z, s32 cellBehavior, s32 flag) {
  struct Actor *actor = Actor_GetById(actorId);

  if (actor != NULL) {
    if (GameFlag_Get(flag))
      Actor_SetPosition(actorId, x, z);
    Actor_SetSpritePriority(actorId, 3);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 3, 1,
                               FIELD_CELL(actor->x.fixed) - 1,
                               FIELD_CELL(actor->z.fixed));
  }
}

/// As IsletCave_PlaceWestEastLog, for a log three cells tall that drifts east
/// and west.
void IsletCave_PlaceNorthSouthLog(s32 actorId, s32 sourceColumn, s32 sourceRow,
                                  s32 x, s32 z, s32 cellBehavior, s32 flag) {
  struct Actor *actor;

  if (Actor_GetById(actorId) != 0) {
    Actor_SetSpritePriority(actorId, 3);
    if (GameFlag_Get(flag))
      Actor_SetPosition(actorId, x, z);
    actor = Actor_GetById(actorId);
    actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
    MapGrid_CopyCellAttributes(sourceColumn, sourceRow, 1, 3,
                               FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed) - 1);
  }
}

/// Stepping onto a log plays its animation from the start.
void IsletCave_SettleLog(s32 actorId) {
  Actor_SetAnimationById(actorId, 1);
  Actor_SetAnimationById(actorId, 2);
  return;
}

/// Places the seven logs on the water where their flags left them.
void IsletCave_PlaceLogs(void) {
  IsletCave_PlaceNorthSouthLog(ACTOR_FIRST_LOG, 27, 0, 136, 504,
                               CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG,
                               FLAG_ISLET_CAVE_FIRST_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_SECOND_LOG, 28, 0, 136, 232,
                               CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG,
                               FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_THIRD_LOG, 29, 0, 856, 152,
                               CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG,
                               FLAG_ISLET_CAVE_THIRD_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_FOURTH_LOG, 30, 0, 792, 152,
                               CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG,
                               FLAG_ISLET_CAVE_FOURTH_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_FIFTH_LOG, 31, 0, 856, 376,
                               CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG,
                               FLAG_ISLET_CAVE_FIFTH_LOG_MOVED);
  IsletCave_PlaceNorthSouthLog(ACTOR_SIXTH_LOG, 32, 0, 792, 376,
                               CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG,
                               FLAG_ISLET_CAVE_SIXTH_LOG_MOVED);
  IsletCave_PlaceWestEastLog(ACTOR_SEVENTH_LOG, 33, 0, 824, 120,
                             CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG,
                             FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
  return;
}

/// Stepping onto the first log.
void IsletCave_SettleFirstLog(void) {
  IsletCave_SettleLog(ACTOR_FIRST_LOG);
  return;
}

/// Stepping onto the second log.
void IsletCave_SettleSecondLog(void) {
  IsletCave_SettleLog(ACTOR_SECOND_LOG);
  return;
}

/// Stepping onto the third log.
void IsletCave_SettleThirdLog(void) {
  IsletCave_SettleLog(ACTOR_THIRD_LOG);
  return;
}

/// Stepping onto the fourth log.
void IsletCave_SettleFourthLog(void) {
  IsletCave_SettleLog(ACTOR_FOURTH_LOG);
  return;
}

/// Stepping onto the fifth log.
void IsletCave_SettleFifthLog(void) {
  IsletCave_SettleLog(ACTOR_FIFTH_LOG);
  return;
}

/// Stepping onto the sixth log.
void IsletCave_SettleSixthLog(void) {
  IsletCave_SettleLog(ACTOR_SIXTH_LOG);
  return;
}

/// Stepping onto the seventh log.
void IsletCave_SettleSeventhLog(void) {
  IsletCave_SettleLog(ACTOR_SEVENTH_LOG);
  return;
}

/// Rides log actorId one stage, `dx` and `dz` pixels, with the leader walking
/// backwards on it to the cell it stops at.
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

/// Rides log `logId` `dx` pixels across the water and moves its footprint with
/// it, then sets or clears its flag (`flagCall`) to say where it lies.
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

/// Walking east onto the first log rides it east across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogEast, ACTOR_FIRST_LOG,
                                  32, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FIRST_LOG_MOVED)

/// Walking west onto the first log rides it west across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFirstLogWest, ACTOR_FIRST_LOG,
                                  -32, GameFlag_Set,
                                  FLAG_ISLET_CAVE_FIRST_LOG_MOVED)

/// Walking east onto the second log rides it east across the water, in
/// two stages.
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
  GameFlag_Clear(FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
}

/// Walking west onto the second log rides it west across the water, in
/// two stages.
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
  GameFlag_Set(FLAG_ISLET_CAVE_SECOND_LOG_MOVED);
}

/// Walking east onto the third log rides it east across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogEast, ACTOR_THIRD_LOG,
                                  80, GameFlag_Set,
                                  FLAG_ISLET_CAVE_THIRD_LOG_MOVED)

/// Walking west onto the third log rides it west across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideThirdLogWest, ACTOR_THIRD_LOG,
                                  -80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_THIRD_LOG_MOVED)

/// Walking east onto the fourth log rides it east across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFourthLogEast, ACTOR_FOURTH_LOG,
                                  80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FOURTH_LOG_MOVED)

/// Walking west onto the fourth log rides it west across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFourthLogWest, ACTOR_FOURTH_LOG,
                                  -80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FOURTH_LOG_MOVED)

/// Walking east onto the fifth log rides it east across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFifthLogEast, ACTOR_FIFTH_LOG,
                                  80, GameFlag_Set,
                                  FLAG_ISLET_CAVE_FIFTH_LOG_MOVED)

/// Walking west onto the fifth log rides it west across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideFifthLogWest, ACTOR_FIFTH_LOG,
                                  -80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_FIFTH_LOG_MOVED)

/// Walking east onto the sixth log rides it east across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideSixthLogEast, ACTOR_SIXTH_LOG,
                                  80, GameFlag_Clear,
                                  FLAG_ISLET_CAVE_SIXTH_LOG_MOVED)

/// Walking west onto the sixth log rides it west across the water.
DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS(IsletCave_RideSixthLogWest, ACTOR_SIXTH_LOG,
                                  -80, GameFlag_Set,
                                  FLAG_ISLET_CAVE_SIXTH_LOG_MOVED)

/// Walking north onto the seventh log rides it north across the water, in
/// three stages.
void IsletCave_RideSeventhLogNorth(void) {
  FIXED fromX = Actor_GetById(ACTOR_SEVENTH_LOG)->x.fixed;
  s32 column = fromX;
  s32 step = 112;
  s32 fromRow = Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed;

  step = -step;
  fromRow = FIELD_CELL(fromRow);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, step);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, step);
  IsletCave_RideLog(ACTOR_SEVENTH_LOG, 0, -128);
  wait(2);
  column = FIELD_CELL(column) - 1;
  MapGrid_CopyCellAttributes(
      column, fromRow, 3, 1, column,
      FIELD_CELL(Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed));
  Map_CopyCellAttributeRect(0, 0, 3, 1, column, fromRow);
  GameFlag_Set(FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

/// Walking south onto the seventh log rides it south across the water, in
/// three stages.
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
  fromRow = FIELD_CELL(fromRow);
  MapGrid_CopyCellAttributes(
      column, fromRow, 3, 1, column,
      FIELD_CELL(Actor_GetById(ACTOR_SEVENTH_LOG)->z.fixed));
  Map_CopyCellAttributeRect(0, 0, 3, 1, column, fromRow);
  GameFlag_Clear(FLAG_ISLET_CAVE_SEVENTH_LOG_MOVED);
}

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
  FieldEvent_SetAfterBattleDestination(SCENE_ISLET_CAVE_SENTINEL,
                                       ENTRANCE_AFTER_BATTLE);
  Battle_SetType(BATTLE_TYPE_NO_ESCAPE);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_TABLE_HIDDEN_BOSSES,
                                        HIDDEN_BOSS_SENTINEL);
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
    Actor_TurnToFaceEachOther(gMainRoutineWork.leaderActorId, ACTOR_SERAC, 0);
  }
}

#undef DEFINE_ISLET_CAVE_ARRIVE_AT
#undef DEFINE_ISLET_CAVE_RIDE_LOG_ACROSS

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
    SC_SET(SW_SCALE_Y, FIXED_ONE * 13 / 10),
    SC_SET(SW_SCALE_X, FIXED_ONE * 13 / 10),
    SC_END,
};

s32 gIsletCave_ShrunkenSpriteScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_ADD(SW_Y, toFIXED(-4)),
    SC_SET(SW_SCALE_X, FIXED_ONE * 4 / 5),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 4 / 5),
    SC_CALL(IsletCave_ClearScriptedActorSpriteFlags),
    SC_END,
};

s32 gIsletCave_TremorTargetScript[] = {
    SC_CALL(IsletCave_HopWingedStatue),
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
    ACTOR_PLACEMENT(SPRITE_SENTINEL, FLAG_SENTINEL_DEFEATED, (s32)gIsletCave_SentinelScript, 184, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFirstLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFirstLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSecondLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSecondLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideThirdLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideThirdLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFourthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFourthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFifthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideFifthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSixthLogEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSixthLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSeventhLogNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_RideSeventhLogSouth)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_FIRST_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleFirstLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_SECOND_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleSecondLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_THIRD_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleThirdLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_FOURTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleFourthLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_FIFTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleFifthLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_SIXTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleSixthLog)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ISLET_CAVE_SEVENTH_LOG, CONDITION_ALWAYS, EVENT(IsletCave_SettleSeventhLog)},
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
    {TRIGGER_STEP, 10, FLAG_SENTINEL_DEFEATED, EVENT(IsletCave_SentinelAwaits)},
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
