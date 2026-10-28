// 2026-10-26: differ 66; resource 0x1e0 is FieldEffect_Leaves in the directory
// (ResourceId_Leaves, to name there when this adopts); the native keeps 384
// in r8 across the particle loop and spills the heading.
// Draft, differ 67 (133 before statements reordered 2026-10-26) of 165: register choice, and one constant. The native
// keeps 384 in r8 from VramCache_Load's size to the spin's add, the owner
// in r9, the index in r10 and the vram slot in r11, and spills the heading
// (sp 4); this keeps the heading in r11 and builds 384 again in the loop.
// The native loads the resource id 480 from the pool (ldr r0, =0x1e0)
// where TLA's split constants build it (movs 240; lsls 1), which suggests
// the id is not a plain constant there. The heading as (u16) into an s32
// measured 133, as a u16 local 187.
// Starts a 96-particle burst (heap slot 0x5c work). Decodes resource
// 480 into the work and loads 384 bytes of it as tiles, sets up 96 4-by-4
// sprites (depth 250, hidden) and scatters as many particles around the
// effect owner, each heading from the owner toward the first touched cell
// with a random speed, an alternating spin and a start delayed by half its
// index; then plays cue 220 and schedules Func_080e2c38.
#include "types.h"
#include "callback_scheduler.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "object_runtime.h"

s32 ArcTan2(s32 y, s32 x);

struct BurstSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 hidden : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 layer : 4;
  u8 unk0a[20];
  s16 depth;
  u8 unk20[8];
};

struct BurstParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 heading;
  s32 speed;
  s32 spin;
  s32 delay;
};

struct BurstWork {
  u8 tiles[384];
  struct BurstSprite sprites[96];
  struct BurstParticle particles[96];
  s16 resource;
  s16 vram;
  u8 unk1b04[2];
  s16 frame;
  s16 active;
};

struct BurstCell {
  s16 x;
  s16 z;
};

void Func_080eaf98(struct BurstSprite *sprite, s32 width, s32 height,
                   u32 attributes, s32 tiles);
void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Func_080e2c38(void);
extern char ResourceId_Leaves;

void Func_080e2acc(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  struct ObjectRuntime *owner = *(struct ObjectRuntime **)((u8 *)Ram_HeapSlots->effectWork + 16);
  struct BurstCell *cell = (struct BurstCell *)((u8 *)Ram_HeapSlots->eventWork + 0xcc0);
  s32 dx = (cell->x << 20) - owner->x + 0x80000;
  s32 dz = (cell->z << 20) - owner->z + 0x80000;
  s32 heading = (u16)ArcTan2(dz, dx);
  struct BurstSprite *sprite;
  struct BurstParticle *particle;
  s32 resource;
  s32 vram;
  s32 i;

  work->frame = 0;
  work->active = 1;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Leaves), work);
  resource = Resource_FindFreeEntry();
  vram = VramCache_Load(resource, 384, work);
  work->resource = resource;
  work->vram = vram;

  sprite = work->sprites;
  particle = work->particles;
  for (i = 0; i < 96; i++, sprite++, particle++) {
    Func_080eaf98(sprite, 4, 4, 0, vram);
    sprite->depth = 250;
    sprite->priority = 0;
    sprite->hidden = 1;
    sprite->layer = 0;
    particle->x = owner->x;
    particle->y = owner->y;
    particle->z = owner->z;
    Vector_AddPolarOffset(Random16() * 8 + 0x80000, Random16(),
                          &particle->x);
    particle->heading = heading;
    particle->speed = Random16() + 0x10000;
    particle->spin = (Random16() >> 6) + 384;
    if (i & 1)
      particle->spin = -particle->spin;
    particle->delay = -i / 2;
  }
  Sound_PlayFar(220);
  Scheduler_AddOrUpdateCallback(Func_080e2c38, 0x480);
}
