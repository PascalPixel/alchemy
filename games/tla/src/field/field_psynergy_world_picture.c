/// @file
/// The world map screen: its markers and cursor (The Broken Seal's
/// world_picture.c), and where the leader stands on it.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"

#include "field/world_map.h"
#include "version.h"

/// The generated marker pictures: four 16-colour 8 by 8 tiles, 32 bytes each.
#define WORLD_MAP_MARKER_TILE_BYTES (4 * TILE_SIZE_4BPP)
/// The word of a marker tile's pixel row, eight 4-bit pixels a word.
#define MARKER_ROW_WORD(tile, row) ((tile) * (TILE_SIZE_4BPP / 4) + (row))
/// The first five of a marker tile row's eight 4-bit pixels, the leftmost in
/// the low bits; the rest are 0.
#define WORLD_PICTURE_MARKER_ROW(p0, p1, p2, p3, p4)                           \
  ((p0) | (p1) << 4 | (p2) << 8 | (p3) << 12 | (p4) << 16)

/// The bytes reserved for the World Map picture's marker tiles.
// BUG: the original reserves 32 bytes but writes and uploads 128, as The
// Broken Seal's does. The bump allocator takes bytes, not words; reserve the
// whole tile buffer with BUGFIX.
#ifdef BUGFIX
#define WORLD_MAP_MARKER_ALLOCATION_SIZE WORLD_MAP_MARKER_TILE_BYTES
#else
#define WORLD_MAP_MARKER_ALLOCATION_SIZE 32
#endif

/// Sets up the World Map screen: the marker tiles and their OAM entries, the
/// cursor where the leader is on the map (or mid-screen when the area is
/// not on it), and the name window.
void WorldPicture_Initialize(void) {
  u32 *markerTiles;
  struct WorldMapWork *work = Ram_WorldMapWork;
  u32 *oam;
  FIXED screenX;
  FIXED screenY;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;
  FIXED right;
  FIXED bottom;
  s32 left;

  markerTiles = Heap_AllocEwram(WORLD_MAP_MARKER_ALLOCATION_SIZE);
  oam = (u32 *)work->markers;
  Actor_GetById(Party_GetLeaderActor());
  screenX = 0;
  screenY = 0;
  work->vramBlock = VramCache_FindFreeEntry();
  work->searchX = 0;
  work->searchZ = 0;
  Dma_Fill32(0, markerTiles, WORLD_MAP_MARKER_TILE_BYTES);
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
  // Tile 0, in colour 15, marks where the party is; tiles 1 and 2, in colours
  // 4 and 7, a place by the marker its row asks for, and tile 2 the ship;
  // tile 3, in colour 15, is a larger dot.
  markerTiles[MARKER_ROW_WORD(0, 0)] =
      WORLD_PICTURE_MARKER_ROW(15, 15, 0, 0, 0);
  markerTiles[MARKER_ROW_WORD(0, 1)] =
      WORLD_PICTURE_MARKER_ROW(15, 15, 1, 0, 0);
  markerTiles[MARKER_ROW_WORD(0, 2)] = WORLD_PICTURE_MARKER_ROW(0, 1, 1, 0, 0);
  markerTiles[MARKER_ROW_WORD(1, 0)] = WORLD_PICTURE_MARKER_ROW(4, 4, 0, 0, 0);
  markerTiles[MARKER_ROW_WORD(1, 1)] = WORLD_PICTURE_MARKER_ROW(4, 4, 1, 0, 0);
  markerTiles[MARKER_ROW_WORD(1, 2)] = WORLD_PICTURE_MARKER_ROW(0, 1, 1, 0, 0);
  markerTiles[MARKER_ROW_WORD(2, 0)] = WORLD_PICTURE_MARKER_ROW(7, 7, 0, 0, 0);
  markerTiles[MARKER_ROW_WORD(2, 1)] = WORLD_PICTURE_MARKER_ROW(7, 7, 1, 0, 0);
  markerTiles[MARKER_ROW_WORD(2, 2)] = WORLD_PICTURE_MARKER_ROW(0, 1, 1, 0, 0);
  markerTiles[MARKER_ROW_WORD(3, 0)] =
      WORLD_PICTURE_MARKER_ROW(0, 15, 15, 0, 0);
  markerTiles[MARKER_ROW_WORD(3, 1)] =
      WORLD_PICTURE_MARKER_ROW(15, 15, 15, 15, 0);
  markerTiles[MARKER_ROW_WORD(3, 2)] =
      WORLD_PICTURE_MARKER_ROW(15, 15, 15, 15, 1);
  markerTiles[MARKER_ROW_WORD(3, 3)] =
      WORLD_PICTURE_MARKER_ROW(0, 15, 15, 1, 1);
  markerTiles[MARKER_ROW_WORD(3, 4)] = WORLD_PICTURE_MARKER_ROW(0, 0, 1, 1, 0);
  tileAttributes = VramCache_Load(work->vramBlock, WORLD_MAP_MARKER_TILE_BYTES,
                                  markerTiles) |
                   OBJ_PRIORITY(1);
  for (i = 0; i < WORLD_PICTURE_PLACE_COUNT; i++) {
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = tileAttributes;
  }
  Heap_Release(markerTiles);
  if (GameFlag_Get(FLAG_WORLD_MAP_LEFT)) {
    work->x = toFIXED(264);
    work->z = toFIXED(16);
  } else {
    WorldPicture_GetLeaderPoint(&screenX, &screenY);
    work->x = toFIXED(screenX);
    work->z = toFIXED(screenY);
  }
  screenX = work->x - toFIXED(DISPLAY_WIDTH / 2);
  screenY = work->z - toFIXED(DISPLAY_HEIGHT / 2);
  left = 0;
  right = toFIXED(272);
  bottom = toFIXED(224);
  if (screenX < left)
    screenX = left;
  if (screenX > right)
    screenX = right;
  if (screenY < left)
    screenY = left;
  if (screenY > bottom)
    screenY = bottom;
  Ram_WorldMapCursorStart[0] = work->x;
  Ram_WorldMapCursorStart[1] = work->z;
  work->windowSlot = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gVramCacheEntries[work->windowSlot].offset;
  work->window = Window_CreateFar(0, 0, 0, 0, WINDOW_FLAG_INSTANT);
  work->shown = -1;
  work->speed = toFIXED(1);
  cursorAttributes = OBJ_SIZE_16X16;
  *oam++ = 0;
  *oam++ = cursorAttributes;
  cursorAttributes = OBJ_PRIORITY(1);
  cursorAttributes |= tile >> 5;
  *oam = cursorAttributes;
  *Ram_WorldMapCursor = &work->x;
  Ram_WorldMapScroll[0] = screenX;
  Ram_WorldMapScroll[1] = screenY;
}

/// Closes the World Map screen: returns its VRAM blocks and removes the
/// name window.
void WorldPicture_Release(void) {
  VramCache_FreeEntry(gWorldMapWork.vramBlock);
  VramCache_FreeEntry(gWorldMapWork.windowSlot);
  Window_CloseFar((u32)gWorldMapWork.window, 2);
}

/// Unused: nothing calls it. Puts the leader at the world map position of
/// picture point (x, z), the inverse of WorldPicture_GetLeaderPoint's scale,
/// and the camera on it.
void WorldPicture_PlaceLeaderAt(s32 x, s32 z) {
  s32 owner = Party_GetLeaderActor();
  s32 *camera = *(s32 **)Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_Lookup(owner);

  if (actor != NULL) {
    x = FIXEDtoINT(x);
    x = (x * 16384 + 269 * 16384) / 853;
    z = FIXEDtoINT(z);
    actor->x.fixed = toFIXED(x);
    actor->z.fixed = toFIXED((z * 16384 + 112 * 16384) / 640);
    camera[0] = actor->x.fixed;
    camera[2] = actor->z.fixed;
  }
}

/// The leader's point on the world map picture: its world map position scaled
/// by 853 and 640 in 16384, a far corner of the map folded in
/// (FLAG_LEADER_OFF_MAP_PICTURE set while the leader is there); off the world
/// map, the picture's fixed point (264, 16).
void WorldPicture_GetLeaderPoint(s32 *screenX, s32 *screenY) {
  s32 leader = Party_GetLeaderActor();
  struct FieldWork *state = Ram_HeapSlots->fieldWork;
  struct Actor *actor;
  s32 x;
  s32 z;

  GameFlag_Clear(FLAG_LEADER_OFF_MAP_PICTURE);
  if (state->mode == ACTOR_SYSTEM_WORLD_MAP) {
    actor = Actor_Lookup(leader);
    if (actor == 0)
      return;
    x = actor->x.part[0].integer;
    z = actor->z.part[0].integer;
    if (z < 1832) {
      if (x < 12000) {
        GameFlag_Set(FLAG_LEADER_OFF_MAP_PICTURE);
        // From patch 1 this case is offset from the leader's position; the
        // Japanese release puts it at one fixed point.
#if TLA_PATCH_1
        x += 65;
        z += 1966;
#else
        x = 8872;
        z = 3320;
#endif
      } else {
        x -= 627;
        z += 6084;
      }
    }
    *screenX = WORLD_PICTURE_X(x);
    *screenY = WORLD_PICTURE_Y(z);
  } else {
    *screenX = 264;
    *screenY = 16;
  }
}

/// Returns a World Map place's marker: the high nibble of its value when
/// gWorldPicture_FirstListOnly is set, else the low nibble (0 and 15 show
/// none).
u32 WorldMapPlace_GetMarker(const struct FlagEntry *place) {
  if (gWorldPicture_FirstListOnly != 0)
    return place->value >> 4;
  return place->value & 0xf;
}
