/// @file
/// The ground particles, the rising particles and the floating motes.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/field/object.h"

/* VIEW(ground_particles): tag struct GroundParticleWork as struct
 * GroundParticleWork__ground_particles; visual_attributes.c has another
 * definition; tried with the definition the rest of the object has, it does not
 * compile in tla-ja:
 * games/tla/src/field/object/visual_attributes/ground_particles.c:34: structure
 * has no member named `entry'. */
struct GroundParticleWork__ground_particles {
  s32 entry;
  s32 tileBase;
  struct GroundParticle__visual_attributes particles[64];
  s32 unk808;
  s32 stopped;
  s32 unk810;
};

/// Unused: nothing calls GroundParticles_StartFar. The Broken Seal's
/// Weather_StartRain, kept whole: 64 sprites placed at the leader's position on
/// the terrain, their tiles decoded into VRAM, the blend set, and
/// GroundParticles_Update scheduled to move them. The Lost Age's rain is
/// Weather_StartRain.
void GroundParticles_Start(void) {
  struct GroundParticleWork__ground_particles *work;
  s32 *leader;
  struct GroundParticle__visual_attributes *particle;
  // words is the decode buffer, then each particle's attribute cursor.
  u32 *words;
  u32 i;
  // The fill's value; as a literal 0 the fill differs.
  u32 zero = 0;

  work = HeapSlot_AllocEwram(
      116, sizeof(struct GroundParticleWork__ground_particles));
  leader = Ram_HeapSlots->mapWork->origin;
  Scene_QueueMusic(SE_170);
  particle = work->particles;
  Dma_Fill32(zero, work, sizeof(struct GroundParticleWork__ground_particles));
  words = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gFieldEffect_GroundParticleTiles, words);
  work->entry = VramCache_FindFreeEntry();
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
    particle->posY = MapGrid_GetTerrainHeightFar(0, x, z);
    particle->timer = (i & 15) + 1;
    particle++;
  }
  Io_Write16(BLDCNT_TGT2_ALL, &REG_BLDCNT);
  Io_Write16(BLEND_ALPHA(8, 16), &REG_BLDALPHA);
  Io_Write16(0, &REG_BLDY);
  Task_Add(GroundParticles_Update, TASK_PASS_VBLANK);
}

/// Task of RisingParticles_Start: updates the 64 rising particles; each starts
/// on the ground at a random point of the strip for 120 frames, drifts right by
/// a random amount each frame, rises with its age, and is drawn at priority 3,
/// its tile stepping back by 8 at 90 and 60 frames left and flickering between
/// two tiles every 8 frames.
void RisingParticles_Update(void) {
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
      particle->posY = MapGrid_GetTerrainHeightFar(0, x, y);
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
      Oam_AddSpriteAtDepthClamped(particle, 240);
    }
    particle->timer--;
  }
}

/// Starts the rising particles: allocates and clears heap slot
/// 0x74, decodes the particle tiles into VRAM, sets each of the 64 particles'
/// sprite words and places it on the ground at a random point of the strip
/// (x 288 to 488 pixels, z 60 to 80), staggering their timers by 2 frames, and
/// schedules the update.
void RisingParticles_Start(void) {
  struct RisingParticleWork *work = HeapSlot_AllocEwram(116, 0x814);
  struct RisingParticle *particle = work->particles;
  volatile u32 fill;
  /* FAKEMATCH(pin): the fill's zero held in r2, built after the copy of work.
   */
  register u32 zero asm("r2");
  s32 priority;
  u8 *tiles;
  u32 i;

  zero = 0;
  Dma_Fill32Cell(fill, zero, work, 0x814);
  tiles = HeapSlot_AllocEwram(56, 0x400);
  Resource_DecodeByteLzInRam(gData_080f385e, tiles);
  work->vramEntry = VramCache_FindFreeEntry();
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
    particle->posY = MapGrid_GetTerrainHeightFar(0, x, z);
    particle->drift = 0;
    particle->timer = i * 2;
  }
  priority = 0x480;
  Task_Add(RisingParticles_Update, priority);
}

struct MoteWork {
  s32 entry;
  s32 tileBase;
  struct MoteParticle particles[64];
};

/// The Broken Seal's routine with 64 motes in the heap slot 0x74
/// workspace, started by FieldMotes_Start.
void FieldMotes_Update(void) {
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

      if (GameFlag_Get(0x166)) {
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
        Oam_AddSpriteAtDepthClamped(mote, 240);
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
      mote->posY = MapGrid_GetTerrainHeightFar(0, x, y);
      mote->timer = 120;
      mote->delay = 0;
      spawned++;
    }
  }
}
