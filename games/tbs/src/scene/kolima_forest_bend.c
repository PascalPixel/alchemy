/// @file
/// Kolima Forest's bend, between the river and Tret's tree. A floodgate switch
/// raises and lowers the river: with the water high the logs float (26 pixels
/// up) and can be rolled along it; with it low they lie on the riverbed. A
/// short log slides between two places, and a sign warns the party off the
/// switch.
///
/// The water level, the short log's place and each log's cells are kept in
/// gSceneState (gKolimaForestBendLayout, gKolimaForestBendReturned and
/// gKolimaForestBendRecords), so they survive a battle.

#include "battle_effect_runtime.h"
#include "flags.h"
#include "unit/import.h"
#include "unit/scene/kolima_forest_bend.h"
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

enum {
  SHORT_LOG = 8, ///< slides between x 72 and x 104
  FLOODGATE_SWITCH = 9,
  SIGN = 10 ///< reads a warning. "Do not touch Floodgate Switch!"
};

static void KolimaForestBend_MarkShortLogCells(void);
static void KolimaForestBend_CreateLogs(struct BendLog *log);
static void KolimaForestBend_RollLog(struct BendLog *logs);

/// Where the party enters from the river and from Tret's tree.
const struct SceneEntrance *KolimaForestBend_GetEntrances(void) {
  return gKolimaForestBendEntrances;
}

const struct SceneRegion *KolimaForestBend_GetRegions(void) { return 0; }

/// The paths back to the river and on to Tret's tree.
const u32 *KolimaForestBend_GetExits(void) { return gKolimaForestBendExits; }

/// The short log, the switch and the sign.
const struct ScenePlacement *KolimaForestBend_GetPlacements(void) {
  return gKolimaForestBendPlacements;
}

/// The logs, the switch, the sign, the jump and the chest.
const struct SceneEvent *KolimaForestBend_GetEvents(void) {
  return gKolimaForestBendEvents;
}

/// Walking into a log: it rolls ahead, and the cells of the logs are marked
/// again where they now lie.
static void KolimaForestBend_PushLog(void) {
  KolimaForestBend_RollLog(gKolimaForestBendRecords);
  Map_CopyCellAttributes(0, 0x40, 0x20, 0x20, 0, 0);
  State_StampRecordCells(gKolimaForestBendRecords, 0xff);
  KolimaForestBend_MarkShortLogCells();
}

/// Walking south into the gap: the leader jumps across.
static void KolimaForestBend_JumpSouth(void) { Field_TryJumpForward(); }

#include "common/map_panel.h"

/// Gives the short log's column of cells its collision: column 4 at the
/// west place, column 6 at the east.
static void KolimaForestBend_MarkShortLogCells(void) {
  if (*(s16 *)gKolimaForestBendReturned == 1) {
    Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  } else {
    Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  }
}

/// Draws the river at its level and sets each log to match: high water floats
/// the logs 26 pixels up over their own cells, low water lays them on the
/// riverbed.
static void KolimaForestBend_DrawWaterLevel(void) {
  struct FieldActor *piece;
  struct BendLog *record = gKolimaForestBendRecords;

  if (*gKolimaForestBendLayout != 0) {
    Map_CopyCells(65, 53, 2, 1, 79, 29);
    Map_CopyCells(65, 40, 2, 4, 15, 28);
  } else {
    Map_CopyCells(65, 50, 2, 5, 79, 25);
  }

  if (*gKolimaForestBendLayout != 0) {
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

    if (*gKolimaForestBendLayout == 1) {
      Object_SetMode(piece, 4);
      piece->priorityFlags = 3;
      piece->motionFlags = 0;
      piece->y.fixed = PIXELS(26);

      if (record->vertical != 0) {
        Map_CopyCells(68, 40, 1, 4, record->x + 32, record->z);
      } else {
        Map_CopyCells(70, 40, 4, 1, record->x + 32, record->z);
      }
    } else {
      Object_SetMode(piece, 1);
      piece->priorityFlags = 1;
      piece->motionFlags = 2;
      piece->y.fixed = 0;
    }
    record++;
  }

  Map_CopyCells(70, 42, 1, 1, 10, 50);

  if (*gKolimaForestBendLayout == 1) {
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    State_StampRecordCells(gKolimaForestBendRecords, 254);
  } else {
    Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    State_StampRecordCells(gKolimaForestBendRecords, 255);
  }

  KolimaForestBend_MarkShortLogCells();
}

/// HBlank callback: selects the captured shake scroll above the split line.
static void KolimaForestBend_ShakeLines(void) {
  u16 line;
  u32 *source;
  volatile u32 *destination;

  line = REG_VCOUNT;
  source = (u32 *)((u8 *)gBgScroll + 4);
  destination = (volatile u32 *)0x04000014;

  if (line == 227 || line <= 52) {
    if ((((u32)Engine_RandomNext() * 100) >> 16) <
        gKolimaForestBend_ShakeChance) {
      source = gKolimaForestBend_ShakeScroll;
    }
  }

  *destination = *source++;
  destination = (volatile u32 *)0x04000018;
  *destination++ = *source++;
  *destination = *source;
}

/// Captures three background scroll pairs and adds the vertical shake offset.
static void KolimaForestBend_CaptureShakeScroll(void) {
  u32 *destination;
  const u32 *source;
  u16 *scroll;

  source = (const u32 *)((u8 *)gBgScroll + 4);
  destination = gKolimaForestBend_ShakeScroll;
  *destination++ = *source++;
  *destination++ = *source++;
  *destination = *source;
  scroll = (u16 *)gKolimaForestBend_ShakeScroll;
  scroll[1] += 0xb0;
  scroll[3] += 0xb0;
  scroll[5] += 0xb0;
}

/// Searching the floodgate switch flips it. "Isaac flipped the switch." The
/// camera turns to the floodgate, which opens or closes; the ground rumbles
/// and the top of the screen shakes harder for 100 frames, and the river rises
/// or falls.
static void KolimaForestBend_FlipFloodgateSwitch(void) {
  s32 nextShakeChance;

  Engine_EventBegin();
  Engine_CameraSetSpeed(0x10000, 0x2000);
  Engine_CameraMoveTo(PIXELS(264), -1, PIXELS(448), 1);
  Engine_CameraWaitForMove();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(SE_232);
  if (*gKolimaForestBendLayout == 0) {
    Actor_SetPosition(FLOODGATE_SWITCH, PIXELS(256), PIXELS(462));
    Map_CopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(79, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(67, 34, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(69, 34, 2, 5, 79, 25);
    Engine_ActorSetAnimation(FLOODGATE_SWITCH, 1);
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
    Actor_SetPosition(FLOODGATE_SWITCH, PIXELS(256), PIXELS(480));
    Map_CopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(76, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(65, 45, 2, 4, 15, 28);
    Map_CopyCells(71, 50, 2, 5, 79, 25);
    Engine_ActorSetAnimation(FLOODGATE_SWITCH, 2);
    Engine_AudioPlayCue(SE_230);
    Engine_TaskWait(6);
    Map_CopyCells(69, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(67, 50, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(65, 50, 2, 5, 79, 25);
    Engine_TaskWait(30);
  }
  if (*gKolimaForestBendLayout == 0) {
    Map_DrawCellPanel(9, 19, 16, 5, *gKolimaForestBendLayout, 9, 30);
    Map_DrawCellPanel(9, 51, 16, 5, 1, 9, 30);
    Map_DrawCellPanel(41, 51, 16, 5, 2, 9, 30);
  } else {
    Map_DrawCellPanel(9, 19, 16, 5, 0, 9, 30);
    Map_DrawCellPanel(9, 83, 16, 5, 1, 9, 30);
    Map_DrawCellPanel(41, 83, 16, 5, 2, 9, 30);
  }

  // The shake: each frame captures the scroll, and the HBlank handler shows
  // the upper lines shifted, more often as the chance climbs to 100%.
  gKolimaForestBend_ShakeChance = 0;
  FieldScheduler_AddCallback(KolimaForestBend_CaptureShakeScroll, 0xc80);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(IRQ_HBLANK, 0, KolimaForestBend_ShakeLines);
  Engine_AudioPlayCue(SE_231);
  gKolimaForestBend_ShakeChance = 0;
  do {
    Engine_TaskWait(1);
    nextShakeChance = (gKolimaForestBend_ShakeChance + 1);
    gKolimaForestBend_ShakeChance += 1;
  } while (nextShakeChance <= 100);
  Engine_AudioPlayCue(SE_289);
  if (*gKolimaForestBendLayout == 0) {
    Map_DrawCellPanel(9, 19, 16, 5, *gKolimaForestBendLayout, 9, 19);
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
  Engine_TaskRemoveCallback(KolimaForestBend_CaptureShakeScroll);
  *(u16 *)gKolimaForestBendLayout ^= 1;
  KolimaForestBend_DrawWaterLevel();
  Engine_MapRedraw();
  Engine_EventEnd();
}

/// Walking west into the short log: the leader shoves it west to x 72.
static void KolimaForestBend_PushShortLogWest(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(6);
  Engine_AudioPlayCue(SE_239);
  Actor_SetSpeed(SHORT_LOG, 0x8000, 0x3333);
  Engine_ActorSetAnimation(SHORT_LOG, 2);
  Engine_ActorSetDestination(SHORT_LOG, 72, 176);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(0, 2);
  Actor_SetSpeed(0, 0x4ccc, 0x3333);
  Engine_ActorSetDestinationOffset(0, -8, 0);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(0, 1);
  Engine_ActorWaitForMove(SHORT_LOG);
  Engine_ActorSetAnimation(SHORT_LOG, 1);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_213);
  Map_CopyCellAttributes(5, 9, 1, 4, 6, 9);
  Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  *gKolimaForestBendReturned = 1;
  Engine_EventEnd();
}

/// Walking east into the short log: the leader shoves it east to x 104.
static void KolimaForestBend_PushShortLogEast(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(6);
  Engine_AudioPlayCue(SE_239);
  Actor_SetSpeed(SHORT_LOG, 0x8000, 0x3333);
  Engine_ActorSetAnimation(SHORT_LOG, 2);
  Engine_ActorSetDestination(SHORT_LOG, 104, 176);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(0, 2);
  Actor_SetSpeed(0, 0x4ccc, 0x3333);
  Engine_ActorSetDestinationOffset(0, 8, 0);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(0, 1);
  Engine_ActorWaitForMove(SHORT_LOG);
  Engine_ActorSetAnimation(SHORT_LOG, 1);
  Engine_AudioPlayCue(SE_288);
  Engine_AudioPlayCue(SE_213);
  Map_CopyCellAttributes(5, 9, 1, 4, 4, 9);
  Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  *gKolimaForestBendReturned = 0;
  Engine_EventEnd();
}

/// On entry: the logs' records come back from gSceneState, or on a first visit
/// from the defaults with the water high; the logs are made and the river is
/// drawn at its level. The forest keeps its cursed colours until the curse is
/// lifted.
s32 KolimaForestBend_Initialize(void) {
  struct FieldActor *actor;

  gKolimaForestBendReturned = (u16 *)gSceneState;
  gKolimaForestBendLayout = (s16 *)gSceneState + 1;
  // The return flag and water level occupy the first two halfwords.
  gKolimaForestBendRecords = (struct BendLog *)(gSceneState + 2 * sizeof(u16));
  Map_CopyCells(32, 0, 64, 32, 0, 64);
  Map_CopyCellAttributes(0, 0, 32, 32, 0, 64);
  Map_CopyCellAttributes(32, 0, 32, 32, 0, 32);
  if (Engine_GameFlagIsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
    Dma_Set(gKolimaForestBend_DefaultRecords, gKolimaForestBendRecords,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x12, REG_DMA3);
    *gKolimaForestBendReturned = 0;
    *gKolimaForestBendLayout = 1;
  }
  KolimaForestBend_CreateLogs(gKolimaForestBendRecords);
  State_StampRecordCells(gKolimaForestBend_DefaultRecords, 255);
  KolimaForestBend_DrawWaterLevel();
  Engine_ActorSetAnimation(FLOODGATE_SWITCH, 0);
  Object_GetById(FLOODGATE_SWITCH)->motionFlags = 0;
  actor = Object_GetById(SIGN);
  actor->radius = 8;
  actor->scaleX = 0xc000;
  actor->scaleY = 0xc000;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
    KolimaPalette_Curse(4);
  return 0;
}

/// Makes each log at the middle of its four cells, standing on the ground.
static void KolimaForestBend_CreateLogs(struct BendLog *entry) {
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
    /* FAKEMATCH: y is updated through an s32 lvalue, since the union's s16
     * member would order the next entry load after this store; held as union
     * FieldObject, 4 instructions differ in tbs-en. */
    *(s32 *)&actor->y += height;
    actor->terrainHeight = height;
  }
}

/// Writes a collision code into the four cells of each log.
void State_StampRecordCells(const struct BendLog *records, s32 value) {

  const struct BendLog *record = records;
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
static struct BendLog *KolimaForestBend_FindLogAt(struct BendLog *run, s32 x,
                                                  s32 y) {
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
static s32 KolimaForestBend_CheckLogFits(s32 x, s32 z, s32 mode) {
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
static void KolimaForestBend_RollLog(struct BendLog *runs) {
  struct FieldActor *actor;
  struct FieldWork *work;
  struct BendLog *run;
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
  run = KolimaForestBend_FindLogAt(runs, point[0] / 0x100000,
                                   point[2] / 0x100000);
  if (run == 0)
    return;
  for (i = 0; i <= 10; i++) {
    point[0] = run->x << 20;
    point[2] = run->z << 20;
    Vector_AddPolarOffset(0x100000, facing, point);
    if (KolimaForestBend_CheckLogFits(point[0] / 0x100000, point[2] / 0x100000,
                                      run->vertical) != 0)
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
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(6);
  actor->speed = 0x8000;
  actor->acceleration = 0x3333;
  Event_PlayCue(SE_239);
  Object_SetMode(actor, gKolimaForestBend_PushAnimations[quarter]);
  Engine_ObjectSetPosition(actor, x, 0, z);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(0, 2);
  work = Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(*work));
  ObjectDispatch_StartCamera((struct DispatchObject *)work->camera, (s32)actor);
  Actor_SetSpeed(0, 0x4ccc, 0x3333);
  Actor_SetDestinationOffset(0, gKolimaForestBend_PushStepX[quarter],
                             gKolimaForestBend_PushStepZ[quarter]);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(0, 1);
  FieldObject_WaitForMove(actor);
  Object_SetMode(actor, 1);
  Event_PlayCue(SE_288);
  Event_PlayCue(SE_213);
  Engine_EventWait(15);
  Engine_EventEnd();
}

#include "common/kolima_palette.h"

/// The logs on a first visit, as struct BendLog: sprite 0xcd runs north to
/// south and 0xcf west to east.
// clang-format off
struct BendLog gKolimaForestBend_DefaultRecords[] = {
    {0xcd, 11, 9, 1, NULL},
    {0xcf, 12, 12, 0, NULL},
    {0xcf, 17, 12, 0, NULL},
    {0xcd, 13, 13, 1, NULL},
    {0xcf, 10, 18, 0, NULL},
    {-1},
};

u8 gKolimaForestBend_PushAnimations[] = {2, 2, 3, 3};

s8 gKolimaForestBend_PushStepX[] = {8, 0, -8, 0};

s8 gKolimaForestBend_PushStepZ[] = {0, 8, 0, -8};

struct SceneEntrance gKolimaForestBendEntrances[] = {
    { 0, CONDITION_ALWAYS, 407, 0, 276, FACING_WEST, 0, -1, -1, -1, -1, 0 },
    { 1, CONDITION_ALWAYS, 50, 32, 369, FACING_EAST, 0, -1, -1, -1, -1, 0 },
    { 2, CONDITION_ALWAYS, 162, 32, 84, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

/// Exit 1 to the river (scene 42), exit 2 to Tret's tree (scene 44).
u32 gKolimaForestBendExits[] = {
    SCENE_EXITS(43),
    SCENE_EXIT(1, 42, 2),
    SCENE_EXIT(2, 44, 1),
    SCENE_EXITS_END,
};

struct ScenePlacement gKolimaForestBendPlacements[] = {
    { 0xcd, CONDITION_ALWAYS, 7,
        PIXELS(104), 0, PIXELS(176), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xd6, CONDITION_ALWAYS, 7,
        PIXELS(256), 0, PIXELS(464), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x112, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(348), 0, PIXELS(456), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gKolimaForestBendEvents[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestBend_JumpSouth)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 11, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestBend_PushLog)},
    {TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestBend_PushShortLogWest)},
    {TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestBend_PushShortLogEast)},
    {EVENT_TALK, SIGN, CONDITION_ALWAYS, MSGNO(MsgKolimaDoNotTouchFloodgate)},
    {SEARCH(SEARCH_UNNAMED), FLOODGATE_SWITCH, CONDITION_ALWAYS,
     EVENT_SCRIPT(KolimaForestBend_FlipFloodgateSwitch)},
    // A Fur Coat (item 0x5b).
    {SEARCH(SEARCH_CHEST), 100, 0xf00, FIND_ITEM(ITEM_FUR_COAT)},
    {SCENE_EVENTS_END},
};
