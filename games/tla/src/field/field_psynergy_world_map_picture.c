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

/// The world picture on BG1, in 256 colours. The large picture
/// (RESOURCE_WORLD_MAP_PICTURE_LARGE) is 64 by 48 tiles, each row of tiles
/// packed on its own and decoded into the map-cell buffer, which holds 24 rows
/// and wraps. BG1 holds 32 by 24 of its tiles from char block 1 on, wrapping
/// the same way, and BG1's map (WorldMap_LoadGraphics) numbers them from the
/// fifth row, where the still picture starts.
#define PICTURE_WIDTH 512
#define PICTURE_HEIGHT 384
#define PICTURE_TILES_ACROSS (PICTURE_WIDTH / 8)
#define PICTURE_ROW_BYTES (PICTURE_TILES_ACROSS * TILE_SIZE_8BPP)
/// The picture's tiles BG1 holds, where its map's tile numbers wrap.
#define WORLD_PICTURE_TILES (WORLD_PICTURE_TILE_ROWS * BG_SCREEN_WIDTH)
/// The scroll that shows the picture's bottom right corner.
#define PICTURE_SCROLL_MAX_X toFIXED(PICTURE_WIDTH - DISPLAY_WIDTH)
#define PICTURE_SCROLL_MAX_Y toFIXED(PICTURE_HEIGHT - DISPLAY_HEIGHT)
/// The tiles drawn for one screen: 30 by 20, and the column and row partly
/// scrolled in.
#define PICTURE_DRAWN_COLUMNS (DISPLAY_WIDTH / 8 + 1)
#define PICTURE_DRAWN_ROWS (DISPLAY_HEIGHT / 8 + 1)

/// The cursor's limits on the picture, in pixels; y goes from 0.
///@{
#define WORLD_MAP_CURSOR_MIN_X 16
#define WORLD_MAP_CURSOR_MAX_X (PICTURE_WIDTH - 1)
#define WORLD_MAP_CURSOR_MAX_Y 365
///@}
#define WORLD_MAP_CURSOR_MIN_SPEED toFIXED(1) ///< pixels a frame
#define WORLD_MAP_CURSOR_MAX_SPEED toFIXED(6) ///< pixels a frame
/// Added to the cursor's speed each frame the d-pad is held.
#define WORLD_MAP_CURSOR_ACCELERATION (toFIXED(1) / 8)
/// How near the cursor a marker's name shows, in pixels.
#define WORLD_MAP_NAME_REACH 8
#define WORLD_MAP_MARKER_BLEND_STEPS 32 ///< gWorldMap_MarkerBlendCycle's rows

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
  bestDistance = 8192 * 8192;
  best = -1;
  cursorX = FIXEDtoINT(work->x);
  cursorY = FIXEDtoINT(work->z);
  if (work->searchX != 0) {
    cursorX = FIXEDtoINT(work->searchX);
    cursorY = FIXEDtoINT(work->searchZ);
  }
  for (count = 0; count < WORLD_PICTURE_PLACE_COUNT; count++) {
    place = &places[count];
    if (place->flag == 0)
      break;
    flag = FLAG_PLACE_VISITED(place->flag);
    state = WorldMapPlace_GetMarker(place);

    if (state != WORLD_MAP_MARKER_NONE && state != WORLD_MAP_MARKER_SHIP &&
        GameFlag_Get(flag)) {
      s32 sx = WORLD_PICTURE_X(place->x);
      s32 sy = WORLD_PICTURE_Y(place->z);
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
  *screenX = WORLD_PICTURE_X(place->x);
  *screenY = WORLD_PICTURE_Y(place->z);
}

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

// The scrolled name's left edge, in pixels left of its place.
#if LANGUAGE == LANGUAGE_GERMAN
#define WORLD_MAP_NAME_LEFT 3
#else
#define WORLD_MAP_NAME_LEFT 1
#endif

#define WORLD_MAP_HIGH(value) (((s16 *)&(value))[1])
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

/// Each frame of the world map screen: moves the cursor with the d-pad, faster
/// the longer it is held, or glides it toward its target; draws a marker at
/// each place visited, at the ship once it sails and at the party; with the
/// d-pad released draws the cursor onto the nearest marker in reach, and shows
/// that marker's name beside it.
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
  FIXED pos[3];
  s32 x;
  s32 y;
  s32 dx;
  s32 dz;
  FIXED distance;
  FIXED step;
  s32 cursorX;
  s32 cursorY;
  u16 angle;
  s32 tile;
  s32 always;
  s32 flag;
  s32 i;

  Party_GetLeaderActor();
  work = Ram_WorldMapWork;
  marker = work->markers;
  tileBase = gVramCacheEntries[work->vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = WORLD_MAP_NAME_REACH * WORLD_MAP_NAME_REACH;
  nearestX = 0;
  nearestY = 0;
  places = FlagEntries_GetTable();
  scrollX = work->scrollX & FIELD_PIXEL_MASK;
  scrollY = work->scrollZ & FIELD_PIXEL_MASK;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) &
                                     (WORLD_MAP_MARKER_BLEND_STEPS - 1)];

  if (gWorldPicture_GlideTargetX != 0) {
    x = gWorldPicture_GlideTargetX;
    y = gWorldPicture_GlideTargetY;
    dx = (x - work->x) / FIXED_ONE;
    dz = (y - work->z) / FIXED_ONE;
    distance = toFIXED(Iwram_Sqrt(dx * dx + dz * dz));
    dx = x - work->x;
    dz = y - work->z;
    if (distance < toFIXED(64))
      distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
    step = distance / 4;
    if (step > toFIXED(8))
      step = toFIXED(8);
    if (distance < toFIXED(1) / 2) {
      work->x = x;
      work->z = y;
      gWorldPicture_GlideTargetX = 0;
      gWorldPicture_GlideTargetY = 0;
    } else {
      if (distance > step) {
        dx = Iwram_MulFX(Iwram_RatioMulQ14(distance, dx), step);
        dz = Iwram_MulFX(Iwram_RatioMulQ14(distance, dz), step);
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
      if (pos[0] < toFIXED(WORLD_MAP_CURSOR_MIN_X))
        pos[0] = toFIXED(WORLD_MAP_CURSOR_MIN_X);
      if (pos[0] > toFIXED(WORLD_MAP_CURSOR_MAX_X))
        pos[0] = toFIXED(WORLD_MAP_CURSOR_MAX_X);
      if (pos[2] < 0)
        pos[2] = 0;
      if (pos[2] > toFIXED(WORLD_MAP_CURSOR_MAX_Y))
        pos[2] = toFIXED(WORLD_MAP_CURSOR_MAX_Y);
      work->x = pos[0];
      work->z = pos[2];
      Ram_WorldMapCursorStart[0] = pos[0];
      Ram_WorldMapCursorStart[1] = pos[2];
      if (work->speed < WORLD_MAP_CURSOR_MAX_SPEED)
        work->speed += WORLD_MAP_CURSOR_ACCELERATION;
    }
  }

  cursorX = WORLD_MAP_HIGH(work->x);
  cursorY = WORLD_MAP_HIGH(work->z);
  place = places;
  i = 0;
  do {
    if (place->flag == 0) {
      if (gWorldPicture_FirstListOnly != 0)
        break;
      place = &places[WORLD_PICTURE_PLACE_COUNT];
      i = WORLD_PICTURE_PLACE_COUNT;
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
        if (!GameFlag_Get(FLAG_SHIP_CAN_SAIL))
          continue;
        if (gMainRoutineWork.ownerOverride != 0)
          continue;
        name = -2;
        tile = 2;
        always = 0;
        placeX = WORLD_PICTURE_X(FIXEDtoINT(gMainRoutineWork.shipX));
        placeY = WORLD_PICTURE_Y(FIXEDtoINT(gMainRoutineWork.shipZ));
      } else {
        if (!GameFlag_Get(flag))
          continue;
        always = 1;
        placeX = WORLD_PICTURE_X(place->x);
        placeY = WORLD_PICTURE_Y(place->z);
      }
    }
    marker->objectMode = always;
    marker->tile = tileBase + tile;
    if (gWorldPicture_Zoomed == 0) {
      x = placeX - FIXEDtoINT(scrollX) - 1;
      y = placeY - FIXEDtoINT(scrollY) - 1;
      if ((u32)(x + 4) > 247)
        continue;
      if (y < -4)
        continue;
      if (y > 163)
        continue;
      marker->x = x & OBJ_X_MASK;
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
      Oam_AddSpriteAtDepthClamped(marker++, WORLD_PICTURE_MARKER_DEPTH);
  } while (++i, place++, i < WORLD_PICTURE_PLACE_COUNT);

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        work->x = toFIXED(nearestX);
        work->z = toFIXED(nearestY);
        work->speed = WORLD_MAP_CURSOR_MIN_SPEED;
      } else {
        angle = Trig_Atan(nearestY - cursorY, nearestX - cursorX);
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
      work->speed = WORLD_MAP_CURSOR_MIN_SPEED;
    }
  }

  if (nearest != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    if (gWorldPicture_Zoomed == 0) {
      marker->x = nearestX - FIXEDtoINT(scrollX) - 2;
      marker->y = nearestY - FIXEDtoINT(scrollY) - 2;
    } else {
      marker->x = WORLD_MAP_SCALE(nearestX) - 2;
      marker->y = WORLD_MAP_SCALE(nearestY) - 2;
    }
    Oam_AddSpriteAtDepthClamped(marker, WORLD_PICTURE_MARKER_DEPTH);
  }

  marker = &gWorldPicture_CursorMarker;
  if (gWorldPicture_Zoomed == 0) {
    marker->affineIndex = 0;
    marker->x = cursorX - FIXEDtoINT(scrollX) - 17;
    marker->y = cursorY - FIXEDtoINT(scrollY) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - 17;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Oam_AddSpriteAtDepthClamped(marker, WORLD_PICTURE_MARKER_DEPTH);

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
          cursorX -= FIXEDtoINT(scrollX);
          cursorY -= FIXEDtoINT(scrollY);
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
        work->nameBox.width = width;
        work->nameBox.height = height;
        work->nameBox.x = cursorX;
        work->nameBox.y = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = work->nameBox.width;
    height = work->nameBox.height;
    if (gWorldPicture_Zoomed == 0) {
      cursorX = nearestX - WORLD_MAP_NAME_LEFT;
      cursorY = nearestY - 11;
      cursorX -= FIXEDtoINT(scrollX);
      cursorY -= FIXEDtoINT(scrollY);
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
    width = cursorX - work->nameBox.x;
    height = cursorY - work->nameBox.y;
    WindowSpriteList_Move(work->window, width, height);
    work->nameBox.x = cursorX;
    work->nameBox.y = cursorY;
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      ((BLEND_WEIGHT_MAX - blend) << 8) | blend,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 0, IO_WRITE_HALFWORD);
}
#undef WORLD_MAP_NAME_LEFT
#undef WORLD_MAP_HIGH
#undef WORLD_MAP_SCALE

/// Draws the world picture on BG1 with the cursor in the middle of the screen
/// (or the whole world, RESOURCE_WORLD_MAP_PICTURE, at the origin while
/// gWorldMapWork.centered is set): the palette, then 21 rows of 31 tiles.
void WorldMapPicture_Draw(void) {
  struct WorldMapWork *work = &gWorldMapWork;
  u8 *buffer;
  s32 resource = RESOURCE_WORLD_MAP_PICTURE_LARGE;
  s32 *cursor = work->cursor;
  s32 x = *cursor++ - toFIXED(DISPLAY_WIDTH / 2);
  s32 z = *cursor - toFIXED(DISPLAY_HEIGHT / 2);
  u32 *graphics;
  u32 column;
  u32 firstRow;
  u32 row;
  s32 bufferRow;
  s32 unwrapped;
  u32 i;
  FIXED right;
  FIXED bottom;
  s32 tile;
  s32 cell;

  if (work->centered != 0) {
    resource = RESOURCE_WORLD_MAP_PICTURE;
    x = 0;
    z = 0;
  }
  graphics = Resource_GetTableEntry(resource);
  Dma_Set((u8 *)graphics + graphics[0], (void *)BG_PLTT, DMA_COPY_WORDS(120),
          REG_DMA3);
  *(u16 *)BG_PLTT = 0;
  buffer = Ram_MapCellBuffer;
  right = PICTURE_SCROLL_MAX_X;
  bottom = PICTURE_SCROLL_MAX_Y;
  if (x < 0)
    x = 0;
  if (x > right)
    x = right;
  if (z < 0)
    z = 0;
  if (z > bottom)
    z = bottom;
  gBgScroll[1].x = FIXEDtoINT(x);
  gBgScroll[1].y = FIXEDtoINT(z);
  x = FIELD_HALF_CELL((u32)x);
  column = x;
  firstRow = FIELD_HALF_CELL((u32)z);
  for (row = 0; row < PICTURE_DRAWN_ROWS; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    if (bufferRow >= WORLD_PICTURE_TILE_ROWS)
      bufferRow -= WORLD_PICTURE_TILE_ROWS;
    cell = bufferRow * PICTURE_TILES_ACROSS + column;
    // z is reused for the tile's place in BG1's map.
    z = (column & (BG_SCREEN_WIDTH - 1)) + unwrapped * BG_SCREEN_WIDTH;
    tile = (z + WORLD_PICTURE_MAP_START) % WORLD_PICTURE_TILES +
           WORLD_PICTURE_FIRST_TILE;
    Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
                        buffer + bufferRow * PICTURE_ROW_BYTES);
    for (i = 0; i < PICTURE_DRAWN_COLUMNS; i++) {
      Dma_Set(buffer + cell * TILE_SIZE_8BPP,
              (void *)(VRAM + tile * TILE_SIZE_8BPP),
              DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
      cell++;
      tile++;
      // BG1's map is 32 tiles wide: the next column wraps to its left edge.
      if (i + column == BG_SCREEN_WIDTH - 1)
        tile -= BG_SCREEN_WIDTH;
    }
  }
}

/// Draws one row of the world picture on BG1 as it scrolls in, from a column to
/// the picture's right edge, at most 31 tiles (the Japanese release always
/// draws 32): decodes the row into the map-cell buffer and copies its tiles to
/// BG1's.
void WorldMapPicture_DrawRow(u32 column, s32 row) {
  s32 resource = RESOURCE_WORLD_MAP_PICTURE_LARGE;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u32 i;

  if (gWorldPicture_Zoomed != 0)
    resource = RESOURCE_WORLD_MAP_PICTURE;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row >= WORLD_PICTURE_TILE_ROWS)
    row -= WORLD_PICTURE_TILE_ROWS;
  cell = row * PICTURE_TILES_ACROSS + column;
  tile = (column & (BG_SCREEN_WIDTH - 1)) + row * BG_SCREEN_WIDTH;
  tile = (tile + WORLD_PICTURE_MAP_START) % WORLD_PICTURE_TILES +
         WORLD_PICTURE_FIRST_TILE;
  Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
                      Ram_MapCellBuffer + row * PICTURE_ROW_BYTES);
  // From patch 1 the copy stops at the picture's last column instead of
  // running past it.
#if TLA_PATCH_1
  for (i = 0; i < PICTURE_DRAWN_COLUMNS && column + i < PICTURE_TILES_ACROSS;
       i++) {
#else
  for (i = 0; i < BG_SCREEN_WIDTH; i++) {
#endif
    Dma_Set(Ram_MapCellBuffer + cell * TILE_SIZE_8BPP,
            (void *)(VRAM + tile * TILE_SIZE_8BPP),
            DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
    cell++;
    tile++;
    if (i + column == BG_SCREEN_WIDTH - 1)
      tile -= BG_SCREEN_WIDTH;
  }
}

/// Draws one column of the world picture on BG1 as it scrolls in, 21 tiles
/// down from a row, from the rows already decoded in the map-cell buffer.
void WorldMapPicture_DrawColumn(s32 column, s32 row) {
  u32 i;

  for (i = 0; i < PICTURE_DRAWN_ROWS; i++) {
    s32 line = row + i;
    s32 cell = line;
    s32 tile;
    s32 rowStart;

    if (line >= WORLD_PICTURE_TILE_ROWS)
      cell = line - WORLD_PICTURE_TILE_ROWS;
    tile = ((column & (BG_SCREEN_WIDTH - 1)) + line * BG_SCREEN_WIDTH +
            WORLD_PICTURE_MAP_START) %
           WORLD_PICTURE_TILES;
    cell *= PICTURE_TILES_ACROSS;
    /* FAKEMATCH(temporary): the row's first cell copied before the column is
     * added to it; as cell = column + cell. */
    rowStart = cell;
    cell = column + rowStart;
    cell *= TILE_SIZE_8BPP;
    tile *= TILE_SIZE_8BPP;
    Dma_Set(Ram_MapCellBuffer + cell,
            (u8 *)(VRAM + WORLD_PICTURE_FIRST_TILE * TILE_SIZE_8BPP) + tile,
            DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
    /* FAKEMATCH(dead): line set to 0 at the step's end, where nothing reads
     * it; as no store. */
    line = 0;
  }
}

/// The world map cursor's position (x, then y, 16.16 pixels).
extern FIXED *gWorldPicture_Cursor;

#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/// The world map screen's scroll follows the cursor. The target puts the
/// cursor in the middle of the screen (clamped to the picture); unless the
/// whole world shows, the scroll moves a quarter of the way there each frame,
/// at most 8 pixels, and stops when nearer than a quarter pixel. Each time the
/// scroll crosses into a new column or row of tiles, that column
/// (WorldMapPicture_DrawColumn) or row (WorldMapPicture_DrawRow) is drawn; BG1
/// scrolls with it.
void WorldMapPicture_UpdateScroll(void) {
  s32 *cursor = gWorldPicture_Cursor;
  s32 x = *cursor++ - toFIXED(DISPLAY_WIDTH / 2);
  s32 y = *cursor - toFIXED(DISPLAY_HEIGHT / 2);
  s32 dx;
  s32 dy;
  FIXED distance;
  FIXED step;
  u32 column;
  u32 row;
  s32 limit;
  s32 bottom = PICTURE_SCROLL_MAX_Y;

  if (x < 0)
    x = 0;
  limit = PICTURE_SCROLL_MAX_X;
  if (x > limit)
    x = limit;
  if (y < 0)
    y = 0;
  if (y > bottom)
    y = bottom;
  if (gWorldPicture_Zoomed == 0) {
    dx = (x - SCROLL_X) / FIXED_ONE;
    dy = (y - SCROLL_Y) / FIXED_ONE;
    distance = toFIXED(Iwram_Sqrt(dx * dx + dy * dy));
    dx = x - SCROLL_X;
    dy = y - SCROLL_Y;
    // Near the target the whole-pixel distance is too coarse.
    if (distance < toFIXED(64))
      distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dy, dy));
    step = distance / 4;
    if (step > toFIXED(8))
      step = toFIXED(8);
    if (distance >= toFIXED(1) / 4) {
      if (distance > step) {
        dx = Iwram_MulFX(Iwram_RatioMulQ14(distance, dx), step);
        dy = Iwram_MulFX(Iwram_RatioMulQ14(distance, dy), step);
      }
      x = SCROLL_X + dx;
      y = SCROLL_Y + dy;
      if (x < 0)
        x = 0;
      if (y < 0)
        y = 0;
      if (x > limit)
        x = limit;
      // DEAD: y held to x's limit, which a scroll toward a target within the
      // picture never passes; the ROM keeps the compare and the store.
      if (y > limit)
        y = limit;
    }
    column = FIELD_HALF_CELL((u32)x);
    row = FIELD_HALF_CELL((u32)y);
    if ((SCROLL_X ^ x) & toFIXED(8)) {
      if (SCROLL_X < x)
        WorldMapPicture_DrawColumn(column + DISPLAY_WIDTH / 8, row);
      else
        WorldMapPicture_DrawColumn(column, row);
    }
    if ((SCROLL_Y ^ y) & toFIXED(8)) {
      if (SCROLL_Y < y)
        WorldMapPicture_DrawRow(column, row + DISPLAY_HEIGHT / 8);
      else
        WorldMapPicture_DrawRow(column, row);
    }
    gBgScroll[1].x = FIXEDtoINT(x);
    gBgScroll[1].y = FIXEDtoINT(y);
  }
  SCROLL_X = x;
  SCROLL_Y = y;
}

/// Where the cursor goes back to when Start is let go: the place R or L last
/// stepped to, 16.16 pixels.
extern s32 gWorldPicture_CursorStartX;
extern s32 gWorldPicture_CursorStartY; ///< as gWorldPicture_CursorStartX
/// The world map picture's scroll as Select toggles the zoom, 16.16 pixels.
extern s32 gWorldPicture_ScrollX;
extern s32 gWorldPicture_ScrollY; ///< as gWorldPicture_ScrollX

void WorldMap_SetReturnForScene(s32 scene, s32 entrance);

/// The world map screen, over the map-cell buffer, until A or B closes it: in
/// mode 0 the player looks (FIELD_RAISED_TRIGGER_WORLD_PICTURE); in mode 1, for
/// Teleport, A chooses the place whose name shows and B cancels
/// (FIELD_RAISED_TRIGGER_TELEPORT). While Start is held the cursor glides to
/// the party, and back when it is let go; R and L step it between the places,
/// and Select shows the whole world or the scrolling picture.
s32 WorldPicture_Show(s32 mode) {
  s32 savedDelay;
  u8 savedFlags[ARRAY_COUNT(((struct MapWork *)NULL)->anim)];
  s32 screenX;
  s32 screenY;
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct ActorSystemWork *objects = Ram_HeapSlots->actorSystem;
  s16 blend;
  s32 messageShown = 0;
  s32 zoomed = 0;
  s32 gliding = 0;
  struct WorldMapWork *work = &gWorldMapWork;
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
  s32 value;
  struct WorldMapChoice *choice = &gWorldPictureChoice;
  struct MainRoutineWork *routine = &gMainRoutineWork;

  work->mode = mode;
  if (mode != 0 || event->mode != ACTOR_SYSTEM_WORLD_MAP)
    ObjectTable_Snapshot();
  if (event->mode == ACTOR_SYSTEM_WORLD_MAP) {
    savedDelay = event->transitionFrames;
    event->transitionFrames = 6;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    layer = map->anim;
    for (i = 0; i < (s32)ARRAY_COUNT(savedFlags); i++) {
      savedFlags[i] = layer->paused;
      layer->paused = 1;
      layer++;
    }
    objects->suspended = 1;
    wait(1);
    ((struct WorldMapView *)map)->viewHeld = 1;
    do {
      /* FAKEMATCH(dead): the read in a once-only block; as the plain statement.
       */
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
    // BG1's map numbers the picture's tiles as WorldMap_LoadGraphics does,
    // two a word: from the fifth row, then on round.
    tiles = (u32 *)BG_SCREEN_BLOCK(6);
    tile = TILE_PAIR(WORLD_PICTURE_FIRST_TILE + WORLD_PICTURE_MAP_START);
    for (row = 0; row < WORLD_PICTURE_TILE_ROWS - 4; row++)
      for (column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
        *tiles++ = tile;
        tile += TILE_PAIR_STEP;
      }
    tile = TILE_PAIR(WORLD_PICTURE_FIRST_TILE);
    for (row = 0; row < WORLD_PICTURE_TILE_ROWS; row++)
      for (column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
        *tiles++ = tile;
        tile += TILE_PAIR_STEP;
      }
    tile = TILE_PAIR(WORLD_PICTURE_FIRST_TILE);
    for (row = 0; row < 4; row++)
      for (column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
        *tiles++ = tile;
        tile += TILE_PAIR_STEP;
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
    if ((GameFlag_Get(FLAG_WORLD_MAP_LEFT) ||
         GameFlag_Get(FLAG_LEADER_OFF_MAP_PICTURE)) &&
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
        *cursorX = toFIXED(screenX);
        // DEAD: input set again to the &gInput it already holds; the ROM keeps
        // the load.
        input = &gInput;
        *cursorZ = toFIXED(screenY);
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
        *cursorX = toFIXED(screenX);
        // wayZ only ever holds &gWorldPicture_CursorStartY, set where it is
        // used.
        wayZ = &gWorldPicture_CursorStartY;
        *cursorZ = toFIXED(screenY);
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & KEY_L) {
        WorldPicture_StepToPlace(&screenX, &screenY, -1);
        *cursorX = toFIXED(screenX);
        wayZ = &gWorldPicture_CursorStartY;
        *cursorZ = toFIXED(screenY);
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & KEY_SELECT) {
        work->shown = -1;
        WindowSprite_PrepareForRedrawFar(work->window);
        Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
        zoomed ^= 1;
        if (*cursorX != 0) {
          s32 minimum = 0;
          s32 limitX = toFIXED(272);
          s32 limitZ = toFIXED(224);

          work->x = *cursorX;
          work->z = *cursorZ;
          scrollX = &gWorldPicture_ScrollX;
          *scrollX = work->x - toFIXED(120);
          scrollZ = &gWorldPicture_ScrollY;
          *scrollZ = work->z - toFIXED(80);
          value = *scrollX;
          if (value < minimum) {
            *scrollX = minimum;
            value = *scrollX;
          }
          if (value > limitX)
            *scrollX = limitX;
          value = *scrollZ;
          if (value < minimum) {
            *scrollZ = minimum;
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
        if (work->shown == -1)
          continue;
        place = FlagEntries_GetTable();
        place += work->shown;
        routine->teleportScene = SCENE_WORLD_MAP;
        choice->flag = place->flag;
        WorldMap_SetReturnForScene(place->scene, place->entrance);
        goto done;
      }
      if (input->repeat & KEY_B) {
        if (mode != 0) {
          routine->teleportScene = -1;
          {
            /* FAKEMATCH(temporary): none, -1 held in a local; as -1 at its
             * uses. */
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
    for (i = 0; i < (s32)ARRAY_COUNT(savedFlags); i++) {
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

/// The World Map's places, ended by a zero flag.
extern const struct FlagEntry gFlagEntries[];

/// On the world map, zooms the view out to eight thirteenths while L is held,
/// then back, six frames each way (FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM).
void WorldMap_ZoomOutWhileLHeld(void) {
  struct MainRoutineWork *party;

  if (gFieldWork->mode == ACTOR_SYSTEM_WORLD_MAP) {
    Leader_StopMoving();
    WorldMap_StartZoom(FIXED_ONE * 8 / 13, 6);
    while (gInput.held & KEY_L)
      wait(1);
    party = &gMainRoutineWork;

    // DEAD: both arms alike; the ROM keeps the test and both calls.
    if (party->ownerOverride != 0)
      WorldMap_StartZoom(FIXED_ONE, 6);
    else
      WorldMap_StartZoom(FIXED_ONE, 6);
  }
}

/// Returns the flag entry table, 20-byte entries ended by a zero flag.
const struct FlagEntry *FlagEntries_GetTable(void) { return gFlagEntries; }
#undef PICTURE_WIDTH
#undef PICTURE_HEIGHT
#undef PICTURE_TILES_ACROSS
#undef PICTURE_ROW_BYTES
#undef WORLD_PICTURE_TILES
#undef PICTURE_SCROLL_MAX_X
#undef PICTURE_SCROLL_MAX_Y
#undef PICTURE_DRAWN_COLUMNS
#undef PICTURE_DRAWN_ROWS
#undef WORLD_MAP_CURSOR_MIN_X
#undef WORLD_MAP_CURSOR_MAX_X
#undef WORLD_MAP_CURSOR_MAX_Y
#undef WORLD_MAP_CURSOR_MIN_SPEED
#undef WORLD_MAP_CURSOR_MAX_SPEED
#undef WORLD_MAP_CURSOR_ACCELERATION
#undef WORLD_MAP_NAME_REACH
#undef WORLD_MAP_MARKER_BLEND_STEPS
#undef SCROLL_X
#undef SCROLL_Y
