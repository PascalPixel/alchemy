/// @file
/// Kolima Forest (SceneId_KolimaForestFloodgate), the fourth of the forest's
/// five maps, between SceneId_KolimaForestCliff and the map where Tret stands
/// (SceneId_KolimaForestTree). A Floodgate Switch
/// (MB_KOLIMA_FOREST_FLOODGATE_SWITCH), beside a sign that reads "Do not touch
/// Floodgate Switch!", raises and lowers the river: with the water high the
/// logs float (26 pixels up) and can be rolled along it; with it low they lie
/// on the riverbed. A short log slides between two places, the leader can jump
/// south across a gap (coord event 10), and a chest holds a Fur Coat. The
/// forest keeps its cursed colours until Tret lifts the curse
/// (FLAG_KOLIMA_CURSE_LIFTED).
///
/// The water level, the short log's place and each log's cells are kept in
/// gMapLocalState (gKolimaForestFloodgateMap, gKolimaForestFloodgateReturned
/// and gKolimaForestFloodgateRecords), so they survive a battle. Under each
/// log KolimaForestFloodgate_StampLogCells writes MB_JUMP_FORWARD while
/// the water is high, so the leader jumps across the floating logs, and
/// MB_WALL while it is low and after a roll, which
/// KolimaForestFloodgate_CheckLogFits also treats as taken.
///
/// Flipping the switch from high water opens the floodgate: the water bursts
/// through in spray (SPLASH_ANIMATION_BURST), the top lines of the screen shake
/// and the river falls; flipping it back closes the gate in churning spray
/// (SPLASH_ANIMATION_CHURN) and the river rises
/// (places/kolima-forest-floodgate/events/KolimaForestFloodgate_FlipFloodgateSwitch.gif).

#include "speaker.h"
#include "flags.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_floodgate.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "unit/field/field.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "map_grid_copy.h"
#include "types.h"
#include "script_layer.h"
#include "engine_call.h"
#include "mem.h"
#include "call.h"
#include "dma.h"
#include "map_grid.h"
#include "io_reg.h"
#include "irq.h"
#include "text/msg_ids.h"
#include "map_tables.h"
#include "map_grid_scroll.h"
#include "macros.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"
#include "heap.h"

/// The object events, numbering gKolimaForestFloodgateObjectEventTemplates'
/// rows from LOCALID_FIRST_PLACED.
enum {
  /// A short log that slides between x 72 and x 104.
  /// ★ Named from what the player sees: a log lying north to south (its
  /// render)
  LOCALID_SHORT_LOG = LOCALID_FIRST_PLACED,
  /// The water at the floodgate, which bursts as it opens and churns as it
  /// closes.
  /// ★ Named from what the player sees: white and pale blue spray,
  /// SPRITE_SPLASH (its render)
  LOCALID_FLOODGATE_SPRAY,
  /// ★ Named from the game: "Do not touch Floodgate Switch!"
  /// (MsgKolimaDoNotTouchFloodgate)
  LOCALID_SIGN
};

/// The metatile behavior of the Floodgate Switch's cell, which a search flips.
enum KolimaForestFloodgateMetatileBehavior {
  /// ★ Named from the game: "flipped the switch." (MsgFieldFlippedSwitch)
  MB_KOLIMA_FOREST_FLOODGATE_SWITCH = 9
};

static void KolimaForestFloodgate_MarkShortLogCells(void);
static void KolimaForestFloodgate_CreateLogs(struct FloodgateLog *log);
static void KolimaForestFloodgate_RollLog(struct FloodgateLog *logs);

/// Where the party enters, from SceneId_KolimaForestCliff and from
/// SceneId_KolimaForestTree.
const struct MapWarpDest *KolimaForestFloodgate_GetWarpDests(void) {
  return gKolimaForestFloodgateWarpDests;
}

const struct MapWarpRegion *KolimaForestFloodgate_GetRegions(void) { return 0; }

/// The ways back (SceneId_KolimaForestCliff) and on to Tret
/// (SceneId_KolimaForestTree).
const u32 *KolimaForestFloodgate_GetWarps(void) {
  return gKolimaForestFloodgateWarps;
}

/// The short log, the switch and the sign.
const struct ObjectEventTemplate *
KolimaForestFloodgate_GetObjectEventTemplates(void) {
  return gKolimaForestFloodgateObjectEventTemplates;
}

/// The logs, the switch, the sign, the jump and the chest.
const struct MapEvent *KolimaForestFloodgate_GetEvents(void) {
  return gKolimaForestFloodgateEvents;
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking into a log: it rolls
/// ahead, and the cells of the logs are marked again where they now lie.
static void KolimaForestFloodgate_PushLog(void) {
  KolimaForestFloodgate_RollLog(gKolimaForestFloodgateRecords);
  MapGrid_CopyCellAttributes(0, 0x40, 0x20, 0x20, 0, 0);
  KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords, MB_WALL);
  KolimaForestFloodgate_MarkShortLogCells();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking south into the gap
/// (coord event 10): the leader jumps across.
static void KolimaForestFloodgate_JumpSouth(void) { Field_TryJumpForward(); }

#include "common/map_grid_panel.h"

/// Gives the short log's column of cells its metatile behavior: column 4 at the
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
  struct ObjectEvent *piece;
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
    piece = record->objectEvent;

    if (*gKolimaForestFloodgateMap == 1) {
      Object_SelectAnimation(piece, 4);
      piece->priorityFlags =
          OBJ_EVENT_PRIORITY_AUTOMATIC | OBJ_EVENT_PRIORITY_UNDERFOOT;
      piece->motionFlags = 0;
      piece->y.fixed = PIXELS(26);

      if (record->vertical != 0) {
        MapGrid_CopyCells(68, 40, 1, 4, record->x + 32, record->z);
      } else {
        MapGrid_CopyCells(70, 40, 4, 1, record->x + 32, record->z);
      }
    } else {
      Object_SelectAnimation(piece, 1);
      piece->priorityFlags = OBJ_EVENT_PRIORITY_AUTOMATIC;
      piece->motionFlags = OBJ_EVENT_FALLS;
      piece->y.fixed = 0;
    }
    record++;
  }

  MapGrid_CopyCells(70, 42, 1, 1, 10, 50);

  if (*gKolimaForestFloodgateMap == 1) {
    MapGrid_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords,
                                        MB_JUMP_FORWARD);
  } else {
    MapGrid_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords, MB_WALL);
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
  scroll[1] += 0xb0;
  scroll[3] += 0xb0;
  scroll[5] += 0xb0;
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), searching the Floodgate
/// Switch flips it. "Isaac flipped the switch." The camera turns to the
/// floodgate, which opens or closes; the ground rumbles and the top of the
/// screen shakes harder for 100 frames, and the river rises or falls.
static void KolimaForestFloodgate_FlipFloodgateSwitch(void) {
  s32 nextShakeChance;

  FieldScript_Start();
  FieldCamera_SetSpeedParameters(PIXELS(1), PIXELS(1) / 8);
  FieldCamera_MoveWithinMapBounds(PIXELS(264), -1, PIXELS(448), 1);
  FieldCamera_WaitForArrival();
  FieldMessage_Show(MsgFieldFlippedSwitch, 1);
  Sound_PlaySong(SE_CRUNCH);
  if (*gKolimaForestFloodgateMap == 0) {
    ObjectEvent_SetPosition(LOCALID_FLOODGATE_SPRAY, PIXELS(256), PIXELS(462));
    MapGrid_CopyCells(77, 34, 1, 2, 83, 25);
    WaitFrames(3);
    MapGrid_CopyCells(78, 34, 1, 2, 83, 25);
    WaitFrames(3);
    MapGrid_CopyCells(79, 34, 1, 2, 83, 25);
    WaitFrames(30);
    MapGrid_CopyCells(67, 34, 2, 5, 79, 25);
    WaitFrames(6);
    MapGrid_CopyCells(69, 34, 2, 5, 79, 25);
    Import_ObjectEventSetAnimation(LOCALID_FLOODGATE_SPRAY,
                                   SPLASH_ANIMATION_BURST);
    Sound_PlaySong(SE_LOG_FALL);
    WaitFrames(6);
    MapGrid_CopyCells(71, 34, 2, 5, 79, 25);
    WaitFrames(6);
    MapGrid_CopyCells(73, 34, 2, 5, 79, 25);
    MapGrid_CopyCells(75, 38, 2, 1, 79, 29);
    WaitFrames(4);
    MapGrid_CopyCells(77, 38, 2, 1, 79, 29);
    WaitFrames(6);
    MapGrid_CopyCells(79, 38, 2, 1, 79, 29);
    WaitFrames(8);
    MapGrid_CopyCells(65, 53, 2, 1, 79, 29);
    MapGrid_CopyCells(65, 40, 2, 4, 15, 28);
  } else {
    ObjectEvent_SetPosition(LOCALID_FLOODGATE_SPRAY, PIXELS(256), PIXELS(480));
    MapGrid_CopyCells(78, 34, 1, 2, 83, 25);
    WaitFrames(3);
    MapGrid_CopyCells(77, 34, 1, 2, 83, 25);
    WaitFrames(3);
    MapGrid_CopyCells(76, 34, 1, 2, 83, 25);
    WaitFrames(30);
    MapGrid_CopyCells(65, 45, 2, 4, 15, 28);
    MapGrid_CopyCells(71, 50, 2, 5, 79, 25);
    Import_ObjectEventSetAnimation(LOCALID_FLOODGATE_SPRAY,
                                   SPLASH_ANIMATION_CHURN);
    Sound_PlaySong(SE_WATER_DRAIN);
    WaitFrames(6);
    MapGrid_CopyCells(69, 50, 2, 5, 79, 25);
    WaitFrames(6);
    MapGrid_CopyCells(67, 50, 2, 5, 79, 25);
    WaitFrames(6);
    MapGrid_CopyCells(65, 50, 2, 5, 79, 25);
    WaitFrames(30);
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
  FieldScheduler_AddCallback(KolimaForestFloodgate_CaptureShakeScroll,
                             TASK_PRIORITY_MAP);
  WaitFrames(1);
  Irq_SetHandler(IRQ_HBLANK, 0, KolimaForestFloodgate_ShakeLines);
  Sound_PlaySong(SE_WATER_LEVEL);
  gKolimaForestFloodgate_ShakeChance = 0;
  do {
    WaitFrames(1);
    nextShakeChance = (gKolimaForestFloodgate_ShakeChance + 1);
    gKolimaForestFloodgate_ShakeChance += 1;
  } while (nextShakeChance <= 100);
  Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  if (*gKolimaForestFloodgateMap == 0) {
    MapGrid_DrawCellPanel(9, 19, 16, 5, *gKolimaForestFloodgateMap, 9, 19);
    MapGrid_DrawCellPanel(9, 51, 16, 5, 1, 9, 19);
    MapGrid_DrawCellPanel(41, 51, 16, 5, 2, 9, 19);
  } else {
    MapGrid_DrawCellPanel(9, 19, 16, 5, 0, 9, 19);
    MapGrid_DrawCellPanel(9, 83, 16, 5, 1, 9, 19);
    MapGrid_DrawCellPanel(41, 83, 16, 5, 2, 9, 19);
  }
  WaitFrames(1);
  Irq_SetHandler(IRQ_HBLANK, 0, 0);
  WaitFrames(1);
  Task_Remove(KolimaForestFloodgate_CaptureShakeScroll);
  *(u16 *)gKolimaForestFloodgateMap ^= 1;
  KolimaForestFloodgate_DrawWaterLevel();
  MapGrid_CenterOnFollowedPoint();
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking west into the short
/// log: the leader shoves it west to x 72.
static void KolimaForestFloodgate_PushShortLogWest(void) {

  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_PUSH);
  FieldScript_WaitFrames(6);
  Sound_PlaySong(SE_HEAVY_MOVE);
  ObjectEvent_SetSpeed(LOCALID_SHORT_LOG, PIXELS(1) / 2, PIXELS(1) / 5);
  Import_ObjectEventSetAnimation(LOCALID_SHORT_LOG, 2);
  ObjectMotion_ResetAndSetPosition(LOCALID_SHORT_LOG, 72, 176);
  FieldScript_WaitFrames(6);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, -8, 0);
  FieldScript_WaitFrames(24);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SHORT_LOG);
  Import_ObjectEventSetAnimation(LOCALID_SHORT_LOG, 1);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  Sound_PlaySong(SE_HEAVY_STOP);
  MapGrid_CopyCellAttributes(5, 9, 1, 4, 6, 9);
  MapGrid_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  *gKolimaForestFloodgateReturned = 1;
  FieldScript_End();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking east into the short
/// log: the leader shoves it east to x 104.
static void KolimaForestFloodgate_PushShortLogEast(void) {

  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_PUSH);
  FieldScript_WaitFrames(6);
  Sound_PlaySong(SE_HEAVY_MOVE);
  ObjectEvent_SetSpeed(LOCALID_SHORT_LOG, PIXELS(1) / 2, PIXELS(1) / 5);
  Import_ObjectEventSetAnimation(LOCALID_SHORT_LOG, 2);
  ObjectMotion_ResetAndSetPosition(LOCALID_SHORT_LOG, 104, 176);
  FieldScript_WaitFrames(6);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  ObjectMotion_OffsetPositionAndResetMotion(LOCALID_PARTY_LEADER, 8, 0);
  FieldScript_WaitFrames(24);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  ObjectMotion_CommitCurrentPositionAndActivate(LOCALID_SHORT_LOG);
  Import_ObjectEventSetAnimation(LOCALID_SHORT_LOG, 1);
  Sound_PlaySong(SE_STOP_ACTION_SE);
  Sound_PlaySong(SE_HEAVY_STOP);
  MapGrid_CopyCellAttributes(5, 9, 1, 4, 4, 9);
  MapGrid_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  *gKolimaForestFloodgateReturned = 0;
  FieldScript_End();
}

/// Sets up the logs and the river's level on entry. The logs come back
/// from gMapLocalState, or on a first visit from the defaults with the water
/// high; the logs are made and the river is drawn at its level. The forest
/// keeps its cursed colours until the curse is lifted.
s32 KolimaForestFloodgate_Initialize(void) {
  struct ObjectEvent *objectEvent;

  gKolimaForestFloodgateReturned = (u16 *)gMapLocalState;
  gKolimaForestFloodgateMap = (s16 *)gMapLocalState + 1;
  // The return flag and water level occupy the first two halfwords.
  gKolimaForestFloodgateRecords =
      (struct FloodgateLog *)(gMapLocalState + 2 * sizeof(u16));
  MapGrid_CopyCells(32, 0, 64, 32, 0, 64);
  MapGrid_CopyCellAttributes(0, 0, 32, 32, 0, 64);
  MapGrid_CopyCellAttributes(32, 0, 32, 32, 0, 32);
  if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) == 0) {
    Dma_Set(gKolimaForestFloodgate_DefaultRecords,
            gKolimaForestFloodgateRecords,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x12, REG_DMA3);
    *gKolimaForestFloodgateReturned = 0;
    *gKolimaForestFloodgateMap = 1;
  }
  KolimaForestFloodgate_CreateLogs(gKolimaForestFloodgateRecords);
  KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgate_DefaultRecords,
                                      MB_WALL);
  KolimaForestFloodgate_DrawWaterLevel();
  Import_ObjectEventSetAnimation(LOCALID_FLOODGATE_SPRAY, 0);
  ObjectEvent_GetById(LOCALID_FLOODGATE_SPRAY)->motionFlags = 0;
  objectEvent = ObjectEvent_GetById(LOCALID_SIGN);
  objectEvent->radius = 8;
  objectEvent->scaleX = 0xc000;
  objectEvent->scaleY = 0xc000;
  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (GameFlag_Get(FLAG_KOLIMA_CURSE_LIFTED) == 0)
    KolimaPalette_Curse(4);
  return 0;
}

/// Makes each log at the middle of its four cells, standing on the ground.
static void KolimaForestFloodgate_CreateLogs(struct FloodgateLog *entry) {
  struct ObjectEvent *objectEvent;
  s32 height;
  s32 x;
  s32 z;

  for (; entry->sprite != -1; entry++) {
    if (entry->vertical == 0) {
      x = (entry->x << 20) + 0x200000;
      z = (entry->z << 20) + 0x80000;
    } else {
      x = (entry->x << 20) + 0x80000;
      z = (entry->z << 20) + 0x200000;
    }
    objectEvent = ObjectEvent_Create(entry->sprite, x, 0, z);
    if (objectEvent == NULL)
      return;
    entry->objectEvent = objectEvent;
    Object_SelectAnimation(objectEvent, 1);
    ObjectEvent_SetSpriteFlags(objectEvent, 0);
    objectEvent->collisionFlags = 0;
    objectEvent->radius = 32;
    height = MapGrid_GetTerrainHeight(0, objectEvent->x.part.pixel,
                                      objectEvent->z.part.pixel)
             << 16;
    // Through an s32 view of y, not y.fixed, the step to the next entry is
    // scheduled before this store (1 of 82 instructions differ in tbs-en).
    *(s32 *)&objectEvent->y += height;
    objectEvent->groundHeight = height;
  }
}

/// Writes a metatile behavior into the four cells of each log.
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

    if (cell->behavior == MB_WALL ||
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
  struct ObjectEvent *objectEvent;
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
  objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  facing = (objectEvent->facing + 0x2000) & 0xc000;
  SET_VECTOR(point, (objectEvent->x.fixed & 0xfff00000) + 0x80000,
             objectEvent->y.fixed,
             (objectEvent->z.fixed & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x100000, facing, point);
  run = KolimaForestFloodgate_FindLogAt(runs, point[0] / 0x100000,
                                        point[2] / 0x100000);
  if (run == 0)
    return;
  for (i = 0; i <= 10; i++) {
    point[0] = run->x << 20;
    point[2] = run->z << 20;
    Vector_AddPolarOffset(0x100000, facing, point);
    if (KolimaForestFloodgate_CheckLogFits(
            point[0] / 0x100000, point[2] / 0x100000, run->vertical) != 0)
      break;
    moved = 1;
    if (run->vertical == 0) {
      x = point[0] + 0x200000;
      z = point[2] + 0x80000;
    } else {
      x = point[0] + 0x80000;
      z = point[2] + 0x200000;
    }
    run->x = point[0] / 0x100000;
    run->z = point[2] / 0x100000;
  }
  if (moved == 0)
    return;
  SET_VECTOR(point, (objectEvent->x.fixed & 0xfff00000) + 0x80000,
             objectEvent->y.fixed,
             (objectEvent->z.fixed & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x80000, facing, point);
  objectEvent = run->objectEvent;
  quarter = facing / 0x4000;
  FieldScript_Start();
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_PUSH);
  FieldScript_WaitFrames(6);
  objectEvent->speed = PIXELS(1) / 2;
  objectEvent->acceleration = PIXELS(1) / 5;
  Script_PlaySong(SE_HEAVY_MOVE);
  Object_SelectAnimation(objectEvent,
                         gKolimaForestFloodgate_PushAnimations[quarter]);
  Object_SetPosition(objectEvent, x, 0, z);
  FieldScript_WaitFrames(6);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_WALK);
  work = AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(*work));
  Object_StartCamera((struct Object *)work->camera, (s32)objectEvent);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  ObjectEvent_SetDestinationOffset(LOCALID_PARTY_LEADER,
                                   gKolimaForestFloodgate_PushStepX[quarter],
                                   gKolimaForestFloodgate_PushStepZ[quarter]);
  FieldScript_WaitFrames(24);
  Import_ObjectEventSetAnimation(LOCALID_PARTY_LEADER, ANIM_STAND);
  Object_WaitForMove(objectEvent);
  Object_SelectAnimation(objectEvent, 1);
  Script_PlaySong(SE_STOP_ACTION_SE);
  Script_PlaySong(SE_HEAVY_STOP);
  FieldScript_WaitFrames(15);
  FieldScript_End();
}

#include "common/kolima_palette.h"

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

struct MapWarpDest gKolimaForestFloodgateWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 407, 0, 276, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_CLIFF, CONDITION_ALWAYS, 50, 32, 369, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { KOLIMA_FOREST_FLOODGATE_FROM_KOLIMA_FOREST_TREE, CONDITION_ALWAYS, 162, 32, 84, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gKolimaForestFloodgateWarps[] = {
    MAP_WARPS(KolimaForestFloodgate),
    MAP_WARP(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF, KolimaForestCliff, KOLIMA_FOREST_CLIFF_FROM_KOLIMA_FOREST_FLOODGATE),
    MAP_WARP(KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE, KolimaForestTree, KOLIMA_FOREST_TREE_FROM_KOLIMA_FOREST_FLOODGATE),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gKolimaForestFloodgateObjectEventTemplates[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(104), 0, PIXELS(176), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SPLASH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION,
        PIXELS(256), 0, PIXELS(464), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(348), 0, PIXELS(456), FACING_SOUTH, TALK_FACING_KEEP, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gKolimaForestFloodgateEvents[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_JumpSouth)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_PushLog)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_PushShortLogWest)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_PushShortLogEast)},
    {MAP_EVENT_TALK, LOCALID_SIGN, CONDITION_ALWAYS,
     MSGNO(MsgKolimaDoNotTouchFloodgate)},
    {BG_EVENT(BG_EVENT_SILENT), MB_KOLIMA_FOREST_FLOODGATE_SWITCH,
     CONDITION_ALWAYS, EVENT_SCRIPT(KolimaForestFloodgate_FlipFloodgateSwitch)},
    {BG_EVENT(BG_EVENT_CHEST), 100, 0xf00, FIND_ITEM(ITEM_FUR_COAT)},
    {MAP_EVENTS_END},
};
