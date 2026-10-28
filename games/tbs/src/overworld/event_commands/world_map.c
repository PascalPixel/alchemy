/// @file
/// The World Map screen: area markers, the cursor and the area's
/// name.
// Part of event_commands.c.

#include "../event_commands_internal.h"

/// The generated marker pictures: four 16-colour 8 by 8 tiles, 32 bytes each.
#define WORLD_MAP_MARKER_TILE_BYTES 0x80
#define WORLD_MAP_SCREEN_WIDTH 240  ///< the picture's width, the screen's
#define WORLD_MAP_SCREEN_HEIGHT 160 ///< the picture's height, the screen's
/// The World Map's size in field pixels each way, which the picture shows at
/// 240 by 160.
#define WORLD_MAP_WORLD_SIZE 4096
#define WORLD_MAP_X_ORIGIN PIXELS(4096) ///< field x of the picture's left edge
/// The first ordinary area actor, the first slot with a row of
/// gWorldPicture_AreaMarkers whatever the leader is.
#define WORLD_MAP_FIRST_AREA_ACTOR 11
#define WORLD_MAP_MARKER_CAPACITY 66 ///< one marker per actor slot
/// The cursor's sprite entry, after the markers'.
#define WORLD_MAP_CURSOR_PART WORLD_MAP_MARKER_CAPACITY
#define WORLD_MAP_CURSOR_MIN_SPEED PIXELS(1) ///< screen pixels a frame
#define WORLD_MAP_CURSOR_MAX_SPEED PIXELS(6) ///< screen pixels a frame
/// Added to the cursor's speed each frame the d-pad is held.
#define WORLD_MAP_CURSOR_ACCELERATION (PIXELS(1) / 8)
#define WORLD_MAP_MARKER_BLEND_STEPS 32 ///< gWorldMap_MarkerBlendCycle's rows
/// The cursor's limits, in screen pixels; y goes from 0.
///@{
#define WORLD_MAP_CURSOR_MIN_X 16
#define WORLD_MAP_CURSOR_MAX_X (WORLD_MAP_SCREEN_WIDTH - 1)
#define WORLD_MAP_CURSOR_MAX_Y 141
///@}

/// The renderer's linked sprite entry and the three words that initialize it.
union WorldPictureMarkerPart {
  struct AnimationSpritePart sprite;
  u32 words[3];
};

#ifdef UBFIX
#define WORLD_MAP_MARKER_VIEW(marker) ((marker)->sprite)
#else
#define WORLD_MAP_MARKER_VIEW(marker) (*(marker))
#endif

STATIC_ASSERT(sizeof(union WorldPictureMarkerPart) ==
              sizeof(struct AnimationSpritePart));

// BUG: the original reserves 32 bytes but writes and uploads 128. The bump
// allocator takes bytes, not words; reserve the whole tile buffer with BUGFIX.
#ifdef BUGFIX
#define WORLD_MAP_MARKER_ALLOCATION_SIZE WORLD_MAP_MARKER_TILE_BYTES
#else
#define WORLD_MAP_MARKER_ALLOCATION_SIZE 32
#endif

/// Scratch work while WorldPicture_Show owns gMapGridCellBuffer. Setup runs
/// before the marker task; cleanup removes its window before map graphics reuse
/// it.
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
  // Packed visible areas, then a nearest-section frame; the last is the
  // cursor.
#ifdef UBFIX
  union WorldPictureMarkerPart markers[WORLD_MAP_MARKER_CAPACITY + 1];
#else
  struct AnimationSpritePart markers[WORLD_MAP_MARKER_CAPACITY + 1];
#endif
};

STATIC_ASSERT(offsetof(struct WorldPictureWork, markers) == 0x20);
STATIC_ASSERT(sizeof(struct WorldPictureWork) == 0x344);
STATIC_ASSERT(WORLD_MAP_MARKER_CAPACITY ==
              sizeof(((struct FieldWork *)0)->actors) /
                  sizeof(((struct FieldWork *)0)->actors[0]));

/// Sets up the World Map screen: the marker tiles and their OAM entries, the
/// cursor where the leader is on the map (or mid-screen when the area is
/// not on it), and the name window.
void FieldEffect_SetupResourcesAndWindow(void) {
#ifdef UBFIX
  union WorldPictureMarkerPart *marker;
#else
  u32 *oam;
#endif
  u32 *markerTiles;
  struct WorldPictureWork *work;
  struct Actor *actor;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;

  markerTiles = AllocEwram(WORLD_MAP_MARKER_ALLOCATION_SIZE);
  work = (struct WorldPictureWork *)gMapGridCellBuffer;
#ifdef UBFIX
  marker = work->markers;
#else
  // UB: the flat word view runs across sprite entries. Typed word/sprite
  // unions change tbs-en setup from 368 to 364 bytes.
  oam = (u32 *)work->markers;
#endif
  actor = Actor_GetById(gCommon.leaderActorId);
  work->markerVramBlock = VramCache_FindFreeEntry();
  Dma_Fill32(0, markerTiles, WORLD_MAP_MARKER_TILE_BYTES);
  markerTiles[0] = 255;
  markerTiles[1] = 0x1ff;
  markerTiles[2] = 272;
  markerTiles[8] = 68;
  markerTiles[9] = 324;
  markerTiles[10] = 272;
  markerTiles[16] = 119;
  markerTiles[17] = 0x177;
  markerTiles[18] = 272;
  markerTiles[24] = 0xff0;
  markerTiles[25] = 0xffff;
  markerTiles[26] = 0x1ffff;
  markerTiles[27] = 0x11ff0;
  markerTiles[28] = 0x1100;
  tileAttributes = VramCache_Load(work->markerVramBlock,
                                  WORLD_MAP_MARKER_TILE_BYTES, markerTiles) |
                   0x400;
  for (i = 0; i < WORLD_MAP_MARKER_CAPACITY; i++) {
#ifdef UBFIX
    marker->words[0] = 0;
    marker->words[1] = 0;
    marker->words[2] = tileAttributes;
    marker++;
#else
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = tileAttributes;
#endif
  }
  Free(markerTiles);
  if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT)) {
    work->cursorX.fixed = WORLD_MAP_SCREEN_WIDTH << 15;
    work->cursorY.fixed = WORLD_MAP_SCREEN_HEIGHT << 15;
  } else {
    work->cursorX.fixed = (((actor->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
                           WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE)
                          << 16;
    work->cursorY.fixed =
        ((actor->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT) / WORLD_MAP_WORLD_SIZE)
        << 16;
  }
  work->cursorVramBlock = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramCacheEntries[work->cursorVramBlock].offset;
  work->nameWindow = Window_CreateFar(0, 0, 0, 0, WINDOW_FLAG_INSTANT);
  work->namedMarker = -1;
  work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
#ifdef UBFIX
  marker->words[0] = 0;
  marker->words[1] = MENU_SPRITE_SHAPE_16X16;
#else
  *oam++ = 0;
  *oam++ = MENU_SPRITE_SHAPE_16X16;
#endif
  cursorAttributes = 0x400;
  cursorAttributes |= tile >> 5;
#ifdef UBFIX
  marker->words[2] = cursorAttributes;
#else
  *oam = cursorAttributes;
#endif
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
void FieldEffect_CleanupResourcesAndWindow(void) {
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
#ifdef UBFIX
  union WorldPictureMarkerPart *marker;
#else
  struct AnimationSpritePart *marker;
#endif
  s32 x;
  s32 y;
  s32 angle;

#if WORLD_PICTURE_CURSOR_SNAPS
  leader = gCommon.leaderActorId;
  work = (struct WorldPictureWork *)Ram_MapGridCellBuffer;
  marker = work->markers;
  place = gWorldPicture_AreaMarkers;
  tileBase = gVramCacheEntries[work->markerVramBlock].offset >> 5;
#else
  work = (struct WorldPictureWork *)Ram_MapGridCellBuffer;
  leader = gCommon.leaderActorId;
  place = gWorldPicture_AreaMarkers;
  tileBase = gVramCacheEntries[work->markerVramBlock].offset >> 5;
  marker = work->markers;
#endif
  nearestMarker = -1;
  nearestDistanceSquared = WORLD_PICTURE_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) &
                                     (WORLD_MAP_MARKER_BLEND_STEPS - 1)];

  // SceneSequence_Run creates the selected actor before raised trigger -888
  // opens this screen. No actor task removes it while the marker task runs.
  if (!GameFlag_GetFar(FLAG_WORLD_MAP_LEFT) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L or R: the current area.
    areaActor = Actor_Lookup(gCommon.leaderActorId);
#ifdef UBFIX
    // The native null branch leaves this frame's cursor coordinates unset.
    // A missing selected actor violates the screen's lifetime contract;
    // skipping that frame changes tbs-en from 1088 to 1092 bytes.
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
      if (position.x < PIXELS(WORLD_MAP_CURSOR_MIN_X))
        position.x = PIXELS(WORLD_MAP_CURSOR_MIN_X);
      if (position.x > PIXELS(WORLD_MAP_CURSOR_MAX_X))
        position.x = PIXELS(WORLD_MAP_CURSOR_MAX_X);
      if (position.z < 0)
        position.z = 0;
      if (position.z > PIXELS(WORLD_MAP_CURSOR_MAX_Y))
        position.z = PIXELS(WORLD_MAP_CURSOR_MAX_Y);
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

  // Actor id 0 consumes no marker row. Ordinary rows begin at actor id 11; a
  // selected slot in 1..10 also consumes one. Zero rows reserve actors
  // with no marker and -1 ends the table. A row is (tile << 16 | flag, map id).
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
    WORLD_MAP_MARKER_VIEW(marker).objectMode = mode;
    WORLD_MAP_MARKER_VIEW(marker).tile = tileBase + tile;
    WORLD_MAP_MARKER_VIEW(marker).x = x - 1;
    WORLD_MAP_MARKER_VIEW(marker).y = y - 1;
    width = x - cursorX;
    height = y - cursorY;
    if (width * width + height * height < nearestDistanceSquared) {
      // UB: marker 0 has no table message. Its unused native read keeps the
      // 1088-byte tbs-en body; skip the read for that marker with UBFIX.
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
      Oam_AddSpriteAtDepth(&WORLD_MAP_MARKER_VIEW(marker++), y);
  }
#if WORLD_PICTURE_CURSOR_SNAPS
  // With the d-pad released, the cursor moves onto the nearest marker.
  if (gKeysHeld == 0) {
    if (nearestMarker != -1) {
      if (nearestDistanceSquared <= 1) {
        work->cursorX.fixed = nearestX << 16;
        work->cursorY.fixed = nearestY << 16;
        work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
      } else {
        if (work->cursorSpeed < PIXELS(2))
          work->cursorSpeed = PIXELS(2);
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
    WORLD_MAP_MARKER_VIEW(marker).objectMode = OBJ_MODE_NORMAL;
    WORLD_MAP_MARKER_VIEW(marker).tile = tileBase + 3;
    WORLD_MAP_MARKER_VIEW(marker).x = nearestX - 2;
    WORLD_MAP_MARKER_VIEW(marker).y = nearestY - 2;
    Oam_AddSpriteAtDepth(&WORLD_MAP_MARKER_VIEW(marker), nearestY);
  }

  // The cursor.
  marker = &work->markers[WORLD_MAP_CURSOR_PART];
  WORLD_MAP_MARKER_VIEW(marker).affineIndex = 0;
  WORLD_MAP_MARKER_VIEW(marker).x = cursorX - 17;
  WORLD_MAP_MARKER_VIEW(marker).y = cursorY + 1;
  Oam_AddSpriteAtDepth(&WORLD_MAP_MARKER_VIEW(marker), 246);

  // Redraw only when the selected marker changes, including to no marker.
  if (work->namedMarker != nearestMarker) {
    work->namedMarker = nearestMarker;
    WindowSprite_PrepareForRedrawFar(work->nameWindow);
    if (nearestMarker != -1) {
      if (nearestMarker == 0)
        nearestName = MsgPresentLocation;
      else
        nearestName = Area_GetNameIndex(nearestName, 1) + MsgDebugEntryName;
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

  // Pulse the markers' transparency: they weigh blend sixteenths against
  // the picture's 16 less blend (REG_BLDALPHA's EVA and EVB).
  // Written out: BLEND_ALPHA(blend, 16 - blend) takes its operands the other
  // way round and changes the registers.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend,
                      IO_WRITE_16);
}

/// Borrows the map-cell buffer for the still map until A or B closes it.
/// SceneSequence_Run reaches this for raised trigger -888 in World Map mode.
/// The marker task stops before map graphics are restored, so neither uses the
/// other's scratch storage. The picture argument is unused; the resource is
/// fixed.
void WorldPicture_Show(s32 picture) {
  struct MapGridScrollAnimView *gridState = HEAP_SLOT_BLOCK(HEAP_SLOT_MAP_GRID);
  struct FieldWork *work =
      AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));
  struct ActorSystemWork *menu = HEAP_SLOT_BLOCK(HEAP_SLOT_ACTOR_SYSTEM);
  s32 resource = (s32)&ResourceId_WorldMapPicture;
  struct TilesetAnim *layer;
  u8 savedFlags[sizeof(gridState->animations) /
                sizeof(gridState->animations[0])];
  s32 i;
  u8 *graphics;
  s32 savedStatus;
  s16 blend;

  if (work->mode != ACTOR_SYSTEM_WORLD_MAP)
    return;
  work->camera->paused = 1;
  savedStatus = work->transitionFrames;
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
  WaitFrames(1);
  blend = REG_BLDALPHA;
  // The picture's 224 BG colours (448 bytes), then its 240 by 160 pixels of
  // 256 colours, a byte each, decoded into the borrowed buffer and copied
  // to BG1's tiles.
  graphics = Resource_GetTableEntry(resource);
  Dma_Set(graphics, (void *)BG_PLTT,
          (u32)(DMA_ENABLE | DMA_32BIT) << 16 | (448 / 4), REG_DMA3);
  BG_PLTT[0] = 0;
  Decompress_TaggedLz(graphics + 448, gMapGridCellBuffer);
  Dma_Set(gMapGridCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          (u32)(DMA_ENABLE | DMA_32BIT) << 16 |
              (WORLD_MAP_SCREEN_WIDTH * WORLD_MAP_SCREEN_HEIGHT / 4),
          REG_DMA3);
  Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
  Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                 DISPCNT_OBJ_1D_MAP,
             &REG_DISPCNT);
  FieldEffect_SetupResourcesAndWindow();
  Task_Add(WorldPicture_UpdateMarkers, TASK_PASS_FRAME);
  if (GameFlag_GetFar(FLAG_WORLD_MAP_LEFT))
    FieldMessage_ShowFar(MsgNotOnMap, FIELD_MESSAGE_ZOOM);
  do {
    WaitFrames(1);
  } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
  Task_Remove(WorldPicture_UpdateMarkers);
  FieldEffect_CleanupResourcesAndWindow();
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
  work->transitionFrames = savedStatus;
  work->camera->paused = 0;
}

/// On the World Map, shows only the scene's actors within 320 pixels
/// sideways, 600 above and 400 below the camera.
void Field_UpdateActorVisibilityBounds(void) {
  struct Actor *object;
  s32 x;
  s32 y;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  u32 id;

  // The selected actor lookup has no used result; the bounds follow the
  // camera.
  Actor_GetById(gCommon.leaderActorId);
  object = gFieldWork->camera;
  x = object->x.fixed;
  left = x - (320 << 16);
  right = x + (320 << 16);
  y = object->z.fixed;
  top = y - (600 << 16);
  bottom = y + (400 << 16);

  id = ACTOR_FIRST_PLACED;
  do {
    object = Actor_Lookup(id);

    if (object != 0) {
      s32 ox = object->x.fixed;
      s32 oy = object->z.fixed;

      if (ox < left || ox > right || oy < top || oy > bottom) {
        object->spriteStorageKind = ACTOR_SPRITE_NONE;
      } else {
        object->spriteStorageKind = ACTOR_SPRITE_ONE;
      }
    }
    id++;
  } while (id < WORLD_MAP_MARKER_CAPACITY);
}

/// On the World Map, zooms the view out (0x9d89) while L is held, then back.
/// Each zoom takes 6 frames; 0x9d89 is about 0.62, which sets the view 1.6
/// times as far (FieldEffect_ScheduleRatioTransition). SceneSequence_Run runs
/// it for event -889; the overlay tasks are suspended meanwhile.
void FieldEffect_RunVisibilityTransition(void) {
  struct FieldWork *work =
      AllocHeapSlotEwram(HEAP_SLOT_FIELD, sizeof(struct FieldWork));

  if (work->mode == ACTOR_SYSTEM_WORLD_MAP) {
    Task_SuspendOverlayTasks();
    Field_UpdateActorVisibilityBounds();
    Leader_StopMoving();
    FieldEffect_ScheduleRatioTransition(0x9d89, 6);
    if (gKeysHeld & KEY_L) {
      do {
        WaitFrames(1);
      } while (gKeysHeld & KEY_L);
    }
    FieldEffect_ScheduleRatioTransition(Q16_ONE, 6);
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
#undef WORLD_MAP_MARKER_VIEW
#undef WORLD_MAP_MARKER_ALLOCATION_SIZE
#undef WORLD_PICTURE_CURSOR_SNAPS
#undef WORLD_PICTURE_NAME_RANGE
#undef WORLD_MAP_NAME_LEFT
