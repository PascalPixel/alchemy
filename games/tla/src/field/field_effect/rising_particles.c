#include "types.h"
#include "dma.h"
#include "edition.h"
#include "field_coordinate.h"
#include "fixed_math.h"
#include "frame.h"
#include "ram_buffer.h"

s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

/// One rising particle: its sprite, world position, drift and frames left.
struct RisingParticle {
  /// The sprite: set as three words by the start, drawn field by field.
  union {
    u32 words[3];
    struct {
      u8 unk00[4];
      /// OAM attributes 0 to 2.
      ///@{
      u16 y : 8; ///< screen pixels
      u16 attr0 : 2;
      u16 blendMode : 2;
      u16 attr0b : 2;
      u16 shape : 2;
      u16 x : 9; ///< screen pixels
      u16 attr1 : 5;
      u16 size : 2;
      u16 tile : 10;
      u16 priority : 2;
      u16 palette : 4;
      ///@}
      u8 unk0a[2];
    } attributes;
  } sprite;
  s32 posX;
  s32 posY;
  s32 posZ;
  s32 drift;
  u16 timer;
  u8 unk1e[2];
};

/// Heap slot 0x74 while the rising particles run.
struct RisingParticleWork {
  s32 vramEntry;
  s32 tile;
  struct RisingParticle particles[64];
};

void *HeapSlot_AllocEwram(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
u32 Random16(void);
void Task_Add(void (*func)(void), s32 priority);
void Camera_WorldToScreen(union FieldCoordinate *position);
void Func_080140d8(void *sprite, s32 depth);

extern const u8 gData_080f385e[];

/// Updates the 64 rising particles: each starts on the ground at a random
/// point of the strip for 120 frames, drifts right by a random amount each
/// frame, rises with its age, and is drawn at priority 3, its tile stepping
/// back by 8 at 90 and 60 frames left and flickering between two tiles every 8
/// frames.
void Func_080d69f4(void) {
  struct RisingParticleWork *work = Ram_HeapSlots->unk70[1];
  struct RisingParticle *particle = work->particles;
  union FieldCoordinate screen[3];
  u32 i;
  s32 x;
  s32 y;

  for (i = 0; i < 64; i++, particle++) {
    if (particle->timer == 0) {
      x = Random16() * 200 + Q16_ONE * 288;
      y = Q16_ONE * 80 - Random16() * 20;
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
    y = screen[2].part.pixel - particle->timer - 8;
    particle->drift += Random16() + Q16_ONE / 2;
    if ((u32)(x + 16) < 256 && y >= -32 && y < 160) {
      if (particle->timer < 60)
        particle->sprite.attributes.tile = work->tile + 16;
      else if (particle->timer < 90)
        particle->sprite.attributes.tile = work->tile + 8;
      else
        particle->sprite.attributes.tile = work->tile;
      particle->sprite.attributes.tile =
          ((gFrameCount >> 3) & 1) * 4 + particle->sprite.attributes.tile;
      particle->sprite.attributes.x = x;
      particle->sprite.attributes.shape = 0;
      particle->sprite.attributes.size = 1;
      particle->sprite.attributes.priority = 3;
      particle->sprite.attributes.blendMode = 1;
      particle->sprite.attributes.y = y;
      Func_080140d8(particle, 240);
    }
    particle->timer--;
  }
}

/// Starts the rising particles: allocates and clears heap slot
/// 0x74, decodes the particle tiles into VRAM, sets each of the 64 particles'
/// sprite words and places it on the ground at a random point of the strip
/// (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2 frames, and
/// schedules the update.
void Func_080d6b90(void) {
  struct RisingParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct RisingParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH: the register variable builds the fill word's zero in r2,
   * after the DMA address is started (differ 8 without it: the zero
   * takes r3 first). */
  register u32 zero asm("r2");
  s32 priority;
  u8 *tiles;
  u32 i;

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = Resource_FindFreeEntry();
  work->tile = VramCache_Load(work->vramEntry, 0x300, tiles);
  HeapSlot_Release(56);
  for (i = 0; i < 64; i++, particle++) {
    u32 *sprite = particle->sprite.words;
    s32 x;
    s32 z;

    *sprite++ = 0;
    *sprite++ = 0x40000400;
    *sprite = 0xd400;
    x = Random16() * 200 + 0x1200000;
    z = 0x500000 - Random16() * 20;
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->drift = 0;
    particle->timer = i * 2;
  }
  priority = 0x480;
  /* FAKEMATCH: the barrier builds the priority before the task function's pool
   * load, as the native does (differ 2 without it). */
  asm volatile("");
  Task_Add(Func_080d69f4, priority);
}
