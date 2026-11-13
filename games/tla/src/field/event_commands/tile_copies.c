/// @file
/// Copying the map cells the leader's layer touches within a rectangle, and
/// the routines after it.

#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "event.h"
#include "event_work.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "gamepak/far_call/field.h"

/* VIEW(f0e2774): tag struct MapCell as struct MapCell__f0e2774; map.h has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/tile_copies.c:57: dereferencing pointer to
 * incomplete type. */
struct MapCell__f0e2774 {
  u8 unk00[2];
  u8 unk02;
};

/* VIEW(f0e2774): variable gData_02001000 as struct TouchCopies gData_02001000;
 * module/tla.h has s8 gData_02001000[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/tile_copies.c:61: request for member
 * `count' in something not a structure or union. */
extern struct TouchCopies gData_02001000__f0e2774 __asm__("gData_02001000");

struct MapCell__f0e2774 *Func_080dbb78(s32 x, s32 z, s32 layer);

void Func_080e2774(s32 x, s32 z, s16 layerIndex, s16 width, s16 height, s32 toX,
                   s32 toZ) {
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchLayer *layer =
      ((struct TouchMap *)Ram_HeapSlots->mapWork)->layers;
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 lift;
  s32 scrollX = layer->x >> 20;
  s32 scrollZ = layer->z >> 20;
  s32 layerX, layerZ;
  s32 left, top, i;

  if (layerIndex <= 2)
    layer += layerIndex;
  layerX = layer->x >> 20;
  layerZ = layer->z >> 20;
  left = x - layerX;
  top = z - layerZ;
  lift = -(owner->terrainHeight >> 20);

  /* FAKEMATCH(signature): gData_02001000 typed as 1491 TouchCells before the
   * copies, which it does not hold, so the cell's z is read after the last
   * copy's address is built; as u8 unk0000[0x174c], 2 of 169 instructions
   * differ in tla-en. */
  for (i = 0; cell[i].x != -1; i++) {
    s32 cz, cx, cellX, cellZ;

    cx = cell[i].x;
    cellX = cx - scrollX;
    cz = cell[i].z;
    cellZ = cz + lift - scrollZ;

    if (cellX >= left && cellX < left + width && cellZ >= top &&
        cellZ < top + height) {
      s32 dx = cellX - left;
      s32 dz = cellZ - top;
      s32 n;

      Func_080201f0(toX + dx, toZ + dz, 1, 1, x + dx, z + dz);
      Func_080dbb78(cx << 20, cz << 20, 2)->unk02 = 0;
      n = gData_02001000__f0e2774.count;
      if (n < 64) {
        gData_02001000__f0e2774.copies[n].toX = toX + dx;
        gData_02001000__f0e2774.copies[n].toZ = toZ + dz;
        gData_02001000__f0e2774.copies[n].fromX = x + dx;
        gData_02001000__f0e2774.copies[n].fromZ = z + dz;
        gData_02001000__f0e2774.copies[n].cellX = cell[i].x;
        gData_02001000__f0e2774.copies[n].cellZ = cell[i].z;
        gData_02001000__f0e2774.count++;
      }
    }
  }
}

#include "fixed_math.h"
#include "actor_view.h"

void Func_080e28d4(s32 sprite, s32 x, s32 z) {
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_GetById(ownerId);
  struct Actor *object = Func_080200c0(sprite, x << 16, 0, z << 16);
  s32 from[3];
  s32 to[3];
  s32 i;
  s32 steps;

  if (object == NULL)
    return;
  Actor_SetSingleChildFlagsFar(object, 0);
  object->gravity = 0x5999;
  object->velocityY = 0x50000;
  object->scaleY = object->scaleX = 0x6666;
  do {
    wait(1);
    object->scaleY = object->scaleX = object->scaleX + 0x3d7;
    object->facing += 0x1000;
  } while (object->scaleX <= 0xffff);
  object->scaleY = object->scaleX = 0x10000;
  Actor_SetMode(owner, 1);
  FacingObject_TurnPairToFaceEachOther(object, owner);
  owner->timer = owner->facing;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ownerId, 3);
  FieldEffect_SetAttached(ownerId, 258);
  Event_Wait(32);
  object->gravity = 0x10000;
  object->velocityY = 0x60000;
  Sound_PlayFar(152);
  Event_Wait(15);
  object->velocityY = 0x60000;
  Sound_PlayFar(152);
  Event_Wait(23);
  Sound_PlayFar(146);

  from[0] = object->x;
  from[1] = object->y;
  from[2] = object->z;
  to[0] = owner->x;
  to[1] = owner->y;
  to[2] = owner->z;
  steps = 20;
  for (i = 0; i < steps; i++) {
    object->x = from[0] + i * (to[0] - from[0]) / 20;
    object->y =
        from[1] + i * (to[1] - from[1]) / 20 + Trig_Sin((i << 15) / 20) * 40;
    object->z = from[2] + i * (to[2] - from[2]) / 20;
    wait(1);
  }
}

void Func_080e2a68(s32 radius, s32 angle, u16 *offsets, u8 *pixels) {
  s32 i;

  if (radius > 31)
    radius = 31;
  for (i = 0; i <= 29; i++) {
    s32 x = ((radius * Trig_Cos(angle)) >> 16) + 32;
    s32 y = ((radius * Trig_Sin(angle)) >> 16) + 32;
    s32 value = i / 2 + 225;

    pixels[offsets[y * 64 + x]] = value;
    angle += 0x170;
  }
}

#include "task.h"
#include "resource.h"
#include "system.h"
#include "effect_sprite.h"

void Func_080e2acc(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  struct Actor *owner = *(struct Actor **)(effect + 16);
  struct BurstCell *cell = (struct BurstCell *)(event + 0xcc0);
  s32 dx = (cell->x << 20) - owner->x + 0x80000;
  s32 dz = (cell->z << 20) - owner->z + 0x80000;
  s32 heading = (u16)ArcTan2(dz, dx);
  struct EffectSprite *sprite;
  struct BurstParticle *particle;
  s32 resource;
  s32 vram;
  s32 i;

  work->frame = 0;
  work->active = 1;
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Leaves), work);
  resource = Resource_FindFreeEntry();
  vram = VramCache_Load(resource, 384, work);
  work->resource = resource;
  work->vram = vram;

  for (i = 0, particle = work->particles; i < 96; i++, particle++) {
    sprite = &work->sprites[i];
    Func_080eaf98(sprite, 4, 4, 0, vram);
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    particle->x = owner->x;
    particle->y = owner->y;
    particle->z = owner->z;
    Vector_AddPolarOffset(Random16() * 8 + 0x80000, Random16(), &particle->x);
    particle->heading = heading;
    particle->speed = Random16() + 0x10000;
    particle->spin = (Random16() >> 6) + 384;
    if (i & 1)
      particle->spin = -particle->spin;
    particle->delay = -i / 2;
  }
  Sound_PlayFar(220);
  Task_Add(Func_080e2c38, TASK_PASS_VBLANK);
}
