// Under -mthumb-slow-loads (2026-10-26): differ 4 of 232 in tla-en, instruction
// order only. An empty asm before `spawnZ = 0;` lowers it to 2; no second one
// closes it. Permute (4 minutes) best differ 4.
#include "types.h"
#include "mem.h"
#include "ram_buffer.h"
#include "callback_scheduler.h"
#include "map.h"
#include "frame.h"
#include "system.h"

struct Sparkle {
  u8 padding00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 matrix : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 padding0a[2];
  s32 posX;
  s32 height;
  s32 posZ;
  s32 delay;
  u16 timer;
  u8 padding1e[2];
};

struct SparkleWork {
  u8 padding00[4];
  s32 tileBase;
  struct Sparkle sparkles[64];
  u8 padding808[8];
  s32 stopped;
};

struct SparklePicture {
  s16 dy;
  u16 tile;
};

extern const struct SparklePicture gData_080f0b7c[];

s32 GameFlag_Test(s32 flag);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void Func_080140d8(void *entry, s32 priority);

void FieldEffect_UpdateSparkles(void) {
  /* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip
   * stores together. */
  struct SparkleWork *work;
  u32 spawned;
  u32 i;
  struct MapWork *map;
  s32 *camera;
  s32 spawnX;
  s32 spawnZ;
  struct Sparkle *sparkle;
  const u16 *frame;
  s32 *leader;
  s32 cameraX;
  s32 cameraZ;
  s32 sx;
  s32 sy;
  s32 delay;
  s32 x;
  u32 a;
  u32 b;
  u32 frames;

  work = Ram_HeapSlots->unk70[1];
  spawned = 0;
  map = Ram_HeapSlots->mapWork;
  camera = &map->viewX;
  spawnX = 0;
  spawnZ = 0;
  i = 0;
  sparkle = work->sparkles;
  delay = 0;
  sx = 0;
  sy = 0;
  do {
    if (sparkle->timer != 0) {
      cameraX = camera[0];
      cameraZ = camera[1];
      if (GameFlag_Test(0x166) != 0)
        sparkle->timer++;
      frame = (const u16 *)&gData_080f0b7c[sparkle->timer >> 1];
      a = Random16();
      b = Random16();
      x = ((sparkle->posX - cameraX) >> 16) + (((a & 1) + (b & 1)) >> 1);
      sx = x - 1;
      sy = (sparkle->posZ - sparkle->height - cameraZ) / 0x10000 +
           *(const s16 *)frame;
      frame++;
      if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
        sparkle->tile = work->tileBase + *frame;
        sparkle->x = sx;
        sparkle->shape = 0;
        sparkle->size = 1;
        frames = *(unsigned long *)&gFrameCount;
        sparkle->y = sy;
        sparkle->matrix = ((frames >> 1) & 1) << 3;
        Func_080140d8(sparkle, 240);
      }
      sparkle->timer--;
    }
    if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
      if (delay != 0) {
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = Map_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 62 - delay;
        sparkle->delay = 0;
        spawned++;
        delay += 4;
      } else if ((Random16() & 255) == 0) {
        leader = map->origin;
        spawnX = leader[0] + (Random16() << 8) - 0x800000;
        spawnZ = leader[2] + (Random16() << 8) - 0x800000;
        sparkle->posX = spawnX;
        sparkle->posZ = spawnZ;
        sparkle->height = Map_GetTerrainHeightFar(0, sx, sy);
        sparkle->timer = 30;
        sparkle->delay = delay;
        spawned++;
        delay = 4;
      }
    }
    i++;
    sparkle++;
  } while (i <= 63);
}
