// Draft, differ 195 of 189: updates 64 rising particles of the heap slot 0x74
// workspace: each starts on the ground at a random point of a fixed strip for
// 120 frames, drifts right by a random amount each frame, rises with its age,
// and is drawn at priority 3, its tile stepping back by 8 at 90 and 60 frames
// left and flickering between two tiles every 8 frames. No ☀️ twin found. The
// statements and blocks follow the native one for one; what differs is
// loop.c's choice of invariants and the allocation after it: the native
// hoists the tile field's 0xfffffc00 mask into r11 and builds 0x3ff in place
// at each use, keeps the count on the stack and the screen vector's address
// in r10; agscc hoists 0x3ff (r11) and 0xfffffc00 (r9), keeps the count in a
// register and the particle in r6 (native r5). The screen vector is
// FieldCoordinate (as s32 read through a s16 cast its halves' addresses are
// hoisted too: differ 250).
#include "types.h"
#include "field_coordinate.h"
#include "ram_buffer.h"

struct RisingParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 attr0 : 2;
  u16 blendMode : 2;
  u16 attr0b : 2;
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
  s32 drift;
  u16 timer;
  u8 unk1e[2];
};

struct RisingParticleWork {
  u8 unk00[4];
  s32 tile;
  struct RisingParticle particles[64];
};

u32 Random16(void);
s32 Map_GetTerrainHeightFar(u8 layer, s32 x, s32 z);
void Camera_WorldToScreen(union FieldCoordinate *position);
void Func_080140d8(struct RisingParticle *sprite, s32 depth);

extern u32 gFrameCount;

void Func_080d69f4(void) {
  struct RisingParticleWork *work = Ram_HeapSlots->unk70[1];
  struct RisingParticle *particle = work->particles;
  union FieldCoordinate screen[3];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    s32 x;
    s32 y;

    if (particle->timer == 0) {
      s32 newX = Random16() * 200 + 0x1200000;
      s32 newZ = 0x500000 - Random16() * 20;

      particle->posX = newX;
      particle->posZ = newZ;
      particle->posY = Map_GetTerrainHeightFar(0, newX, newZ);
      particle->timer = 120;
      particle->drift = 0;
    }
    screen[0].fixed = particle->posX + particle->drift;
    screen[1].fixed = particle->posY;
    screen[2].fixed = particle->posZ;
    Camera_WorldToScreen(screen);
    x = screen[0].part.pixel;
    y = screen[2].part.pixel - particle->timer;
    particle->drift += Random16() + 0x8000;
    y -= 8;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      if (particle->timer < 60)
        particle->tile = work->tile + 16;
      else if (particle->timer < 90)
        particle->tile = work->tile + 8;
      else
        particle->tile = work->tile;
      particle->tile += ((gFrameCount >> 3) & 1) * 4;
      particle->x = x;
      particle->y = y;
      particle->size = 1;
      particle->priority = 3;
      particle->shape = 0;
      particle->blendMode = 1;
      Func_080140d8(particle, 240);
    }
    particle->timer--;
  }
}
