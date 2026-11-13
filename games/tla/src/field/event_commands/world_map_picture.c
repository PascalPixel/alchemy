/// @file
/// The world map screen's picture: drawing it, scrolling it with the cursor,
/// its area markers, and zooming the view out while L is held.

// ---- games/tla/src/field/event_commands/world_map_picture.c ----
#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "flag_call.h"
#include "gamepak/far_call/window.h"
#include "lib.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "scene.h"
#include "text/msg_ids.h"
#include "world_map.h"

// The world map screen's work, laid out as field/world_map.h has it.

u32 Func_080ec298(struct WorldMapPlace *place);

// The place's position on the world map screen.
#define PLACE_SCREEN_X(place) ((place)->x * 853 / 16384 - 269)
#define PLACE_SCREEN_Y(place) ((place)->z * 640 / 16384 - 112)

void Func_080ec2b8(s32 *screenX, s32 *screenY, s32 step) {
  struct WorldMapPlace *places;
  struct WorldMapPlace *place;
  struct WorldMapPlace *next;
  s32 best;
  s32 bestDistance;
  s32 cursorX;
  s32 cursorY;
  s32 count;
  s32 wraps;
  s32 flag;
  u32 state;
  struct WorldMapWork *work = Ram_WorldMapWork;

  Party_GetLeaderActor();
  places = FlagEntries_GetTable();
  bestDistance = 0x4000000;
  best = -1;
  cursorX = work->x >> 16;
  cursorY = work->z >> 16;
  if (work->unk644 != 0) {
    cursorX = work->unk644 >> 16;
    cursorY = work->unk648 >> 16;
  }
  for (count = 0; count < 128; count++) {

    place = &places[count];
    if (place->flag == 0)
      break;
    flag = place->flag + 0x180;
    state = Func_080ec298(place);

    if (state != 0 && state != 15 && GameFlag_Test(flag)) {
      s32 sx = PLACE_SCREEN_X(place);
      s32 sy = PLACE_SCREEN_Y(place);
      s32 dx = sx - cursorX;
      s32 dy = sy - cursorY;
      s32 distance = dx * dx + dy * dy;

      if (bestDistance > distance) {
        bestDistance = distance;
        best = count;
      }
    }
  }
  if (best == -1) {
    Func_080ec1d0(screenX, screenY);
    return;
  }
  place = &places[best];
  if (bestDistance <= 4) {
    wraps = 0;
    do {
      best += step;
      if (best < 0) {
        best = count - 1;
        wraps++;
      } else {
        next = &places[best];
        if (next->flag == 0) {
          best = 0;
          wraps++;
        } else {
          flag = next->flag + 0x180;
          state = Func_080ec298(next);

          if (state != 0 && state != 15 && GameFlag_Test(flag)) {
            place = next;
            break;
          }
        }
      }
    } while (wraps <= 1);
  }
  *screenX = PLACE_SCREEN_X(place);
  *screenY = PLACE_SCREEN_Y(place);
}

// Moves a list of window sprites.

#include "graphics/render_input.h"

/// Moves every window sprite of a list by dx, dy: the output's position and its
/// queued sprite's OAM x and y.
void Func_080ec484(struct WindowSpriteList *list, s32 dx, s32 dy) {
  struct WindowSprite *output;
  struct QueuedSprite *sprite;

  if (list == NULL)
    return;
  output = list->head;
  while (output != NULL) {
    output->x += dx;
    output->y += dy;
    sprite = &output->sprite;
    output = output->next;
    sprite->oam.x += dx;
    sprite->oam.y += dy;
  }
}
#undef PLACE_SCREEN_X
#undef PLACE_SCREEN_Y

// ---- games/tla/src/field/event_commands/f0ec4d4.c ----
#include "input.h"
#include "main_routine_work.h"
#include "io_write_queue.h"
#include "iwram_call.h"
#include "edition.h"
#include "version.h"
#include "fixed_math.h"
#include "frame.h"

// The scrolled name's left edge, in pixels left of its place.
#if LANGUAGE == LANGUAGE_GERMAN
#define WORLD_MAP_NAME_LEFT 3
#else
#define WORLD_MAP_NAME_LEFT 1
#endif

/* VIEW(f0ec4d4): tag struct WorldMapWork as struct WorldMapWork__f0ec4d4;
 * f0ec2b8.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/world_map_picture.c:221: structure has no
 * member named `scrollX'. */
struct WorldMapWork__f0ec4d4 {
  u16 vramBlock;
  u16 windowSlot;
  s32 x;
  s32 z;
  u8 unk0c[6];
  s16 shown;
  u8 unk14[4];
  s32 speed;
  void *window;
  struct WorldMapMarker markers[129];
  u8 unk62c[4];
  s32 scrollX;
  s32 scrollZ;
};

/* VIEW(f0ec4d4): tag struct WorldMapPlace as struct WorldMapPlace__f0ec4d4;
 * f0ec2b8.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/world_map_picture.c:294: structure has no
 * member named `name'. */
struct WorldMapPlace__f0ec4d4 {
  u8 flag;
  u8 value;
  s16 unk02;
  s16 x;
  s16 z;
  u8 unk08[4];
  s16 name;
  s16 nameEntrance;
  u8 unk10[4];
};

/* VIEW(f0ec2b8): tag struct ResourceTableEntry as struct
 * ResourceTableEntry__f0ec2b8; module/tla.h has another definition; tried with
 * the definition the rest of the object has, its members read unk00 as unk0, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/world_map_picture.c:226: invalid use of
 * undefined type `struct ResourceTableEntry'. */
struct ResourceTableEntry__f0ec2b8 {
  u16 unk00;
  u16 offset;
};

#define WORK Ram_WorldMapWork
/* VIEW(f0ec2b8): variable gResourceTableEntries as struct ResourceTableEntry
 * gResourceTableEntries[]; vram_tab.h has struct VramBlockCacheEntry
 * gResourceTableEntries[]; tried as the 5 declarations' extern struct
 * ResourceTableEntry gResourceTableEntries[], it does not compile in tla-ja:
 * games/tla/src/field/event_commands/world_map_picture.c:230: invalid use of
 * undefined type `struct ResourceTableEntry'. */
extern struct ResourceTableEntry__f0ec2b8
    gResourceTableEntries__f0ec2b8[] __asm__("gResourceTableEntries");
#define NAME_BOX_X Ram_WorldMapNameBox[0]
#define NAME_BOX_Y Ram_WorldMapNameBox[1]
#define NAME_BOX_WIDTH Ram_WorldMapNameBox[2]
#define NAME_BOX_HEIGHT Ram_WorldMapNameBox[3]

void Text_DrawMessageFar(s32 message, s32 window, s32 x, s32 y);

#define WORLD_MAP_HIGH(value) (((s16 *)&(value))[1])
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

void WorldPicture_UpdateMarkers(void) {
  struct WorldMapWork__f0ec4d4 *work;
  struct WorldMapMarker *marker;
  s32 tileBase;
  s32 blend;
  s32 nearest;
  s32 nearestDistance;
  s32 nearestX;
  s32 nearestY;
  s32 name;
  s32 nearestName;
  s32 nearestEntrance;
  s32 entrance;
  struct WorldMapPlace__f0ec4d4 *places;
  s32 scrollX;
  s32 scrollY;
  struct WorldMapPlace__f0ec4d4 *place;
  s32 width;
  s32 height;
  s32 placeX;
  s32 placeY;
  s32 pos[3];
  s32 x;
  s32 y;
  s32 dx;
  s32 dz;
  s32 distance;
  s32 step;
  s32 cursorX;
  s32 cursorY;
  u16 angle;
  s32 tile;
  s32 always;
  s32 flag;
  s32 i;

  Party_GetLeaderActor();
  work = WORK;
  marker = work->markers;
  tileBase = gResourceTableEntries__f0ec2b8[work->vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = 64;
  nearestX = 0;
  nearestY = 0;
  places = FlagEntries_GetTable();
  scrollX = work->scrollX & 0xffff0000;
  scrollY = work->scrollZ & 0xffff0000;
  blend = Data_080f1100[(gFrameCount >> 1) & 31];

  if (Data_0202a644 != 0) {
    x = Data_0202a644;
    y = Data_0202a648;
    dx = (x - work->x) / Q16_ONE;
    dz = (y - work->z) / Q16_ONE;
    distance = Iwram_Sqrt(dx * dx + dz * dz) << 16;
    dx = x - work->x;
    dz = y - work->z;
    if (distance < Q16_ONE * 64)
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
    step = distance / 4;
    if (step > Q16_ONE * 8)
      step = Q16_ONE * 8;
    if (distance < Q16_ONE / 2) {
      work->x = x;
      work->z = y;
      Data_0202a644 = 0;
      Data_0202a648 = 0;
    } else {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), step);
      }
      work->x += dx;
      work->z += dz;
    }
  } else {
    angle = Data_080f1120[(gInput.held >> 4) & 15];
    if (angle != 0xffff) {
      pos[0] = work->x;
      pos[1] = 0;
      pos[2] = work->z;
      Vector_AddPolarOffset(work->speed, angle, pos);
      if (pos[0] < Q16_ONE * 16)
        pos[0] = Q16_ONE * 16;
      if (pos[0] > Q16_ONE * 511)
        pos[0] = Q16_ONE * 511;
      if (pos[2] < 0)
        pos[2] = 0;
      if (pos[2] > Q16_ONE * 365)
        pos[2] = Q16_ONE * 365;
      work->x = pos[0];
      work->z = pos[2];
      Ram_WorldMapCursorStart[0] = pos[0];
      Ram_WorldMapCursorStart[1] = pos[2];
      if (work->speed < Q16_ONE * 6)
        work->speed += Q16_ONE / 8;
    }
  }

  cursorX = WORLD_MAP_HIGH(work->x);
  cursorY = WORLD_MAP_HIGH(work->z);
  place = places;
  i = 0;
  do {
    if (place->flag == 0) {
      if (gData_0202a642 != 0)
        break;
      place = &places[128];
      i = 128;
      name = -1;
      tile = 0;
      always = 0;
      Func_080ec1d0(&placeX, &placeY);
    } else {
      name = place->name;
      entrance = place->nameEntrance;
      flag = place->flag + 0x180;
      tile = Func_080ec298(place);
      if (tile == 0)
        continue;
      if (tile == 15) {
        if (!GameFlag_Test(0x8de))
          continue;
        if (gMainRoutineWork.ownerOverride != 0)
          continue;
        name = -2;
        tile = 2;
        always = 0;
        placeX = (gMainRoutineWork.savedX >> 16) * 853 / 16384 - 269;
        placeY = (gMainRoutineWork.savedZ >> 16) * 640 / 16384 - 112;
      } else {
        if (!GameFlag_Test(flag))
          continue;
        always = 1;
        placeX = place->x * 853 / 16384 - 269;
        placeY = place->z * 640 / 16384 - 112;
      }
    }
    marker->objectMode = always;
    marker->tile = tileBase + tile;
    if (Data_0202a640 == 0) {
      x = placeX - (scrollX >> 16) - 1;
      y = placeY - (scrollY >> 16) - 1;
      if ((u32)(x + 4) > 247)
        continue;
      if (y < -4)
        continue;
      if (y > 163)
        continue;
      marker->x = x & 0x1ff;
      marker->y = y;
    } else {
      marker->x = WORLD_MAP_SCALE(placeX) - 1;
      marker->y = WORLD_MAP_SCALE(placeY) - 1;
    }
    width = placeX - cursorX;
    height = placeY - cursorY;
    if (width * width + height * height < nearestDistance) {
      nearest = i;
      nearestName = name;
      nearestEntrance = entrance;
      nearestDistance = width * width + height * height;
      nearestX = placeX;
      nearestY = placeY;
    }
    if (always != 0 || (gFrameCount & 15) <= 7)
      Func_080140d8(marker++, 246);
  } while (++i, place++, i < 128);

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        work->x = nearestX << 16;
        work->z = nearestY << 16;
        work->speed = Q16_ONE;
      } else {
        angle = ArcTan2(nearestY - cursorY, nearestX - cursorX);
        pos[0] = work->x;
        pos[1] = 0;
        pos[2] = work->z;
        Vector_AddPolarOffset(work->speed, (u16)angle, pos);
        work->x = pos[0];
        work->z = pos[2];
      }
      cursorX = WORLD_MAP_HIGH(work->x);
      cursorY = WORLD_MAP_HIGH(work->z);
    } else {
      work->speed = Q16_ONE;
    }
  }

  if (nearest != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    if (Data_0202a640 == 0) {
      marker->x = nearestX - (scrollX >> 16) - 2;
      marker->y = nearestY - (scrollY >> 16) - 2;
    } else {
      marker->x = WORLD_MAP_SCALE(nearestX) - 2;
      marker->y = WORLD_MAP_SCALE(nearestY) - 2;
    }
    Func_080140d8(marker, 246);
  }

  marker = &Data_0202a620;
  if (Data_0202a640 == 0) {
    marker->affineIndex = 0;
    marker->x = cursorX - (scrollX >> 16) - 17;
    marker->y = cursorY - (scrollY >> 16) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - 17;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Func_080140d8(marker, 246);

  if (work->shown != nearest) {
    // The Japanese release prepares the window and draws the new name in the
    // same frame; the later ones draw it 6 frames after preparing the window.
#if TLA_PATCH_1
    if (Data_0202a654[0] == 0) {
      WindowSprite_PrepareForRedrawFar(work->window);
      Data_0202a654[0] = 1;
      Data_0202a656[0] = 6;
    } else if (--Data_0202a656[0] <= 0) {
      Data_0202a654[0] = 0;
      work->shown = nearest;
#else
    {
      work->shown = nearest;
      WindowSprite_PrepareForRedrawFar(work->window);
#endif
      if (nearest != -1) {
        if (nearestName == -1)
          nearestName = (s32)MsgPresentLocation;
        else if (nearestName == -2)
          nearestName = (s32)MsgBoat;
        else
          nearestName = Scene_GetAreaNameIndex(nearestName, nearestEntrance) +
                        (s32)MsgDebugAreaName;
        Text_MeasureMessageFar(nearestName, &width, &height);
        if (Data_0202a640 == 0) {
          cursorX = nearestX - WORLD_MAP_NAME_LEFT;
          cursorY = nearestY - 11;
          cursorX -= scrollX >> 16;
          cursorY -= scrollY >> 16;
        } else {
          cursorX = WORLD_MAP_SCALE(nearestX) - 1;
          cursorY = WORLD_MAP_SCALE(nearestY) - 11;
        }
        if (cursorX + width > 239) {
          cursorX = 232 - width;
          cursorY += -10;
        }
        if (cursorY < 0)
          cursorY = 0;
        Text_DrawMessageFar(nearestName, work->window, cursorX, cursorY);
        {
          // One local carries width, then height, into the name box.
          s32 value = width;
          NAME_BOX_WIDTH = value;
          value = height;
          NAME_BOX_HEIGHT = value;
        }
        NAME_BOX_X = cursorX;
        NAME_BOX_Y = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = NAME_BOX_WIDTH;
    height = NAME_BOX_HEIGHT;
    if (Data_0202a640 == 0) {
      cursorX = nearestX - WORLD_MAP_NAME_LEFT;
      cursorY = nearestY - 11;
      cursorX -= scrollX >> 16;
      cursorY -= scrollY >> 16;
    } else {
      cursorX = WORLD_MAP_SCALE(nearestX) - 1;
      cursorY = WORLD_MAP_SCALE(nearestY) - 11;
    }
    if (cursorX + width > 239) {
      cursorX = 232 - width;
      cursorY += -10;
    }
    if (cursorY < 0)
      cursorY = 0;
    width = cursorX - NAME_BOX_X;
    /* FAKEMATCH(temporary): the name box's y read through a pointer to a
       struct with an s32 member standing in for its own address, so its
       load may alias width's store to the stack and stays after it; as
       NAME_BOX_Y, 10 of 1044 instructions differ in tla-en. */
    height = cursorY - ((struct WorldMapAliasedHalfword *)&NAME_BOX_Y)->value;
    Func_080ec484(work->window, width, height);
    NAME_BOX_X = cursorX;
    NAME_BOX_Y = cursorY;
  }

  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f00, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000052, ((16 - blend) << 8) | blend, 0x20000);
  IO_WRITE_QUEUE_PUSH(0x04000054, 0, 0x20000);
}
#undef WORLD_MAP_NAME_LEFT
#undef WORK
#undef NAME_BOX_X
#undef NAME_BOX_Y
#undef NAME_BOX_WIDTH
#undef NAME_BOX_HEIGHT
#undef WORLD_MAP_HIGH
#undef WORLD_MAP_SCALE

// ---- games/tla/src/field/event_commands/f0ece20.c ----
#include "dma.h"
#include "resource.h"
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

#include "io_write.h"
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
void WorldPicture_Initialize(void);
void WorldPicture_UpdateMarkers(void);
void Func_080ec1d0(s32 *screenX, s32 *screenY);
void Func_080ec2b8(s32 *screenX, s32 *screenY, s32 step);
void FieldMessage_ShowFar(s32 message, s32 mode);
void WindowSprite_PrepareForRedrawFar(void *window);
void Func_080ca368(s32 scene, s32 entrance);
void WorldPicture_Release(void);
void Func_08020270(void);

s32 WorldPicture_Show(s32 mode) {
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
  const struct FlagEntry *place;
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
  WorldPicture_Initialize();
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
      WorldPicture_UpdateMarkers();
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
  WorldPicture_Release();
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

extern const struct FlagEntry gData_080f1140[];

void WorldMap_StartZoom(s32 scale, s32 frames);

/// On the world map, zooms the view out to eight thirteenths while L is held,
/// then back, six frames each way. FieldSequence_Run runs it for event -889.
void WorldMap_ZoomOutWhileLHeld(void) {
  struct MainRoutineWork *party;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    Leader_StopMoving();
    WorldMap_StartZoom(Q16_ONE * 8 / 13, 6);
    while (gInput.held & KEY_L)
      wait(1);
    party = &gMainRoutineWork;

    if (party->ownerOverride != 0)
      WorldMap_StartZoom(Q16_ONE, 6);
    else
      WorldMap_StartZoom(Q16_ONE, 6);
  }
}

/// Returns the flag entry table, 20-byte entries ended by a zero flag.
const struct FlagEntry *FlagEntries_GetTable(void) { return gData_080f1140; }
#undef BG_VRAM
#undef SCROLL_X
#undef SCROLL_Y
