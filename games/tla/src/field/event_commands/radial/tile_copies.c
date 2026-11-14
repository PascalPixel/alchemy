/// @file
/// The Cyclone's helpers: copying the cells it touched, its arcs and its
/// burst, and an object jumping to the leader.
// Part of radial.c.

#include "../radial_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

#include "module/system/system.h"
#include "gamepak/far_call/field.h"
#include "field/field.h"

/// For each cell the Cyclone touched (the event work's point list from
/// FieldPsynergy_GatherNearbyCells) inside the width by height rectangle at
/// (x, z) of the layer, copies the matching cell picture from the rectangle at
/// (toX, toZ) over it, opens it (layer 2's collision 0) and records the copy
/// in gData_02001000, up to 64. Trial Road, Gaia Rock and the Gabomba
/// catacombs call it after a Cyclone.
void Cyclone_CopyTouchedCells(s32 x, s32 z, s16 layerIndex, s16 width,
                              s16 height, s32 toX, s32 toZ) {
  u8 *event = Ram_HeapSlots->fieldWork;
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
   * copies. */
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

      MapGrid_CopyCellPictureCellsRectFar(toX + dx, toZ + dz, 1, 1, x + dx,
                                          z + dz);
      MapGrid_GetCellAt(cx << 20, cz << 20, 2)->unk02 = 0;
      n = gData_02001000.count;
      if (n < 64) {
        gData_02001000.copies[n].toX = toX + dx;
        gData_02001000.copies[n].toZ = toZ + dz;
        gData_02001000.copies[n].fromX = x + dx;
        gData_02001000.copies[n].fromZ = z + dz;
        gData_02001000.copies[n].cellX = cell[i].x;
        gData_02001000.copies[n].cellZ = cell[i].z;
        gData_02001000.count++;
      }
    }
  }
}

/// An object (sprite) pops up at the cell (x, z), grows, turns to the leader,
/// who raises its arms, hops twice and flies into the leader in an arc. Trial
/// Road, Gaia Rock, the Gabomba catacombs, Jupiter Lighthouse and Taopo Swamp
/// call it.
// XXX: what the object is (an item the Cyclone uncovers?); a render at Trial
// Road would settle it.
void FieldItem_JumpToLeader(s32 sprite, s32 x, s32 z) {
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_GetById(ownerId);
  struct Actor *object = Actor_CreateFar(sprite, x << 16, 0, z << 16);
  s32 from[3];
  s32 to[3];
  s32 i;
  s32 steps;

  if (object == NULL)
    return;
  Actor_SetSpriteFlagsFar(object, 0);
  object->gravity = 0x5999;
  object->velocityY = px(5);
  object->scaleY = object->scaleX = 0x6666;
  do {
    wait(1);
    object->scaleY = object->scaleX = object->scaleX + 0x3d7;
    object->facing += TRIG_TURN / 16;
  } while (object->scaleX <= 0xffff);
  object->scaleY = object->scaleX = Q16_ONE;
  Actor_SelectAnimationFar(owner, 1);
  Actor_TurnPairToFaceEachOther(object, owner);
  owner->timer = owner->facing;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ownerId, 3);
  FieldEffect_SetAttached(ownerId, 258);
  Event_Wait(32);
  object->gravity = px(1);
  object->velocityY = px(6);
  Sound_PlayFar(SE_JUMP);
  Event_Wait(15);
  object->velocityY = px(6);
  Sound_PlayFar(SE_JUMP);
  Event_Wait(23);
  Sound_PlayFar(SE_ENEMY_CRY);

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

/// Plots 30 pixels of an arc at radius (at most 31) from the middle of a 64 by
/// 64 picture, from angle on, shading from colour 225 to 239; offsets maps
/// each point to its byte. The Cyclone draws its swirl with it.
void Cyclone_PlotArc(s32 radius, s32 angle, u16 *offsets, u8 *pixels) {
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

void ParticleBurst_Start(void) {
  struct BurstWork *work = (struct BurstWork *)Ram_HeapSlots->battleFxWork[0];
  u8 *effect = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->fieldWork;
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
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Leaves), work);
  resource = VramCache_FindFreeEntry();
  vram = VramCache_Load(resource, 384, work);
  work->resource = resource;
  work->vram = vram;

  for (i = 0, particle = work->particles; i < 96; i++, particle++) {
    sprite = &work->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, vram);
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    SET_POSITION(particle, owner->x, owner->y, owner->z);
    Vector_AddPolarOffset(Random16() * 8 + 0x80000, Random16(), &particle->x);
    particle->heading = heading;
    particle->speed = Random16() + 0x10000;
    particle->spin = (Random16() >> 6) + 384;
    if (i & 1)
      particle->spin = -particle->spin;
    particle->delay = -i / 2;
  }
  Sound_PlayFar(SE_SHIMMER);
  Task_Add(ParticleBurst_Update, TASK_PASS_VBLANK);
}
