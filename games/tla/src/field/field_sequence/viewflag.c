#include "types.h"
#include "ram_buffer.h"

struct SceneRegionEntry {
  s16 id;
  s16 flag;
  u8 unk04[4];
  s32 x;
  u8 unk0c[4];
  s32 z;
  u8 unk14[4];
};

struct MapViewBounds {
  u8 unk000[0xec];
  s32 minX;
  s32 minY;
  s32 maxX;
  s32 maxY;
};

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

void Scene_AssignViewFlags(struct SceneRegionEntry *entry) {
  struct MapViewBounds *view = (struct MapViewBounds *)Ram_HeapSlots->mapWork;

  GameFlag_ClearBit(0x164);
  GameFlag_SetBit(0x165);
  while (entry->id != -1) {
    /* FAKEMATCH(temporary): the flag read through a u16 copy, whose extra
     * instruction keeps loop-invariant motion to one hoisted bound address;
     * as s16 flag = entry->flag, 18 of 57 instructions differ in tla-en. */
    u16 raw = entry->flag;
    s16 flag = raw;

    if (flag == 0) {
      s32 x = entry->x;
      s32 z = entry->z;

      if (view->minX <= x && x <= view->maxX && view->minY <= z &&
          z <= view->maxY)
        entry->flag = 0x164;
      else
        entry->flag = 0x165;
    }
    entry++;
  }
}
