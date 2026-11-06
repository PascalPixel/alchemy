// Matches: differ 0 of 165 (tla-en, 2026-11-06).
// What settled it (from 66): the sprite indexed and the particle a pointer
// stepped beside the index, as the sibling event commands write their
// loops, so loop.c hoists 384 and shares it with VramCache_Load's size and
// the spin; the effect and event work read into locals after the burst
// work; and the sprites typed as the tree's struct EffectSprite, whose byte
// 5 bit is oam.palette256 (not a hidden flag) and byte 9 holds oam.priority
// and oam.palette.
// Resource 0x1e0 is FieldEffect_Leaves in the directory (ResourceId_Leaves,
// to name there when this adopts); the native loads it from the pool.
// Starts a 96-particle burst (heap slot 0x5c work). Decodes resource
// 480 into the work and loads 384 bytes of it as tiles, sets up 96 4-by-4
// sprites (depth 250, 256 colours, priority 0) and scatters as many
// particles around the effect owner, each heading from the owner toward the
// first touched cell with a random speed, an alternating spin and a start
// delayed by half its index; then plays cue 220 and schedules Func_080e2c38.
#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "resource.h"
#include "system.h"
#include "actor.h"
#include "effect_sprite.h"

s32 ArcTan2(s32 y, s32 x);

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
  struct EffectSprite sprites[96];
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

void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);
void Func_080e2c38(void);
extern char ResourceId_Leaves;

void Func_080e2acc(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  struct BurstCell *cell = (struct BurstCell *)(event + 0xcc0);
  s32 dx = (cell->x << 20) - owner->x + 0x80000;
  s32 dz = (cell->z << 20) - owner->z + 0x80000;
  s32 heading = (u16)ArcTan2(dz, dx);
  struct EffectSprite *sprite;
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

  for (i = 0, particle = work->particles; i < 96; i++, particle++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, vram);
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
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
  Task_Add(Func_080e2c38, TASK_PASS_VBLANK);
}
