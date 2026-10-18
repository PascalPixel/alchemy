// Near miss, differ 36 of 89: order, and one constant's form. The native
// stores the age's zero from movs r2, #0 where this loads a halfword 0
// from the pool (ldr r2, =0); as u16 the same. The -33 and -13 masks'
// copies and the 0x80004000 load sit a little differently. The group read
// once into a local measured 36, read at each use 55.
// BattleFx_SpawnScaledArcObjects, ⚓️'s arc effect (heap slot 0xf8,
// arc_effect.h). Sets up the current group's two 40-byte arc sprites from the
// effect's tiles (16 by 16, attributes 0x80004000), shown, layer 0 and at the
// target object's priority, then restarts that group's age, records the
// target and flips to the other group. Not ☀️'s function of that name, which
// creates two objects.
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "vram_tab.h"

struct ArcSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 hidden : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 layer : 4;
  u8 unk0a[30];
};

struct ArcWork {
  struct ArcSprite sprites[2][2];
  s16 age[2];
  s16 resource;
  s16 group;
  struct ObjectRuntime *target;
};

void Func_080eaf98(struct ArcSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tiles);
s32 BattleFx_GetObjectPriority(struct ObjectRuntime *object);

void BattleFx_SpawnScaledArcObjects(struct ObjectRuntime *target) {
  struct ArcWork *work = Ram_HeapSlots->block62;
  s32 tiles = gResourceTableEntries[work->resource].offset >> 5;
  s32 group = work->group;
  struct ArcSprite *sprite = work->sprites[group];
  s32 i;

  for (i = 0; i < 2; i++, sprite++) {
    Func_080eaf98(sprite, 16, 16, 0x80004000, tiles);
    sprite->hidden = 0;
    sprite->layer = 0;
    sprite->priority = BattleFx_GetObjectPriority(target);
  }
  work->age[group] = 0;
  work->target = target;
  work->group ^= 1;
}
