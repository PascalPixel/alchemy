/// @file
/// The World Map screen: area markers, the cursor and the area's
/// name.
// Part of event_commands.c.

#include "event_commands_internal.h"

/// The generated marker pictures: four 16-colour 8 by 8 tiles, 32 bytes each.
#define WORLD_MAP_MARKER_TILE_BYTES 128
#define WORLD_MAP_SCREEN_WIDTH 240  ///< the picture's width, the screen's
#define WORLD_MAP_SCREEN_HEIGHT 160 ///< the picture's height, the screen's
/// The World Map's size in field pixels each way, which the picture shows at
/// 240 by 160.
#define WORLD_MAP_WORLD_SIZE 4096
#define WORLD_MAP_X_ORIGIN px(4096) ///< field x of the picture's left edge
/// The first ordinary area actor, the first slot with a row of
/// gWorldPicture_AreaMarkers whatever the leader is.
#define WORLD_MAP_FIRST_AREA_ACTOR 11
#define WORLD_MAP_MARKER_CAPACITY 66 ///< one marker per actor slot
/// The cursor's sprite entry, after the markers'.
#define WORLD_MAP_CURSOR_PART WORLD_MAP_MARKER_CAPACITY
#define WORLD_MAP_CURSOR_MIN_SPEED px(1) ///< screen pixels a frame
#define WORLD_MAP_CURSOR_MAX_SPEED px(6) ///< screen pixels a frame
/// Added to the cursor's speed each frame the d-pad is held.
#define WORLD_MAP_CURSOR_ACCELERATION (px(1) / 8)
#define WORLD_MAP_MARKER_BLEND_STEPS 32 ///< gWorldMap_MarkerBlendCycle's rows
/// The cursor's limits, in screen pixels; y goes from 0.
///@{
#define WORLD_MAP_CURSOR_MIN_X 16
#define WORLD_MAP_CURSOR_MAX_X (WORLD_MAP_SCREEN_WIDTH - 1)
#define WORLD_MAP_CURSOR_MAX_Y 141
///@}

// BUG: the original reserves 32 bytes but writes and uploads 128. The bump
// allocator takes bytes, not words; reserve the whole tile buffer with BUGFIX.
#ifdef BUGFIX
#define WORLD_MAP_MARKER_ALLOCATION_SIZE WORLD_MAP_MARKER_TILE_BYTES
#else
#define WORLD_MAP_MARKER_ALLOCATION_SIZE 32
#endif

/// Scratch work while WorldPicture_Show owns gMapGridCellBuffer.
struct WorldPictureWork {
  u16 markerVramBlock;           ///< owns the four generated marker tiles
  u16 cursorVramBlock;           ///< owns the cursor picture's tiles
  union FieldCoordinate cursorX; ///< screen column, 16.16 pixels
  union FieldCoordinate cursorY; ///< screen row, 16.16 pixels
  u8 padding0c[6];               ///< not read by this screen
  s16 namedMarker;           ///< actor whose name is drawn; -1 clears the name
  u8 padding14[4];           ///< not read by this screen
  s32 cursorSpeed;           ///< 16.16 screen pixels per frame
  struct Window *nameWindow; ///< owned until this screen closes
  /// The visible areas' markers packed from the start, then the frame round
  /// the nearest; the last entry is the cursor.
  struct AnimationSpritePart markers[WORLD_MAP_MARKER_CAPACITY + 1];
};

STATIC_ASSERT(offsetof(struct WorldPictureWork, markers) == 0x20);
STATIC_ASSERT(sizeof(struct WorldPictureWork) == 0x344);
STATIC_ASSERT(WORLD_MAP_MARKER_CAPACITY ==
              sizeof(((struct FieldWork *)0)->actors) /
                  sizeof(((struct FieldWork *)0)->actors[0]));

/// Sets up the World Map screen: the marker tiles and their OAM entries, the
/// cursor where the leader is on the map (or mid-screen when the area is
/// not on it), and the name window.
void WorldPicture_Initialize(void) {
  u32 *entryWords; // the markers' sprite entries, a word at a time
  u32 *markerTiles;
  struct WorldPictureWork *work;
  struct Actor *actor;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;

  markerTiles = Heap_AllocEwram(WORLD_MAP_MARKER_ALLOCATION_SIZE);
  work = (struct WorldPictureWork *)gMapGridCellBuffer;
  entryWords = (u32 *)work->markers;
  actor = Actor_GetById(gMainRoutineWork.leaderActorId);
  work->markerVramBlock = VramCache_FindFreeEntry();
  Dma_Fill32(0, markerTiles, WORLD_MAP_MARKER_TILE_BYTES);
  // The marker tiles' pixel rows: eight 4-bit pixels a word, the leftmost
  // in the low bits. Each tile holds one dot in its top-left corner, with a
  // shadow in colour 1 to its right and below (# the dot, + the shadow):
  //
  //   tiles 0 to 2      tile 3
  //     ##.               .##..
  //     ##+               ####.
  //     .++               ####+
  //                       .##++
  //                       ..++.
  //
  // Tile 0, in colour 15, marks where the party is; tiles 1 and 2, in colours
  // 4 and 7, are the two kinds an area's row of gWorldPicture_AreaMarkers
  // asks for; tile 3, in colour 15, is the larger dot that blinks over the
  // marker nearest the cursor.
  markerTiles[0] = 0xff;
  markerTiles[1] = 0x1ff;
  markerTiles[2] = 0x110;
  markerTiles[8] = 0x44;
  markerTiles[9] = 0x144;
  markerTiles[10] = 0x110;
  markerTiles[16] = 0x77;
  markerTiles[17] = 0x177;
  markerTiles[18] = 0x110;
  markerTiles[24] = 0xff0;
  markerTiles[25] = 0xffff;
  markerTiles[26] = 0x1ffff;
  markerTiles[27] = 0x11ff0;
  markerTiles[28] = 0x1100;
  tileAttributes = VramCache_Load(work->markerVramBlock,
                                  WORLD_MAP_MARKER_TILE_BYTES, markerTiles) |
                   OBJ_PRIORITY(1);
  for (i = 0; i < WORLD_MAP_MARKER_CAPACITY; i++) {
    *entryWords++ = 0;
    *entryWords++ = 0;
    *entryWords++ = tileAttributes;
  }
  Heap_Release(markerTiles);
  if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT)) {
    work->cursorX.fixed = px(WORLD_MAP_SCREEN_WIDTH) / 2;
    work->cursorY.fixed = px(WORLD_MAP_SCREEN_HEIGHT) / 2;
  } else {
    work->cursorX.fixed = px(((actor->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
                             WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE);
    work->cursorY.fixed = px((actor->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT) /
                             WORLD_MAP_WORLD_SIZE);
  }
  work->cursorVramBlock = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramCacheEntries[work->cursorVramBlock].offset;
  work->nameWindow = Window_CreateFar(0, 0, 0, 0, WINDOW_FLAG_INSTANT);
  work->namedMarker = -1;
  work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
  *entryWords++ = 0;
  *entryWords++ = OBJ_SIZE_16X16;
  cursorAttributes = OBJ_PRIORITY(1);
  cursorAttributes |= tile >> 5;
  *entryWords = cursorAttributes;
}

#if TBS_PATCH_2
/// The cursor snaps to the nearest area when the d-pad is released.
#define WORLD_PICTURE_CURSOR_SNAPS 1
#define WORLD_PICTURE_NAME_RANGE                                               \
  64                          ///< squared distance that shows a marker's name
#define WORLD_MAP_NAME_LEFT 3 ///< how far left of its marker the name goes
#else
#define WORLD_PICTURE_CURSOR_SNAPS 0
#define WORLD_PICTURE_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

/// Closes the World Map screen: returns its VRAM blocks and removes the
/// name window.
void WorldPicture_Release(void) {
  struct WorldPictureWork *work = (struct WorldPictureWork *)gMapGridCellBuffer;

  VramCache_FreeEntry(work->markerVramBlock);
  VramCache_FreeEntry(work->cursorVramBlock);
  Window_CloseFar(work->nameWindow, 2);
}

/// Task of the World Map screen: shows a marker at each
/// area whose flag is set, moves the cursor with the d-pad (to the current
/// area while L or R is held) and shows the name of the marker nearest
/// the cursor.
void WorldPicture_UpdateMarkers(void) {
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
  struct WorldPictureWork *work;
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

#if WORLD_PICTURE_CURSOR_SNAPS
  leader = gMainRoutineWork.leaderActorId;
  work = (struct WorldPictureWork *)Ram_MapGridCellBuffer;
  marker = work->markers;
  place = gWorldPicture_AreaMarkers;
  tileBase = gVramCacheEntries[work->markerVramBlock].offset >> 5;
#else
  work = (struct WorldPictureWork *)Ram_MapGridCellBuffer;
  leader = gMainRoutineWork.leaderActorId;
  place = gWorldPicture_AreaMarkers;
  tileBase = gVramCacheEntries[work->markerVramBlock].offset >> 5;
  marker = work->markers;
#endif
  nearestMarker = -1;
  nearestDistanceSquared = WORLD_PICTURE_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) &
                                     (WORLD_MAP_MARKER_BLEND_STEPS - 1)];

  // FieldSequence_Run creates the selected actor before raised trigger -888
  // opens this screen. No actor task removes it while the marker task runs.
  if (!GameFlag_GetFar(FLAG_WORLD_MAP_LEFT) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L or R: the current area.
    areaActor = Actor_Lookup(gMainRoutineWork.leaderActorId);
#ifdef UBFIX
    // UB: with no selected actor the ROM leaves this frame's cursor
    // coordinates unset; UBFIX skips the frame.
    if (areaActor == NULL)
      return;
#endif
    if (areaActor != NULL) {
      cursorX = ((areaActor->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
                WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE;
      cursorY = areaActor->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT /
                WORLD_MAP_WORLD_SIZE;
    }
  } else {
    // The d-pad: faster the longer it is held.
    angle = gWorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
    if (angle != 0xffff) {
      position.x = work->cursorX.fixed;
      position.y = 0;
      position.z = work->cursorY.fixed;
      Vector_AddPolarOffset(work->cursorSpeed, angle, &position);
      if (position.x < px(WORLD_MAP_CURSOR_MIN_X))
        position.x = px(WORLD_MAP_CURSOR_MIN_X);
      if (position.x > px(WORLD_MAP_CURSOR_MAX_X))
        position.x = px(WORLD_MAP_CURSOR_MAX_X);
      if (position.z < 0)
        position.z = 0;
      if (position.z > px(WORLD_MAP_CURSOR_MAX_Y))
        position.z = px(WORLD_MAP_CURSOR_MAX_Y);
      work->cursorX.fixed = position.x;
      work->cursorY.fixed = position.z;
      if (work->cursorSpeed < WORLD_MAP_CURSOR_MAX_SPEED)
        work->cursorSpeed += WORLD_MAP_CURSOR_ACCELERATION;
    } else {
      work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
    }
    cursorX = work->cursorX.part.pixel;
    cursorY = work->cursorY.part.pixel;
  }

  // Actor id 0 consumes no marker row.
  for (markerIndex = 0; markerIndex < WORLD_MAP_MARKER_CAPACITY;
       markerIndex++) {
    if (markerIndex != leader && markerIndex < WORLD_MAP_FIRST_AREA_ACTOR)
      continue;
    if (markerIndex == 0) {
      if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT))
        continue;
      tile = 0;
      flag = leader;
      mode = 0;
    } else {
      flag = *place++;
      message = *place++;
      if (flag == 0)
        continue;
#if WORLD_PICTURE_CURSOR_SNAPS
      if (flag == -1)
        break;
      tile = flag >> 16;
      mode = 1;
#else
      tile = flag >> 16;
      mode = 1;
      if (flag == -1)
        break;
#endif
    }
    if (!GameFlag_GetFar(flag))
      continue;
    areaActor = Actor_Lookup(markerIndex);
    if (areaActor == NULL)
      continue;

    // Field x starts at 4096; the picture covers 4096 square world pixels.
    x = ((areaActor->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
        WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE;
    y = areaActor->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT /
        WORLD_MAP_WORLD_SIZE;
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
#if WORLD_PICTURE_CURSOR_SNAPS
  // With the d-pad released, the cursor moves onto the nearest marker.
  if (gKeysHeld == 0) {
    if (nearestMarker != -1) {
      if (nearestDistanceSquared <= 1) {
        work->cursorX.fixed = px(nearestX);
        work->cursorY.fixed = px(nearestY);
        work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
      } else {
        if (work->cursorSpeed < px(2))
          work->cursorSpeed = px(2);
        angle = ArcTan2(nearestY - cursorY, nearestX - cursorX);
        position.x = work->cursorX.fixed;
        position.y = 0;
        position.z = work->cursorY.fixed;
        Vector_AddPolarOffset(work->cursorSpeed, (u16)angle, &position);
        work->cursorX.fixed = position.x;
        work->cursorY.fixed = position.z;
        if (work->cursorSpeed < WORLD_MAP_CURSOR_MAX_SPEED)
          work->cursorSpeed += WORLD_MAP_CURSOR_ACCELERATION;
      }
      cursorX = work->cursorX.part.pixel;
      cursorY = work->cursorY.part.pixel;
    } else {
      work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
    }
  }
#endif

  // The blinking frame round the nearest marker.
  if (nearestMarker != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = OBJ_MODE_NORMAL;
    marker->tile = tileBase + 3;
    marker->x = nearestX - 2;
    marker->y = nearestY - 2;
    Oam_AddSpriteAtDepth(marker, nearestY);
  }

  marker = &work->markers[WORLD_MAP_CURSOR_PART];
  marker->affineIndex = 0;
  marker->x = cursorX - 17;
  marker->y = cursorY + 1;
  Oam_AddSpriteAtDepth(marker, 246);

  // Redraw only when the selected marker changes, including to no marker.
  if (work->namedMarker != nearestMarker) {
    work->namedMarker = nearestMarker;
    WindowSprite_PrepareForRedrawFar(work->nameWindow);
    if (nearestMarker != -1) {
      if (nearestMarker == 0)
        nearestName = MsgPresentLocation;
      else
        nearestName = Area_GetNameIndex(nearestName, 1) + MsgDebugAreaName;
      Text_MeasureMessageFar(nearestName, &width, &height);
      cursorX = nearestX;
      cursorY = nearestY;
      cursorX -= WORLD_MAP_NAME_LEFT;
      cursorY -= 11;
      if (cursorX + width > WORLD_MAP_SCREEN_WIDTH) {
        // Past the right edge, it goes up and left of the marker.
        cursorX = WORLD_MAP_SCREEN_WIDTH - 8 - width;
        cursorY = nearestY - 20;
      }
      if (cursorY < 0)
        cursorY = 0;
      Text_DrawMessageFar(nearestName, work->nameWindow, cursorX, cursorY);
    }
  }

  // Pulse the markers' transparency: they weigh blend sixteenths against the
  // picture's 16 less blend (REG_BLDALPHA's EVA and EVB).
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend,
                      IO_WRITE_16);
}

/// Borrows the map-cell buffer for the still map until A or B closes it.
/// FieldSequence_Run reaches this for raised trigger -888 in World Map mode.
void WorldPicture_Show(s32 picture) {
  struct MapGridScrollWork *gridState = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  struct FieldWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  struct ActorSystemWork *menu = HEAP_SLOT_BLOCK(HEAP_SLOT_ACTOR_SYSTEM);
  s32 resource = RESOURCE_WORLD_MAP_PICTURE;
  struct TilesetAnim *layer;
  u8 savedFlags[sizeof(gridState->animations) /
                sizeof(gridState->animations[0])];
  s32 i;
  u8 *graphics;
  s32 savedTransitionFrames;

  if (work->mode != ACTOR_SYSTEM_WORLD_MAP)
    return;
  work->camera->paused = 1;
  savedTransitionFrames = work->transitionFrames;
  work->transitionFrames = 6;
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

    // The picture's 224 BG colours (448 bytes), then its 240 by 160 pixels of
    // 256 colours, a byte each, decoded into the borrowed buffer and copied
    // to BG1's tiles.
    graphics = Resource_GetTableEntry(resource);
    Dma_Set(graphics, (void *)BG_PLTT, DMA_COPY_WORDS(448 / 4), REG_DMA3);
    BG_PLTT[0] = 0;
    Decompress_TaggedLz(graphics + 448, gMapGridCellBuffer);
    Dma_Set(
        gMapGridCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
        DMA_COPY_WORDS(WORLD_MAP_SCREEN_WIDTH * WORLD_MAP_SCREEN_HEIGHT / 4),
        REG_DMA3);
    Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
    Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                   DISPCNT_OBJ_1D_MAP,
               &REG_DISPCNT);
    WorldPicture_Initialize();
    Task_Add(WorldPicture_UpdateMarkers, TASK_PASS_FRAME);
    if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT))
      FieldMessage_ShowFar(MsgNotOnMap, FIELD_MESSAGE_ZOOM);
    do {
      wait(1);
    } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
    Task_Remove(WorldPicture_UpdateMarkers);
    WorldPicture_Release();
    Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
    WorldMap_LoadAreaGraphicsFar();
    IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, IO_WRITE_16);
    layer = gridState->animations;
    for (i = 0; i < (s32)(sizeof(savedFlags) / sizeof(savedFlags[0])); i++) {
      layer->paused = savedFlags[i];
      layer++;
    }
    menu->suspended = 0;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    work->transitionFrames = savedTransitionFrames;
    work->camera->paused = 0;
  }
}

/// On the World Map, shows only the scene's actors within 320 pixels
/// sideways, 600 above and 400 below the camera.
void Field_UpdateActorVisibilityBounds(void) {
  struct Actor *actor;
  s32 x;
  s32 y;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  u32 id;

  // The selected actor lookup has no used result; the bounds follow the
  // camera.
  Actor_GetById(gMainRoutineWork.leaderActorId);
  actor = gFieldWork->camera;
  x = actor->x.fixed;
  left = x - px(320);
  right = x + px(320);
  y = actor->z.fixed;
  top = y - px(600);
  bottom = y + px(400);

  id = ACTOR_FIRST_PLACED;
  do {
    actor = Actor_Lookup(id);

    if (actor != 0) {
      s32 ox = actor->x.fixed;
      s32 oy = actor->z.fixed;

      if (ox < left || ox > right || oy < top || oy > bottom) {
        actor->spriteStorageKind = ACTOR_SPRITE_NONE;
      } else {
        actor->spriteStorageKind = ACTOR_SPRITE_ONE;
      }
    }
    id++;
  } while (id < WORLD_MAP_MARKER_CAPACITY);
}

/// On the World Map, zooms the view out to Q16_ONE * 8 / 13 while L is held,
/// then back.
void WorldMap_ZoomOutWhileLHeld(void) {
  struct FieldWork *work =
      HeapSlot_AllocEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));

  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    Task_SuspendOverlayTasks();
    Field_UpdateActorVisibilityBounds();
    Leader_StopMoving();
    WorldMap_StartZoom(Q16_ONE * 8 / 13, 6);
    while (gKeysHeld & KEY_L) {
      wait(1);
    }
    WorldMap_StartZoom(Q16_ONE, 6);
    Task_ResumeOverlayTasks();
  }
}

#undef WORLD_MAP_MARKER_TILE_BYTES
#undef WORLD_MAP_SCREEN_WIDTH
#undef WORLD_MAP_SCREEN_HEIGHT
#undef WORLD_MAP_WORLD_SIZE
#undef WORLD_MAP_X_ORIGIN
#undef WORLD_MAP_FIRST_AREA_ACTOR
#undef WORLD_MAP_MARKER_CAPACITY
#undef WORLD_MAP_CURSOR_PART
#undef WORLD_MAP_CURSOR_MIN_SPEED
#undef WORLD_MAP_CURSOR_MAX_SPEED
#undef WORLD_MAP_CURSOR_ACCELERATION
#undef WORLD_MAP_MARKER_BLEND_STEPS
#undef WORLD_MAP_CURSOR_MIN_X
#undef WORLD_MAP_CURSOR_MAX_X
#undef WORLD_MAP_CURSOR_MAX_Y
#undef WORLD_MAP_MARKER_ALLOCATION_SIZE
#undef WORLD_PICTURE_CURSOR_SNAPS
#undef WORLD_PICTURE_NAME_RANGE
#undef WORLD_MAP_NAME_LEFT
