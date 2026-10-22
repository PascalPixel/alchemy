/// @file
/// Kolima Forest (SceneId_KolimaForestFloodgate), the fourth of the forest's
/// five maps, between SceneId_KolimaForestCliff and the map where Tret stands
/// (SceneId_KolimaForestTree). A Floodgate Switch (CELL_FLOODGATE_SWITCH),
/// beside a sign that reads "Do not touch Floodgate Switch!", raises and lowers
/// the river: with the water high the logs float (26 pixels up) and can be
/// rolled along it; with it low they lie on the riverbed. A short log slides
/// between two places, the leader can jump south across a gap (trigger 10), and
/// a chest holds a Fur Coat. The forest keeps its cursed colours until Tret
/// lifts the curse (FLAG_KOLIMA_CURSE_LIFTED).
///
/// The water level, the short log's place and each log's cells are kept in
/// gSceneState (gKolimaForestFloodgateLayout, gKolimaForestFloodgateReturned
/// and gKolimaForestFloodgateRecords), so they survive a battle.
///
/// Open questions:
/// - XXX: what the collision codes KolimaForestFloodgate_StampLogCells writes
///   under the logs mean: 254 at high water, 255 at low and after a roll, which
///   KolimaForestFloodgate_CheckLogFits treats as taken; the field's collision
///   table (field/) would settle it.
/// - XXX: whether ACTOR_FLOODGATE_SPRAY's animations 1 and 2 are the water
///   bursting through and settling, as the sprite's frames suggest; a render of
///   KolimaForestFloodgate_FlipFloodgateSwitch would settle it.
/// - XXX: what SE_232, SE_240, SE_230, SE_231, SE_289, SE_239 and SE_213 sound
///   like; a recording of the switch and a log's roll would name them.
/// - XXX (picture): the river at both levels side by side, with the cells
///   KolimaForestFloodgate_DrawWaterLevel copies and the top lines
///   KolimaForestFloodgate_ShakeLines shifts.

#include "battle_effect_runtime.h"
#include "flags.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_floodgate.h"
#include "unit/scene/common/kolima_palette.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "engine_call.h"
#include "scene.h"
#include "mem.h"
#include "call.h"
#include "dma.h"
#include "map.h"
#include "io_reg.h"
#include "irq.h"
#include "text/msg_ids.h"
#include "field_scene.h"
#include "map_scroll.h"
#include "macros.h"
#include "songs.h"
#include "item_ids.h"
#include "heap_state.h"

/// The actors, numbering gKolimaForestFloodgatePlacements' rows from
/// ACTOR_FIRST_PLACED.
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

/// The collision code of the Floodgate Switch's cell, which a search flips.
enum { CELL_FLOODGATE_SWITCH = 9 };

static void KolimaForestFloodgate_MarkShortLogCells(void);
static void KolimaForestFloodgate_CreateLogs(struct FloodgateLog *log);
static void KolimaForestFloodgate_RollLog(struct FloodgateLog *logs);

/// Where the party enters, from SceneId_KolimaForestCliff and from
/// SceneId_KolimaForestTree.
const struct SceneEntrance *KolimaForestFloodgate_GetEntrances(void) {
  return gKolimaForestFloodgateEntrances;
}

const struct SceneRegion *KolimaForestFloodgate_GetRegions(void) { return 0; }

/// The ways back (SceneId_KolimaForestCliff) and on to Tret
/// (SceneId_KolimaForestTree).
const u32 *KolimaForestFloodgate_GetExits(void) {
  return gKolimaForestFloodgateExits;
}

/// The short log, the switch and the sign.
const struct ScenePlacement *KolimaForestFloodgate_GetPlacements(void) {
  return gKolimaForestFloodgatePlacements;
}

/// The logs, the switch, the sign, the jump and the chest.
const struct SceneEvent *KolimaForestFloodgate_GetEvents(void) {
  return gKolimaForestFloodgateEvents;
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking into a log: it rolls
/// ahead, and the cells of the logs are marked again where they now lie.
static void KolimaForestFloodgate_PushLog(void) {
  KolimaForestFloodgate_RollLog(gKolimaForestFloodgateRecords);
  Map_CopyCellAttributes(0, 0x40, 0x20, 0x20, 0, 0);
  KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords, 0xff);
  KolimaForestFloodgate_MarkShortLogCells();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking south into the gap
/// (trigger 10): the leader jumps across.
static void KolimaForestFloodgate_JumpSouth(void) { Field_TryJumpForward(); }

#include "common/map_panel.h"

/// Gives the short log's column of cells its collision: column 4 at the
/// west place, column 6 at the east.
static void KolimaForestFloodgate_MarkShortLogCells(void) {
  if (*(s16 *)gKolimaForestFloodgateReturned == 1) {
    Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  } else {
    Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  }
}

/// Draws the river at its level and sets each log to match: high water floats
/// the logs 26 pixels up over their own cells, low water lays them on the
/// riverbed.
static void KolimaForestFloodgate_DrawWaterLevel(void) {
  struct FieldActor *piece;
  struct FloodgateLog *record = gKolimaForestFloodgateRecords;

  if (*gKolimaForestFloodgateLayout != 0) {
    Map_CopyCells(65, 53, 2, 1, 79, 29);
    Map_CopyCells(65, 40, 2, 4, 15, 28);
  } else {
    Map_CopyCells(65, 50, 2, 5, 79, 25);
  }

  if (*gKolimaForestFloodgateLayout != 0) {
    Map_CopyCells(0, 32, 32, 32, 32, 0);
    Map_CopyCells(32, 32, 32, 32, 64, 0);
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
  } else {
    Map_CopyCells(0, 64, 32, 32, 32, 0);
    Map_CopyCells(32, 64, 32, 32, 64, 0);
    Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
  }

  // Each log floats or rests, and on high water its four cells are drawn.
  while (record->sprite != -1) {
    piece = record->actor;

    if (*gKolimaForestFloodgateLayout == 1) {
      Object_SetMode(piece, 4);
      piece->priorityFlags =
          ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
      piece->motionFlags = 0;
      piece->y.fixed = PIXELS(26);

      if (record->vertical != 0) {
        Map_CopyCells(68, 40, 1, 4, record->x + 32, record->z);
      } else {
        Map_CopyCells(70, 40, 4, 1, record->x + 32, record->z);
      }
    } else {
      Object_SetMode(piece, 1);
      piece->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
      piece->motionFlags = ACTOR_FALLS;
      piece->y.fixed = 0;
    }
    record++;
  }

  Map_CopyCells(70, 42, 1, 1, 10, 50);

  if (*gKolimaForestFloodgateLayout == 1) {
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords, 254);
  } else {
    Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgateRecords, 255);
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
    if ((((u32)Engine_RandomNext() * 100) >> 16) <
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

  Engine_EventBegin();
  Engine_CameraSetSpeed(PIXELS(1), PIXELS(1) / 8);
  Engine_CameraMoveTo(PIXELS(264), -1, PIXELS(448), 1);
  Engine_CameraWaitForMove();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(SE_232);
  if (*gKolimaForestFloodgateLayout == 0) {
    Actor_SetPosition(ACTOR_FLOODGATE_SPRAY, PIXELS(256), PIXELS(462));
    Map_CopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(79, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(67, 34, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(69, 34, 2, 5, 79, 25);
    Engine_ActorSetAnimation(ACTOR_FLOODGATE_SPRAY, 1);
    Engine_AudioPlayCue(SE_240);
    Engine_TaskWait(6);
    Map_CopyCells(71, 34, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(73, 34, 2, 5, 79, 25);
    Map_CopyCells(75, 38, 2, 1, 79, 29);
    Engine_TaskWait(4);
    Map_CopyCells(77, 38, 2, 1, 79, 29);
    Engine_TaskWait(6);
    Map_CopyCells(79, 38, 2, 1, 79, 29);
    Engine_TaskWait(8);
    Map_CopyCells(65, 53, 2, 1, 79, 29);
    Map_CopyCells(65, 40, 2, 4, 15, 28);
  } else {
    Actor_SetPosition(ACTOR_FLOODGATE_SPRAY, PIXELS(256), PIXELS(480));
    Map_CopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(76, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(65, 45, 2, 4, 15, 28);
    Map_CopyCells(71, 50, 2, 5, 79, 25);
    Engine_ActorSetAnimation(ACTOR_FLOODGATE_SPRAY, 2);
    Engine_AudioPlayCue(SE_230);
    Engine_TaskWait(6);
    Map_CopyCells(69, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(67, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(65, 50, 2, 5, 79, 25);
    Engine_TaskWait(30);
  }
  if (*gKolimaForestFloodgateLayout == 0) {
    Map_DrawCellPanel(9, 19, 16, 5, *gKolimaForestFloodgateLayout, 9, 30);
    Map_DrawCellPanel(9, 51, 16, 5, 1, 9, 30);
    Map_DrawCellPanel(41, 51, 16, 5, 2, 9, 30);
  } else {
    Map_DrawCellPanel(9, 19, 16, 5, 0, 9, 30);
    Map_DrawCellPanel(9, 83, 16, 5, 1, 9, 30);
    Map_DrawCellPanel(41, 83, 16, 5, 2, 9, 30);
  }

  // The shake: each frame captures the scroll, and the HBlank handler shows
  // the upper lines shifted, more often as the chance climbs to 100%.
  gKolimaForestFloodgate_ShakeChance = 0;
  FieldScheduler_AddCallback(KolimaForestFloodgate_CaptureShakeScroll,
                             TASK_PRIORITY_SCENE);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(IRQ_HBLANK, 0, KolimaForestFloodgate_ShakeLines);
  Engine_AudioPlayCue(SE_231);
  gKolimaForestFloodgate_ShakeChance = 0;
  do {
    Engine_TaskWait(1);
    nextShakeChance = (gKolimaForestFloodgate_ShakeChance + 1);
    gKolimaForestFloodgate_ShakeChance += 1;
  } while (nextShakeChance <= 100);
  Engine_AudioPlayCue(SE_289);
  if (*gKolimaForestFloodgateLayout == 0) {
    Map_DrawCellPanel(9, 19, 16, 5, *gKolimaForestFloodgateLayout, 9, 19);
    Map_DrawCellPanel(9, 51, 16, 5, 1, 9, 19);
    Map_DrawCellPanel(41, 51, 16, 5, 2, 9, 19);
  } else {
    Map_DrawCellPanel(9, 19, 16, 5, 0, 9, 19);
    Map_DrawCellPanel(9, 83, 16, 5, 1, 9, 19);
    Map_DrawCellPanel(41, 83, 16, 5, 2, 9, 19);
  }
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(IRQ_HBLANK, 0, 0);
  Engine_TaskWait(1);
  Engine_TaskRemoveCallback(KolimaForestFloodgate_CaptureShakeScroll);
  *(u16 *)gKolimaForestFloodgateLayout ^= 1;
  KolimaForestFloodgate_DrawWaterLevel();
  Engine_MapRedraw();
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking west into the short
/// log: the leader shoves it west to x 72.
static void KolimaForestFloodgate_PushShortLogWest(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Engine_EventWait(6);
  Engine_AudioPlayCue(SE_239);
  Actor_SetSpeed(ACTOR_SHORT_LOG, PIXELS(1) / 2, PIXELS(1) / 5);
  Engine_ActorSetAnimation(ACTOR_SHORT_LOG, 2);
  Engine_ActorSetDestination(ACTOR_SHORT_LOG, 72, 176);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, -8, 0);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorWaitForMove(ACTOR_SHORT_LOG);
  Engine_ActorSetAnimation(ACTOR_SHORT_LOG, 1);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_AudioPlayCue(SE_213);
  Map_CopyCellAttributes(5, 9, 1, 4, 6, 9);
  Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  *gKolimaForestFloodgateReturned = 1;
  Engine_EventEnd();
}

/// Kolima Forest (SceneId_KolimaForestFloodgate), walking east into the short
/// log: the leader shoves it east to x 104.
static void KolimaForestFloodgate_PushShortLogEast(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Engine_EventWait(6);
  Engine_AudioPlayCue(SE_239);
  Actor_SetSpeed(ACTOR_SHORT_LOG, PIXELS(1) / 2, PIXELS(1) / 5);
  Engine_ActorSetAnimation(ACTOR_SHORT_LOG, 2);
  Engine_ActorSetDestination(ACTOR_SHORT_LOG, 104, 176);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  Engine_ActorWaitForMove(ACTOR_SHORT_LOG);
  Engine_ActorSetAnimation(ACTOR_SHORT_LOG, 1);
  Engine_AudioPlayCue(SE_STOP_SLOT3);
  Engine_AudioPlayCue(SE_213);
  Map_CopyCellAttributes(5, 9, 1, 4, 4, 9);
  Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  *gKolimaForestFloodgateReturned = 0;
  Engine_EventEnd();
}

/// Sets up the logs and the river's level on entry. The logs' records come back
/// from gSceneState, or on a first visit from the defaults with the water high;
/// the logs are made and the river is drawn at its level. The forest keeps its
/// cursed colours until the curse is lifted.
s32 KolimaForestFloodgate_Initialize(void) {
  struct FieldActor *actor;

  gKolimaForestFloodgateReturned = (u16 *)gSceneState;
  gKolimaForestFloodgateLayout = (s16 *)gSceneState + 1;
  // The return flag and water level occupy the first two halfwords.
  gKolimaForestFloodgateRecords =
      (struct FloodgateLog *)(gSceneState + 2 * sizeof(u16));
  Map_CopyCells(32, 0, 64, 32, 0, 64);
  Map_CopyCellAttributes(0, 0, 32, 32, 0, 64);
  Map_CopyCellAttributes(32, 0, 32, 32, 0, 32);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    Dma_Set(gKolimaForestFloodgate_DefaultRecords,
            gKolimaForestFloodgateRecords,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x12, REG_DMA3);
    *gKolimaForestFloodgateReturned = 0;
    *gKolimaForestFloodgateLayout = 1;
  }
  KolimaForestFloodgate_CreateLogs(gKolimaForestFloodgateRecords);
  KolimaForestFloodgate_StampLogCells(gKolimaForestFloodgate_DefaultRecords,
                                      255);
  KolimaForestFloodgate_DrawWaterLevel();
  Engine_ActorSetAnimation(ACTOR_FLOODGATE_SPRAY, 0);
  Object_GetById(ACTOR_FLOODGATE_SPRAY)->motionFlags = 0;
  actor = Object_GetById(ACTOR_SIGN);
  actor->radius = 8;
  actor->scaleX = 0xc000;
  actor->scaleY = 0xc000;
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
    KolimaPalette_Curse(4);
  return 0;
}

/// Makes each log at the middle of its four cells, standing on the ground.
static void KolimaForestFloodgate_CreateLogs(struct FloodgateLog *entry) {
  struct FieldActor *actor;
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
    actor = Engine_ObjectCreate(entry->sprite, x, 0, z);
    if (actor == NULL)
      return;
    entry->actor = actor;
    Object_SetMode(actor, 1);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->radius = 32;
    height = Map_GetTerrainHeight(0, actor->x.part.pixel, actor->z.part.pixel)
             << 16;
    // Through an s32 view of y, not y.fixed, the step to the next entry is
    // scheduled before this store.
    *(s32 *)&actor->y += height;
    actor->terrainHeight = height;
  }
}

/// Writes a collision code into the four cells of each log.
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
      cell = ((u8 *)gMapCellBuffer) + ((column + (row << 7)) << 2);
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
    struct MapCell *cell = &((struct MapCell *)gMapCellBuffer)[x + (z << 7)];

    if (cell->collisionCode == 0xff ||
        *(u8 *)((cell->attributeB << 2) + (s32)gMapCollision) != 0) {
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
  struct FieldActor *actor;
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
  actor = Object_GetById(ACTOR_PARTY_LEADER);
  facing = (actor->facing + 0x2000) & 0xc000;
  SET_VECTOR(point, (actor->x.fixed & 0xfff00000) + 0x80000, actor->y.fixed,
             (actor->z.fixed & 0xfff00000) + 0x80000);
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
  SET_VECTOR(point, (actor->x.fixed & 0xfff00000) + 0x80000, actor->y.fixed,
             (actor->z.fixed & 0xfff00000) + 0x80000);
  Vector_AddPolarOffset(0x80000, facing, point);
  actor = run->actor;
  quarter = facing / 0x4000;
  Engine_EventBegin();
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
  Engine_EventWait(6);
  actor->speed = PIXELS(1) / 2;
  actor->acceleration = PIXELS(1) / 5;
  Event_PlayCue(SE_239);
  Object_SetMode(actor, gKolimaForestFloodgate_PushAnimations[quarter]);
  Engine_ObjectSetPosition(actor, x, 0, z);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(*work));
  ObjectDispatch_StartCamera((struct DispatchObject *)work->camera, (s32)actor);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 3 * PIXELS(1) / 10, PIXELS(1) / 5);
  Actor_SetDestinationOffset(ACTOR_PARTY_LEADER,
                             gKolimaForestFloodgate_PushStepX[quarter],
                             gKolimaForestFloodgate_PushStepZ[quarter]);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldObject_WaitForMove(actor);
  Object_SetMode(actor, 1);
  Event_PlayCue(SE_STOP_SLOT3);
  Event_PlayCue(SE_213);
  Engine_EventWait(15);
  Engine_EventEnd();
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

struct ScenePlacement gKolimaForestFloodgatePlacements[] = {
    { SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(104), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SPLASH, CONDITION_ALWAYS, ACTOR_RESET_ACTION,
        PIXELS(256), 0, PIXELS(464), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SPRITE_SIGNPOST, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(348), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestFloodgateEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS,
     KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_CLIFF},
    {EVENT_EXIT, 2, CONDITION_ALWAYS,
     KOLIMA_FOREST_FLOODGATE_TO_KOLIMA_FOREST_TREE},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_JumpSouth)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_PushLog)},
    {TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_PushShortLogWest)},
    {TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_PushShortLogEast)},
    {EVENT_TALK, ACTOR_SIGN, CONDITION_ALWAYS,
     MSGNO(MsgKolimaDoNotTouchFloodgate)},
    {SEARCH(SEARCH_UNNAMED), CELL_FLOODGATE_SWITCH, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestFloodgate_FlipFloodgateSwitch)},
    {SEARCH(SEARCH_CHEST), 100, 0xf00, FIND_ITEM(ITEM_FUR_COAT)},
    {SCENE_EVENTS_END},
};
