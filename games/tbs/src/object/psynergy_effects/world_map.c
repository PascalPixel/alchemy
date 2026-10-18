// Part of psynergy_effects.c, compiled only through it.
// The world map screen: place markers, the cursor and the place name.

#include "../psynergy_effects_internal.h"

#define FLAG_NOT_ON_MAP 284 // the current place is not on the world map
#define WORLD_MAP_MARKER_TILE_BYTES 0x80

// BUG: the original reserves 32 bytes but writes and uploads 128. The bump
// allocator takes bytes, not words; reserve the whole tile buffer with BUGFIX.
#ifdef BUGFIX
#define WORLD_MAP_MARKER_ALLOCATION_SIZE WORLD_MAP_MARKER_TILE_BYTES
#else
#define WORLD_MAP_MARKER_ALLOCATION_SIZE 32
#endif

// The world map screen's work, kept in gMapCellBuffer.
struct WorldMapWork {
  u16 vramBlock;           // VRAM block of the markers' tiles
  u16 windowSlot;          // a second VRAM block, returned at the end
  union FieldCoordinate x; // the cursor, in pixels
  union FieldCoordinate z;
  u8 padding0c[6];
  s16 shown; // the marker whose name is shown (-1: none)
  u8 padding14[4];
  s32 speed; // the cursor's speed
  struct UiWindow *window;
  struct AnimationSpritePart markers[67];
};

// Sets up the world map screen: the marker tiles and their OAM entries, the
// cursor where the leader is on the map (or mid-screen when the place is not
// on it), and the place-name window.
void BattleFx_SetupResourcesAndWindow(void) {
  u32 *oam;
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
  oam = (u32 *)work->markers;
  object = Object_GetById(gGameState.selectedActor);
  work->vramBlock = Resource_FindFreeEntry();
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
  tileAttributes = VramCache_Load(work->vramBlock, WORLD_MAP_MARKER_TILE_BYTES,
                                  markerTiles) |
                   0x400;
  for (i = 0; i < 66; i++) {
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = tileAttributes;
  }
  MemFree(markerTiles);
  if (GameFlag_TestFar(284)) {
    work->x.fixed = 240 << 15;
    work->z.fixed = 160 << 15;
  } else {
    work->x.fixed = (((object->x.fixed - 0x10000000) >> 16) * 240 / 4096) << 16;
    work->z.fixed = ((object->z.part.pixel * 160) / 4096) << 16;
  }
  work->windowSlot = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramBlockCache[work->windowSlot].offset;
  work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
  work->shown = -1;
  work->speed = 0x10000;
  *oam++ = 0;
  *oam++ = 0x40000000;
  cursorAttributes = 0x400;
  cursorAttributes |= tile >> 5;
  *oam = cursorAttributes;
}

#if TBS_PATCH_2
// The cursor snaps to the nearest place when the d-pad is released.
#define WORLD_MAP_CURSOR_SNAPS 1
#define WORLD_MAP_NAME_RANGE 64 // squared distance that shows a place's name
#define WORLD_MAP_NAME_LEFT 3   // how far left of its marker the name goes
#else
#define WORLD_MAP_CURSOR_SNAPS 0
#define WORLD_MAP_NAME_RANGE 100
#define WORLD_MAP_NAME_LEFT 1
#endif

// Closes the world map screen: returns its VRAM blocks and removes the
// place-name window.
void BattleFx_CleanupResourcesAndWindow(void) {
  struct WorldMapWork *work = (struct WorldMapWork *)gMapCellBuffer;

  Resource_ResetEntry(work->vramBlock);
  Resource_ResetEntry(work->windowSlot);
  UiWork_FinalizeFar(work->window, 2);
}

// Scheduler callback of the world map screen: shows a marker at each place
// whose flag is set, moves the cursor with the d-pad (to the current place
// while L or R is held) and shows the name of the marker nearest the cursor.
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
  s32 pos[3];
  struct WorldMapWork *work;
  struct FieldActor *placeActor;
  s32 cursorY;
  s32 tile;
  s32 mode;
  s32 flag;
  s32 markerIndex;
  struct AnimationSpritePart *marker;
  s32 x;
  s32 y;
  s32 angle;

#if WORLD_MAP_CURSOR_SNAPS
  leader = gGameState.selectedActor;
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  marker = work->markers;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->vramBlock].offset >> 5;
#else
  work = (struct WorldMapWork *)Ram_MapCellBuffer;
  leader = gGameState.selectedActor;
  place = gWorldMap_PlaceMarkers;
  tileBase = gVramBlockCache[work->vramBlock].offset >> 5;
  marker = work->markers;
#endif
  nearestMarker = -1;
  nearestDistanceSquared = WORLD_MAP_NAME_RANGE;
  blend = gWorldMap_MarkerBlendCycle[(gFrameCount >> 1) & 31];

  //------------------------------------------------------------------
  // The cursor.
  //------------------------------------------------------------------
  if (!GameFlag_TestFar(FLAG_NOT_ON_MAP) && (gKeysHeld & KEYS_SHOULDERS)) {
    // L or R: the current place.
    placeActor = ObjectTable_Get(gGameState.selectedActor);
    if (placeActor != NULL) {
      cursorX = ((placeActor->x.fixed - 0x10000000) >> 16) * 240 / 4096;
      cursorY = placeActor->z.part.pixel * 160 / 4096;
    }
  } else {
    // The d-pad: faster the longer it is held.
    angle = gWorldMap_CursorDirectionAngles[(gKeysHeld >> 4) & 15];
    if (angle != 0xffff) {
      SET_VECTOR(pos, work->x.fixed, 0, work->z.fixed);
      Vector_AddPolarOffset(work->speed, angle, pos);
      if (pos[0] < 0x100000) // x is 16 to 239 pixels
        pos[0] = 0x100000;
      if (pos[0] > 0xef0000)
        pos[0] = 0xef0000;
      if (pos[2] < 0) // y is 0 to 141 pixels
        pos[2] = 0;
      if (pos[2] > 0x8d0000)
        pos[2] = 0x8d0000;
      work->x.fixed = pos[0];
      work->z.fixed = pos[2];
      if (work->speed < 0x60000)
        work->speed += 0x2000;
    } else {
      work->speed = 0x10000;
    }
    cursorX = work->x.part.pixel;
    cursorY = work->z.part.pixel;
  }

  //------------------------------------------------------------------
  // Markers: 0 is the current place, 11 to 65 the places of
  // gWorldMap_PlaceMarkers, (flag, name) pairs whose flag carries the marker's
  // tile in its upper 16 bits.
  //------------------------------------------------------------------
  for (markerIndex = 0; markerIndex <= 65; markerIndex++) {
    if (markerIndex != leader && markerIndex <= 10)
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

    // The world's 4096 pixels square shrunk to 240 by 160.
    x = ((placeActor->x.fixed - 0x10000000) >> 16) * 240 / 4096;
    y = placeActor->z.part.pixel * 160 / 4096;
    marker->objectMode = mode;
    marker->tile = tileBase + tile;
    marker->x = x - 1;
    marker->y = y - 1;
    width = x - cursorX;
    height = y - cursorY;
    if (width * width + height * height < nearestDistanceSquared) {
      nearestName = message;
      nearestMarker = markerIndex;
      nearestDistanceSquared = width * width + height * height;
      nearestX = x;
      nearestY = y;
    }
    if (markerIndex != 0 || (gFrameCount & 15) <= 7) // the current place blinks
      Runtime_PushSlotEntry(marker++, y);
  }
#if WORLD_MAP_CURSOR_SNAPS
  // With the d-pad released, the cursor moves onto the nearest marker.
  if (gKeysHeld == 0) {
    if (nearestMarker != -1) {
      if (nearestDistanceSquared <= 1) {
        work->x.fixed = nearestX << 16;
        work->z.fixed = nearestY << 16;
        work->speed = 0x10000;
      } else {
        if (work->speed < 0x20000)
          work->speed = 0x20000;
        angle = ArcTan2(nearestY - cursorY, nearestX - cursorX);
        SET_VECTOR(pos, work->x.fixed, 0, work->z.fixed);
        Vector_AddPolarOffset(work->speed, (u16)angle, pos);
        work->x.fixed = pos[0];
        work->z.fixed = pos[2];
        if (work->speed < 0x60000)
          work->speed += 0x2000;
      }
      cursorX = work->x.part.pixel;
      cursorY = work->z.part.pixel;
    } else {
      work->speed = 0x10000;
    }
  }
#endif

  // The blinking frame round the nearest marker.
  if (nearestMarker != -1 && (gFrameCount & 15) <= 7) {
    marker->objectMode = 0;
    marker->tile = tileBase + 3;
    marker->x = nearestX - 2;
    marker->y = nearestY - 2;
    Runtime_PushSlotEntry(marker, nearestY);
  }

  // The cursor.
  marker = &work->markers[66];
  marker->affineIndex = 0;
  marker->x = cursorX - 17;
  marker->y = cursorY + 1;
  Runtime_PushSlotEntry(marker, 246);

  //------------------------------------------------------------------
  // The place-name window, redrawn only when the marker changes.
  //------------------------------------------------------------------
  if (work->shown != nearestMarker) {
    work->shown = nearestMarker;
    RenderOutput_PrepareForRedrawFar(work->window);
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
      if (cursorX + width > 240) {
        // Past the right edge, it goes up and left of the marker.
        cursorX = 232 - width;
        cursorY = nearestY - 20;
      }
      if (cursorY < 0)
        cursorY = 0;
      UiText_DrawResourceFar(nearestName, work->window, cursorX, cursorY);
    }
  }

  // Pulse the markers' transparency.
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDCNT, BLDCNT_TGT2_ALL, 0x20000);
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, ((16 - blend) << 8) | blend, 0x20000);
}

// Shows the world map screen from the world map (event mode 3): closes the
// screen, pauses the map's animations, draws the map picture on BG1 and runs
// Map_UpdateWorldMapMarkers until A or B, then puts everything back. Says
// "This place doesn't seem to be on the map..." when it is not.
void Map_ShowWorldMap(s32 picture) {
  struct MapState *map = WORK_SLOT(8);
  struct FieldWork *field =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));
  struct ObjectSystemWork *menu = WORK_SLOT(HEAP_SLOT_MENU_CONTROL);
  s32 resource = (s32)&ResourceId_WorldMapPicture;
  struct MapAnimation *layer;
  u8 savedFlags[16];
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
  for (i = 0; i < 16; i++) {
    savedFlags[i] = layer->paused;
    layer->paused = 1;
    layer++;
  }
  menu->suspended = 1;
  WaitFrames(1);
  blend = REG_BLDALPHA;
  graphics = Resource_GetTableEntry(resource);
  Dma_Set(graphics, (void *)BG_PLTT, (DMA_ENABLE | DMA_32BIT) << 16 | (448 / 4),
          REG_DMA3);
  BG_PLTT[0] = 0;
  Resource_DecodeType01(graphics + 448, gMapCellBuffer);
  Dma_Set(gMapCellBuffer, BG_CHAR_BLOCK(1) + 0x2a00,
          (DMA_ENABLE | DMA_32BIT) << 16 | (240 * 160 / 4), REG_DMA3);
  Io_Write16(BGCNT_256_COLORS | BGCNT_SCREEN_BASE(6) | 2, &REG_BG1CNT);
  Io_Write16(DISPCNT_OBJ_ON | DISPCNT_BG1_ON | DISPCNT_BG0_ON |
                 DISPCNT_OBJ_1D_MAP,
             &REG_DISPCNT);
  BattleFx_SetupResourcesAndWindow();
  Scheduler_AddOrUpdateCallback(Map_UpdateWorldMapMarkers, 0xc80);
  if (GameFlag_TestFar(FLAG_NOT_ON_MAP))
    UiText_ShowPositionedMessageAndWaitFar(MsgNotOnMap, 1);
  do {
    WaitFrames(1);
  } while ((gKeysRepeat & (KEY_A | KEY_B)) == 0);
  Scheduler_RemoveCallback(Map_UpdateWorldMapMarkers);
  BattleFx_CleanupResourcesAndWindow();
  Io_Write16(DISPCNT_OBJ_1D_MAP, &REG_DISPCNT);
  Map_LoadAreaGraphicsFar();
  IO_WRITE_QUEUE_PUSH((u32)&REG_BLDALPHA, (u16)blend, 0x20000);
  layer = map->anim;
  for (i = 0; i < 16; i++) {
    layer->paused = savedFlags[i];
    layer++;
  }
  menu->suspended = 0;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  field->transitionFrames = savedStatus;
  field->camera->movementState = 0;
}

// On the world map, shows only the scene actors within 320 pixels sideways,
// 600 above and 400 below the camera.
void BattleFx_UpdateObjectVisibilityBounds(void) {
  struct ObjectRuntime *object;
  s32 x;
  s32 y;
  s32 left;
  s32 right;
  s32 top;
  s32 bottom;
  u32 id;

  // the selected actor is looked up and the result discarded, as the ROM does
  Object_GetById(gGameState.selectedActor);
  object = ((struct FieldWork *)gEventWork)->camera;
  x = object->x;
  left = x - (320 << 16);
  right = x + (320 << 16);
  y = object->z;
  top = y - (600 << 16);
  bottom = y + (400 << 16);

  id = 8;
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
  } while (id <= 65);
}

// On the world map, zooms the view out (0x9d89) while L is held, then back.
void BattleFx_RunVisibilityTransition(void) {
  struct FieldWork *field =
      Runtime_AllocateBlock(HEAP_SLOT_EVENT, sizeof(struct FieldWork));

  if (field->mode == MODE_WORLD_MAP) {
    Scheduler_SuspendOverlayTasks();
    BattleFx_UpdateObjectVisibilityBounds();
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

#undef FLAG_NOT_ON_MAP
#undef WORLD_MAP_MARKER_TILE_BYTES
#undef WORLD_MAP_MARKER_ALLOCATION_SIZE
#undef WORLD_MAP_CURSOR_SNAPS
#undef WORLD_MAP_NAME_RANGE
#undef WORLD_MAP_NAME_LEFT
