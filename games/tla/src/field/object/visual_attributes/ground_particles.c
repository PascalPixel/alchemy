/// @file
/// Routines first written as ground_particles.c; none has a twin in The Broken
/// Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

/* VIEW(ground_particles): tag struct GroundParticleWork as struct GroundParticleWork__ground_particles; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:2771: structure has no member named `entry'. */
struct GroundParticleWork__ground_particles {
  s32 entry;
  s32 tileBase;
  struct GroundParticle particles[64];
  s32 unk808;
  s32 stopped;
  s32 unk810;
};

void Func_080d2c64(s32 sound);
void Func_080d64f0(void);
s32 Resource_DecodeByteLzInRam(const void *source, void *destination);
extern const u8 gData_080f3798[];

/// Starts the ground particles: 64 sprites placed at the leader's position on
/// the terrain, their tiles decoded into VRAM, the blend set, and
/// Func_080d64f0 scheduled at priority 0x480 to move them.
void Func_080d690c(void) {
  struct GroundParticleWork__ground_particles *work;
  s32 *leader;
  struct GroundParticle *particle;
  // words is the decode buffer, then each particle's attribute cursor.
  u32 *words;
  u32 i;
  // The fill's value; as a literal 0 the fill differs.
  u32 zero = 0;

  work = HeapSlot_AllocEwram(116, sizeof(struct GroundParticleWork__ground_particles));
  leader = Ram_HeapSlots->mapWork->origin;
  Func_080d2c64(170);
  particle = work->particles;
  Dma_Fill32(zero, work, sizeof(struct GroundParticleWork__ground_particles));
  words = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f3798, words);
  work->entry = Resource_FindFreeEntry();
  work->tileBase = VramCache_Load(work->entry, 0x300, words);
  HeapSlot_Release(56);
  for (i = 0; i < 64; i++) {
    s32 x;
    s32 z;

    words = (u32 *)particle;
    *words++ = 0;
    *words++ = 0x40000400;
    *words = 0xd400;
    x = leader[0];
    z = leader[2];
    particle->posX = x;
    particle->posZ = z;
    particle->posY = Map_GetTerrainHeightFar(0, x, z);
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(Func_080d64f0, TASK_PASS_VBLANK);
}


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

u32 Random16(void);
void Camera_WorldToScreen(union FieldCoordinate *position);
/* VIEW(ground_particles): routine Func_080140d8 as void Func_080140d8(void *sprite, s32 depth); f0d6674 has void Func_080140d8(struct GroundParticle *sprite, s32 depth). */
extern void Func_080140d8__ground_particles(void *sprite, s32 depth) __asm__("Func_080140d8");

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
      Func_080140d8__ground_particles(particle, 240);
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
  /* FAKEMATCH(pin): the fill's zero held in r2, built after the copy of
   * work; as a plain local, 8 of 85 instructions differ in tla-en. */
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
  Task_Add(Func_080d69f4, priority);
}

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

s32 GameFlag_Test(s32 flag);

/// The Broken Seal's FieldMotes_Update with 64 motes in the heap slot 0x74
/// workspace, started by FieldMotes_Start.
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
        Func_080140d8__ground_particles(mote, 240);
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
