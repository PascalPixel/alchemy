/// @file
/// The map screen's image: drawing it, scrolling it with the cursor,
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

/// The map screen's image on BG1, in 256 colours. The large image
/// (RESOURCE_WORLD_MAP_PICTURE_LARGE) is 64 by 48 tiles, each row of tiles
/// packed on its own and decoded into the map-cell buffer, which holds 24 rows
/// and wraps. BG1 holds 32 by 24 of its tiles from char block 1 on, wrapping
/// the same way, and BG1's map (WorldMap_LoadGraphics) numbers them from the
/// fifth row, where the still image starts.
#define IMAGE_WIDTH 512
#define IMAGE_HEIGHT 384
#define IMAGE_TILES_ACROSS (IMAGE_WIDTH / 8)
#define IMAGE_ROW_BYTES (IMAGE_TILES_ACROSS * TILE_SIZE_8BPP)
/// The image's tiles BG1 holds, where its map's tile numbers wrap.
#define MAP_SCREEN_TILES (MAP_SCREEN_TILE_ROWS * BG_SCREEN_WIDTH)
/// The scroll that shows the image's bottom right corner.
#define IMAGE_SCROLL_MAX_X toFIXED(IMAGE_WIDTH - DISPLAY_WIDTH)
#define IMAGE_SCROLL_MAX_Y toFIXED(IMAGE_HEIGHT - DISPLAY_HEIGHT)
/// The tiles drawn for one screen: 30 by 20, and the column and row partly
/// scrolled in.
#define IMAGE_DRAWN_COLUMNS (DISPLAY_WIDTH / 8 + 1)
#define IMAGE_DRAWN_ROWS (DISPLAY_HEIGHT / 8 + 1)

/// The cursor's limits on the image, in pixels; y goes from 0.
///@{
#define WORLD_MAP_CURSOR_MIN_X 16
#define WORLD_MAP_CURSOR_MAX_X (IMAGE_WIDTH - 1)
#define WORLD_MAP_CURSOR_MAX_Y 365
///@}
#define WORLD_MAP_CURSOR_MIN_SPEED toFIXED(1) ///< pixels a frame
#define WORLD_MAP_CURSOR_MAX_SPEED toFIXED(6) ///< pixels a frame
/// Added to the cursor's speed each frame the d-pad is held.
#define WORLD_MAP_CURSOR_ACCELERATION (toFIXED(1) / 8)
/// How near the cursor a marker's name shows, in pixels.
#define WORLD_MAP_NAME_REACH 8
#define WORLD_MAP_MARKER_BLEND_STEPS 32 ///< gWorldMap_MarkerBlendCycle's rows

/// R (step 1) and L (step -1) on the map screen: the cursor jumps to the
/// next or the previous marked place round from the one nearest it, and its
/// point on the image is written to screenX and screenY.
void MapScreen_StepToPlace(s32 *screenX, s32 *screenY, s32 step) {
  const struct WorldMapPlace *places;
  const struct WorldMapPlace *place;
  const struct WorldMapPlace *next;
  s32 best;
  s32 bestDistance;
  s32 cursorX;
  s32 cursorY;
  s32 count;
  s32 wraps;
  s32 flag;
  u32 marker;
  struct WorldMapState *worldMapState = Ram_WorldMapState;

  // DEAD: the leader looked up and never used; the ROM keeps the call.
  Party_GetLeaderActor();
  places = WorldMapPlaces_GetTable();
  bestDistance = 8192 * 8192;
  best = -1;
  cursorX = FIXEDtoINT(worldMapState->x);
  cursorY = FIXEDtoINT(worldMapState->z);
  if (worldMapState->searchX != 0) {
    cursorX = FIXEDtoINT(worldMapState->searchX);
    cursorY = FIXEDtoINT(worldMapState->searchZ);
  }
  for (count = 0; count < MAP_SCREEN_PLACE_COUNT; count++) {
    place = &places[count];
    if (place->flag == 0)
      break;
    flag = FLAG_PLACE_VISITED(place->flag);
    marker = WorldMapPlace_GetMarker(place);

    if (marker != WORLD_MAP_MARKER_NONE && marker != WORLD_MAP_MARKER_SHIP &&
        GameFlag_Get(flag)) {
      s32 sx = MAP_SCREEN_X(place->x);
      s32 sy = MAP_SCREEN_Y(place->z);
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
    MapScreen_GetLeaderPoint(screenX, screenY);
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
          marker = WorldMapPlace_GetMarker(next);

          if (marker != WORLD_MAP_MARKER_NONE &&
              marker != WORLD_MAP_MARKER_SHIP && GameFlag_Get(flag)) {
            place = next;
            break;
          }
        }
      }
    } while (wraps <= 1);
  }
  *screenX = MAP_SCREEN_X(place->x);
  *screenY = MAP_SCREEN_Y(place->z);
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
    sprite = WindowSprite_GetSprite(output);
    output = output->next;
    sprite->oam.x += dx;
    sprite->oam.y += dy;
  }
}

/// Where the name of the place nearest the cursor is drawn: its top this many
/// pixels above the place, and its left edge WORLD_MAP_NAME_LEFT pixels left
/// of it (1 when zoomed). A name that would run off the screen's right edge
/// ends WORLD_MAP_NAME_RIGHT_MARGIN pixels short of it instead, and is raised
/// WORLD_MAP_NAME_RAISE more.
#define WORLD_MAP_NAME_ABOVE 11
#define WORLD_MAP_NAME_RIGHT_MARGIN 8
#define WORLD_MAP_NAME_RAISE 10
/// The cursor sprite's left edge, this many pixels left of the cursor's point.
#define MAP_SCREEN_CURSOR_LEFT 17

/// The scrolled name's left edge, in pixels left of its place.
#if LANGUAGE == LANGUAGE_GERMAN
#define WORLD_MAP_NAME_LEFT 3
#else
#define WORLD_MAP_NAME_LEFT 1
#endif

/// The upper halfword of value, its whole pixels.
#define WORLD_MAP_HIGH(value) (((s16 *)&(value))[1])
/// A place on the World Map, in pixels, as a place on the image, 300 pixels
/// to every 640.
#define WORLD_MAP_SCALE(value) ((value) * 300 / 640)

/// The names a marker shows besides its place's own.
#define MARKER_NAME_PARTY -1 ///< "Present location"
#define MARKER_NAME_SHIP -2  ///< "Boat"

/// Each frame of the map screen: moves the cursor with the d-pad, faster
/// the longer it is held, or glides it toward its target; draws a marker at
/// each place visited, at the ship once it sails and at the party; with the
/// d-pad released draws the cursor onto the nearest marker in reach, and shows
/// that marker's name beside it.
void MapScreen_UpdateMarkers(void) {
  struct WorldMapState *state;
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
  const struct WorldMapPlace *places;
  s32 scrollX;
  s32 scrollY;
  const struct WorldMapPlace *place;
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

  // DEAD: the leader looked up and never used; the ROM keeps the call.
  Party_GetLeaderActor();
  state = Ram_WorldMapState;
  marker = state->markers;
  tileBase = gVramCacheEntries[state->vramBlock].offset >> 5;
  nearest = -1;
  nearestDistance = WORLD_MAP_NAME_REACH * WORLD_MAP_NAME_REACH;
  nearestX = 0;
  nearestY = 0;
  places = WorldMapPlaces_GetTable();
  scrollX = state->scrollX & FIELD_PIXEL_MASK;
  scrollY = state->scrollZ & FIELD_PIXEL_MASK;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) &
                                     (WORLD_MAP_MARKER_BLEND_STEPS - 1)];

  if (gMapScreen_GlideTargetX != 0) {
    x = gMapScreen_GlideTargetX;
    y = gMapScreen_GlideTargetY;
    dx = (x - state->x) / FIXED_ONE;
    dz = (y - state->z) / FIXED_ONE;
    distance = toFIXED(Iwram_Sqrt(dx * dx + dz * dz));
    dx = x - state->x;
    dz = y - state->z;
    if (distance < toFIXED(64))
      distance = Math_SqrtFX(Iwram_MulFX(dx, dx) + Iwram_MulFX(dz, dz));
    step = distance / 4;
    if (step > toFIXED(8))
      step = toFIXED(8);
    if (distance < toFIXED(1) / 2) {
      state->x = x;
      state->z = y;
      gMapScreen_GlideTargetX = 0;
      gMapScreen_GlideTargetY = 0;
    } else {
      if (distance > step) {
        dx = Iwram_MulFX(Iwram_DivFX(distance, dx), step);
        dz = Iwram_MulFX(Iwram_DivFX(distance, dz), step);
      }
      state->x += dx;
      state->z += dz;
    }
  } else {
    angle = gWorldMap_CursorDirectionAngles[DPAD_INDEX(gInput.held)];
    if (angle != WORLD_MAP_KEY_HEADING_NONE) {
      pos[0] = state->x;
      pos[1] = 0;
      pos[2] = state->z;
      Vector_AddPolarOffset(state->speed, angle, pos);
      if (pos[0] < toFIXED(WORLD_MAP_CURSOR_MIN_X))
        pos[0] = toFIXED(WORLD_MAP_CURSOR_MIN_X);
      if (pos[0] > toFIXED(WORLD_MAP_CURSOR_MAX_X))
        pos[0] = toFIXED(WORLD_MAP_CURSOR_MAX_X);
      if (pos[2] < 0)
        pos[2] = 0;
      if (pos[2] > toFIXED(WORLD_MAP_CURSOR_MAX_Y))
        pos[2] = toFIXED(WORLD_MAP_CURSOR_MAX_Y);
      state->x = pos[0];
      state->z = pos[2];
      Ram_WorldMapCursorStart[0] = pos[0];
      Ram_WorldMapCursorStart[1] = pos[2];
      if (state->speed < WORLD_MAP_CURSOR_MAX_SPEED)
        state->speed += WORLD_MAP_CURSOR_ACCELERATION;
    }
  }

  cursorX = WORLD_MAP_HIGH(state->x);
  cursorY = WORLD_MAP_HIGH(state->z);
  place = places;
  i = 0;
  do {
    if (place->flag == 0) {
      if (gMapScreen_FirstListOnly != 0)
        break;
      place = &places[MAP_SCREEN_PLACE_COUNT];
      i = MAP_SCREEN_PLACE_COUNT;
      name = MARKER_NAME_PARTY;
      tile = WORLD_MARKER_TILE_PARTY;
      always = 0;
      MapScreen_GetLeaderPoint(&placeX, &placeY);
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
        if (gMainRoutineState.ownerOverride != 0)
          continue;
        name = MARKER_NAME_SHIP;
        tile = WORLD_MARKER_TILE_SHIP;
        always = 0;
        placeX = MAP_SCREEN_X(FIXEDtoINT(gMainRoutineState.shipX));
        placeY = MAP_SCREEN_Y(FIXEDtoINT(gMainRoutineState.shipZ));
      } else {
        if (!GameFlag_Get(flag))
          continue;
        always = 1;
        placeX = MAP_SCREEN_X(place->x);
        placeY = MAP_SCREEN_Y(place->z);
      }
    }
    marker->objectMode = always;
    marker->tile = tileBase + tile;
    if (gMapScreen_Zoomed == 0) {
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
      Oam_AddSpriteAtDepthClamped(marker++, MAP_SCREEN_MARKER_DEPTH);
  } while (++i, place++, i < MAP_SCREEN_PLACE_COUNT);

  if (gInput.held == 0) {
    if (nearest != -1) {
      if (nearestDistance <= 2) {
        state->x = toFIXED(nearestX);
        state->z = toFIXED(nearestY);
        state->speed = WORLD_MAP_CURSOR_MIN_SPEED;
      } else {
        angle = Trig_Atan(nearestY - cursorY, nearestX - cursorX);
        pos[0] = state->x;
        pos[1] = 0;
        pos[2] = state->z;
        Vector_AddPolarOffset(state->speed, (u16)angle, pos);
        state->x = pos[0];
        state->z = pos[2];
      }
      cursorX = WORLD_MAP_HIGH(state->x);
      cursorY = WORLD_MAP_HIGH(state->z);
    } else {
      state->speed = WORLD_MAP_CURSOR_MIN_SPEED;
    }
  }

  if (nearest != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + WORLD_MARKER_TILE_NEAREST;
    if (gMapScreen_Zoomed == 0) {
      marker->x = nearestX - FIXEDtoINT(scrollX) - 2;
      marker->y = nearestY - FIXEDtoINT(scrollY) - 2;
    } else {
      marker->x = WORLD_MAP_SCALE(nearestX) - 2;
      marker->y = WORLD_MAP_SCALE(nearestY) - 2;
    }
    Oam_AddSpriteAtDepthClamped(marker, MAP_SCREEN_MARKER_DEPTH);
  }

  marker = &gMapScreen_CursorMarker;
  if (gMapScreen_Zoomed == 0) {
    marker->affineIndex = 0;
    marker->x = cursorX - FIXEDtoINT(scrollX) - MAP_SCREEN_CURSOR_LEFT;
    marker->y = cursorY - FIXEDtoINT(scrollY) + 1;
  } else {
    marker->x = WORLD_MAP_SCALE(cursorX) - MAP_SCREEN_CURSOR_LEFT;
    marker->y = WORLD_MAP_SCALE(cursorY) + 1;
  }
  Oam_AddSpriteAtDepthClamped(marker, MAP_SCREEN_MARKER_DEPTH);

  if (state->shown != nearest) {
    // The Japanese release prepares the window and draws the new name in the
    // same frame; the later ones draw it 6 frames after preparing the window.
#if TLA_PATCH_1
    if (gMapScreen_NameRedrawPending[0] == 0) {
      WindowSprite_PrepareForRedrawFar(state->window);
      gMapScreen_NameRedrawPending[0] = 1;
      gMapScreen_NameRedrawDelay[0] = 6;
    } else if (--gMapScreen_NameRedrawDelay[0] <= 0) {
      gMapScreen_NameRedrawPending[0] = 0;
      state->shown = nearest;
#else
    {
      state->shown = nearest;
      WindowSprite_PrepareForRedrawFar(state->window);
#endif
      if (nearest != -1) {
        if (nearestName == MARKER_NAME_PARTY)
          nearestName = (s32)MsgPresentLocation;
        else if (nearestName == MARKER_NAME_SHIP)
          nearestName = (s32)MsgBoat;
        else
          nearestName = Area_GetNameIndex(nearestName, nearestEntrance) +
                        (s32)MsgDebugAreaName;
        Text_MeasureMessageFar(nearestName, &width, &height);
        if (gMapScreen_Zoomed == 0) {
          cursorX = nearestX - WORLD_MAP_NAME_LEFT;
          cursorY = nearestY - WORLD_MAP_NAME_ABOVE;
          cursorX -= FIXEDtoINT(scrollX);
          cursorY -= FIXEDtoINT(scrollY);
        } else {
          cursorX = WORLD_MAP_SCALE(nearestX) - 1;
          cursorY = WORLD_MAP_SCALE(nearestY) - WORLD_MAP_NAME_ABOVE;
        }
        if (cursorX + width > DISPLAY_WIDTH - 1) {
          cursorX = DISPLAY_WIDTH - WORLD_MAP_NAME_RIGHT_MARGIN - width;
          cursorY += -WORLD_MAP_NAME_RAISE;
        }
        if (cursorY < 0)
          cursorY = 0;
        Text_DrawMessageFar(nearestName, state->window, cursorX, cursorY);
        state->nameBox.width = width;
        state->nameBox.height = height;
        state->nameBox.x = cursorX;
        state->nameBox.y = cursorY;
      }
    }
  } else if (nearest != -1) {
    width = state->nameBox.width;
    height = state->nameBox.height;
    if (gMapScreen_Zoomed == 0) {
      cursorX = nearestX - WORLD_MAP_NAME_LEFT;
      cursorY = nearestY - WORLD_MAP_NAME_ABOVE;
      cursorX -= FIXEDtoINT(scrollX);
      cursorY -= FIXEDtoINT(scrollY);
    } else {
      cursorX = WORLD_MAP_SCALE(nearestX) - 1;
      cursorY = WORLD_MAP_SCALE(nearestY) - WORLD_MAP_NAME_ABOVE;
    }
    if (cursorX + width > DISPLAY_WIDTH - 1) {
      cursorX = DISPLAY_WIDTH - WORLD_MAP_NAME_RIGHT_MARGIN - width;
      cursorY += -WORLD_MAP_NAME_RAISE;
    }
    if (cursorY < 0)
      cursorY = 0;
    width = cursorX - state->nameBox.x;
    height = cursorY - state->nameBox.y;
    WindowSpriteList_Move(state->window, width, height);
    state->nameBox.x = cursorX;
    state->nameBox.y = cursorY;
  }

  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      ((BLEND_WEIGHT_MAX - blend) << 8) | blend,
                      IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDY, 0, IO_WRITE_HALFWORD);
}
#undef WORLD_MAP_NAME_LEFT
#undef MARKER_NAME_PARTY
#undef MARKER_NAME_SHIP
#undef WORLD_MAP_HIGH
#undef WORLD_MAP_SCALE

/// Draws the map screen on BG1 with the cursor in the middle of the screen
/// (or the whole world, RESOURCE_WORLD_MAP_PICTURE, at the origin while
/// gWorldMapState.centered is set): the palette, then 21 rows of 31 tiles.
void MapScreen_Draw(void) {
  struct WorldMapState *state = &gWorldMapState;
  u8 *buffer;
  s32 resource = RESOURCE_WORLD_MAP_PICTURE_LARGE;
  s32 *cursor = state->cursor;
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

  if (state->centered != 0) {
    resource = RESOURCE_WORLD_MAP_PICTURE;
    x = 0;
    z = 0;
  }
  graphics = Resource_GetTableEntry(resource);
  Dma_Set((u8 *)graphics + graphics[0], (void *)BG_PLTT, DMA_COPY_WORDS(120),
          REG_DMA3);
  *(u16 *)BG_PLTT = 0;
  buffer = Ram_MapCellBuffer;
  right = IMAGE_SCROLL_MAX_X;
  bottom = IMAGE_SCROLL_MAX_Y;
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
  for (row = 0; row < IMAGE_DRAWN_ROWS; row++) {
    bufferRow = firstRow + row;
    unwrapped = bufferRow;
    if (bufferRow >= MAP_SCREEN_TILE_ROWS)
      bufferRow -= MAP_SCREEN_TILE_ROWS;
    cell = bufferRow * IMAGE_TILES_ACROSS + column;
    // z is reused for the tile's place in BG1's map.
    z = (column & (BG_SCREEN_WIDTH - 1)) + unwrapped * BG_SCREEN_WIDTH;
    tile =
        (z + MAP_SCREEN_MAP_START) % MAP_SCREEN_TILES + MAP_SCREEN_FIRST_TILE;
    Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
                        buffer + bufferRow * IMAGE_ROW_BYTES);
    for (i = 0; i < IMAGE_DRAWN_COLUMNS; i++) {
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

/// Draws one row of the map screen on BG1 as it scrolls in, from a column to
/// the image's right edge, at most 31 tiles (the Japanese release always
/// draws 32): decodes the row into the map-cell buffer and copies its tiles to
/// BG1's.
void MapScreen_DrawRow(u32 column, s32 row) {
  s32 resource = RESOURCE_WORLD_MAP_PICTURE_LARGE;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u32 i;

  if (gMapScreen_Zoomed != 0)
    resource = RESOURCE_WORLD_MAP_PICTURE;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row >= MAP_SCREEN_TILE_ROWS)
    row -= MAP_SCREEN_TILE_ROWS;
  cell = row * IMAGE_TILES_ACROSS + column;
  tile = (column & (BG_SCREEN_WIDTH - 1)) + row * BG_SCREEN_WIDTH;
  tile =
      (tile + MAP_SCREEN_MAP_START) % MAP_SCREEN_TILES + MAP_SCREEN_FIRST_TILE;
  Decompress_TaggedLz((u8 *)graphics + graphics[unwrapped + 1],
                      Ram_MapCellBuffer + row * IMAGE_ROW_BYTES);
  // From patch 1 the copy stops at the image's last column instead of
  // running past it.
#if TLA_PATCH_1
  for (i = 0; i < IMAGE_DRAWN_COLUMNS && column + i < IMAGE_TILES_ACROSS; i++) {
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

/// Draws one column of the map screen on BG1 as it scrolls in, 21 tiles
/// down from a row, from the rows already decoded in the map-cell buffer.
void MapScreen_DrawColumn(s32 column, s32 row) {
  u32 i;

  for (i = 0; i < IMAGE_DRAWN_ROWS; i++) {
    s32 line = row + i;
    s32 bufferRow = line;
    s32 cell;
    s32 tile;

    if (line >= MAP_SCREEN_TILE_ROWS)
      bufferRow = line - MAP_SCREEN_TILE_ROWS;
    cell = column + bufferRow * IMAGE_TILES_ACROSS;
    tile = ((column & (BG_SCREEN_WIDTH - 1)) + line * BG_SCREEN_WIDTH +
            MAP_SCREEN_MAP_START) %
           MAP_SCREEN_TILES;
    cell *= TILE_SIZE_8BPP;
    tile *= TILE_SIZE_8BPP;
    Dma_Set(Ram_MapCellBuffer + cell,
            (u8 *)(VRAM + MAP_SCREEN_FIRST_TILE * TILE_SIZE_8BPP) + tile,
            DMA_COPY_WORDS(TILE_SIZE_8BPP / 4), REG_DMA3);
    /* FAKEMATCH(dead): line set to 0 at the step's end, where nothing reads
     * it; as no store. */
    line = 0;
  }
}

/// The world map cursor's position (x, then y, 16.16 pixels).
extern FIXED *gMapScreen_Cursor;

/// The map screen's scroll, x then y.
#define SCROLL_X Ram_WorldMapScroll[0]
#define SCROLL_Y Ram_WorldMapScroll[1]

/// The map screen's scroll follows the cursor. The target puts the
/// cursor in the middle of the screen (clamped to the image); unless the
/// whole world shows, the scroll moves a quarter of the way there each frame,
/// at most 8 pixels, and stops when nearer than a quarter pixel. Each time the
/// scroll crosses into a new column or row of tiles, that column
/// (MapScreen_DrawColumn) or row (MapScreen_DrawRow) is drawn; BG1
/// scrolls with it.
void MapScreen_UpdateScroll(void) {
  s32 *cursor = gMapScreen_Cursor;
  s32 x = *cursor++ - toFIXED(DISPLAY_WIDTH / 2);
  s32 y = *cursor - toFIXED(DISPLAY_HEIGHT / 2);
  s32 dx;
  s32 dy;
  FIXED distance;
  FIXED step;
  u32 column;
  u32 row;
  s32 limit;
  s32 bottom = IMAGE_SCROLL_MAX_Y;

  if (x < 0)
    x = 0;
  limit = IMAGE_SCROLL_MAX_X;
  if (x > limit)
    x = limit;
  if (y < 0)
    y = 0;
  if (y > bottom)
    y = bottom;
  if (gMapScreen_Zoomed == 0) {
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
        dx = Iwram_MulFX(Iwram_DivFX(distance, dx), step);
        dy = Iwram_MulFX(Iwram_DivFX(distance, dy), step);
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
      // image never passes; the ROM keeps the compare and the store.
      if (y > limit)
        y = limit;
    }
    column = FIELD_HALF_CELL((u32)x);
    row = FIELD_HALF_CELL((u32)y);
    if ((SCROLL_X ^ x) & toFIXED(8)) {
      if (SCROLL_X < x)
        MapScreen_DrawColumn(column + DISPLAY_WIDTH / 8, row);
      else
        MapScreen_DrawColumn(column, row);
    }
    if ((SCROLL_Y ^ y) & toFIXED(8)) {
      if (SCROLL_Y < y)
        MapScreen_DrawRow(column, row + DISPLAY_HEIGHT / 8);
      else
        MapScreen_DrawRow(column, row);
    }
    gBgScroll[1].x = FIXEDtoINT(x);
    gBgScroll[1].y = FIXEDtoINT(y);
  }
  SCROLL_X = x;
  SCROLL_Y = y;
}

/// Where the cursor goes back to when Start is let go: the place R or L last
/// stepped to, 16.16 pixels.
extern s32 gMapScreen_CursorStartX;
extern s32 gMapScreen_CursorStartY; ///< as gMapScreen_CursorStartX
/// The map screen's scroll as Select toggles the zoom, 16.16 pixels.
extern s32 gMapScreen_ScrollX;
extern s32 gMapScreen_ScrollY; ///< as gMapScreen_ScrollX

void WorldMap_SetReturnForScene(s32 scene, s32 entrance);

/// The map screen, over the map-cell buffer, until A or B closes it, to
/// look or to choose where to Teleport (enum MapScreenMode). While Start is
/// held the cursor glides to the party, and back when it is let go; R and L
/// step it between the places, and Select shows the whole world or the
/// scrolling image.
s32 MapScreen_Show(s32 mode) {
  s32 savedDelay;
  u8 savedFlags[ARRAY_COUNT(((struct MapState *)NULL)->anim)];
  s32 screenX;
  s32 screenY;
  struct MapState *map = Ram_HeapSlots->mapGrid;
  struct WorldMapView *view = Ram_HeapSlots->mapGrid;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct ActorSystemState *actorSystem = Ram_HeapSlots->actorSystem;
  s16 blend;
  s32 messageShown = 0;
  s32 zoomed = 0;
  s32 gliding = 0;
  struct WorldMapState *state = &gWorldMapState;
  struct MapAnimation *layer;
  const struct WorldMapPlace *place;
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
  struct WorldMapChoice *choice = &gMapScreenChoice;
  struct MainRoutineState *routine = &gMainRoutineState;

  state->mode = mode;
  if (mode != MAP_SCREEN_LOOK || field->mode != ACTOR_SYSTEM_WORLD_MAP)
    ActorTable_Snapshot();
  if (field->mode == ACTOR_SYSTEM_WORLD_MAP) {
    savedDelay = field->transitionFrames;
    field->transitionFrames = 6;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    layer = map->anim;
    for (i = 0; i < (s32)ARRAY_COUNT(savedFlags); i++) {
      savedFlags[i] = layer->paused;
      layer->paused = 1;
      layer++;
    }
    actorSystem->suspended = 1;
    wait(1);
    view->viewHeld = 1;
    do {
      /* FAKEMATCH(dead): the read in a once-only block; as the plain statement.
       */
      blend = REG_BLDALPHA;
    } while (0);
  } else {
    Io_Write8(1, &field->camera->paused);
    savedDelay = field->transitionFrames;
    field->transitionFrames = 6;
    field->transition = 0;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Task_ResetAll();
    WindowSystem_InitFieldFar();
    actorSystem->suspended = 1;
    wait(1);
    blend = REG_BLDALPHA;
    // BG1's map numbers the image's tiles as WorldMap_LoadGraphics does,
    // two a word: from the fifth row, then on round.
    tiles = (u32 *)BG_SCREEN_BLOCK(6);
    tile = TILE_PAIR(MAP_SCREEN_FIRST_TILE + MAP_SCREEN_MAP_START);
    for (row = 0; row < MAP_SCREEN_TILE_ROWS - 4; row++)
      for (column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
        *tiles++ = tile;
        tile += TILE_PAIR_STEP;
      }
    tile = TILE_PAIR(MAP_SCREEN_FIRST_TILE);
    for (row = 0; row < MAP_SCREEN_TILE_ROWS; row++)
      for (column = 0; column < BG_SCREEN_WIDTH / 2; column++) {
        *tiles++ = tile;
        tile += TILE_PAIR_STEP;
      }
    tile = TILE_PAIR(MAP_SCREEN_FIRST_TILE);
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
  wayX = NULL;
  cursorX = NULL;
  cursorZ = NULL;
  scrollX = NULL;
  scrollZ = NULL;
  MapScreen_Initialize();
  for (;;) {
    gMapScreen_Zoomed = zoomed;
    MapScreen_Draw();
    wait(1);
    Io_Write16(BGCNT_PRIORITY(2) | BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) |
                   BGCNT_SIZE(2),
               &REG_BG1CNT);
    Io_Write16(DISPCNT_OBJ_1D_MAP | DISPCNT_BG0_ON | DISPCNT_BG1_ON |
                   DISPCNT_OBJ_ON,
               &REG_DISPCNT);
    if ((GameFlag_Get(FLAG_WORLD_MAP_LEFT) ||
         GameFlag_Get(FLAG_LEADER_OFF_MAP_SCREEN)) &&
        !messageShown) {
      FieldMessage_ShowFar((s32)MsgNotOnMap, FIELD_MESSAGE_ZOOM);
      messageShown = 1;
    }
    keys = &gInput;
    wayX = &gMapScreen_CursorStartX;
    cursorX = &gMapScreen_GlideTargetX;
    cursorZ = &gMapScreen_GlideTargetY;
    for (;;) {
      MapScreen_UpdateScroll();
      MapScreen_UpdateMarkers();
      input = keys;
      wait(1);
      if ((input->repeat & KEY_START) && gliding == 0) {
        MapScreen_GetLeaderPoint(&screenX, &screenY);
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
        *cursorZ = gMapScreen_CursorStartY;
        gliding = 0;
      }
      if (keys->repeat & KEY_R) {
        MapScreen_StepToPlace(&screenX, &screenY, 1);
        *cursorX = toFIXED(screenX);
        // wayZ only ever holds &gMapScreen_CursorStartY, set where it is
        // used.
        wayZ = &gMapScreen_CursorStartY;
        *cursorZ = toFIXED(screenY);
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & KEY_L) {
        MapScreen_StepToPlace(&screenX, &screenY, -1);
        *cursorX = toFIXED(screenX);
        wayZ = &gMapScreen_CursorStartY;
        *cursorZ = toFIXED(screenY);
        *wayX = *cursorX;
        *wayZ = *cursorZ;
      }
      if (input->repeat & KEY_SELECT) {
        state->shown = -1;
        WindowSprite_PrepareForRedrawFar(state->window);
        Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
        zoomed ^= 1;
        if (*cursorX != 0) {
          s32 minimum = 0;
          s32 limitX = toFIXED(272);
          s32 limitZ = toFIXED(224);

          state->x = *cursorX;
          state->z = *cursorZ;
          scrollX = &gMapScreen_ScrollX;
          *scrollX = state->x - toFIXED(120);
          scrollZ = &gMapScreen_ScrollY;
          *scrollZ = state->z - toFIXED(80);
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
        if (mode == MAP_SCREEN_LOOK)
          goto done;
        if (state->shown == -1)
          continue;
        place = WorldMapPlaces_GetTable();
        place += state->shown;
        routine->teleportScene = SCENE_WORLD_MAP;
        choice->flag = place->flag;
        WorldMap_SetReturnForScene(place->scene, place->entrance);
        goto done;
      }
      if (input->repeat & KEY_B) {
        if (mode != MAP_SCREEN_LOOK) {
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
  MapScreen_Release();
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  if (mode != MAP_SCREEN_LOOK || field->mode != ACTOR_SYSTEM_WORLD_MAP) {
    field->exit = EXIT_KEEP_DESTINATION;
    GameFlag_Set(FLAG_KEEP_PARTY_POSITION);
  } else {
    WorldMap_LoadAreaGraphicsFar();
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, IO_WRITE_HALFWORD);
    layer = map->anim;
    for (i = 0; i < (s32)ARRAY_COUNT(savedFlags); i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    actorSystem->suspended = 0;
    gBgScroll[1].x = 0;
    gBgScroll[1].y = 0;
    view->viewHeld = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    field->transitionFrames = savedDelay;
    field->camera->paused = 0;
  }
  return 0;
}

/// The World Map's places, ended by a zero flag.
extern const struct WorldMapPlace gWorldMapPlaces[];

/// On the world map, zooms the view out to eight thirteenths while L is held,
/// then back, six frames each way (FIELD_RAISED_TRIGGER_WORLD_MAP_ZOOM).
void WorldMap_ZoomOutWhileLHeld(void) {
  struct MainRoutineState *party;

  if (gFieldState->mode == ACTOR_SYSTEM_WORLD_MAP) {
    Leader_StopMoving();
    WorldMap_StartZoom(FIXED_ONE * 8 / 13, 6);
    while (gInput.held & KEY_L)
      wait(1);
    party = &gMainRoutineState;

    // DEAD: both arms alike; the ROM keeps the test and both calls.
    if (party->ownerOverride != 0)
      WorldMap_StartZoom(FIXED_ONE, 6);
    else
      WorldMap_StartZoom(FIXED_ONE, 6);
  }
}

/// Returns the flag entry table, 20-byte entries ended by a zero flag.
const struct WorldMapPlace *WorldMapPlaces_GetTable(void) {
  return gWorldMapPlaces;
}
#undef IMAGE_WIDTH
#undef IMAGE_HEIGHT
#undef IMAGE_TILES_ACROSS
#undef IMAGE_ROW_BYTES
#undef MAP_SCREEN_TILES
#undef IMAGE_SCROLL_MAX_X
#undef IMAGE_SCROLL_MAX_Y
#undef IMAGE_DRAWN_COLUMNS
#undef IMAGE_DRAWN_ROWS
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
