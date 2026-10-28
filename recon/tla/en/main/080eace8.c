// Draft, differ 107 of 128: register choice. The native spills the list
// pointer (sp 8), the event work (sp 4) and the target's terrain byte
// address (sp 0), and keeps the cell table, layer 2's table and the shape
// row in r10, r9 and r8; this keeps the list in r11 and the terrain
// address in r9 and spills layer 2's table instead, so most loads differ
// by register.
// Lowers the map again under the switch actor that Func_080eabd0
// raised. The actor is the effect work's object at 20 when an effect runs
// with the event work's halfword at 358 set, Func_080eaeb4's otherwise. If
// it is in the list (ids until 0xffff), its cell's shape takes
// Func_080eab98's tag four half-units lower, the cell entry's byte 3 is
// restored from the actor's word at 76, layer 2's slot is unblocked and the
// event work's word at 0xcbc records the actor.
#include "types.h"
#include "actor.h"
#include "ram_buffer.h"

struct SwitchCell {
  u8 unk00[2];
  u8 block;
  u8 flags;
};

struct RestoreLayer {
  u8 unk00[0x138 - 0x108];
  struct SwitchCell *cells;
  u8 unk34[4];
};

struct RestoreMap {
  u8 unk000[0x108];
  struct RestoreLayer layers[3];
};

extern u8 gMapShapeGrid[];

struct Actor *Actor_GetById(u32 actor);
struct Actor *Func_080eaeb4(void);
s32 Func_080eab70(struct Actor *object);
u32 Func_080202f0(s32 terrain, s32 x, s32 z);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 Func_080eab98(s32 unused, s32 key, s32 value);

void Func_080eace8(u16 *actors) {
  u8 *event = Ram_HeapSlots->eventWork;
  u8 *effect = Ram_HeapSlots->effectWork;
  struct Actor *target;

  if (effect != NULL && *(s16 *)(event + 358) != 0)
    target = *(struct Actor **)(effect + 20);
  else
    target = Func_080eaeb4();
  *(struct Actor **)(event + 0xcbc) = NULL;

  for (; *actors != 0xffff; actors++) {
    if (target == Actor_GetById(*actors)) {
      struct RestoreMap *map = (struct RestoreMap *)Ram_HeapSlots->mapWork;
      struct SwitchCell *cells = map->layers[target->terrainId].cells;
      struct SwitchCell *blocks = map->layers[2].cells;
      u8 *shapes = &gMapShapeGrid[((s32)cells - 0x02010000) >> 2];
      u32 key = Func_080202f0(target->terrainId, target->x, target->z);
      s32 level =
          Map_GetTerrainHeightFar(target->terrainId, target->x, target->z);
      s32 slot = Func_080eab70(target);
      s32 tag = Func_080eab98(target->terrainId, key, (level >> 19) - 4);

      cells[slot].flags = target->unk4c;
      shapes[slot] = tag;
      blocks[slot].block = 0;
      *(struct Actor **)(event + 0xcbc) = target;
    }
  }
}
