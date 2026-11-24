/// @file
/// The map screen R shows on the World Map: its area markers, the cursor
/// and the name of the area nearest it.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The generated marker pictures: four 16-colour 8 by 8 tiles, 32 bytes each.
#define MAP_SCREEN_MARKER_TILES 4
#define MAP_SCREEN_MARKER_TILE_BYTES (MAP_SCREEN_MARKER_TILES * TILE_SIZE_4BPP)
/// The marker tiles: the party's dot, the two kinds of area an AREA_MARKER
/// row asks for (a town or a place on the way; a lighthouse or Sol Sanctum),
/// and the larger dot that blinks over the marker nearest the cursor.
enum MapScreenMarkerTile {
  MAP_SCREEN_MARKER_PARTY,
  MAP_SCREEN_MARKER_AREA,
  MAP_SCREEN_MARKER_LIGHTHOUSE,
  MAP_SCREEN_MARKER_NEAREST
};
/// The first five of a marker tile row's eight 4-bit pixels, the leftmost in
/// the low bits; the rest are 0.
#define MAP_SCREEN_MARKER_ROW(p0, p1, p2, p3, p4)                              \
  ((p0) | (p1) << 4 | (p2) << 8 | (p3) << 12 | (p4) << 16)
/// The marker tile an AREA_MARKER row's first word holds above its flag.
#define MAP_SCREEN_AREA_MARKER_TILE(row) ((row) >> 16)
#define MAP_SCREEN_WIDTH 240  ///< the image's width, the screen's
#define MAP_SCREEN_HEIGHT 160 ///< the image's height, the screen's
/// The image resource: its 14 palettes of BG colours, then its pixels.
#define MAP_SCREEN_PALETTE_BYTES (14 * PALETTE_COLORS * 2)
/// The World Map's size in field pixels each way, which the image shows at
/// 240 by 160.
#define WORLD_MAP_WORLD_SIZE 4096
#define WORLD_MAP_X_ORIGIN toFIXED(4096) ///< field x of the image's left edge
/// The first ordinary area actor, the first slot with a row of
/// gMapScreen_AreaMarkers whatever the leader is.
#define WORLD_MAP_FIRST_AREA_ACTOR 11
#define MAP_SCREEN_MARKER_CAPACITY 66 ///< one marker per actor slot
/// The cursor's sprite entry, after the markers'.
#define MAP_SCREEN_CURSOR_PART MAP_SCREEN_MARKER_CAPACITY
/// The cursor's depth slot, among the windows' and in front of every marker,
/// whose slot is its y on the image.
#define MAP_SCREEN_CURSOR_DEPTH (OAM_DEPTH_WINDOW + 6)
#define MAP_SCREEN_CURSOR_MIN_SPEED toFIXED(1) ///< screen pixels a frame
#define MAP_SCREEN_CURSOR_MAX_SPEED toFIXED(6) ///< screen pixels a frame
/// Added to the cursor's speed each frame the d-pad is held.
#define MAP_SCREEN_CURSOR_ACCELERATION (toFIXED(1) / 8)
/// gWorldMap_MarkerBlendCycle's rows.
#define MAP_SCREEN_MARKER_BLEND_STEPS 32
/// The cursor's limits, in screen pixels; y goes from 0.
///@{
#define MAP_SCREEN_CURSOR_MIN_X 16
#define MAP_SCREEN_CURSOR_MAX_X (MAP_SCREEN_WIDTH - 1)
#define MAP_SCREEN_CURSOR_MAX_Y 141
///@}

// BUG: the marker tiles' size is reserved in words, though the heap counts
// bytes, so 128 bytes are written and uploaded from 32; BUGFIX reserves the
// whole tile buffer.
#ifdef BUGFIX
#define MAP_SCREEN_MARKER_ALLOCATION_SIZE MAP_SCREEN_MARKER_TILE_BYTES
#else
#define MAP_SCREEN_MARKER_ALLOCATION_SIZE (MAP_SCREEN_MARKER_TILE_BYTES / 4)
#endif

/// The map screen's state while MapScreen_Show owns gMapGridCellBuffer.
struct MapScreenState {
  u16 markerVramBlock;            ///< owns the four generated marker tiles
  u16 cursorVramBlock;            ///< owns the cursor picture's tiles
  struct FieldCoordinate cursorX; ///< screen column, 16.16 pixels
  struct FieldCoordinate cursorY; ///< screen row, 16.16 pixels
  u8 unreadAfterCursor[6];        ///< not read by this screen
  s16 namedMarker;           ///< actor whose name is drawn; -1 clears the name
  u8 unreadAfterName[4];     ///< not read by this screen
  FIXED cursorSpeed;         ///< 16.16 screen pixels per frame
  struct Window *nameWindow; ///< owned until this screen closes
  /// The visible areas' markers packed from the start, then the frame round
  /// the nearest; the last entry is the cursor.
  struct AnimationSpritePart markers[MAP_SCREEN_MARKER_CAPACITY + 1];
};

STATIC_ASSERT(offsetof(struct MapScreenState, markers) == 32);
STATIC_ASSERT(sizeof(struct MapScreenState) == 836);
STATIC_ASSERT(MAP_SCREEN_MARKER_CAPACITY ==
              sizeof(((struct FieldState *)0)->actors) /
                  sizeof(((struct FieldState *)0)->actors[0]));

/// Sets up the map screen: the marker tiles and their OAM entries, the
/// cursor where the leader is on the map (or mid-screen when the area is
/// not on it), and the name window.
void MapScreen_Initialize(void) {
  u32 *entryWords; // the markers' sprite entries, a word at a time
  u32 (*markerTiles)[TILE_SIZE_4BPP / 4];
  struct MapScreenState *state;
  struct Actor *actor;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;

  markerTiles = Heap_AllocEwram(MAP_SCREEN_MARKER_ALLOCATION_SIZE);
  state = (struct MapScreenState *)gMapGridCellBuffer;
  entryWords = (u32 *)state->markers;
  actor = Actor_GetById(gMainRoutineState.leaderActorId);
  state->markerVramBlock = VramCache_FindFreeEntry();
  Dma_Fill32(0, markerTiles, MAP_SCREEN_MARKER_TILE_BYTES);
  // Each marker tile holds one dot in its top-left corner, with a shadow in
  // colour 1 to its right and below (# the dot, + the shadow):
  //
  //   tiles 0 to 2      tile 3
  //     ##.               .##..
  //     ##+               ####.
  //     .++               ####+
  //                       .##++
  //                       ..++.
  //
  // The party's dot and the nearest marker's are colour 15, an area's colour
  // 4 and a lighthouse's colour 7.
  markerTiles[MAP_SCREEN_MARKER_PARTY][0] =
      MAP_SCREEN_MARKER_ROW(15, 15, 0, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_PARTY][1] =
      MAP_SCREEN_MARKER_ROW(15, 15, 1, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_PARTY][2] =
      MAP_SCREEN_MARKER_ROW(0, 1, 1, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_AREA][0] = MAP_SCREEN_MARKER_ROW(4, 4, 0, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_AREA][1] = MAP_SCREEN_MARKER_ROW(4, 4, 1, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_AREA][2] = MAP_SCREEN_MARKER_ROW(0, 1, 1, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_LIGHTHOUSE][0] =
      MAP_SCREEN_MARKER_ROW(7, 7, 0, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_LIGHTHOUSE][1] =
      MAP_SCREEN_MARKER_ROW(7, 7, 1, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_LIGHTHOUSE][2] =
      MAP_SCREEN_MARKER_ROW(0, 1, 1, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_NEAREST][0] =
      MAP_SCREEN_MARKER_ROW(0, 15, 15, 0, 0);
  markerTiles[MAP_SCREEN_MARKER_NEAREST][1] =
      MAP_SCREEN_MARKER_ROW(15, 15, 15, 15, 0);
  markerTiles[MAP_SCREEN_MARKER_NEAREST][2] =
      MAP_SCREEN_MARKER_ROW(15, 15, 15, 15, 1);
  markerTiles[MAP_SCREEN_MARKER_NEAREST][3] =
      MAP_SCREEN_MARKER_ROW(0, 15, 15, 1, 1);
  markerTiles[MAP_SCREEN_MARKER_NEAREST][4] =
      MAP_SCREEN_MARKER_ROW(0, 0, 1, 1, 0);
  tileAttributes = VramCache_Load(state->markerVramBlock,
                                  MAP_SCREEN_MARKER_TILE_BYTES, markerTiles) |
                   OBJ_PRIORITY(1);
  for (i = 0; i < MAP_SCREEN_MARKER_CAPACITY; i++) {
    *entryWords++ = 0;
    *entryWords++ = 0;
    *entryWords++ = tileAttributes;
  }
  Heap_Release(markerTiles);
  if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT)) {
    state->cursorX.fixed = toFIXED(MAP_SCREEN_WIDTH) / 2;
    state->cursorY.fixed = toFIXED(MAP_SCREEN_HEIGHT) / 2;
  } else {
    state->cursorX.fixed =
        toFIXED(FIXEDtoINT(actor->x.fixed - WORLD_MAP_X_ORIGIN) *
                MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE);
    state->cursorY.fixed = toFIXED(
        (actor->z.part[0].integer * MAP_SCREEN_HEIGHT) / WORLD_MAP_WORLD_SIZE);
  }
  state->cursorVramBlock = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramCacheEntries[state->cursorVramBlock].offset;
  state->nameWindow = Window_CreateFar(0, 0, 0, 0, WINDOW_FLAG_INSTANT);
  state->namedMarker = -1;
  state->cursorSpeed = MAP_SCREEN_CURSOR_MIN_SPEED;
  *entryWords++ = 0;
  *entryWords++ = OBJ_SIZE_16X16;
  cursorAttributes = OBJ_PRIORITY(1);
  cursorAttributes |= tile >> 5;
  *entryWords = cursorAttributes;
}

#if TBS_PATCH_2
/// The cursor snaps to the nearest area when the d-pad is released.
#define MAP_SCREEN_CURSOR_SNAPS 1
#define MAP_SCREEN_NAME_RANGE                                                  \
  64                           ///< squared distance that shows a marker's name
#define MAP_SCREEN_NAME_LEFT 3 ///< how far left of its marker the name goes
#else
#define MAP_SCREEN_CURSOR_SNAPS 0
#define MAP_SCREEN_NAME_RANGE 100
#define MAP_SCREEN_NAME_LEFT 1
#endif

/// Closes the map screen: returns its VRAM blocks and removes the name
/// window.
void MapScreen_Release(void) {
  struct MapScreenState *state = (struct MapScreenState *)gMapGridCellBuffer;

  VramCache_FreeEntry(state->markerVramBlock);
  VramCache_FreeEntry(state->cursorVramBlock);
  Window_CloseFar(state->nameWindow, 2);
}

/// Task of the map screen: shows a marker at each area whose flag is set,
/// moves the cursor with the d-pad (to the current area while L or R is held)
/// and shows the name of the marker nearest the cursor.
void MapScreen_UpdateMarkers(void) {
  s32 leader;
  const s32 *place;
  s32 tileBase;
  s32 blend;
  s32 nearestMarker;
  s32 nearestDistanceSquared;
  s32 cursorX;
  s32 nearestX;
  s32 nearestY;
  s32 message;
  s32 nearestName;
  s32 height;
  s32 width;
  struct FieldPosition position;
  struct MapScreenState *state;
  struct Actor *areaActor;
  s32 cursorY;
  s32 tile;
  s32 mode;
  s32 flag;
  s32 markerIndex;
  struct AnimationSpritePart *marker;
  s32 x;
  s32 y;
  s32 angle;

#if MAP_SCREEN_CURSOR_SNAPS
  leader = gMainRoutineState.leaderActorId;
  state = (struct MapScreenState *)Ram_MapGridCellBuffer;
  marker = state->markers;
  place = gMapScreen_AreaMarkers;
  tileBase = gVramCacheEntries[state->markerVramBlock].offset >> 5;
#else
  state = (struct MapScreenState *)Ram_MapGridCellBuffer;
  leader = gMainRoutineState.leaderActorId;
  place = gMapScreen_AreaMarkers;
  tileBase = gVramCacheEntries[state->markerVramBlock].offset >> 5;
  marker = state->markers;
#endif
  nearestMarker = -1;
  nearestDistanceSquared = MAP_SCREEN_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) &
                                     (MAP_SCREEN_MARKER_BLEND_STEPS - 1)];

  // FieldSequence_Run creates the selected actor before
  // FIELD_RAISED_TRIGGER_MAP_SCREEN opens this screen. No actor task removes
  // it while the marker task runs.
  if (!GameFlag_GetFar(FLAG_WORLD_MAP_LEFT) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L or R: the current area.
    areaActor = Actor_Lookup(gMainRoutineState.leaderActorId);
#ifdef UBFIX
    // UB: with no selected actor the ROM leaves this frame's cursor
    // coordinates unset; UBFIX skips the frame.
    if (areaActor == NULL)
      return;
#endif
    if (areaActor != NULL) {
      cursorX = FIXEDtoINT(areaActor->x.fixed - WORLD_MAP_X_ORIGIN) *
                MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE;
      cursorY = areaActor->z.part[0].integer * MAP_SCREEN_HEIGHT /
                WORLD_MAP_WORLD_SIZE;
    }
  } else {
    // The d-pad: faster the longer it is held.
    angle = gWorldMap_CursorDirectionAngles[DPAD_INDEX(gKeysHeld)];
    if (angle != DPAD_ANGLE_NONE) {
      position.x = state->cursorX.fixed;
      position.y = 0;
      position.z = state->cursorY.fixed;
      Vector_AddPolarOffset(state->cursorSpeed, angle, &position);
      if (position.x < toFIXED(MAP_SCREEN_CURSOR_MIN_X))
        position.x = toFIXED(MAP_SCREEN_CURSOR_MIN_X);
      if (position.x > toFIXED(MAP_SCREEN_CURSOR_MAX_X))
        position.x = toFIXED(MAP_SCREEN_CURSOR_MAX_X);
      if (position.z < 0)
        position.z = 0;
      if (position.z > toFIXED(MAP_SCREEN_CURSOR_MAX_Y))
        position.z = toFIXED(MAP_SCREEN_CURSOR_MAX_Y);
      state->cursorX.fixed = position.x;
      state->cursorY.fixed = position.z;
      if (state->cursorSpeed < MAP_SCREEN_CURSOR_MAX_SPEED)
        state->cursorSpeed += MAP_SCREEN_CURSOR_ACCELERATION;
    } else {
      state->cursorSpeed = MAP_SCREEN_CURSOR_MIN_SPEED;
    }
    cursorX = state->cursorX.part[0].integer;
    cursorY = state->cursorY.part[0].integer;
  }

  // Actor id 0 consumes no marker row.
  for (markerIndex = 0; markerIndex < MAP_SCREEN_MARKER_CAPACITY;
       markerIndex++) {
    if (markerIndex != leader && markerIndex < WORLD_MAP_FIRST_AREA_ACTOR)
      continue;
    if (markerIndex == 0) {
      if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT))
        continue;
      tile = MAP_SCREEN_MARKER_PARTY;
      flag = leader;
      mode = OBJ_MODE_NORMAL;
    } else {
      flag = *place++;
      message = *place++;
      if (flag == 0)
        continue;
#if MAP_SCREEN_CURSOR_SNAPS
      if (flag == -1)
        break;
      tile = MAP_SCREEN_AREA_MARKER_TILE(flag);
      mode = OBJ_MODE_SEMI_TRANSPARENT;
#else
      tile = MAP_SCREEN_AREA_MARKER_TILE(flag);
      mode = OBJ_MODE_SEMI_TRANSPARENT;
      if (flag == -1)
        break;
#endif
    }
    if (!GameFlag_GetFar(flag))
      continue;
    areaActor = Actor_Lookup(markerIndex);
    if (areaActor == NULL)
      continue;

    // Field x starts at 4096; the image covers 4096 square world pixels.
    x = FIXEDtoINT(areaActor->x.fixed - WORLD_MAP_X_ORIGIN) * MAP_SCREEN_WIDTH /
        WORLD_MAP_WORLD_SIZE;
    y = areaActor->z.part[0].integer * MAP_SCREEN_HEIGHT / WORLD_MAP_WORLD_SIZE;
    marker->objectMode = mode;
    marker->tile = tileBase + tile;
    marker->x = x - 1;
    marker->y = y - 1;
    width = x - cursorX;
    height = y - cursorY;
    if (width * width + height * height < nearestDistanceSquared) {
      // UB: marker 0 has no table message.
#ifdef UBFIX
      if (markerIndex != 0)
#endif
        nearestName = message;
      // These coordinates and the name are valid only with nearestMarker set.
      nearestMarker = markerIndex;
      nearestDistanceSquared = width * width + height * height;
      nearestX = x;
      nearestY = y;
    }
    // Blink the current area for eight frames out of sixteen.
    if (markerIndex != 0 || (gFrameCount & 15) <= 7)
      Oam_AddSpriteAtDepth(marker++, y);
  }
#if MAP_SCREEN_CURSOR_SNAPS
  // With the d-pad released, the cursor moves onto the nearest marker.
  if (gKeysHeld == 0) {
    if (nearestMarker != -1) {
      if (nearestDistanceSquared <= 1) {
        state->cursorX.fixed = toFIXED(nearestX);
        state->cursorY.fixed = toFIXED(nearestY);
        state->cursorSpeed = MAP_SCREEN_CURSOR_MIN_SPEED;
      } else {
        if (state->cursorSpeed < toFIXED(2))
          state->cursorSpeed = toFIXED(2);
        angle = Trig_Atan(nearestY - cursorY, nearestX - cursorX);
        position.x = state->cursorX.fixed;
        position.y = 0;
        position.z = state->cursorY.fixed;
        Vector_AddPolarOffset(state->cursorSpeed, (u16)angle, &position);
        state->cursorX.fixed = position.x;
        state->cursorY.fixed = position.z;
        if (state->cursorSpeed < MAP_SCREEN_CURSOR_MAX_SPEED)
          state->cursorSpeed += MAP_SCREEN_CURSOR_ACCELERATION;
      }
      cursorX = state->cursorX.part[0].integer;
      cursorY = state->cursorY.part[0].integer;
    } else {
      state->cursorSpeed = MAP_SCREEN_CURSOR_MIN_SPEED;
    }
  }
#endif

  // The blinking frame round the nearest marker.
  if (nearestMarker != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = OBJ_MODE_NORMAL;
    marker->tile = tileBase + MAP_SCREEN_MARKER_NEAREST;
    marker->x = nearestX - 2;
    marker->y = nearestY - 2;
    Oam_AddSpriteAtDepth(marker, nearestY);
  }

  marker = &state->markers[MAP_SCREEN_CURSOR_PART];
  marker->affineIndex = 0;
  marker->x = cursorX - 17;
  marker->y = cursorY + 1;
  Oam_AddSpriteAtDepth(marker, MAP_SCREEN_CURSOR_DEPTH);

  // Redraw only when the selected marker changes, including to no marker.
  if (state->namedMarker != nearestMarker) {
    state->namedMarker = nearestMarker;
    WindowSprite_PrepareForRedrawFar(state->nameWindow);
    if (nearestMarker != -1) {
      // The party's marker: "Present location."
      if (nearestMarker == 0)
        nearestName = MsgPresentLocation;
      else
        nearestName = Area_GetNameIndex(nearestName, 1) + MsgDebugAreaName;
      Text_MeasureMessageFar(nearestName, &width, &height);
      cursorX = nearestX;
      cursorY = nearestY;
      cursorX -= MAP_SCREEN_NAME_LEFT;
      cursorY -= 11;
      if (cursorX + width > MAP_SCREEN_WIDTH) {
        // Past the right edge, it goes up and left of the marker.
        cursorX = MAP_SCREEN_WIDTH - 8 - width;
        cursorY = nearestY - 20;
      }
      if (cursorY < 0)
        cursorY = 0;
      Text_DrawMessageFar(nearestName, state->nameWindow, cursorX, cursorY);
    }
  }

  // Pulse the markers' transparency: they weigh blend sixteenths against the
  // image's 16 less blend (REG_BLDALPHA's EVA and EVB).
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA,
                      (BLEND_WEIGHT_MAX - blend) << 8 | blend,
                      IO_WRITE_HALFWORD);
}

/// The map screen, shown over the World Map when the player presses R, until
/// A or B closes it; it borrows the map-cell buffer for its pixels.
void MapScreen_Show(s32 picture) {
  struct MapGridScrollState *gridState = gMapGridState[0];
  struct FieldState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));
  struct ActorSystemState *menu = HEAP_SLOT_BLOCK(HEAP_SLOT_ACTOR_SYSTEM);
  s32 resource = RESOURCE_WORLD_MAP_PICTURE;
  struct TilesetAnim *layer;
  u8 savedFlags[sizeof(gridState->animations) /
                sizeof(gridState->animations[0])];
  s32 i;
  u8 *graphics;
  s32 savedTransitionFrames;

  if (state->mode != ACTOR_SYSTEM_WORLD_MAP)
    return;
  state->camera->paused = 1;
  savedTransitionFrames = state->transitionFrames;
  state->transitionFrames = 6;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  layer = gridState->animations;
  for (i = 0; i < (s32)(sizeof(savedFlags) / sizeof(savedFlags[0])); i++) {
    savedFlags[i] = layer->paused;
    layer->paused = 1;
    layer++;
  }
  menu->suspended = 1;
  wait(1);
  {
    s16 blend = REG_BLDALPHA;

    // The image's BG colours, then its 240 by 160 pixels of 256 colours, a
    // byte each, decoded into the borrowed buffer and copied to BG1's tiles.
    graphics = Resource_GetTableEntry(resource);
    Dma_Set(graphics, (void *)BG_PLTT,
            DMA_COPY_WORDS(MAP_SCREEN_PALETTE_BYTES / 4), REG_DMA3);
    BG_PLTT[0] = 0;
    Decompress_TaggedLz(graphics + MAP_SCREEN_PALETTE_BYTES,
                        gMapGridCellBuffer);
    Dma_Set(gMapGridCellBuffer,
            BG_CHAR_BLOCK(0) + WORLD_MAP_STILL_FIRST_TILE * TILE_SIZE_8BPP,
            DMA_COPY_WORDS(MAP_SCREEN_WIDTH * MAP_SCREEN_HEIGHT / 4), REG_DMA3);
    Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | BGCNT_PRIORITY(2),
               &REG_BG1CNT);
    Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                   DISPCNT_OBJ_1D_MAP,
               &REG_DISPCNT);
    MapScreen_Initialize();
    Task_Add(MapScreen_UpdateMarkers, TASK_PASS_FRAME);
    if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT)) {
      // "This place doesn't seem to be on the map..."
      FieldMessage_ShowFar(MsgNotOnMap, FIELD_MESSAGE_ZOOM);
    }
    do {
      wait(1);
    } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
    Task_Remove(MapScreen_UpdateMarkers);
    MapScreen_Release();
    Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
    WorldMap_LoadAreaGraphicsFar();
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, IO_WRITE_HALFWORD);
    layer = gridState->animations;
    for (i = 0; i < (s32)(sizeof(savedFlags) / sizeof(savedFlags[0])); i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    menu->suspended = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    state->transitionFrames = savedTransitionFrames;
    state->camera->paused = 0;
  }
}

/// On the World Map, shows only the scene's actors within 320 pixels
/// sideways, 600 above and 400 below the camera.
void Field_UpdateActorVisibilityBounds(void) {
  struct Actor *actor;
  FIXED x;
  FIXED y;
  FIXED left;
  FIXED right;
  FIXED top;
  FIXED bottom;
  u32 id;

  // The selected actor lookup has no used result; the bounds follow the
  // camera.
  Actor_GetById(gMainRoutineState.leaderActorId);
  actor = gFieldState->camera;
  x = actor->x.fixed;
  left = x - toFIXED(320);
  right = x + toFIXED(320);
  y = actor->z.fixed;
  top = y - toFIXED(600);
  bottom = y + toFIXED(400);

  for (id = ACTOR_FIRST_PLACED; id < MAP_SCREEN_MARKER_CAPACITY; id++) {
    actor = Actor_Lookup(id);

    if (actor != 0) {
      FIXED ox = actor->x.fixed;
      FIXED oy = actor->z.fixed;

      if (ox < left || ox > right || oy < top || oy > bottom) {
        actor->spriteStorageKind = ACTOR_SPRITE_NONE;
      } else {
        actor->spriteStorageKind = ACTOR_SPRITE_ONE;
      }
    }
  }
}

/// On the World Map, zooms the view out to FIXED_ONE * 8 / 13 while L is held,
/// then back.
void WorldMap_ZoomOutWhileLHeld(void) {
  struct FieldState *state =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldState));

  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    Task_SuspendOverlayTasks();
    Field_UpdateActorVisibilityBounds();
    Leader_StopMoving();
    WorldMap_StartZoom(FIXED_ONE * 8 / 13, 6);
    while (gKeysHeld & KEY_L) {
      wait(1);
    }
    WorldMap_StartZoom(FIXED_ONE, 6);
    Task_ResumeOverlayTasks();
  }
}

#undef MAP_SCREEN_MARKER_TILES
#undef MAP_SCREEN_MARKER_TILE_BYTES
#undef MAP_SCREEN_MARKER_ROW
#undef MAP_SCREEN_AREA_MARKER_TILE
#undef MAP_SCREEN_PALETTE_BYTES
#undef MAP_SCREEN_CURSOR_DEPTH
#undef MAP_SCREEN_WIDTH
#undef MAP_SCREEN_HEIGHT
#undef WORLD_MAP_WORLD_SIZE
#undef WORLD_MAP_X_ORIGIN
#undef WORLD_MAP_FIRST_AREA_ACTOR
#undef MAP_SCREEN_MARKER_CAPACITY
#undef MAP_SCREEN_CURSOR_PART
#undef MAP_SCREEN_CURSOR_MIN_SPEED
#undef MAP_SCREEN_CURSOR_MAX_SPEED
#undef MAP_SCREEN_CURSOR_ACCELERATION
#undef MAP_SCREEN_MARKER_BLEND_STEPS
#undef MAP_SCREEN_CURSOR_MIN_X
#undef MAP_SCREEN_CURSOR_MAX_X
#undef MAP_SCREEN_CURSOR_MAX_Y
#undef MAP_SCREEN_MARKER_ALLOCATION_SIZE
#undef MAP_SCREEN_CURSOR_SNAPS
#undef MAP_SCREEN_NAME_RANGE
#undef MAP_SCREEN_NAME_LEFT
