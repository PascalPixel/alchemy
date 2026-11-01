// Draft, differ 62 of 185, skeleton gap 3 (tla-en, 2026-11-01)
// Twin of The Broken Seal's Weather_UpdateRain (same frame table walk, same
// bounds, same sprite fields, same 16-frame timer); overlay 67b's copy of
// overlay 679's Func_02001940 (the same listing). Three forms of the twin or
// the listing closed the gap from 8: the for loop (a label loop lets CSE fold
// the -13 mask's copy; the count-down register seen earlier no longer appears),
// timer = 16 set in each arm of the leader test, and particle declared before
// cameraX, which puts its adds #8 where the listing has it.
// Left, all allocation: the listing has frame in r4 and y in r6, this has them
// swapped, and the priority store's field and mask copy are r3/r2 here r2/r3.
// r4 is call-used, so frame and y race for it: global.c ranks y 1.615 (14 refs,
// live 26) over frame 1.600 (24 refs, live 60). Both this and the local
// allocator's field-over-mask order would come out as the listing's if these
// blocks had loop depth 0 (y 0.538, frame 0.600; field 2.4, mask 2), but they
// are in a natural loop. Separately, with y stored after the size, frame takes
// r4 and differ falls to 34 at gap 4 (the y store then sits after the b.n over
// the pool, not in the x store's load slot).
// Measured, no gain: every placement of the priority, x and y stores (120);
// y stored after the tile, or after a tile or tileBase temporary; new variables
// for the new position (differ 113, 177); frame, x, y declared in either order
// or outside the loop; frame built in two steps, from a [][5] table or by bytes;
// x or y split or their terms swapped; u32, s16 and u8 bit-fields; leader
// assigned apart; unsigned bounds tests. priority = 0 before priority = 1 gets
// differ 25 at gap 6 but is a dead store.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct GroundParticle {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 unk1e[2];
};

struct GroundParticleWork {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[64];
  s32 *leader;
};

extern s16 gData_02002cd4[];

s32 GameFlag_Test(s32 flag);
s32 Random16Far(void);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020028d0(void *entry, s32 slot);

void Func_020025d4(void) {
  struct MapWork *view = Ram_HeapSlots->mapWork;
  struct GroundParticleWork *work = Ram_HeapSlots->unk70[1];
  s32 *camera = &view->viewX;
  struct GroundParticle *particle = work->particles;
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  u32 i;

  for (i = 0; i < 64; i++, particle++) {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      continue;
    if (GameFlag_Test(0x166))
      particle->timer++;
    frame = &gData_02002cd4[5 * particle->timer];
    x = (particle->posX - cameraX) / 0x10000 + *frame++;
    y = (particle->posZ - particle->posY - cameraZ) / 0x10000 + *frame++;
    if ((x >= -16 && x <= 239) && y >= -32 && y <= 159) {
      particle->priority = 1;
      particle->x = x;
      particle->y = y;
      particle->tile = work->tileBase + *(u16 *)frame;
      frame++;
      particle->shape = *(u8 *)frame;
      particle->size = *(u8 *)(frame + 1);
      Func_020028d0(particle, 240);
    }
    if (particle->timer == 0) {
      s32 *leader = work->leader;

      if (leader != NULL) {
        x = leader[0] + Random16Far() * 240 - 0x780000;
        y = leader[2] + Random16Far() * 160 - 0x500000;
        particle->posX = x;
        particle->posZ = y;
        particle->posY = Map_GetTerrainHeight(0, x, y);
        particle->timer = 16;
      } else {
        particle->posX = 0;
        particle->posZ = 0;
        particle->posY = 0;
        particle->timer = 16;
      }
    }
  }
}
