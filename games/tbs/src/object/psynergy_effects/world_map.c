/// @file
/// The world map screen: place markers, the cursor and the place name.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// The generated marker pictures: four 16-colour 8 by 8 tiles, 32 bytes each.
#define WORLD_MAP_MARKER_TILE_BYTES 0x80
#define WORLD_MAP_SCREEN_WIDTH 240  ///< the picture's width, the screen's
#define WORLD_MAP_SCREEN_HEIGHT 160 ///< the picture's height, the screen's
/// The world map's size in field pixels each way, which the picture shows at
/// 240 by 160.
#define WORLD_MAP_WORLD_SIZE 4096
#define WORLD_MAP_X_ORIGIN PIXELS(4096) ///< field x of the picture's left edge
/// The first ordinary place actor, the first slot with a row of
/// gWorldMap_PlaceMarkers whatever the leader is.
#define WORLD_MAP_FIRST_PLACE_ACTOR 11
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
union WorldMapMarkerPart {
  struct AnimationSpritePart sprite;
  u32 words[3];
};

#ifdef UBFIX
#define WORLD_MAP_MARKER_VIEW(marker) ((marker)->sprite)
#else
#define WORLD_MAP_MARKER_VIEW(marker) (*(marker))
#endif

STATIC_ASSERT(sizeof(union WorldMapMarkerPart) ==
              sizeof(struct AnimationSpritePart));

// BUG: the original reserves 32 bytes but writes and uploads 128. The bump
// allocator takes bytes, not words; reserve the whole tile buffer with BUGFIX.
#ifdef BUGFIX
#define WORLD_MAP_MARKER_ALLOCATION_SIZE WORLD_MAP_MARKER_TILE_BYTES
#else
#define WORLD_MAP_MARKER_ALLOCATION_SIZE 32
#endif

/// Scratch work while Map_ShowWorldMap owns gMapCellBuffer. Setup runs before
/// the marker task; cleanup removes its window before map graphics reuse it.
struct WorldMapWork {
  u16 markerVramBlock;           ///< owns the four generated marker tiles
  u16 cursorVramBlock;           ///< owns the cursor picture's tiles
  union FieldCoordinate cursorX; ///< screen column, 16.16 pixels
  union FieldCoordinate cursorY; ///< screen row, 16.16 pixels
  u8 padding0c[6];               ///< not read by this screen
  s16 namedMarker; ///< actor whose name is drawn; -1 clears the name
  u8 padding14[4]; ///< not read by this screen
  s32 cursorSpeed; ///< 16.16 screen pixels per frame
  struct UiWindow *nameWindow; ///< owned until this screen closes
  // Packed visible places, then a nearest-place frame; the last is the cursor.
#ifdef UBFIX
  union WorldMapMarkerPart markers[WORLD_MAP_MARKER_CAPACITY + 1];
#else
  struct AnimationSpritePart markers[WORLD_MAP_MARKER_CAPACITY + 1];
#endif
};

STATIC_ASSERT(offsetof(struct WorldMapWork, markers) == 0x20);
STATIC_ASSERT(sizeof(struct WorldMapWork) == 0x344);
STATIC_ASSERT(WORLD_MAP_MARKER_CAPACITY ==
              sizeof(((struct FieldWork *)0)->actors) /
                  sizeof(((struct FieldWork *)0)->actors[0]));

/// Sets up the world map screen: the marker tiles and their OAM entries, the
/// cursor where the leader is on the map (or mid-screen when the place is not
/// on it), and the place-name window.
void BattleFx_SetupResourcesAndWindow(void) {
#ifdef UBFIX
  union WorldMapMarkerPart *marker;
#else
  u32 *oam;
#endif
  u32 *markerTiles;
  struct WorldMapWork *work;
  struct FieldActor *object;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;

  markerTiles =
      Runtime_BumpAllocateAlternatePool(WORLD_MAP_MARKER_ALLOCATION_SIZE);
  work = (struct WorldMapWork *)gMapCellBuffer;
#ifdef UBFIX
  marker = work->markers;
#else
  // UB: the flat word view runs across sprite records. Typed word/sprite
  // unions change tbs-en setup from 368 to 364 bytes.
  oam = (u32 *)work->markers;
#endif
  object = Object_GetById(gGameState.selectedActor);
  work->markerVramBlock = Resource_FindFreeEntry();
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
  MemFree(markerTiles);
  if (GameFlag_TestFar(FLAG_NOT_ON_MAP)) {
    work->cursorX.fixed = WORLD_MAP_SCREEN_WIDTH << 15;
    work->cursorY.fixed = WORLD_MAP_SCREEN_HEIGHT << 15;
  } else {
    work->cursorX.fixed = (((object->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
                           WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE)
                          << 16;
    work->cursorY.fixed = ((object->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT) /
                           WORLD_MAP_WORLD_SIZE)
                          << 16;
  }
  work->cursorVramBlock = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramBlockCache[work->cursorVramBlock].offset;
  work->nameWindow = UiWindow_CreateFar(0, 0, 0, 0, 2);
  work->namedMarker = -1;
  work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
#ifdef UBFIX
  marker->words[0] = 0;
  marker->words[1] = 0x40000000;
#else
  *oam++ = 0;
  *oam++ = 0x40000000;
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
/// The cursor snaps to the nearest place when the d-pad is released.
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 ///< squared distance that shows a place's name
#define WORLD_MAP_NAME_LEFT 3   ///< how far left of its marker the name goes
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

/// Closes the world map screen: returns its VRAM blocks and removes the
/// place-name window.
void BattleFx_CleanupResourcesAndWindow(void) {
  struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

  Resource_ResetEntry(work->markerVramBlock);
  Resource_ResetEntry(work->cursorVramBlock);
  UiWork_FinalizeFar(work->nameWindow, 2);
}

/// Scheduler callback of the world map screen: shows a marker at each place
/// whose flag is set, moves the cursor with the d-pad (to the current place
/// while L or R is held) and shows the name of the marker nearest the cursor.
void Map_UpdateWorldMapMarkers(void) {
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
  struct FieldPosition pos;
  struct WorldMapWork *work;
  struct FieldActor *placeActor;
  s32 cursorY;
  s32 tile;
  s32 mode;
  s32 flag;
  s32 markerIndex;
#ifdef UBFIX
  union WorldMapMarkerPart *marker;
#else
  struct AnimationSpritePart *marker;
#endif
  s32 x;
  s32 y;
  s32 angle;

#if WORLD_MAP_CURSOR_SNAPS
  leader = gGameState.selectedActor;
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  marker = work->markers;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->markerVramBlock].offset >> 5;
#else
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  leader = gGameState.selectedActor;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->markerVramBlock].offset >> 5;
  marker = work->markers;
#endif
  nearestMarker = -1;
  nearestDistanceSquared = WORLD_MAP_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) &
                                     (WORLD_MAP_MARKER_BLEND_STEPS - 1)];

  // Field_RunScene creates the selected actor before event -888 opens this
  // screen. No actor task removes it while the marker callback runs.
  if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L or R: the current place.
    placeActor = ObjectTable_Get(gGameState.selectedActor);
#ifdef UBFIX
    // The native null branch leaves this frame's cursor coordinates unset.
    // A missing selected actor violates the screen's lifetime contract;
    // skipping that frame changes tbs-en from 1088 to 1092 bytes.
    if (placeActor == NULL)
      return;
#endif
    if (placeActor != NULL) {
      cursorX = ((placeActor->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
                WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE;
      cursorY = placeActor->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT /
                WORLD_MAP_WORLD_SIZE;
    }
  } else {
    // The d-pad: faster the longer it is held.
    angle = gWorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
    if (angle != 0xffff) {
      pos.x = work->cursorX.fixed;
      pos.y = 0;
      pos.z = work->cursorY.fixed;
      Vector_AddPolarOffset(work->cursorSpeed, angle, &pos);
      if (pos.x < PIXELS(WORLD_MAP_CURSOR_MIN_X))
        pos.x = PIXELS(WORLD_MAP_CURSOR_MIN_X);
      if (pos.x > PIXELS(WORLD_MAP_CURSOR_MAX_X))
        pos.x = PIXELS(WORLD_MAP_CURSOR_MAX_X);
      if (pos.z < 0)
        pos.z = 0;
      if (pos.z > PIXELS(WORLD_MAP_CURSOR_MAX_Y))
        pos.z = PIXELS(WORLD_MAP_CURSOR_MAX_Y);
      work->cursorX.fixed = pos.x;
      work->cursorY.fixed = pos.z;
      if (work->cursorSpeed < WORLD_MAP_CURSOR_MAX_SPEED)
        work->cursorSpeed += WORLD_MAP_CURSOR_ACCELERATION;
    } else {
      work->cursorSpeed = WORLD_MAP_CURSOR_MIN_SPEED;
    }
    cursorX = work->cursorX.part.pixel;
    cursorY = work->cursorY.part.pixel;
  }

  // Actor 0 consumes no place row. Ordinary rows begin at actor 11; a
  // selected slot in 1..10 also consumes one. Zero rows reserve actors with
  // no marker and -1 ends the table. A row is (tile << 16 | flag, scene id).
  for (markerIndex = 0; markerIndex < WORLD_MAP_MARKER_CAPACITY;
       markerIndex++) {
    if (markerIndex != leader && markerIndex < WORLD_MAP_FIRST_PLACE_ACTOR)
      continue;
    if (markerIndex == 0) {
      if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
        continue;
      tile = 0;
      flag = leader;
      mode = 0;
    } else {
      flag = *place++;
      message = *place++;
      if (flag == 0)
        continue;
#if WORLD_MAP_CURSOR_SNAPS
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
    if (!GameFlag_TestFar(flag))
      continue;
    placeActor = ObjectTable_Get(markerIndex);
    if (placeActor == NULL)
      continue;

    // Field x starts at 4096; the picture covers 4096 square world pixels.
    x = ((placeActor->x.fixed - WORLD_MAP_X_ORIGIN) >> 16) *
        WORLD_MAP_SCREEN_WIDTH / WORLD_MAP_WORLD_SIZE;
    y = placeActor->z.part.pixel * WORLD_MAP_SCREEN_HEIGHT /
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
    // Blink the current place for eight frames out of sixteen.
    if (markerIndex != 0 || (gFrameCount & 15) <= 7)
      Runtime_PushSlotEntry(&WORLD_MAP_MARKER_VIEW(marker++), y);
  }
#if WORLD_MAP_CURSOR_SNAPS
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
        pos.x = work->cursorX.fixed;
        pos.y = 0;
        pos.z = work->cursorY.fixed;
        Vector_AddPolarOffset(work->cursorSpeed, (u16)angle, &pos);
        work->cursorX.fixed = pos.x;
        work->cursorY.fixed = pos.z;
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
    WORLD_MAP_MARKER_VIEW(marker).objectMode = 0;
    WORLD_MAP_MARKER_VIEW(marker).tile = tileBase + 3;
    WORLD_MAP_MARKER_VIEW(marker).x = nearestX - 2;
    WORLD_MAP_MARKER_VIEW(marker).y = nearestY - 2;
    Runtime_PushSlotEntry(&WORLD_MAP_MARKER_VIEW(marker), nearestY);
  }

  // The cursor.
  marker = &work->markers[WORLD_MAP_CURSOR_PART];
  WORLD_MAP_MARKER_VIEW(marker).affineIndex = 0;
  WORLD_MAP_MARKER_VIEW(marker).x = cursorX - 17;
  WORLD_MAP_MARKER_VIEW(marker).y = cursorY + 1;
  Runtime_PushSlotEntry(&WORLD_MAP_MARKER_VIEW(marker), 246);

  // Redraw only when the selected marker changes, including to no marker.
  if (work->namedMarker != nearestMarker) {
    work->namedMarker = nearestMarker;
    RenderOutput_PrepareForRedrawFar(work->nameWindow);
    if (nearestMarker != -1) {
      if (nearestMarker == 0)
        nearestName = MsgPresentLocation;
      else
        nearestName =
            Scene_GetAreaNameIndex(nearestName, 1) + MsgDebugEntryName;
      UiText_MeasureResourceEntriesFar(nearestName, &width, &height);
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
      UiText_DrawResourceFar(nearestName, work->nameWindow, cursorX, cursorY);
    }
  }

  // Pulse the markers' transparency: they weigh blend sixteenths against
  // the picture's 16 less blend (REG_BLDALPHA's EVA and EVB).
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_16);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend,
                      IO_WRITE_16);
}

/// Borrows the map-cell buffer for the still map until A or B closes it.
/// Field_RunScene reaches this for event -888 in world-map mode. The marker
/// task stops before map graphics are restored, so neither uses the other's
/// scratch storage. The picture argument is unused; the resource is fixed.
void Map_ShowWorldMap(s32 picture) {
  struct MapState *map = WORK_SLOT(8);
  struct FieldWork *field =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
  s32 resource = (s32)&ResourceId_WorldMapPicture;
  struct MapAnimation *layer;
  u8 savedFlags[sizeof(map->anim) / sizeof(map->anim[0])];
  s32 i;
  u8 *graphics;
  s32 savedStatus;
  s16 blend;

  if (field->mode != MODE_WORLD_MAP)
    return;
  field->camera->movementState = 1;
  savedStatus = field->transitionFrames;
  field->transitionFrames = 6;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  layer = map->anim;
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
  Resource_DecodeType01(graphics + 448, gMapCellBuffer);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          (u32)(DMA_ENABLE | DMA_32BIT) << 16 |
              (WORLD_MAP_SCREEN_WIDTH * WORLD_MAP_SCREEN_HEIGHT / 4),
          REG_DMA3);
  Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
  Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                 DISPCNT_OBJ_1D_MAP,
             &REG_DISPCNT);
  BattleFx_SetupResourcesAndWindow();
  Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers,
                                SCHEDULER_KEY_MAIN_LOOP);
  if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
    UiText_ShowPositionedMessageAndWaitFar(MsgNotOnMap, 1);
  do {
    WaitFrames(1);
  } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
  Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
  BattleFx_CleanupResourcesAndWindow();
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  Map_LoadAreaGraphicsFar();
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, IO_WRITE_16);
  layer = map->anim;
  for (i = 0; i < (s32)(sizeof(savedFlags) / sizeof(savedFlags[0])); i++) {
    layer->paused = savedFlags[i];
    layer++;
  }
  menu->suspended = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  field->transitionFrames = savedStatus;
  field->camera->movementState = 0;
}

/// On the world map, shows only the scene actors within 320 pixels sideways,
/// 600 above and 400 below the camera.
void Field_UpdateObjectVisibilityBounds(void) {
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  u32 id;

  // The selected actor lookup has no used result; the bounds follow the camera.
  Object_GetById(gGameState.selectedActor);
  object = ((struct FieldWork *)gEventWork)->camera;
  x = object->x;
  left = x - (320 << 16);
  right = x + (320 << 16);
  y = object->z;
  top = y - (600 << 16);
  bottom = y + (400 << 16);

  id = ACTOR_FIRST_PLACED;
  do {
    object = ObjectTable_Get(id);

    if (object != 0) {
      s32 ox = object->x;
      s32 oy = object->z;

      if (ox < left || ox > right || oy < top || oy > bottom) {
        object->animationKind = 0;
      } else {
        object->animationKind = 1;
      }
    }
    id++;
  } while (id < WORLD_MAP_MARKER_CAPACITY);
}

/// On the world map, zooms the view out (0x9d89) while L is held, then back.
/// Each zoom takes 6 frames; 0x9d89 is about 0.62, which sets the view 1.6
/// times as far (BattleFx_ScheduleRatioTransition). Field_RunScene runs it
/// for event -889; the overlay tasks are suspended meanwhile.
void BattleFx_RunVisibilityTransition(void) {
  struct FieldWork *field =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));

  if (field->mode == MODE_WORLD_MAP) {
    Scheduler_SuspendOverlayTasks();
    Field_UpdateObjectVisibilityBounds();
    Leader_StopMoving();
    BattleFx_ScheduleRatioTransition(0x9d89, 6);
    if (gKeysHeld & KEY_L) {
      do {
        WaitFrames(1);
      } while (gKeysHeld & KEY_L);
    }
    BattleFx_ScheduleRatioTransition(0x10000, 6);
    Scheduler_ResumeOverlayTasks();
  }
}

#undef WORLD_MAP_MARKER_TILE_BYTES
#undef WORLD_MAP_SCREEN_WIDTH
#undef WORLD_MAP_SCREEN_HEIGHT
#undef WORLD_MAP_WORLD_SIZE
#undef WORLD_MAP_X_ORIGIN
#undef WORLD_MAP_FIRST_PLACE_ACTOR
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
#undef WORLD_MAP_CURSOR_SNAPS
#undef WORLD_MAP_NAME_RANGE
#undef WORLD_MAP_NAME_LEFT
