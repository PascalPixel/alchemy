// Draft, differ 153 (157 before statements reordered 2026-10-26) of 167: Func_080d64f0's sibling for the same 64 ground
// particles with fixed sprites: each counts down its frame (held while flag
// 358 is set), is drawn at its frame's offset (Data_080f0b50, pairs) with the
// work's tile, shape 1 and size 2 when on screen, and when its count runs out
// is placed again at a random point (x within -64 to +192, z within -128 to
// +128 pixels of the map's focus), on the ground, for 12 frames. No ☀️ twin
// found. The statements and blocks follow the native one for one; register
// choice differs throughout: the native keeps the particle in r6 (here r7),
// x in r7 and y in r4, so every particle access and the screen tests carry a
// different register, and it rebuilds 0xffff in the second rounding where
// this draft reuses a register.
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

extern const s16 Data_080f0b50[];

void Func_080d6674(void) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *origin = (s32 *)(map + 228);
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
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
    frame = &Data_080f0b50[particle->timer * 2];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tile;
      particle->shape = 1;
      particle->size = 2;
      Func_080140d8(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;
      s32 newX = focus[0] + (Random16() << 8) - 0x400000;
      s32 newZ = focus[2] + (Random16() << 8) - 0x800000;

      particle->posX = newX;
      particle->posZ = newZ;
      particle->posY = Map_GetTerrainHeightFar(0, newX, newZ);
      particle->timer = 12;
    }
  }
}
