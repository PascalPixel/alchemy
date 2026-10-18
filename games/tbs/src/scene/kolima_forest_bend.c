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

// Board placement, repainting and push helpers.
struct IcePlacement;
static void State_ApplyRectByLayoutSelector(void);
static void KolimaForestBend_PlaceObjects(struct IcePlacement *entry);
static void Scene_PushBlockAlongRun(struct TileRun2 *runs);

// Returns the board's entrances through the overlay's byte-pointer API.
u8 *KolimaForestBend_GetEntrances(void) { return gKolimaForestBendEntrances; }

// This scene has no exit regions.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns exits to the adjacent forest scenes.
u8 *KolimaForestBend_GetExits(void) { return gKolimaForestBendExits; }

// Returns the board's initial actor placements.
u8 *KolimaForestBend_GetPlacements(void) { return gKolimaForestBendPlacements; }

// Returns the switch, push and jump events.
u8 *KolimaForestBend_GetEvents(void) { return gKolimaForestBendEvents; }

// Pushes a board piece, then restores attributes and stamps its new cells.
static void EV11_SCRIPT(void) {
  Scene_PushBlockAlongRun(gKolimaForestBendRecords);
  Map_CopyCellAttributes(0, 0x40, 0x20, 0x20, 0, 0);
  State_StampRecordCells((s32)gKolimaForestBendRecords, 0xff);
  State_ApplyRectByLayoutSelector();
}

// Uses the shared forward-jump action at the gap.
static void EV10_TOUCH(void) { Field_TryJumpForward(); }

struct IcePlacement {
  s16 type;
  s16 x;
  s16 z;
  s16 turned;
  struct FieldActor *actor;
};

struct TileRun2 {
  s16 id;
  s16 x;
  s16 z;
  s16 vertical;
  struct FieldActor *object;
};

// Shared panel drawing stays in its linked position.
#include "common/map_panel.h"

// Selects the four-cell attribute strip from the returned-piece state.
static void State_ApplyRectByLayoutSelector(void) {
  if (*(s16 *)gKolimaForestBendReturned == 1) {
    Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  } else {
    Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  }
}

// Repaints the selected board layout and updates its pieces' height and cells.
static void Scene_RepaintBoardRecords(void) {
  struct FieldActor *piece;
  s16 *record = gKolimaForestBendRecords;

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

  // Keep each piece's display and four occupied cells in the same layout.
  while (record[0] != -1) {
    piece = *(struct FieldActor **)(record + 4);

    if (*gKolimaForestBendLayout == 1) {
      Object_SetMode(piece, 4);
      piece->priorityFlags = 3;
      piece->motionFlags = 0;
      piece->y.fixed = PIXELS(26);

      if (record[3] != 0) {
        Map_CopyCells(68, 40, 1, 4, record[1] + 32, record[2]);
      } else {
        Map_CopyCells(70, 40, 4, 1, record[1] + 32, record[2]);
      }
    } else {
      Object_SetMode(piece, 1);
      piece->priorityFlags = 1;
      piece->motionFlags = 2;
      piece->y.fixed = 0;
    }
    record += 6;
  }

  Map_CopyCells(70, 42, 1, 1, 10, 50);

  if (*gKolimaForestBendLayout == 1) {
    Map_CopyCellAttributes(0, 32, 32, 32, 0, 0);
    State_StampRecordCells(gKolimaForestBendRecords, 254);
  } else {
    Map_CopyCellAttributes(0, 64, 32, 32, 0, 0);
    State_StampRecordCells(gKolimaForestBendRecords, 255);
  }

  State_ApplyRectByLayoutSelector();
}

// HBlank callback: selects the captured shake scroll above the split line.
static void State_UpdateScrollRegistersWithPreset(void) {
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

// Captures three background scroll pairs and adds the vertical shake offset.
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

// Animates the switch, shakes the board, then flips its layout.
static void EV09_SEARCH(void) {
  s32 nextShakeChance;

  Engine_EventBegin();
  Engine_CameraSetSpeed(0x10000, 0x2000);
  Engine_CameraMoveTo(0x1080000, -1, 0x1c00000, 1);
  Engine_CameraWaitForMove();
  Engine_MessageShowCentered(MsgFieldFlippedSwitch, 1);
  Engine_AudioPlayCue(232);
  if (*gKolimaForestBendLayout == 0) {
    Actor_SetPosition(9, 0x1000000, 0x1ce0000);
    Map_CopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(79, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(67, 34, 2, 5, 79, 25);
    Engine_TaskWait(6);
    Map_CopyCells(69, 34, 2, 5, 79, 25);
    Engine_ActorSetAnimation(9, 1);
    Engine_AudioPlayCue(240);
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
    Actor_SetPosition(9, 0x1000000, 0x1e00000);
    Map_CopyCells(78, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(77, 34, 1, 2, 83, 25);
    Engine_TaskWait(3);
    Map_CopyCells(76, 34, 1, 2, 83, 25);
    Engine_TaskWait(30);
    Map_CopyCells(65, 45, 2, 4, 15, 28);
    Map_CopyCells(71, 50, 2, 5, 79, 25);
    Engine_ActorSetAnimation(9, 2);
    Engine_AudioPlayCue(230);
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

  // Temporarily install the frame capture and HBlank shake callbacks.
  gKolimaForestBend_ShakeChance = 0;
  FieldScheduler_AddCallback(KolimaForestBend_CaptureShakeScroll, 0xc80);
  Engine_TaskWait(1);
  Runtime_SetIrqHandler(IRQ_HBLANK, 0, State_UpdateScrollRegistersWithPreset);
  Engine_AudioPlayCue(231);
  gKolimaForestBend_ShakeChance = 0;
  do {
    Engine_TaskWait(1);
    nextShakeChance = (gKolimaForestBend_ShakeChance + 1);
    gKolimaForestBend_ShakeChance += 1;
  } while (nextShakeChance <= 100);
  Engine_AudioPlayCue(0x121);
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
  Scene_RepaintBoardRecords();
  Engine_MapRedraw();
  Engine_EventEnd();
}

// Returns actor 8 to the left position and restores its attribute strip.
static void EV13_TOUCH(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(6);
  Engine_AudioPlayCue(239);
  Actor_SetSpeed(8, 0x8000, 0x3333);
  Engine_ActorSetAnimation(8, 2);
  Engine_ActorSetDestination(8, 72, 176);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(0, 2);
  Actor_SetSpeed(0, 0x4ccc, 0x3333);
  Engine_ActorSetDestinationOffset(0, -8, 0);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(0, 1);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(8, 1);
  Engine_AudioPlayCue(0x120);
  Engine_AudioPlayCue(213);
  Map_CopyCellAttributes(5, 9, 1, 4, 6, 9);
  Map_CopyCellAttributes(0, 0, 1, 4, 4, 9);
  *gKolimaForestBendReturned = 1;
  Engine_EventEnd();
}

// Moves actor 8 to the right position and selects its alternate strip.
static void EV12_TOUCH(void) {

  Engine_EventBegin();
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(6);
  Engine_AudioPlayCue(239);
  Actor_SetSpeed(8, 0x8000, 0x3333);
  Engine_ActorSetAnimation(8, 2);
  Engine_ActorSetDestination(8, 104, 176);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(0, 2);
  Actor_SetSpeed(0, 0x4ccc, 0x3333);
  Engine_ActorSetDestinationOffset(0, 8, 0);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(0, 1);
  Engine_ActorWaitForMove(8);
  Engine_ActorSetAnimation(8, 1);
  Engine_AudioPlayCue(0x120);
  Engine_AudioPlayCue(213);
  Map_CopyCellAttributes(5, 9, 1, 4, 4, 9);
  Map_CopyCellAttributes(0, 0, 1, 4, 6, 9);
  *gKolimaForestBendReturned = 0;
  Engine_EventEnd();
}

// Restores the board records, places its pieces and applies the chosen layout.
s32 KolimaForestBend_SetupBoard(void) {
  struct FieldActor *actor;

  gKolimaForestBendReturned = (u16 *)gSceneState;
  (*(u16 **)&gKolimaForestBendLayout) = (u16 *)gSceneState + 1;
  gKolimaForestBendRecords = (s16 *)gSceneState + 2;
  Map_CopyCells(32, 0, 64, 32, 0, 64);
  Map_CopyCellAttributes(0, 0, 32, 32, 0, 64);
  Map_CopyCellAttributes(32, 0, 32, 32, 0, 32);
  if (Engine_GameFlagIsSet(0x109) == 0) {
    Dma_Set(gKolimaForestBend_DefaultRecords, gKolimaForestBendRecords,
            (DMA_ENABLE | DMA_32BIT) << 16 | 0x12, REG_DMA3);
    *gKolimaForestBendReturned = 0;
    *(*(u16 **)&gKolimaForestBendLayout) = 1;
  }
  KolimaForestBend_PlaceObjects(gKolimaForestBendRecords);
  State_StampRecordCells(gKolimaForestBend_DefaultRecords, 255);
  Scene_RepaintBoardRecords();
  Engine_ActorSetAnimation(9, 0);
  Object_GetById(9)->motionFlags = 0;
  actor = Object_GetById(10);
  actor->radius = 8;
  actor->scaleX = 0xc000;
  actor->scaleY = 0xc000;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0)
    SceneEffect_AdjustPaletteColors(4);
  return 0;
}

// Creates each four-cell piece at its orientation-dependent centre and height.
static void KolimaForestBend_PlaceObjects(struct IcePlacement *entry) {
  struct FieldActor *actor;
  s32 height;
  s32 x;
  s32 z;

  for (; entry->type != -1; entry++) {
    if (entry->turned == 0) {
      x = (entry->x << 20) + 0x200000;
      z = (entry->z << 20) + 0x80000;
    } else {
      x = (entry->x << 20) + 0x80000;
      z = (entry->z << 20) + 0x200000;
    }
    actor = Engine_ObjectCreate(entry->type, x, 0, z);
    if (actor == NULL)
      return;
    entry->actor = actor;
    Object_SetMode(actor, 1);
    Engine_ActorSetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->radius = 32;
    height = Map_GetTerrainHeight(0, actor->x.part.pixel, actor->z.part.pixel)
             << 16;
    /* FAKEMATCH: y is updated through an s32 lvalue; the coordinate union has
     * an s16 member, which would order the next entry load after this store.
     * Measured 2026-10-13: actor->y += height matches when FieldActor's x, y
     * and z are s32 and the pixel half is read as ((s16 *)&c)[1]; all six
     * editions stay identical with three other functions holding a union
     * FieldObject handle. Held as union FieldObject: differ 4. */
    *(s32 *)&actor->y += height;
    actor->terrainHeight = height;
  }
}

// Writes the collision code to all four cells of each piece's recorded run.
void State_StampRecordCells(s16 *records, s32 value) {

  s16 *record = records;
  if (record[0] == -1)
    return;
  do {
    s32 column = record[1];
    s32 row = record[2];
    s32 along = record[3];
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
    record += 6;
  } while (record[0] != -1);
}

// Finds the horizontal or vertical four-cell run containing the queried cell.
static const struct TileRun *SceneData_FindTileRunAt(const struct TileRun *run,
                                                     s32 x, s32 y) {
  while (run->id != -1) {
    s32 x0 = run->x;
    s32 x1 = x0;
    s32 y0 = run->y;
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

// Rejects a four-cell run when a cell is blocked or has collision code 0xff.
static s32 State_CheckFourCellRun(s32 x, s32 z, s32 mode) {
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

// Finds the piece ahead and advances it through at most eleven clear cells.
static void Scene_PushBlockAlongRun(struct TileRun2 *runs) {
  struct FieldActor *actor;
  struct TileRun2 *run;
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
  run = SceneData_FindTileRunAt(runs, point[0] / 0x100000, point[2] / 0x100000);
  if (run == 0)
    return;
  for (i = 0; i <= 10; i++) {
    point[0] = run->x << 20;
    point[2] = run->z << 20;
    Vector_AddPolarOffset(0x100000, facing, point);
    if (State_CheckFourCellRun(point[0] / 0x100000, point[2] / 0x100000,
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
  actor = run->object;
  quarter = facing / 0x4000;
  Engine_EventBegin();
  Engine_ActorSetAnimation(0, 8);
  Engine_EventWait(6);
  actor->speed = 0x8000;
  actor->acceleration = 0x3333;
  Event_PlayCue(239);
  Object_SetMode(actor, gKolimaForestBend_PushAnimations[quarter]);
  Engine_ObjectSetPosition(actor, x, 0, z);
  Engine_EventWait(6);
  Engine_ActorSetAnimation(0, 2);
  ObjectDispatch_InitFromTable4WithArgument(
      *(s32 *)(Runtime_AllocateBlock(27, 0xccc) + 0x1e0), actor);
  Actor_SetSpeed(0, 0x4ccc, 0x3333);
  Actor_SetDestinationOffset(0, gKolimaForestBend_PushStepX[quarter],
                             gKolimaForestBend_PushStepZ[quarter]);
  Engine_EventWait(24);
  Engine_ActorSetAnimation(0, 1);
  FieldObject_WaitForMove(actor);
  Object_SetMode(actor, 1);
  Event_PlayCue(0x120);
  Event_PlayCue(213);
  Engine_EventWait(15);
  Engine_EventEnd();
}

// Shared palette adjustment stays after the board movement code.
#include "common/kolima_palette.h"

// clang-format off
u32 gKolimaForestBend_DefaultRecords[] = {
    0xb00cd, 0x10009, 0, 0xc00cf, 12, 0, 0x1100cf, 12,
    0, 0xd00cd, 0x1000d, 0, 0xa00cf, 18, 0, 0xffff,
    0, 0,
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
     EVENT_SCRIPT(EV10_TOUCH)},
    {0x202, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_SCRIPT)},
    {TOUCH_WALKING_INTO(FACING_WEST), 13, CONDITION_ALWAYS,
     EVENT_SCRIPT(EV13_TOUCH)},
    {TOUCH_WALKING_INTO(FACING_EAST), 12, CONDITION_ALWAYS,
     EVENT_SCRIPT(EV12_TOUCH)},
    {EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKolimaDoNotTouchFloodgate)},
    {SEARCH(SEARCH_UNNAMED), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_SEARCH)},
    {SEARCH(SEARCH_CHEST), 100, 0xf00, FIND_ITEM(0x5b)},
    {SCENE_EVENTS_END},
};
