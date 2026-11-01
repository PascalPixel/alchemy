// Matches: differ 0 (tla-en, 2026-11-01): updates the 64 ground particles of the
// heap slot 0x74 workspace. Each counts down its frame (held a frame longer
// while flag 358 is set), is drawn from its picture's offsets, tile, shape and
// size when on screen, and when its count runs out is placed again at a random
// point within 128 pixels of the map's focus, on the ground, for 16 frames.
// Twin of ☀️'s Weather_UpdateRain: the port took its store order (x, y, tile,
// shape, size; then posX, posZ, posY, timer), its reads of the frame table
// through u16 and u8, and its reuse of x and y for the new position, which
// settled the 63 mask, the spill and the allocation (39 of 176 before). The
// one change of behaviour from the twin is kept: the height call here takes
// and returns 16.16 values, with no shifts.
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
      particle->y = y;
      particle->tile = work->tile + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Func_080140d8(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *focus = *(s32 **)map;

      x = focus[0] + (Random16() << 8) - 0x800000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
      particle->timer = 16;
    }
  }
}
