/// @file
/// The world map screen: its markers and cursor (The Broken Seal's
/// world_picture.c), and where the leader stands on it.

#include "types.h"
#include "dma.h"
#include "heap_release.h"
#include "resource.h"
#include "vram_tab.h"
#include "event_work.h"
#include "flag_call.h"
#include "ram_buffer.h"
#include "field/world_map.h"
#include "gamepak/far_call/window.h"
#include "module/field/event_commands.h"

struct Actor *Actor_GetById(u32 objectId);

void WorldPicture_Initialize(void) {
  u32 *markerTiles;
  struct WorldMapWork *work = Ram_WorldMapWork;
  u32 *oam;
  s32 screenX;
  s32 screenY;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  u32 cursorAttributes;
  s32 right;
  s32 bottom;
  s32 left;

  markerTiles = Heap_AllocEwram(32);
  oam = work->markers;
  Actor_GetById(Party_GetLeaderActor());
  screenX = 0;
  screenY = 0;
  work->vramBlock = VramCache_FindFreeEntry();
  work->unk644 = 0;
  work->unk648 = 0;
  Dma_Fill32(0, markerTiles, 0x80);
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
  tileAttributes = VramCache_Load(work->vramBlock, 0x80, markerTiles) | 0x400;
  for (i = 0; i < 128; i++) {
    *oam++ = 0;
    *oam++ = 0;
    *oam++ = tileAttributes;
  }
  Heap_Release(markerTiles);
  if (GameFlag_Get(284)) {
    work->x = 264 << 16;
    work->z = 16 << 16;
  } else {
    WorldPicture_GetLeaderPoint(&screenX, &screenY);
    work->x = screenX << 16;
    work->z = screenY << 16;
  }
  screenX = work->x - (120 << 16);
  screenY = work->z - (80 << 16);
  left = 0;
  right = 272 << 16;
  bottom = 224 << 16;
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
  tile = gResourceTableEntries[work->windowSlot].offset;
  work->window = Window_CreateFar(0, 0, 0, 0, 2);
  work->shown = -1;
  work->speed = 0x10000;
  cursorAttributes = 0x40000000;
  *oam++ = 0;
  *oam++ = cursorAttributes;
  cursorAttributes = 0x400;
  cursorAttributes |= tile >> 5;
  *oam = cursorAttributes;
  *Ram_WorldMapCursor = &work->x;
  Ram_WorldMapScroll[0] = screenX;
  Ram_WorldMapScroll[1] = screenY;
}

#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"

void WorldPicture_Release(void) {
  VramCache_FreeEntry(gWorldMapWork.vramBlock);
  VramCache_FreeEntry(gWorldMapWork.windowSlot);
  Window_CloseFar((u32)gWorldMapWork.window, 2);
}

#include "actor.h"

/// Unused: nothing calls it. Puts the leader at the world map position of
/// picture point (x, z), the inverse of WorldPicture_GetLeaderPoint's scale,
/// and the camera on it.
void WorldPicture_PlaceLeaderAt(s32 x, s32 z) {
  s32 owner = Party_GetLeaderActor();
  s32 *camera = *(s32 **)Ram_HeapSlots->mapWork;
  struct Actor *object = ObjectTable_Get(owner);

  if (object != NULL) {
    x >>= 16;
    x = ((x << 14) + 0x434000) / 853;
    z >>= 16;
    object->x = x << 16;
    object->z = (((z << 14) + 0x1c0000) / 640) << 16;
    camera[0] = object->x;
    camera[2] = object->z;
  }
}

#include "version.h"

/// The leader's point on the world map picture: its world map position scaled
/// by 853 and 640 in 16384, a far corner of the map folded in (flag 286 set
/// while the leader is there); off the world map, the picture's fixed point
/// (264, 16).
void WorldPicture_GetLeaderPoint(s32 *screenX, s32 *screenY) {
  s32 leader = Party_GetLeaderActor();
  u8 *state = Ram_HeapSlots->eventWork;
  u8 *object;
  s32 x;
  s32 z;

  GameFlag_Clear(286);
  if (*(s8 *)(state + 394) == 3) {
    object = ObjectTable_Get(leader);
    if (object == 0)
      return;
    x = *(s16 *)(object + 10);
    z = *(s16 *)(object + 18);
    if (z < 1832) {
      if (x < 12000) {
        GameFlag_Set(286);
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
    *screenX = 853 * x / 16384 - 269;
    *screenY = z * 640 / 16384 - 112;
  } else {
    *screenX = 264;
    *screenY = 16;
  }
}

/// Returns entry[1]'s high nibble when gData_0202a642 is set, else its low
/// nibble.
u32 Func_080ec298(u8 *entry) {
  if (gData_0202a642 != 0)
    return entry[1] >> 4;
  return entry[1] & 15;
}
