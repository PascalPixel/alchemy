/// @file
/// The world map screen: its markers and cursor (The Broken Seal's
/// world_picture.c), and where the leader stands on it.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"

#include "field/world_map.h"
#include "version.h"

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

  markerTiles = Heap_AllocEwram(32);
  oam = (u32 *)work->markers;
  Actor_GetById(Party_GetLeaderActor());
  screenX = 0;
  screenY = 0;
  work->vramBlock = VramCache_FindFreeEntry();
  work->searchX = 0;
  work->searchZ = 0;
  Dma_Fill32(0, markerTiles, 128);
  // Four 8 by 8 tiles, a pixel a nibble: three marker dots and the cursor's.
  markerTiles[0] = 0x000000ff;
  markerTiles[1] = 0x000001ff;
  markerTiles[2] = 0x00000110;
  markerTiles[8] = 0x00000044;
  markerTiles[9] = 0x00000144;
  markerTiles[10] = 0x00000110;
  markerTiles[16] = 0x00000077;
  markerTiles[17] = 0x00000177;
  markerTiles[18] = 0x00000110;
  markerTiles[24] = 0x00000ff0;
  markerTiles[25] = 0x0000ffff;
  markerTiles[26] = 0x0001ffff;
  markerTiles[27] = 0x00011ff0;
  markerTiles[28] = 0x00001100;
  tileAttributes =
      VramCache_Load(work->vramBlock, 128, markerTiles) | OBJ_PRIORITY(1);
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
  screenX = work->x - toFIXED(120);
  screenY = work->z - toFIXED(80);
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
  work->window = Window_CreateFar(0, 0, 0, 0, 2);
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
    x >>= 16;
    x = ((x << 14) + 269 * 16384) / 853;
    z >>= 16;
    actor->x.fixed = toFIXED(x);
    actor->z.fixed = toFIXED(((z << 14) + toFIXED(28)) / 640);
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
