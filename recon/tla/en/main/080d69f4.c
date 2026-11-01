// Draft, differ 29 of 189, skeleton gap 5 (tla-en, 2026-11-01)
// Updates 64 rising particles of the heap slot 0x74 workspace: each starts on
// the ground at a random point of a fixed strip for 120 frames, drifts right by
// a random amount each frame, rises with its age, and is drawn at priority 3,
// its tile stepping back by 8 at 90 and 60 frames left and flickering between
// two tiles every 8 frames. No ☀️ twin found.
//
// Allocation and hoisting now agree with the native: particle r5, x r6, y r7,
// i on the stack, screen r10, tile mask r11, 0x3ff rebuilt at each use. What
// settled it: x and y are one variable each across both phases, the y store
// comes last (its live length puts x ahead of y in global.c's order),
// `(frame bit) * 4 + tile` in that operand order, shape before size. A real
// `for`: goto loses both hoists (differ 171). All 720 orders of the six sprite
// stores were measured; this one is alone at 29.
//
// Left, three sites (the b.n offsets only follow the missing instruction):
// 1. Native `ldrh r2 (timer); ldrsh r3; subs r3, r3, r2; adds r7, r3, #0`, the
//    draft `ldrsh r2; ldrh r3; subs r7, r2, r3`. Combine merges every form of
//    the copy: a temporary, `t = pixel; t -= timer; y = t`, one-pass blocks,
//    static inlines, statement expressions, an empty asm. Loading timer first
//    through a local gets the order but costs a spill register and rotates
//    reload's scratch (differ 127); y = timer; y = pixel - y gets it with
//    skeleton gap 4 but its extra refs put y ahead of x (differ 55). A copy
//    present at global.c would do the same to y's priority (20 refs against
//    16), so it may come from elsewhere; a two-word struct for x and y spills
//    and has the copy at the wrong site, and y split from the first phase's z
//    lands in r4 (y must cross a call).
// 2. Preheader: native `movs r1, #0` right after `mov r9, r3` and the store
//    after `movs r2, #4`; here sched2 ranks them low (priorities 2 and 1, the
//    pool load 4). Moving i's initialisation, `i != 64` and s32 i do not help.
// 3. `ldrb r2, [r5, #5]` falls before `strh r3, [r5, #6]`; may follow from 1.
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
  s32 x;
  s32 y;

  for (i = 0; i < 64; i++, particle++) {
    if (particle->timer == 0) {
      x = Random16() * 200 + 0x1200000;
      y = 0x500000 - Random16() * 20;
      particle->posX = x;
      particle->posZ = y;
      particle->posY = Map_GetTerrainHeightFar(0, x, y);
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
      particle->tile = ((gFrameCount >> 3) & 1) * 4 + particle->tile;
      particle->x = x;
      particle->shape = 0;
      particle->size = 1;
      particle->priority = 3;
      particle->blendMode = 1;
      particle->y = y;
      Func_080140d8(particle, 240);
    }
    particle->timer--;
  }
}
