// First draft of Func_080ebf94: sets up the world map screen, as ☀️'s
// BattleEffect_SetupResourcesAndWindow (games/tbs/src/object/psynergy_effects.c):
// the marker tiles and their 128 OAM entries, the cursor at the leader's place
// (or a fixed spot when flag 284 is set), the screen scroll clamped to the
// map, and the place-name window. Differ 108 of 192 (draft 188). Left: the
// constant 272 of three tile rows takes r2 and is scheduled earlier in the
// native; the clamps keep a zero in r6 and the bounds in r1 and r0 across
// them; after the clamps the native reloads the cursor from the work before
// storing it to Data_0202a64c (this draft reuses the registers), and the last
// OAM words and Data_0202a62c stores are ordered differently. Mostly order and
// register choice; the reload is a difference of shape not yet found.
// 2026-10-19: the clamp bounds as locals, as in Func_080ed0a0
// (games/tla/src/battle/effect/world_map_scroll.c), bring it to 100. Left
// there: the native zero for the clamps is the r6 the OAM stores reuse, and the
// native reloads the work's x and z for Data_0202a64c after the clamps, as if
// the stores to the address-taken screen values could alias them.
#include "types.h"
#include "dma.h"
#include "mem.h"
#include "resource.h"
#include "vram_tab.h"
#include "event_work.h"
#include "flag_call.h"

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

extern struct WorldMapWork gWorldMapWork;
extern s32 Data_0202a64c[2];
extern s32 Data_0202a62c[3];
extern u8 Data_0202a004[];

void *Object_GetById(s32 id);
s32 Resource_LoadFixedBlockBIntoFreeSlotFar(void);
void *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 mode);
void Func_080ec1d0(s32 *screenX, s32 *screenY);

void Func_080ebf94(void) {
  u32 *markerTiles;
  struct WorldMapWork *work = &gWorldMapWork;
  u32 *oam;
  s32 screenX;
  s32 screenY;
  s32 tileAttributes;
  u32 i;
  u32 tile;
  s32 right;
  s32 bottom;

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
  markerTiles[8] = 68;
  markerTiles[9] = 324;
  markerTiles[16] = 119;
  markerTiles[17] = 0x177;
  markerTiles[24] = 0xff0;
  markerTiles[25] = 0xffff;
  markerTiles[26] = 0x1ffff;
  markerTiles[27] = 0x11ff0;
  markerTiles[2] = 272;
  markerTiles[10] = 272;
  markerTiles[18] = 272;
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
  right = 272 << 16;
  bottom = 224 << 16;
  if (screenX < 0)
    screenX = 0;
  if (screenX > right)
    screenX = right;
  if (screenY < 0)
    screenY = 0;
  if (screenY > bottom)
    screenY = bottom;
  Data_0202a64c[0] = work->x;
  Data_0202a64c[1] = work->z;
  work->windowSlot = Resource_LoadFixedBlockBIntoFreeSlotFar();
  tile = gResourceTableEntries[work->windowSlot].offset;
  work->window = UiWindow_CreateFar(0, 0, 0, 0, 2);
  work->shown = -1;
  work->speed = 0x10000;
  *oam++ = 0;
  *oam++ = 0x40000000;
  *oam = 0x400 | (tile >> 5);
  Data_0202a62c[0] = (s32)Data_0202a004;
  Data_0202a62c[1] = screenX;
  Data_0202a62c[2] = screenY;
}
