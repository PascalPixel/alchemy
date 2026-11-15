/// @file
/// The world map screen's picture: drawing it, scrolling it with the cursor,
/// its area markers, and zooming the view out while L is held.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "resource_ids.h"
#include "flags.h"

#include "text/msg_ids.h"
#include "graphics/render_input.h"
#include "system/input.h"
#include "system/io_write_queue.h"
#include "edition.h"
#include "system/dma.h"
#include "graphics/resource.h"
#include "system/bg_scroll.h"
#include "field/world_map.h"
#include "system/io_reg.h"
#include "system/io_write.h"
#include "scene_ids.h"
#include "field/field_sequence_event.h"
#include "field/actor.h"
#include "field/map_grid.h"
#include "field/sprite.h"
#include "field/flag_call.h"

// The world map screen's work, laid out as field/world_map.h has it.

// The place's position on the world map screen.
#define PLACE_SCREEN_X(place) ((place)->x * 853 / 16384 - 269)
#define PLACE_SCREEN_Y(place) ((place)->z * 640 / 16384 - 112)

/// R (step 1) and L (step -1) on the world map screen: the cursor jumps to the
/// next or the previous marked place round from the one nearest it, and its
/// point on the picture is written to screenX and screenY.
void WorldPicture_StepToPlace(s32 *screenX, s32 *screenY, s32 step) {
  const struct FlagEntry *places;
  const struct FlagEntry *place;
  const struct FlagEntry *next;
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
  cursorX = FIELD_PIXEL(work->x);
  cursorY = FIELD_PIXEL(work->z);
  if (work->searchX != 0) {
    cursorX = FIELD_PIXEL(work->searchX);
    cursorY = FIELD_PIXEL(work->searchZ);
  }
  for (count = 0; count < 128; count++) {

    place = &places[count];
    if (place->flag == 0)
      break;
    flag = FLAG_PLACE_VISITED(place->flag);
    state = WorldMapPlace_GetMarker(place);

    if (state != WORLD_MAP_MARKER_NONE && state != WORLD_MAP_MARKER_SHIP &&
        GameFlag_Get(flag)) {
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
    WorldPicture_GetLeaderPoint(screenX, screenY);
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
          flag = FLAG_PLACE_VISITED(next->flag);
          state = WorldMapPlace_GetMarker(next);

          if (state != WORLD_MAP_MARKER_NONE &&
              state != WORLD_MAP_MARKER_SHIP && GameFlag_Get(flag)) {
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

/// Moves every window sprite of a list by dx, dy: the output's position and its
/// queued sprite's OAM x and y.
void WindowSpriteList_Move(struct WindowSpriteList *list, s32 dx, s32 dy) {
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

// The scrolled name's left edge, in pixels left of its place.
#if LANGUAGE == LANGUAGE_GERMAN
#define WORLD_MAP_NAME_LEFT 3
#else
#define WORLD_MAP_NAME_LEFT 1
#endif

#define WORK Ram_WorldMapWork
#define NAME_BOX_X Ram_WorldMapNameBox[0]
#define NAME_BOX_Y Ram_WorldMapNameBox[1]
#define NAME_BOX_WIDTH Ram_WorldMapNameBox[2]
#define NAME_BOX_HEIGHT Ram_WorldMapNameBox[3]

#define WORLD_MAP_HIGH(value) (((s16 *)&(value))[1])
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

/// Task of the World Map screen: shows a marker at each
/// area whose flag is set, moves the cursor with the d-pad (to the current
/// area while L or R is held) and shows the name of the marker nearest
/// the cursor.
void WorldPicture_UpdateMarkers(void) {
  struct WorldMapWork *work;
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
  const struct FlagEntry *places;
  s32 scrollX;
  s32 scrollY;
  const struct FlagEntry *place;
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
  tileBase = gVramCacheEntries[work->vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = 64;
  nearestX = 0;
  nearestY = 0;
  places = FlagEntries_GetTable();
  scrollX = work->scrollX & 0xffff0000;
  scrollY = work->scrollZ & 0xffff0000;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

  if (gWorldPicture_GlideTargetX != 0) {
    x = gWorldPicture_GlideTargetX;
    y = gWorldPicture_GlideTargetY;
    dx = (x - work->x) / Q16_ONE;
    dz = (y - work->z) / Q16_ONE;
    distance = px(Iwram_Sqrt(dx * dx + dz * dz));
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
      gWorldPicture_GlideTargetX = 0;
      gWorldPicture_GlideTargetY = 0;
    } else {
      if (distance > step) {
        dx = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dx), step);
        dz = Iwram_MulQ16(Iwram_RatioMulQ14(distance, dz), step);
      }
      work->x += dx;
      work->z += dz;
    }
  } else {
    angle = gWorldMap_CursorDirectionAngles[DPAD_INDEX(gInput.held)];
    if (angle != WORLD_MAP_KEY_HEADING_NONE) {
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
      WorldPicture_GetLeaderPoint(&placeX, &placeY);
    } else {
      name = place->visitScene;
      entrance = place->visitEntrance;
      flag = FLAG_PLACE_VISITED(place->flag);
      tile = WorldMapPlace_GetMarker(place);
      if (tile == WORLD_MAP_MARKER_NONE)
        continue;
      if (tile == WORLD_MAP_MARKER_SHIP) {
        if (!GameFlag_Get(FLAG_SHIP_SAILABLE))
          continue;
        if (gMainRoutineWork.ownerOverride != 0)
          continue;
        name = -2;
        tile = 2;
        always = 0;
        placeX = FIELD_PIXEL(gMainRoutineWork.shipX) * 853 / 16384 - 269;
        placeY = FIELD_PIXEL(gMainRoutineWork.shipZ) * 640 / 16384 - 112;
      } else {
        if (!GameFlag_Get(flag))
          continue;
        always = 1;
        placeX = place->x * 853 / 16384 - 269;
        placeY = place->z * 640 / 16384 - 112;
      }
    }
    marker->objectMode = always;
    marker->tile = tileBase + tile;
    if (gWorldPicture_Zoomed == 0) {
      x = placeX - FIELD_PIXEL(scrollX) - 1;
      y = placeY - FIELD_PIXEL(scrollY) - 1;
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
      Oam_AddSpriteAtDepthClamped(marker++, 246);
  } while (++i, place++, i < 128);

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        work->x = px(nearestX);
        work->z = px(nearestY);
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
    if (gWorldPicture_Zoomed == 0) {
      marker->x = nearestX - FIELD_PIXEL(scrollX) - 2;
      marker->y = nearestY - FIELD_PIXEL(scrollY) - 2;
    } else {
      marker->x = WORLD_MAP_SCALE(nearestX) - 2;
      marker->y = WORLD_MAP_SCALE(nearestY) - 2;
    }
    Oam_AddSpriteAtDepthClamped(marker, 246);
  }

  marker = &gWorldPicture_CursorMarker;
  if (gWorldPicture_Zoomed == 0) {
    marker->affineIndex = 0;
    marker->x = cursorX - FIELD_PIXEL(scrollX) - 17;
    marker->y = cursorY - FIELD_PIXEL(scrollY) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - 17;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Oam_AddSpriteAtDepthClamped(marker, 246);

  if (work->shown != nearest) {
    // The Japanese release prepares the window and draws the new name in the
    // same frame; the later ones draw it 6 frames after preparing the window.
#if TLA_PATCH_1
    if (gWorldPicture_NameRedrawPending[0] == 0) {
      WindowSprite_PrepareForRedrawFar(work->window);
      gWorldPicture_NameRedrawPending[0] = 1;
      gWorldPicture_NameRedrawDelay[0] = 6;
    } else if (--gWorldPicture_NameRedrawDelay[0] <= 0) {
      gWorldPicture_NameRedrawPending[0] = 0;
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
          nearestName = Area_GetNameIndex(nearestName, nearestEntrance) +
                        (s32)MsgDebugAreaName;
        Text_MeasureMessageFar(nearestName, &width, &height);
        if (gWorldPicture_Zoomed == 0) {
          cursorX = nearestX - WORLD_MAP_NAME_LEFT;
          cursorY = nearestY - 11;
          cursorX -= FIELD_PIXEL(scrollX);
          cursorY -= FIELD_PIXEL(scrollY);
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
    if (gWorldPicture_Zoomed == 0) {
      cursorX = nearestX - WORLD_MAP_NAME_LEFT;
      cursorY = nearestY - 11;
      cursorX -= FIELD_PIXEL(scrollX);
      cursorY -= FIELD_PIXEL(scrollY);
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
    /* FAKEMATCH(temporary): the name box's y read through a pointer to a struct
     * with an s32 member standing in for its own address. */
    height = cursorY - ((struct WorldMapAliasedHalfword *)&NAME_BOX_Y)->value;
    WindowSpriteList_Move(work->window, width, height);
    NAME_BOX_X = cursorX;
    NAME_BOX_Y = cursorY;
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 0, IO_WRITE_HALFWORD);
}
#undef WORLD_MAP_NAME_LEFT
#undef WORK
#undef NAME_BOX_X
#undef NAME_BOX_Y
#undef NAME_BOX_WIDTH
#undef NAME_BOX_HEIGHT
#undef WORLD_MAP_HIGH
#undef WORLD_MAP_SCALE

/// Draws the world map picture on BG1, scrolled to the cursor in gWorldMapWork
/// (resource 36, or 35 at the origin when gWorldMapWork's origin flag is set):
/// the palette, then 21 rows of 31 tiles from the decoded rows, wrapping the
/// 24-row buffer and the 32-column screen.
void WorldMapPicture_Draw(void) {
  struct WorldMapWork *work = &gWorldMapWork;
  u8 *buffer;
  s32 resource = (s32)&ResourceId_WorldMapPictureLarge;
  s32 *cursor = work->cursor;
  s32 x = *cursor++ - px(120);
  s32 z = *cursor - px(80);
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
    resource = (s32)&ResourceId_WorldMapPicture;
    x = 0;
    z = 0;
  }
  graphics = Resource_GetTableEntry(resource);
  Dma_Set((u8 *)graphics + graphics[0], (void *)BG_PLTT, DMA_COPY_WORDS(120),
          REG_DMA3);
  *(u16 *)BG_PLTT = 0;
  buffer = Ram_MapCellBuffer;
  right = px(272);
  bottom = px(224);
  if (x < 0)
    x = 0;
  if (x > right)
    x = right;
  if (z < 0)
    z = 0;
  if (z > bottom)
    z = bottom;
  gBgScroll[1].x = FIELD_PIXEL(x);
  gBgScroll[1].y = FIELD_PIXEL(z);
  x = (u32)x >> 19;
  column = x;
  firstRow = (u32)z >> 19;
  for (row = 0; row <= 20; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    /* FAKEMATCH(barrier): an empty asm reading x. */
    asm("" : : "r"(x));
    if (bufferRow > 23)
      bufferRow -= 24;
    // z is reused for the map cell's index; a local of its own differs.
    z = (column & 31) + (unwrapped << 5);
    tile = (z + 128) % 768 + 256;
    Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
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
      /* FAKEMATCH(barrier): VRAM's address through an empty asm that also reads
       * i. */
      asm("" : "=r"(to) : "0"(to), "r"(i));
      for (; i <= 30; i++) {
        Dma_Set((void *)x, (void *)((tile << 6) + to), DMA_COPY_WORDS(16),
                REG_DMA3);
        x += 64;
        tile++;
        if (i + column == 31)
          tile -= 32;
      }
    }
  }
}

/// Set while the world map shows its alternative picture, which does not
/// scroll.

#define BG_VRAM 0x06000000

/// Draws one row of the world map picture on BG1, from a column to the right
/// edge of the map (at most 31 tiles; Japanese always draws 32), for
/// scrolling: decodes the picture's row into the cell buffer, which holds 24
/// rows and wraps, and copies its cells into the 32-column screen from tile
/// 256, wrapping at column 32.
void WorldMapPicture_DrawRow(u32 column, s32 row) {
  s32 resource = (s32)&ResourceId_WorldMapPictureLarge;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u32 i;

  if (gWorldPicture_Zoomed != 0)
    resource = (s32)&ResourceId_WorldMapPicture;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row > 23)
    row -= 24;
  cell = (row << 6) + column;
  tile = (column & 31) + (row << 5);
  tile = (tile + 128) % 768 + 256;
  Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
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

/// Adds a cell to a column number.
static inline s32 WorldMap_AddColumn(s32 column, s32 cell) {
  /* FAKEMATCH(wrapper): the column added through an inline. */
  return column + cell;
}

/// Draws one column of the world map picture on BG1, 21 rows down from a
/// row, for scrolling: copies the column's cells from the decoded rows in the
/// cell buffer, which wraps after 24 rows, into the 32-column screen from
/// tile 256; the column twin of WorldMapPicture_DrawRow.
void WorldMapPicture_DrawColumn(s32 column, s32 row) {
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
    line = BG_VRAM + (256 << 6);
    Dma_Set(Ram_MapCellBuffer + cell, (u8 *)line + tile, DMA_COPY_WORDS(16),
            REG_DMA3);
  }
}

/// The world map cursor's position (x, then y, 16.16 pixels).
extern s32 *gWorldPicture_Cursor;

#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/// The world map screen's scroll follows the cursor. The target puts the
/// cursor 120 by 80 pixels in (clamped to the map); unless the map is held the
/// scroll moves a quarter of the way there each frame, at most 8 pixels, and
/// stops when nearer than a quarter pixel. Each time the scroll crosses an
/// 8-pixel column or row, the new column (WorldMapPicture_DrawColumn) or row
/// (WorldMapPicture_DrawRow) of the picture is drawn; BG1 scrolls with it. All
/// values are 16.16 pixels.
void WorldMapPicture_UpdateScroll(void) {
  s32 *cursor = gWorldPicture_Cursor;
  s32 x = *cursor++ - px(120);
  s32 y = *cursor - px(80);
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
  if (gWorldPicture_Zoomed == 0) {
    dx = (x - SCROLL_X) / Q16_ONE;
    dy = (y - SCROLL_Y) / Q16_ONE;
    distance = px(Iwram_Sqrt(dx * dx + dy * dy));
    dx = x - SCROLL_X;
    dy = y - SCROLL_Y;
    // Near the target the whole-pixel distance is too coarse.
    if (distance < px(64))
      distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dy, dy));
    step = distance / 4;
    if (step > px(8))
      step = px(8);
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
        WorldMapPicture_DrawColumn(column + 30, row);
      else
        WorldMapPicture_DrawColumn(column, row);
    }
    if ((SCROLL_Y ^ y) & 0x80000) {
      if (SCROLL_Y < y)
        WorldMapPicture_DrawRow(column, row + 20);
      else
        WorldMapPicture_DrawRow(column, row);
    }
    gBgScroll[1].x = FIELD_PIXEL(x);
    gBgScroll[1].y = FIELD_PIXEL(y);
  }
  SCROLL_X = x;
  SCROLL_Y = y;
}

extern s32 gWorldPicture_CursorStartX;
extern s32 gWorldPicture_CursorStartY;
extern s32 gWorldPicture_ScrollX;
extern s32 gWorldPicture_ScrollY;

void WorldMap_SetReturnForScene(s32 scene, s32 entrance);

/// Borrows the map-cell buffer for the still map until A or B closes it.
/// FieldSequence_Run reaches this for raised trigger -888 in World Map mode.
s32 WorldPicture_Show(s32 mode) {
  s32 savedDelay;
  u8 savedFlags[16];
  s32 screenX;
  s32 screenY;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct ObjectSystemWork *objects = Ram_HeapSlots->actorSystem;
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
  if (mode != 0 || event->mode != ACTOR_SYSTEM_WORLD_MAP)
    ObjectTable_Snapshot();
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    savedDelay = event->transitionFrames;
    event->transitionFrames = 6;
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
    ((struct WorldMapView *)map)->viewHeld = 1;
    do {
      /* FAKEMATCH(dead): the read in a once-only block; as the plain statement,
       * 12 of 548 instructions differ in tla-en. */
      blend = REG_BLDALPHA;
    } while (0);
  } else {
    Io_Write8(1, &event->camera->paused);
    savedDelay = event->transitionFrames;
    event->transitionFrames = 6;
    event->transition = 0;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Task_ResetAll();
    WindowSystem_InitFieldFar();
    objects->suspended = 1;
    wait(1);
    blend = REG_BLDALPHA;
    tiles = (u32 *)BG_SCREEN_BLOCK(6);
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
    Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6),
               &REG_BG1CNT);
    Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                   DISPCNT_OBJ_ON,
               &REG_DISPCNT);
  }
  // Default: no pointer yet.
  wayX = NULL;
  cursorX = NULL;
  cursorZ = NULL;
  scrollX = NULL;
  scrollZ = NULL;
  WorldPicture_Initialize();
  for (;;) {
    gWorldPicture_Zoomed = zoomed;
    WorldMapPicture_Draw();
    wait(1);
    Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) |
                   BGCNT_SIZE(2),
               &REG_BG1CNT);
    Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                   DISPCNT_OBJ_ON,
               &REG_DISPCNT);
    if ((GameFlag_Get(FLAG_WORLD_MAP_LEFT) || GameFlag_Get(0x11e)) &&
        !messageShown) {
      FieldMessage_ShowFar((s32)MsgNotOnMap, FIELD_MESSAGE_ZOOM);
      messageShown = 1;
    }
    keys = &gInput;
    wayX = &gWorldPicture_CursorStartX;
    cursorX = &gWorldPicture_GlideTargetX;
    cursorZ = &gWorldPicture_GlideTargetY;
    for (;;) {
      WorldMapPicture_UpdateScroll();
      WorldPicture_UpdateMarkers();
      input = keys;
      wait(1);
      if ((input->repeat & KEY_START) && gliding == 0) {
        WorldPicture_GetLeaderPoint(&screenX, &screenY);
        *cursorX = px(screenX);
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the load.
        input = &gInput;
        *cursorZ = px(screenY);
        gliding = 1;
      }
      if (!(keys->held & KEY_START) && gliding == 1) {
        *cursorX = *wayX;
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the reload.
        input = &gInput;
        *cursorZ = gWorldPicture_CursorStartY;
        gliding = 0;
      }
      if (keys->repeat & KEY_R) {
        WorldPicture_StepToPlace(&screenX, &screenY, 1);
        *cursorX = px(screenX);
        // wayZ only ever holds &gWorldPicture_CursorStartY, set where it is
        // used.
        wayZ = &gWorldPicture_CursorStartY;
        *cursorZ = px(screenY);
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & KEY_L) {
        WorldPicture_StepToPlace(&screenX, &screenY, -1);
        *cursorX = px(screenX);
        wayZ = &gWorldPicture_CursorStartY;
        *cursorZ = px(screenY);
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & KEY_SELECT) {
        world->shown = -1;
        WindowSprite_PrepareForRedrawFar(world->window);
        Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
        zoomed ^= 1;
        if (*cursorX != 0) {
          world->x = *cursorX;
          world->z = *cursorZ;
          zero = 0;
          limitX = 0x1100000;
          limitZ = 0xe00000;
          scrollX = &gWorldPicture_ScrollX;
          *scrollX = world->x - 0x780000;
          scrollZ = &gWorldPicture_ScrollY;
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
      if (input->repeat & KEY_A) {
        if (mode == 0)
          goto done;
        if (world->shown == -1)
          continue;
        place = FlagEntries_GetTable();
        place += world->shown;
        routine->teleportScene = SCENE_WORLD_MAP;
        choice->flag = place->flag;
        WorldMap_SetReturnForScene(place->scene, place->entrance);
        goto done;
      }
      if (input->repeat & KEY_B) {
        if (mode != 0) {
          routine->teleportScene = -1;
          {
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
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  if (mode != 0 || event->mode != ACTOR_SYSTEM_WORLD_MAP) {
    event->exit = EXIT_KEEP_DESTINATION;
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  } else {
    WorldMap_LoadAreaGraphicsFar();
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, IO_WRITE_HALFWORD);
    layer = map->anim;
    for (i = 0; i < 16; i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    objects->suspended = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    ((struct WorldMapView *)map)->viewHeld = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    event->transitionFrames = savedDelay;
    event->camera->paused = 0;
  }
  return 0;
}

extern const struct FlagEntry gFlagEntries[];

/// On the world map, zooms the view out to eight thirteenths while L is held,
/// then back, six frames each way. FieldSequence_Run runs it for event -889.
void WorldMap_ZoomOutWhileLHeld(void) {
  struct MainRoutineWork *party;

  if (gFieldWork->mode == ACTOR_SYSTEM_WORLD_MAP) {
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
const struct FlagEntry *FlagEntries_GetTable(void) { return gFlagEntries; }
#undef BG_VRAM
#undef SCROLL_X
#undef SCROLL_Y
