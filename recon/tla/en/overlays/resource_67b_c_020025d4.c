// Draft, differ 90 of 185: overlay 67b's copy of overlay 679's Func_02001940
// (the same listing), whose draft holds its notes.
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
  s32 cameraX = camera[0];
  s32 cameraZ = camera[1];
  struct GroundParticle *particle = work->particles;
  u32 i;

  i = 0;
loop:
  {
    s16 *frame;
    s32 x;
    s32 y;

    if (--particle->timer == 0xffff)
      goto next;
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
      } else {
        particle->posX = 0;
        particle->posZ = 0;
        particle->posY = 0;
      }
      particle->timer = 16;
    }
  }
next:
  i++;
  particle++;
  if (i < 64)
    goto loop;
}
