// Draft, differ 47 of 176 (142 before; 2026-10-26: the bitfield y stored after
// the shape and the new z after the timer, plain reorders that give the
// native's invariants and allocation; left, instruction order and the new
// position's sum, which the native forms through r1 and a spill): updates the 64 ground particles of the heap slot
// 0x74 workspace. Each counts down its frame (held a frame longer while flag
// 358 is set), is drawn from its picture's offsets, tile, shape and size when on
// screen, and when its count runs out is placed again at a random point within
// 128 pixels of the map's focus, on the ground, for 16 frames. No ☀️ twin
// found. The statements and blocks follow the native one for one; what
// differs is loop.c's choice of invariants: the native hoists the bitfields'
// 63 mask into r11 and keeps 0xff800000 in the loop, agscc the reverse (63
// built in place, 0xff800000 in r6 across the loop). That moves the rest of
// the allocation: the native keeps the frame pointer in r0, x in r1 and y in
// r4, spills the new x across Random16 (frame 12, here 8), and keeps the work
// in r8 and the map in r10 where this draft swaps them.
#include "types.h"
#include "flag_call.h"
#include "ram_buffer.h"

struct GroundParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 attr0 : 6;
  u16 shape : 2;
  u16 x : 9;
  u16 attr1 : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  u8 unk18[4];
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  u8 unk00[4];
  s32 tile;
  struct GroundParticle particles[64];
};

u32 Random16(void);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void Func_080140d8(struct GroundParticle *sprite, s32 depth);

extern const s16 Data_080f0ab0[];

void Func_080d64f0(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  s32 originZ = origin[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(358))
      particle->timer++;
    frame = &Data_080f0ab0[particle->timer * 5];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      particle->priority = 1;
      particle->x = x;
      particle->tile = work->tile + *frame++;
      particle->shape = *frame;
      particle->y = y;
      particle->size = frame[1];
      Func_080140d8(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;
      s32 newX = focus[0] + (Random16() << 8) - 0x800000;
      s32 newZ = focus[2] + (Random16() << 8) - 0x800000;

      particle->posX = newX;
      particle->posY = Map_GetTerrainHeightFar(0, newX, newZ);
      particle->timer = 16;
      particle->posZ = newZ;
    }
  }
}
