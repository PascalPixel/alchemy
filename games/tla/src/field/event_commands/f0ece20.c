#include "types.h"
#include "dma.h"
#include "resource.h"
#include "ram_buffer.h"
#include "bg_scroll.h"
#include "field/world_map.h"

extern char ResourceId_Data023, ResourceId_Data024;

/// Draws the world map picture on BG1, scrolled to the cursor in gWorldMapWork
/// (resource 36, or 35 at the origin when the halfword at 0x640 is set): the
/// palette, then 21 rows of 31 tiles from the decoded rows, wrapping the 24-row
/// buffer and the 32-column screen.
void Func_080ece20(void) {
  struct WorldMapWork *work = &gWorldMapWork;
  u8 *buffer;
  s32 resource = (s32)&ResourceId_Data024;
  s32 *cursor = work->cursor;
  s32 x = *cursor++ - (120 << 16);
  s32 z = *cursor - (80 << 16);
  u32 *graphics;
  u32 column;
  u32 firstRow;
  u32 row;
  s32 bufferRow;
  s32 unwrapped;
  u32 i;
  s32 right;
  s32 bottom;
  s32 tile;

  if (work->centered != 0) {
    resource = (s32)&ResourceId_Data023;
    x = 0;
    z = 0;
  }
  graphics = Resource_GetTableEntry(resource);
  Dma_Set((u8 *)graphics + graphics[0], (void *)0x05000000, 0x84000078,
          (volatile u32 *)0x040000d4);
  *(u16 *)0x05000000 = 0;
  buffer = Ram_MapCellBuffer;
  right = 272 << 16;
  bottom = 224 << 16;
  if (x < 0)
    x = 0;
  if (x > right)
    x = right;
  if (z < 0)
    z = 0;
  if (z > bottom)
    z = bottom;
  gBgScroll[1].x = x >> 16;
  gBgScroll[1].y = z >> 16;
  x = (u32)x >> 19;
  column = x;
  firstRow = (u32)z >> 19;
  for (row = 0; row <= 20; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    /* FAKEMATCH(barrier): an empty asm reading x, which keeps it live over the
       row's start; without it, 118 of 149 instructions differ in tla-en. */
    asm("" : : "r"(x));
    if (bufferRow > 23)
      bufferRow -= 24;
    // z is reused for the map cell's index; a local of its own differs.
    z = (column & 31) + (unwrapped << 5);
    tile = (z + 128) % 768 + 256;
    Resource_DecodeType01((u8 *)graphics + graphics[unwrapped + 1],
                          buffer + (bufferRow << 12));
    // x is reused for the cell pointer; a local of its own differs.
    x = bufferRow << 6;
    x += column;
    {
      s32 to;

      i = 0;
      to = 0x06000000;
      x <<= 6;
      x += (s32)buffer;
      /* FAKEMATCH(barrier): VRAM's address through an empty asm that also
         reads i, which builds it once in r7 ahead of the loop; without it, 10
         of 149 instructions differ in tla-en. */
      asm("" : "=r"(to) : "0"(to), "r"(i));
      for (; i <= 30; i++) {
        Dma_Set((void *)x, (void *)((tile << 6) + to), 0x84000010,
                (volatile u32 *)0x040000d4);
        x += 64;
        tile++;
        if (i + column == 31)
          tile -= 32;
      }
    }
  }
}

#include "version.h"
#include "io_reg.h"

/// Set while the world map shows its alternative picture, which does not
/// scroll.
extern s16 Data_0202a640;

#define BG_VRAM 0x06000000

/// Draws one row of the world map picture on BG1, from a column to the right
/// edge of the map (at most 31 tiles; Japanese always draws 32), for
/// scrolling: decodes the picture's row into the cell buffer, which holds 24
/// rows and wraps, and copies its cells into the 32-column screen from tile
/// 256, wrapping at column 32.
void Func_080ecf74(u32 column, s32 row) {
  s32 resource = (s32)&ResourceId_Data024;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u32 i;

  if (Data_0202a640 != 0)
    resource = (s32)&ResourceId_Data023;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row > 23)
    row -= 24;
  cell = (row << 6) + column;
  tile = (column & 31) + (row << 5);
  tile = (tile + 128) % 768 + 256;
  Resource_DecodeType01((u8 *)graphics + graphics[unwrapped + 1],
                        Ram_MapCellBuffer + (row << 12));
  // From patch 1 the copy stops at the map's last column instead of running
  // past it.
#if TLA_PATCH_1
  for (i = 0; i <= 30 && column + i <= 63; i++) {
#else
  for (i = 0; i < 32; i++) {
#endif
    Dma_Set(Ram_MapCellBuffer + (cell << 6), (void *)(BG_VRAM + (tile << 6)),
            DMA_COPY_WORDS(16), REG_DMA3);
    cell++;
    tile++;
    if (i + column == 31)
      tile -= 32;
  }
}

static inline s32 WorldMap_AddColumn(s32 column, s32 cell) {
  /* FAKEMATCH(wrapper): the column added through an inline, so the sum
   * keeps the column as its first operand; as column + cell, 2 of 51
   * instructions differ in tla-en. */
  return column + cell;
}

/// Draws one column of the world map picture on BG1, 21 rows down from a
/// row, for scrolling: copies the column's cells from the decoded rows in the
/// cell buffer, which wraps after 24 rows, into the 32-column screen from
/// tile 256; the column twin of Func_080ecf74.
void Func_080ed030(s32 column, s32 row) {
  u32 i;

  for (i = 0; i <= 20; i++) {
    s32 line = row + i;
    s32 cell = line;
    s32 tile;

    if (line > 23)
      cell = line - 24;
    tile = ((column & 31) + (line << 5) + 128) % 768;
    cell <<= 6;
    cell = WorldMap_AddColumn(column, cell);
    cell <<= 6;
    tile <<= 6;
    // line is reused for the screen's address; as (u8 *)BG_VRAM + (256 << 6) +
    // tile in the call, 6 of 51 instructions differ in tla-en.
    line = BG_VRAM + (256 << 6);
    Dma_Set(Ram_MapCellBuffer + cell, (u8 *)line + tile, DMA_COPY_WORDS(16),
            REG_DMA3);
  }
}

#include "iwram_call.h"
#include "fixed_math.h"

/// The world map cursor's position (x, then y, 16.16 pixels).
extern s32 *Data_0202a62c;

s32 FixedSqrt(s32 value);

#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/// The world map screen's scroll follows the cursor. The target puts the
/// cursor 120 by 80 pixels in (clamped to the map); unless the map is held the
/// scroll moves a quarter of the way there each frame, at most 8 pixels, and
/// stops when nearer than a quarter pixel. Each time the scroll crosses an
/// 8-pixel column or row, the new column (Func_080ed030) or row
/// (Func_080ecf74) of the picture is drawn; BG1 scrolls with it. All values
/// are 16.16 pixels.
void Func_080ed0a0(void) {
  s32 *cursor = Data_0202a62c;
  s32 x = *cursor++ - (120 << 16);
  s32 y = *cursor - (80 << 16);
  s32 dx;
  s32 dy;
  s32 distance;
  s32 step;
  u32 column;
  u32 row;
  s32 limit;
  s32 bottom = 0xe00000;

  if (x < 0)
    x = 0;
  limit = 0x1100000;
  if (x > limit)
    x = limit;
  if (y < 0)
    y = 0;
  if (y > bottom)
    y = bottom;
  if (Data_0202a640 == 0) {
    dx = (x - SCROLL_X) / Q16_ONE;
    dy = (y - SCROLL_Y) / Q16_ONE;
    distance = Iwram_Sqrt(dx * dx + dy * dy) << 16;
    dx = x - SCROLL_X;
    dy = y - SCROLL_Y;
    // Near the target the whole-pixel distance is too coarse.
    if (distance < 0x400000)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy));
    step = distance / 4;
    if (step > 0x80000)
      step = 0x80000;
    if (distance >= 0x4000) {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dy = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dy), step);
      }
      x = SCROLL_X + dx;
      y = SCROLL_Y + dy;
      if (x < 0)
        x = 0;
      if (y < 0)
        y = 0;
      if (x > limit)
        x = limit;
      if (y > limit)
        y = limit;
    }
    column = (u32)x >> 19;
    row = (u32)y >> 19;
    if ((SCROLL_X ^ x) & 0x80000) {
      if (SCROLL_X < x)
        Func_080ed030(column + 30, row);
      else
        Func_080ed030(column, row);
    }
    if ((SCROLL_Y ^ y) & 0x80000) {
      if (SCROLL_Y < y)
        Func_080ecf74(column, row + 20);
      else
        Func_080ecf74(column, row);
    }
    gBgScroll[1].x = x >> 16;
    gBgScroll[1].y = y >> 16;
  }
  SCROLL_X = x;
  SCROLL_Y = y;
}

#include "input.h"
#include "main_routine_work.h"
#include "io_write.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "event_work.h"
#include "actor.h"
#include "map.h"
#include "field/sprite.h"
#include "flag_call.h"
#include "text/msg_ids.h"

extern s32 Data_0202a644;
extern s32 Data_0202a648;
extern s32 Data_0202a64c;
extern s32 Data_0202a650;
extern s32 Data_0202a630;
extern s32 Data_0202a634;

void ObjectTable_Snapshot(void);
void WindowSystem_InitFieldFar(void);
void Task_ResetAll(void);
void wait(s32 frames);
void Func_080ebf94(void);
void Func_080ec4d4(void);
void Func_080ec1d0(s32 *screenX, s32 *screenY);
void Func_080ec2b8(s32 *screenX, s32 *screenY, s32 step);
void FieldMessage_ShowFar(s32 message, s32 mode);
void WindowSprite_PrepareForRedrawFar(void *window);
void Func_080ca368(s32 scene, s32 entrance);
void BattleEffect_CleanupResourcesAndWindow(void);
void Func_08020270(void);

s32 Func_080ed2a4(s32 mode) {
  s32 savedDelay;
  u8 savedFlags[16];
  s32 screenX;
  s32 screenY;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct EventWork *event = Ram_HeapSlots->eventWork;
  struct ObjectSystemWork *objects = Ram_HeapSlots->objectSystem;
  s16 blend;
  s32 messageShown = 0;
  s32 zoomed = 0;
  s32 gliding = 0;
  struct WorldMapWork *world = &gWorldMapWork;
  struct MapAnimation *layer;
  struct FlagEntry *place;
  u32 *tiles;
  u32 tile;
  s32 row;
  s32 column;
  s32 i;
  struct InputState *input;
  struct InputState *keys;
  s32 *wayX;
  s32 *wayZ;
  s32 *cursorX;
  s32 *cursorZ;
  s32 *scrollX;
  s32 *scrollZ;
  s32 zero;
  s32 limitX;
  s32 limitZ;
  s32 value;
  struct WorldMapChoice *choice = &Data_02000500;
  struct MainRoutineWork *routine = &gMainRoutineWork;

  world->mode = mode;
  if (mode != 0 || event->controlMode != 3)
    ObjectTable_Snapshot();
  if (event->controlMode == 3) {
    savedDelay = event->delay;
    event->delay = 6;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      savedFlags[i] = layer->paused;
      layer->paused = 1;
      layer++;
    }
    objects->suspended = 1;
    wait(1);
    ((struct MapViewWork *)map)->picturePending = 1;
    do {
      // A once-only block: as a plain statement, 16 of 548 instructions differ.
      blend = REG_BLDALPHA;
    } while (0);
  } else {
    do {
      // A once-only block: as a plain statement, 281 of 548 instructions
      // differ.
      event->viewCenter->movementState = 1;
    } while (0);
    savedDelay = event->delay;
    event->delay = 6;
    event->screenEffect = 0;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Task_ResetAll();
    WindowSystem_InitFieldFar();
    objects->suspended = 1;
    wait(1);
    blend = REG_BLDALPHA;
    tiles = (u32 *)0x06003000;
    tile = 0x01810180;
    for (row = 0; row < 20; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    tile = 0x01010100;
    for (row = 0; row < 24; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    tile = 0x01010100;
    for (row = 0; row < 4; row++)
      for (column = 0; column < 16; column++) {
        *tiles++ = tile;
        tile += 0x20002;
      }
    map->origin = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    Io_Write16(0x682, &REG_BG1CNT);
    Io_Write16(0x1340, &REG_DISPCNT);
  }
  // Default: no pointer yet.
  wayX = NULL;
  cursorX = NULL;
  cursorZ = NULL;
  scrollX = NULL;
  scrollZ = NULL;
  Func_080ebf94();
  for (;;) {
    Data_0202a640 = zoomed;
    Func_080ece20();
    wait(1);
    Io_Write16(0x8682, &REG_BG1CNT);
    Io_Write16(0x1340, &REG_DISPCNT);
    if ((GameFlag_Test(284) || GameFlag_Test(286)) && !messageShown) {
      FieldMessage_ShowFar((s32)MsgNotOnMap, 1);
      messageShown = 1;
    }
    keys = &gInput;
    wayX = &Data_0202a64c;
    cursorX = &Data_0202a644;
    cursorZ = &Data_0202a648;
    for (;;) {
      Func_080ed0a0();
      Func_080ec4d4();
      input = keys;
      wait(1);
      if ((input->repeat & 8) && gliding == 0) {
        Func_080ec1d0(&screenX, &screenY);
        *cursorX = screenX << 16;
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the reload.
        input = &gInput;
        *cursorZ = screenY << 16;
        gliding = 1;
      }
      if (!(keys->held & 8) && gliding == 1) {
        *cursorX = *wayX;
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the reload.
        input = &gInput;
        *cursorZ = Data_0202a650;
        gliding = 0;
      }
      if (keys->repeat & 0x100) {
        Func_080ec2b8(&screenX, &screenY, 1);
        *cursorX = screenX << 16;
        // wayZ only ever holds &Data_0202a650, set where it is used.
        wayZ = &Data_0202a650;
        *cursorZ = screenY << 16;
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & 0x200) {
        Func_080ec2b8(&screenX, &screenY, -1);
        *cursorX = screenX << 16;
        wayZ = &Data_0202a650;
        *cursorZ = screenY << 16;
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & 4) {
        world->shown = -1;
        WindowSprite_PrepareForRedrawFar(world->window);
        Io_Write16(64, &REG_DISPCNT);
        zoomed ^= 1;
        if (*cursorX != 0) {
          world->x = *cursorX;
          world->z = *cursorZ;
          zero = 0;
          limitX = 0x1100000;
          limitZ = 0xe00000;
          scrollX = &Data_0202a630;
          *scrollX = world->x - 0x780000;
          scrollZ = &Data_0202a634;
          *scrollZ = world->z - 0x500000;
          value = *scrollX;
          if (value < 0) {
            *scrollX = zero;
            value = *scrollX;
          }
          if (value > limitX)
            *scrollX = limitX;
          value = *scrollZ;
          if (value < 0) {
            *scrollZ = zero;
            value = *scrollZ;
          }
          if (value > limitZ)
            *scrollZ = limitZ;
        }
        break;
      }
      if (input->repeat & 1) {
        if (mode == 0)
          goto done;
        if (world->shown == -1)
          continue;
        place = FlagEntries_GetTable();
        place += world->shown;
        routine->unk2be = (s32)&SceneId_WorldMap;
        choice->flag = place->flag;
        Func_080ca368(place->scene, place->entrance);
        goto done;
      }
      if (input->repeat & 2) {
        if (mode != 0) {
          routine->unk2be = 0xffff;
          {
            // The -1 in a local of its own; as choice->flag = -1, 40 of 548
            // instructions differ in tla-en.
            s32 none = -1;

            choice->flag = none;
          }
        }
        goto done;
      }
    }
  }
done:
  BattleEffect_CleanupResourcesAndWindow();
  Io_Write16(64, &REG_DISPCNT);
  if (mode != 0 || event->controlMode != 3) {
    event->countdown = 999;
    GameFlag_SetBit(265);
  } else {
    Func_08020270();
    IO_WRITE_QUEUE_PUSH(0x04000052, (u16)blend, 0x20000);
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    objects->suspended = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    ((struct MapViewWork *)map)->picturePending = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    event->delay = savedDelay;
    event->viewCenter->movementState = 0;
  }
  return 0;
}
