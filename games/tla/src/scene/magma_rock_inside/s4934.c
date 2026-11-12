#include "../../../../games/tla/src/scene/tundaria_tower/tundaria_tower.h"
#include "map.h"

s32 Func_02004d20(s32 layer, s32 x, s32 z);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02004ef0(s32 layer, s32 shape, s32 height);

void Func_02004934(s32 level, s32 layer, s32 owner) {
  struct Actor *object = Actor_GetById(owner);
  u8 *shape = Ram_MapShapeGrid + (Ram_HeapSlots->mapWork->layers[layer].cells -
                                  (struct MapCell *)Ram_MapCellBuffer);
  s32 value = Func_02004d20(object->terrainId, object->x, object->z);
  s32 height =
      Map_GetTerrainHeight(object->terrainId, object->x, object->z) >> 19;

  height += level;

  shape += object->x / 0x100000 + object->z / 0x100000 * 128;
  *shape = Func_02004ef0(object->terrainId, value, height);
}

#include "local.h"

void Func_020049c0(s32 a, s32 b) {
  Func_02004934(a, b, gMainRoutineWork.currentOwner);
}

#include "types.h"
#include "system/ram_buffer.h"

struct Particle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 age;
};

struct ParticleSprite {
  u8 unk0[8];
  u16 attr;
  u8 unkA[30];
};

struct ParticleWork {
  u8 tiles[0x500];
  struct Particle particles[64];
  struct ParticleSprite sprites[64];
  s16 next;
  u16 tile;
};

void Func_02004f18(struct ParticleSprite *sprite, struct Particle *particle);
void Func_02004f20(struct Particle *particle, s32 unk1, s32 step);

void Func_020049d8(void) {
  struct ParticleWork *work = Ram_HeapSlots->menuRuntime;
  struct Particle *particle = work->particles;
  struct ParticleSprite *sprite = work->sprites;
  s32 i;
  s32 step = -0x8000;

  i = 63;
next:
  if ((u32)particle->age <= 19) {
    u16 *base = &work->tile;
    s32 mask = 7;
    s32 tile = (u16)((*base + ((particle->age >> 2) & mask) * 8) & 0x3ff);
    s32 attr = sprite->attr;

    sprite->attr = (attr & ~0x3ff) | tile;
    Func_02004f18(sprite, particle);
    Func_02004f20(particle, 63, step);
    particle->age++;
  }
  i--;
  sprite++;
  particle++;
  if (i >= 0)
    goto next;
}

#include "ram_buffer.h"
#include "event_inline.h"

/// One falling rock: where it is, how it moves and how long it has fallen.
struct Rock {
  s32 position[3];
  s32 velocity[3];
  s32 timer;
};

/// A falling rock's sprite.
struct RockSprite {
  u8 unk00[5];
  u8 unk05;
  u8 unk06[2];
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
  u8 unk0a[0x14];
  u16 unk1e;
  u8 unk20[8];
};

/// The falling rocks' work: their tiles, 64 rocks and their sprites, in a
/// ring that next walks round.
struct RockWork {
  u8 tiles[0x500];
  struct Rock rocks[64];
  struct RockSprite sprites[64];
  s16 next;
  u16 tileBase;
  s16 resource;
};

/// Spawns count rocks around (x, y, z), each pushed out at a random angle and
/// thrown up and out along it.
void Func_02004a5c(s32 x, s32 y, s32 z, s32 count) {
  struct RockWork *work;
  s32 i;

  /* FAKEMATCH(barrier): an empty asm before the work is read keeps the spills
   * of x and y at the entry; as an initializer, 4 of 88 instructions differ in
   * tla-en. */
  asm("");
  work = Ram_HeapSlots->menuRuntime;
  // A goto loop carries no loop notes, so the offsets are rebuilt on every
  // pass, as the bytes have them; a for loop hoists them.
  if (count > 0) {
    i = count;
  loop:;
    {
      s32 angle = Random16Far();
      struct Rock *rock = &work->rocks[work->next];

      rock->timer = 0;
      rock->position[0] = x;
      rock->position[1] = y;
      rock->position[2] = z;
      FieldVector_AddPolarOffset((Random16Far() << 3) + 0x80000, angle,
                                 rock->position);
      rock->velocity[0] = 0;
      rock->velocity[1] = 0x50000;
      rock->velocity[2] = 0;
      FieldVector_AddPolarOffset((Random16Far() << 1) + 0x20000, angle,
                                 rock->velocity);
      work->next++;
      work->next &= 63;
    }
    i--;
    if (i != 0)
      goto loop;
  }
}
