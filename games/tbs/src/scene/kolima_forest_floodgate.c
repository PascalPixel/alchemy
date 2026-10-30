/// @file
/// Kolima Forest (SceneId_KolimaForestFloodgate), the fourth of the forest's
/// five scenes, between SceneId_KolimaForestCliff and the scene where Tret
/// stands (SceneId_KolimaForestTree). A Floodgate Switch
/// (CELL_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH), beside a sign that reads "Do
/// not touch Floodgate Switch!", raises and lowers the river: with the water
/// high the logs float (26 pixels up) and can be rolled along it; with it low
/// they lie on the riverbed. A short log slides between two places, the leader
/// can jump south across a gap (step trigger 10), and a chest holds a Fur Coat.
/// The forest keeps its cursed colours until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED).
///
/// The water level, the short log's place and each log's cells are kept in
/// gSceneLocalState (gKolimaForestFloodgateMap, gKolimaForestFloodgateReturned
/// and gKolimaForestFloodgateRecords), so they survive a battle. Under each
/// log KolimaForestFloodgate_StampLogCells writes CELL_BEHAVIOR_JUMP_FORWARD
/// while the water is high, so the leader jumps across the floating logs, and
/// CELL_BEHAVIOR_WALL while it is low and after a roll, which
/// KolimaForestFloodgate_CheckLogFits also treats as taken.
///
/// Flipping the switch from high water opens the floodgate: the water bursts
/// through in spray (SPLASH_ANIMATION_BURST), the top lines of the screen shake
/// and the river falls; flipping it back closes the gate in churning spray
/// (SPLASH_ANIMATION_CHURN) and the river rises
/// (places/kolima-forest-floodgate/events/KolimaForestFloodgate_FlipFloodgateSwitch.gif).

#include "speaker.h"
#include "flags.h"
#include "module/import.h"
#include "module/scene/kolima_forest_floodgate.h"
#include "module/scene/shared/kolima_palette.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "event.h"
#include "system_call.h"
#include "heap_release.h"
#include "dma.h"
#include "map_grid.h"
#include "io_reg.h"
#include "irq.h"
#include "text/msg_ids.h"
#include "scene_tables.h"
#include "map_grid_scroll.h"
#include "macros.h"
#include "sounds.h"
#include "sound_driver_symbols.h"
#include "item_ids.h"
#include "heap.h"

/// The actors, numbering gKolimaForestFloodgateActorPlacements'
/// rows from ACTOR_FIRST_PLACED.
enum {
  /// A short log that slides between x 72 and x 104.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  ACTOR_SHORT_LOG = ACTOR_FIRST_PLACED,
  /// The water at the floodgate, which bursts as it opens and churns as it
  /// closes.
  /// ★ Named from what the player sees: white and pale blue spray,
  /// SPRITE_SPLASH (its render)
  ACTOR_FLOODGATE_SPRAY,
  /// ★ Named from the game: "Do not touch Floodgate Switch!"
  /// (MsgKolimaDoNotTouchFloodgate)
  ACTOR_SIGN
};

/// The cell behavior of the Floodgate Switch's cell, which a search flips.
enum KolimaForestFloodgateCellBehavior {
  /// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
  CELL_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH = 9
};

static void KolimaForestFloodgate_MarkShortLogCells(void);
static void KolimaForestFloodgate_CreateLogs(struct FloodgateLog *log);
static void KolimaForestFloodgate_RollLog(struct FloodgateLog *logs);

/// Where the party enters, from SceneId_KolimaForestCliff and from
/// SceneId_KolimaForestTree.
const struct SceneEntrance *KolimaForestFloodgate_GetEntrances(void) {
  return gKolimaForestFloodgateEntrances;
}

const struct SceneExitRegion *KolimaForestFloodgate_GetRegions(void) {
  return 0;
}

/// The ways back (SceneId_KolimaForestCliff) and on to Tret
/// (SceneId_KolimaForestTree).
const u32 *KolimaForestFloodgate_GetExits(void) {
  return gKolimaForestFloodgateExits;
}

/// The short log, the switch and the sign.
const struct ActorPlacement *KolimaForestFloodgate_GetActorPlacements(void) {
  return gKolimaForestFloodgateActorPlacements;
}

/// The logs, the switch, the sign, the jump and the chest.
const struct Trigger *KolimaForestFloodgate_GetTriggers(void) {
  return gKolimaForestFloodgateTriggers;
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking into a log: it rolls
/// ahead, and the cells of the logs are marked again where they now lie.
static void KolimaForestFloodgate_PushLog(void) {
  KolimaForestFloodgate_RollLog(gKolimaForestFloodgateRecords);
  MapGrid_CopyCellAttributes(0, 64, 32, 32, 0, 0);
  KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords,
                                      CELL_BEHAVIOR_WALL);
  KolimaForestFloodgate_MarkShortLogCells();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking south into the gap
/// (step trigger 10): the leader jumps across.
static void KolimaForestFloodgate_JumpSouth(void) { Field_TryJumpForward(); }

#include "shared/map_grid_panel.h"

/// Gives the short log's column of cells its cell behavior: column 4 at the
/// west place, column 6 at the east.
static void KolimaForestFloodgate_MarkShortLogCells(void) {
  if (*(s16 *)gKolimaForestFloodgateReturned == 1) {
    MapGrid_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  } else {
    MapGrid_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  }
}

/// Draws the river at its level and sets each log to match: high water floats
/// the logs 26 pixels up over their own cells, low water lays them on the
/// riverbed.
static void KolimaForestFloodgate_DrawWaterLevel(void) {
  struct Actor *piece;
  struct FloodgateLog *record = gKolimaForestFloodgateRecords;

  if (*gKolimaForestFloodgateMap != 0) {
    MapGrid_CopyCells(65, 53, 2, 1, 79, 29);
    MapGrid_CopyCells(65, 40, 2, 4, 15, 28);
  } else {
    MapGrid_CopyCells(65, 50, 2, 5, 79, 25);
  }

  if (*gKolimaForestFloodgateMap != 0) {
    MapGrid_CopyCells(0, 32, 32, 32, 32, 0);
    MapGrid_CopyCells(32, 32, 32, 32, 64, 0);
    MapGrid_CopyCellAttributes(0, 32, 32, 32, 0, 0);
  } else {
    MapGrid_CopyCells(0, 64, 32, 32, 32, 0);
    MapGrid_CopyCells(32, 64, 32, 32, 64, 0);
    MapGrid_CopyCellAttributes(0, 64, 32, 32, 0, 0);
  }

  // Each log floats or rests, and on high water its four cells are drawn.
  while (record->sprite != -1) {
    piece = record->actor;

    if (*gKolimaForestFloodgateMap == 1) {
      Actor_SelectAnimation(piece, 4);
      piece->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
      piece->motionFlags = 0;
      piece->y.fixed = PIXELS(26);

      if (record->vertical != 0) {
        MapGrid_CopyCells(68, 40, 1, 4, record->x + 32, record->z);
      } else {
        MapGrid_CopyCells(70, 40, 4, 1, record->x + 32, record->z);
      }
    } else {
      Actor_SelectAnimation(piece, 1);
      piece->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
      piece->motionFlags = ACTOR_FALLS;
      piece->y.fixed = 0;
    }
    record++;
  }

  MapGrid_CopyCells(70, 42, 1, 1, 10, 50);

  if (*gKolimaForestFloodgateMap == 1) {
    MapGrid_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords,
                                        CELL_BEHAVIOR_JUMP_FORWARD);
  } else {
    MapGrid_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords,
                                        CELL_BEHAVIOR_WALL);
  }

  KolimaForestFloodgate_MarkShortLogCells();
}

/// HBlank callback: selects the captured shake scroll above the split line.
static void KolimaForestFloodgate_ShakeLines(void) {
  u16 line;
  u32 *source;
  volatile u32 *destination;

  line = REG_VCOUNT;
  source = (u32 *)((u8 *)gBgScroll + 4);
  destination = (volatile u32 *)&REG_BG1HOFS;

  if (line == 227 || line <= 52) {
    if ((((u32)Import_RandomNext() * 100) >> 16) <
        gKolimaForestFloodgate_ShakeChance) {
      source = gKolimaForestFloodgate_ShakeScroll;
    }
  }

  *destination = *source++;
  destination = (volatile u32 *)&REG_BG2HOFS;
  *destination++ = *source++;
  *destination = *source;
}

/// Captures three background scroll pairs and adds the vertical shake offset.
static void KolimaForestFloodgate_CaptureShakeScroll(void) {
  u32 *destination;
  const u32 *source;
  u16 *scroll;

  source = (const u32 *)((u8 *)gBgScroll + 4);
  destination = gKolimaForestFloodgate_ShakeScroll;
  *destination++ = *source++;
  *destination++ = *source++;
  *destination = *source;
  scroll = (u16 *)gKolimaForestFloodgate_ShakeScroll;
  scroll[1] += 176;
  scroll[3] += 176;
  scroll[5] += 176;
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), searching the Floodgate
/// Switch flips it. "Isaac flipped the switch." The camera turns to the
/// floodgate, which opens or closes; the ground rumbles and the top of the
/// screen shakes harder for 100 frames, and the river rises or falls.
static void KolimaForestFloodgate_FlipFloodgateSwitch(void) {
  s32 nextShakeChance;

  Event_Start();
  FieldCamera_SetSpeedParameters(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(448), 1);
  FieldCamera_WaitForArrival();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_Play(SE_CRUNCH);
  if (*gKolimaForestFloodgateMap == 0) {
    Actor_SetPosition(ACTOR_FLOODGATE_SPRAY, PIXELS(256), PIXELS(462));
    MapGrid_CopyCells(77, 34, 1, 2, 83, 25);
    wait(3);
    MapGrid_CopyCells(78, 34, 1, 2, 83, 25);
    wait(3);
    MapGrid_CopyCells(79, 34, 1, 2, 83, 25);
    wait(30);
    MapGrid_CopyCells(67, 34, 2, 5, 79, 25);
    wait(6);
    MapGrid_CopyCells(69, 34, 2, 5, 79, 25);
    Import_ActorSetAnimation(ACTOR_FLOODGATE_SPRAY, SPLASH_ANIMATION_BURST);
    Sound_Play(SE_LOG_FALL);
    wait(6);
    MapGrid_CopyCells(71, 34, 2, 5, 79, 25);
    wait(6);
    MapGrid_CopyCells(73, 34, 2, 5, 79, 25);
    MapGrid_CopyCells(75, 38, 2, 1, 79, 29);
    wait(4);
    MapGrid_CopyCells(77, 38, 2, 1, 79, 29);
    wait(6);
    MapGrid_CopyCells(79, 38, 2, 1, 79, 29);
    wait(8);
    MapGrid_CopyCells(65, 53, 2, 1, 79, 29);
    MapGrid_CopyCells(65, 40, 2, 4, 15, 28);
  } else {
    Actor_SetPosition(ACTOR_FLOODGATE_SPRAY, PIXELS(256), PIXELS(480));
    MapGrid_CopyCells(78, 34, 1, 2, 83, 25);
    wait(3);
    MapGrid_CopyCells(77, 34, 1, 2, 83, 25);
    wait(3);
    MapGrid_CopyCells(76, 34, 1, 2, 83, 25);
    wait(30);
    MapGrid_CopyCells(65, 45, 2, 4, 15, 28);
    MapGrid_CopyCells(71, 50, 2, 5, 79, 25);
    Import_ActorSetAnimation(ACTOR_FLOODGATE_SPRAY, SPLASH_ANIMATION_CHURN);
    Sound_Play(SE_WATER_DRAIN);
    wait(6);
    MapGrid_CopyCells(69, 50, 2, 5, 79, 25);
    wait(6);
    MapGrid_CopyCells(67, 50, 2, 5, 79, 25);
    wait(6);
    MapGrid_CopyCells(65, 50, 2, 5, 79, 25);
    wait(30);
  }
  if (*gKolimaForestFloodgateMap == 0) {
    MapGrid_DrawCellPanel(9, 19, 16, 5, *gKolimaForestFloodgateMap, 9, 30);
    MapGrid_DrawCellPanel(9, 51, 16, 5, 1, 9, 30);
    MapGrid_DrawCellPanel(41, 51, 16, 5, 2, 9, 30);
  } else {
    MapGrid_DrawCellPanel(9, 19, 16, 5, 0, 9, 30);
    MapGrid_DrawCellPanel(9, 83, 16, 5, 1, 9, 30);
    MapGrid_DrawCellPanel(41, 83, 16, 5, 2, 9, 30);
  }

  // The shake: each frame captures the scroll, and the HBlank handler shows
  // the upper lines shifted, more often as the chance climbs to 100%.
  gKolimaForestFloodgate_ShakeChance = 0;
  Task_AddFrame(KolimaForestFloodgate_CaptureShakeScroll, TASK_PASS_FRAME);
  wait(1);
  Irq_SetHandler(IRQ_HBLANK, 0, KolimaForestFloodgate_ShakeLines);
  Sound_Play(SE_WATER_LEVEL);
  gKolimaForestFloodgate_ShakeChance = 0;
  do {
    wait(1);
    nextShakeChance = (gKolimaForestFloodgate_ShakeChance + 1);
    gKolimaForestFloodgate_ShakeChance += 1;
  } while (nextShakeChance <= 100);
  Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  if (*gKolimaForestFloodgateMap == 0) {
    MapGrid_DrawCellPanel(9, 19, 16, 5, *gKolimaForestFloodgateMap, 9, 19);
    MapGrid_DrawCellPanel(9, 51, 16, 5, 1, 9, 19);
    MapGrid_DrawCellPanel(41, 51, 16, 5, 2, 9, 19);
  } else {
    MapGrid_DrawCellPanel(9, 19, 16, 5, 0, 9, 19);
    MapGrid_DrawCellPanel(9, 83, 16, 5, 1, 9, 19);
    MapGrid_DrawCellPanel(41, 83, 16, 5, 2, 9, 19);
  }
  wait(1);
  Irq_SetHandler(IRQ_HBLANK, 0, 0);
  wait(1);
  Task_Remove(KolimaForestFloodgate_CaptureShakeScroll);
  *(u16 *)gKolimaForestFloodgateMap ^= 1;
  KolimaForestFloodgate_DrawWaterLevel();
  MapGrid_CenterOnFollowedPoint();
  Event_End();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking west into the short
/// log: the leader shoves it west to x 72.
static void KolimaForestFloodgate_PushShortLogWest(void) {

  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(6);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SetSpeed(ACTOR_SHORT_LOG, PIXELS(1) / 2, PIXELS(1) / 5);
  Import_ActorSetAnimation(ACTOR_SHORT_LOG, 2);
  Actor_ResetAndSetPosition(ACTOR_SHORT_LOG, 72, 176);
  Event_Wait(6);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, -8, 0);
  Event_Wait(24);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_SHORT_LOG);
  Import_ActorSetAnimation(ACTOR_SHORT_LOG, 1);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  MapGrid_CopyCellAttributes(5, 9, 1, 4, 6, 9);
  MapGrid_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  *gKolimaForestFloodgateReturned = 1;
  Event_End();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking east into the short
/// log: the leader shoves it east to x 104.
static void KolimaForestFloodgate_PushShortLogEast(void) {

  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(6);
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SetSpeed(ACTOR_SHORT_LOG, PIXELS(1) / 2, PIXELS(1) / 5);
  Import_ActorSetAnimation(ACTOR_SHORT_LOG, 2);
  Actor_ResetAndSetPosition(ACTOR_SHORT_LOG, 104, 176);
  Event_Wait(6);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  Actor_OffsetPositionAndResetMotion(ACTOR_PARTY_LEADER, 8, 0);
  Event_Wait(24);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_WaitForMoveById(ACTOR_SHORT_LOG);
  Import_ActorSetAnimation(ACTOR_SHORT_LOG, 1);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  MapGrid_CopyCellAttributes(5, 9, 1, 4, 4, 9);
  MapGrid_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  *gKolimaForestFloodgateReturned = 0;
  Event_End();
}

/// Sets up the logs and the river's level on entry. The logs come back
/// from gSceneLocalState, or on a first visit from the defaults with the water
/// high; the logs are made and the river is drawn at its level. The forest
/// keeps its cursed colours until the curse is lifted.
s32 KolimaForestFloodgate_Initialize(void) {
  struct Actor *actor;

  gKolimaForestFloodgateReturned = (u16 *)gSceneLocalState;
  gKolimaForestFloodgateMap = (s16 *)gSceneLocalState + 1;
  // The return flag and water level occupy the first two halfwords.
  gKolimaForestFloodgateRecords =
      (struct FloodgateLog *)(gSceneLocalState + 2 * sizeof(u16));
  MapGrid_CopyCells(32, 0, 64, 32, 0, 64);
  MapGrid_CopyCellAttributes(0, 0, 32, 32, 0, 64);
  MapGrid_CopyCellAttributes(32, 0, 32, 32, 0, 32);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Dma_Set(gKolimaForestFloodgate_DefaultRecords,
            gKolimaForestFloodgateRecords, (DMA_ENABLE | DMA_32BIT) << 16 | 18,
            REG_DMA3);
    *gKolimaForestFloodgateReturned = 0;
    *gKolimaForestFloodgateMap = 1;
  }
  KolimaForestFloodgate_CreateLogs(gKolimaForestFloodgateRecords);
  KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgate_DefaultRecords,
                                      CELL_BEHAVIOR_WALL);
  KolimaForestFloodgate_DrawWaterLevel();
  Import_ActorSetAnimation(ACTOR_FLOODGATE_SPRAY, 0);
  Actor_GetById(ACTOR_FLOODGATE_SPRAY)->motionFlags = 0;
  actor = Actor_GetById(ACTOR_SIGN);
  actor->radius = 8;
  actor->scaleX = 3 * Q16_ONE / 4;
  actor->scaleY = 3 * Q16_ONE / 4;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0)
    KolimaPalette_Curse(4);
  return 0;
}

/// Makes each log at the middle of its four cells, standing on the ground.
static void KolimaForestFloodgate_CreateLogs(struct FloodgateLog *entry) {
  struct Actor *actor;
  s32 height;
  s32 x;
  s32 z;

  for (; entry->sprite != -1; entry++) {
    if (entry->vertical == 0) {
      x = (entry->x << 20) + PIXELS(32);
      z = (entry->z << 20) + PIXELS(8);
    } else {
      x = (entry->x << 20) + PIXELS(8);
      z = (entry->z << 20) + PIXELS(32);
    }
    actor = FieldActor_Create(entry->sprite, x, 0, z);
    if (actor == NULL)
      return;
    entry->actor = actor;
    Actor_SelectAnimation(actor, 1);
    Actor_SetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->radius = 32;
    height =
        MapGrid_GetTerrainHeight(0, actor->x.part.pixel, actor->z.part.pixel)
        << 16;
    // Through an s32 view of y, not y.fixed, the step to the next entry is
    // scheduled before this store (1 of 82 instructions differ in tbs-en).
    *(s32 *)&actor->y += height;
    actor->groundHeight = height;
  }
}

/// Writes a cell behavior into the four cells of each log.
void KolimaForestFloodgate_StampLogCells(const struct FloodgateLog *records,
                                         s32 value) {

  const struct FloodgateLog *record = records;
  if (record->sprite == -1)
    return;
  do {
    s32 column = record->x;
    s32 row = record->z;
    s32 along = record->vertical;
    s32 i;
    for (i = 3; i >= 0; i--) {
      u8 *cell;
      cell = ((u8 *)gMapGridCellBuffer) + ((column + (row << 7)) << 2);
      cell[2] = value;
      if (along == 0)
        column++;
      else
        row++;
    }
    record++;
  } while (record->sprite != -1);
}

/// The log lying across the cell, or none.
static struct FloodgateLog *
KolimaForestFloodgate_FindLogAt(struct FloodgateLog *run, s32 x, s32 y) {
  while (run->sprite != -1) {
    s32 x0 = run->x;
    s32 x1 = x0;
    s32 y0 = run->z;
    s32 y1 = y0;

    if (run->vertical == 0)
      x1 += 3;
    else
      y1 += 3;

    if (x >= x0 && x <= x1 && y >= y0 && y <= y1)
      return run;
    run++;
  }
  return 0;
}

/// Whether four cells from the one given, across or down, are free for a log:
/// none is marked 0xff or holds a wall.
static s32 KolimaForestFloodgate_CheckLogFits(s32 x, s32 z, s32 mode) {
  s32 i;

  for (i = 0; i <= 3; i++) {
    struct MapGridCell *cell =
        &((struct MapGridCell *)gMapGridCellBuffer)[x + (z << 7)];

    if (cell->behavior == CELL_BEHAVIOR_WALL ||
        *(u8 *)((cell->shape << 2) + (s32)gMapGridShapes) != 0) {
      return -1;
    }
    if (mode == 0) {
      x++;
    } else {
      z++;
    }
  }
  return 0;
}

/// Finds the log ahead of the leader and rolls it on, a cell at a time, up to
/// eleven cells or until a cell is not free; the leader follows its shove.
static void KolimaForestFloodgate_RollLog(struct FloodgateLog *runs) {
  struct Actor *actor;
  struct FieldWork *work;
  struct FloodgateLog *run;
  s32 point[3];
  s32 facing;
  s32 moved;
  s32 i;
  s32 x;
  s32 z;
  s32 quarter;

  moved = 0;
  actor = Actor_GetById(ACTOR_PARTY_LEADER);
  facing = (actor->facing + TRIG_TURN / 8) & 0xc000;
  SET_VECTOR(point, (actor->x.fixed & 0xfff00000) + PIXELS(8), actor->y.fixed,
             (actor->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(16), facing, point);
  run = KolimaForestFloodgate_FindLogAt(runs, point[0] / PIXELS(16),
                                        point[2] / PIXELS(16));
  if (run == 0)
    return;
  for (i = 0; i <= 10; i++) {
    point[0] = run->x << 20;
    point[2] = run->z << 20;
    Vector_AddPolarOffset(PIXELS(16), facing, point);
    if (KolimaForestFloodgate_CheckLogFits(
            point[0] / PIXELS(16), point[2] / PIXELS(16), run->vertical) != 0)
      break;
    moved = 1;
    if (run->vertical == 0) {
      x = point[0] + PIXELS(32);
      z = point[2] + PIXELS(8);
    } else {
      x = point[0] + PIXELS(8);
      z = point[2] + PIXELS(32);
    }
    run->x = point[0] / PIXELS(16);
    run->z = point[2] / PIXELS(16);
  }
  if (moved == 0)
    return;
  SET_VECTOR(point, (actor->x.fixed & 0xfff00000) + PIXELS(8), actor->y.fixed,
             (actor->z.fixed & 0xfff00000) + PIXELS(8));
  Vector_AddPolarOffset(PIXELS(8), facing, point);
  actor = run->actor;
  quarter = facing / (TRIG_TURN / 4);
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Event_Wait(6);
  actor->speedLimit = PIXELS(1) / 2;
  actor->acceleration = PIXELS(1) / 5;
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SelectAnimation(actor, gKolimaForestFloodgate_PushAnimations[quarter]);
  FieldActor_SetMoveTarget(actor, x, 0, z);
  Event_Wait(6);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  work = HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(*work));
  Actor_StartCamera(work->camera, (s32)actor);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER,
                             gKolimaForestFloodgate_PushStepX[quarter],
                             gKolimaForestFloodgate_PushStepZ[quarter]);
  Event_Wait(24);
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Actor_WaitForMove(actor);
  Actor_SelectAnimation(actor, 1);
  Event_PlaySound(SE_STOP_ACTION_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  Event_Wait(15);
  Event_End();
}

#include "shared/kolima_palette.h"

/// The logs on a first visit, as struct FloodgateLog, each in the sprite of the
/// way it runs: one sprite for north to south, another for west to east.
// clang-format off
struct FloodgateLog gKolimaForestFloodgate_DefaultRecords[] = {
    {SPRITE_LOG_NORTH_SOUTH, 11, 9, 1, NULL},
    {SPRITE_LOG_WEST_EAST, 12, 12, 0, NULL},
    {SPRITE_LOG_WEST_EAST, 17, 12, 0, NULL},
    {SPRITE_LOG_NORTH_SOUTH, 13, 13, 1, NULL},
    {SPRITE_LOG_WEST_EAST, 10, 18, 0, NULL},
    {-1},
};

u8 gKolimaForestFloodgate_PushAnimations[] = {2, 2, 3, 3};

s8 gKolimaForestFloodgate_PushStepX[] = {8, 0, -8, 0};

s8 gKolimaForestFloodgate_PushStepZ[] = {0, 8, 0, -8};

struct SceneEntrance gKolimaForestFloodgateEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 407, 0, 276, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 50, 32, 369, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 162, 32, 84, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gKolimaForestFloodgateExits[] = {
    SCENE_EXITS(KolimaForestFloodgate),
    SCENE_EXIT(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE),
    SCENE_EXIT(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE),
    SCENE_EXITS_END,
};

struct ActorPlacement gKolimaForestFloodgateActorPlacements[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(104), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(256), 0, PIXELS(464), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(348), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gKolimaForestFloodgateTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT(KolimaForestFloodgate_JumpSouth)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 11, CONDITION_ALWAYS,
     EVENT(KolimaForestFloodgate_PushLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS,
     EVENT(KolimaForestFloodgate_PushShortLogWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
     EVENT(KolimaForestFloodgate_PushShortLogEast)},
    {TRIGGER_TALK, ACTOR_SIGN, CONDITION_ALWAYS,
     MSGNO(MsgKolimaDoNotTouchFloodgate)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT),
     CELL_BEHAVIOR_KOLIMA_FOREST_FLOODGATE_SWITCH, CONDITION_ALWAYS,
     EVENT(KolimaForestFloodgate_FlipFloodgateSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_KOLIMA_FOREST_FUR_COAT,
     FIND_ITEM(ITEM_FUR_COAT)},
    {TRIGGERS_END},
};
