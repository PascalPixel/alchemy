// Near miss, differ 18 of 231: The Broken Seal twin (FieldMotes_Update) with 64
// motes in the heap slot 0x74 workspace, started by FieldMotes_Start
// (080d6e58); the terrain height takes and returns whole coordinates here.
// Left: instruction order only, in six places: the native issues each of the
// four tile masks' pool loads (ldr r1, =0xfffffc00) one slot earlier, starts
// the jitter's 1 (movs r3, #1) after the position load and copies the
// workspace to fp after the stack stores. The age is the decremented timer
// held as s16 and subtracted as u16, as the native keeps it.
#include "types.h"
#include "frame.h"
#include "map.h"
#include "ram_buffer.h"

struct MoteParticle {
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
  u8 attr3[2];
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 delay;
  u16 timer;
};

struct MoteWork {
  s32 entry;
  s32 tileBase;
  struct MoteParticle particles[64];
};

u32 Random16(void);
s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Func_080140d8(void *entry, s32 priority);

void Func_080d6c60(void) {
  struct MoteWork *work = Ram_HeapSlots->unk70[1];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u32 spawned = 0;
  u32 i;
  struct MoteParticle *mote;

  for (i = 0, mote = work->particles; i < 64; i++, mote++) {
    s32 x;
    s32 y;

    if (--mote->timer != 0xffff) {
      s32 *camera = &map->viewX;
      s32 cameraX = camera[0];
      s32 cameraZ = camera[1];
      s16 age = mote->timer;
      u32 firstJitterDraw;
      u32 secondJitterDraw;

      if (GameFlag_Test(0x166)) {
        mote->timer++;
        mote->delay--;
      }
      firstJitterDraw = Random16();
      secondJitterDraw = Random16();
      x = ((mote->posX - cameraX) >> 16) +
          (((firstJitterDraw & 1) + (secondJitterDraw & 1)) >> 1) - 1;
      y = ((mote->posZ - mote->posY - cameraZ) >> 16) - (u16)age;
      if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
        if (mote->timer < 60) {
          mote->tile = work->tileBase + 16;
          mote->delay += 3;
        } else if (mote->timer < 90) {
          mote->tile = work->tileBase + 8;
          mote->delay++;
        } else {
          mote->tile = work->tileBase;
        }
        if ((gFrameCount >> 3) & 1)
          mote->tile += 4;
        mote->x = x;
        mote->y = y - (mote->delay >> 2);
        mote->shape = 0;
        mote->size = 1;
        Func_080140d8(mote, 240);
      } else {
        mote->timer = 0;
      }
    }
    if (spawned < 8 && mote->timer == 0) {
      s32 *leader = map->origin;

      x = leader[0] + (Random16() << 8) - 0x800000;
      y = leader[2] + (Random16() << 8) - 0x800000;
      mote->posX = x;
      mote->posZ = y;
      mote->posY = Map_GetTerrainHeightFar(0, x, y);
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}
