#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "animspr.h"
#include "resource.h"
#include "vram_tab.h"

void Func_080140d8(void *entry, s32 priority);

struct FadeActor {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
};

struct FadeOverlayWork {
  struct AnimationSpritePart marks[2];
  struct FadeActor *actor;
};

/// The blank overlay's task, which Func_080d1cc0 starts: each frame it draws
/// a 16-pixel mark beside the actor where the map's third layer flags a cell
/// two rows up (left or right of the actor), and another for the row one up.
void Func_080d1ae4(void) {
  struct AnimationSpritePart *mark = Ram_HeapSlots->unk84[3];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s16 *camera = (s16 *)&map->viewX;
  s16 cameraX = camera[1];
  s16 cameraZ = camera[3];
  struct FadeActor *actor = ((struct FadeOverlayWork *)mark)->actor;
  s32 x;
  s32 row;
  u32 tile;
  struct MapCell *cells;
  struct MapCell *a;
  struct MapCell *b;
  struct AnimationSpritePart *m;

  if (actor == 0)
    return;
  x = ((s16 *)&actor->x)[1];
  row = (actor->z - actor->y) >> 16;
  tile = gResourceTableEntries[94].offset >> 5;
  cells = map->layers[2].cells;
  a = cells + (x + 8) / 16 + (row / 16 - 2) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 2) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = 0x40000000;
    ((u32 *)m)[2] = 0x400;
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 32;
    mark = m + 1;
    Func_080140d8(m, 1);
  }
  a = cells + (x + 8) / 16 + (row / 16 - 1) * 128;
  b = cells + (x - 8) / 16 + (row / 16 - 1) * 128;
  if ((a->attributeB & 1) || (b->attributeB & 1)) {
    m = mark;
    ((u32 *)m)[1] = 0x40000000;
    ((u32 *)m)[2] = 0x400;
    m->tile = tile;
    m->x = (u16)(x - cameraX) - 8;
    m->y = (u16)(((u16)row & 0xf0) - cameraZ) - 16;
    Func_080140d8(m, 1);
  }
}
