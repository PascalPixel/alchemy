// Draft, differ 2 of 167, skeleton gap 1 (tla-en, 2026-11-01)
// Twin of ☀️'s Weather_UpdateRain (games/tbs/src/field/event_commands/weather.c)
// and Func_080d64f0's sibling for the same 64 ground particles, here with
// fixed sprites: each counts down its frame (held while flag 358 is set), is
// drawn at its picture's offset (Data_080f0b50, pairs) with the work's tile,
// shape 1 and size 2 when on screen, and when its count runs out is placed
// again at a random point (x within -64 to +192, z within -128 to +128 pixels
// of the map's focus), on the ground, for 12 frames.
// Porting the twin's forms took differ from 153 to 4: x and y reused for the
// new point, its screen test, and the bare terrain call. Declaring particle
// before originZ, unlike the twin, fixed the next pair.
// Left: one scheduling pair, the native's mov r2, r8 (for work->tile) comes
// before strh r3, [r6, #6] (particle->x). Tried without gain: orders of the
// sprite field stores, a local for the tile, tile and x casts, the tile's
// type, the declaration order of frame, x and y, spellings of the screen
// test, const off frame, and splitting the frame additions (far worse).
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
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0];
  struct GroundParticle *particle = work->particles;
  s32 originZ = origin[1];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    const s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(358))
      particle->timer++;
    frame = &Data_080f0b50[2 * particle->timer];
    x = (particle->posX - originX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - originZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
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

      x = focus[0] + (Random16() << 8) - 0x400000;
      y = focus[2] + (Random16() << 8) - 0x800000;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
      particle->timer = 12;
    }
  }
}
