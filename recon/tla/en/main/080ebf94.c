// Draft of Func_080ebf94, differ 2 of 192 (2026-10-26; 98 before): sets up
// the world map screen, as ☀️'s FieldEffect_SetupResourcesAndWindow
// (games/tbs/src/overworld/field_services/world_map.c): the marker tiles and
// their 128 OAM entries, the cursor at the leader's place (or a fixed spot
// when flag 284 is set), the screen scroll clamped to the map, and the
// place-name window. The work, the cursor pointer and the cursor's starting
// place are reached at their constant addresses (ram_buffer.h): the work's x
// and z are then read again after the clamps, and the stores after the cursor
// pointer go through its address plus 4, as natively. The tile rows in ☀️'s
// order, a zero bound for the clamps and the cursor's attributes built as ☀️
// builds them give the rest. Left, scheduling only: after the cursor's first
// OAM word the native shifts 0x40000000 into place before copying the moved
// OAM pointer to its stack slot (lsls r3, r3, #23; adds r1, r2, #0), where
// alchemy-gcc copies first; the stack store may alias the OAM stores, which
// makes the copy's chain the longer. Barriers before, between and after the
// two words and the word as a struct member changed nothing; permuter 4
// minutes: nothing.
#include "types.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "vram_tab.h"
#include "event_work.h"
#include "flag_call.h"
#include "ram_buffer.h"

struct WorldMapWork {
  u16 vramBlock;
  u16 windowSlot;
  s32 x;
  s32 z;
  u8 padding0c[6];
  s16 shown;
  u8 padding14[4];
  s32 speed;
  void *window;
  u32 markers[129 * 3];
  u8 padding638[24];
  s32 unk644;
  s32 unk648;
};

void *Object_GetById(s32 id);
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Func_080ec1d0(s32 *screenX, s32 *screenY);

void Func_080ebf94(void) {
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

  markerTiles = Runtime_BumpAllocateAlternatePool(32);
  oam = work->markers;
  Object_GetById(EventRuntime_GetControlledOwner());
  screenX = 0;
  screenY = 0;
  work->vramBlock = Resource_FindFreeEntry();
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
  MemFree(markerTiles);
  if (GameFlag_Test(284)) {
    work->x = 264 << 16;
    work->z = 16 << 16;
  } else {
    Func_080ec1d0(&screenX, &screenY);
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
  *oam++ = 0;
  *oam++ = 0x40000000;
  cursorAttributes = 0x400;
  cursorAttributes |= tile >> 5;
  *oam = cursorAttributes;
  *Ram_WorldMapCursor = &work->x;
  Ram_WorldMapScroll[0] = screenX;
  Ram_WorldMapScroll[1] = screenY;
}
