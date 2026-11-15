/// @file
/// The Cyclone's helpers: copying the cells it touched, its arcs and its
/// burst, and an object jumping to the leader.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

#include "system/system.h"
#include "gamepak/far_call/field.h"
#include "field/field.h"

/// For each cell the Cyclone touched (the event work's point list from
/// FieldPsynergy_GatherNearbyCells) inside the width by height rectangle at
/// (x, z) of the layer, copies the matching cell picture from the rectangle at
/// (toX, toZ) over it, opens it (layer 2's collision 0) and records the copy
/// in gSceneLocalState, up to 64. Trial Road, Gaia Rock and the Gabomba
/// catacombs call it after a Cyclone.
void Cyclone_CopyTouchedCells(s32 x, s32 z, s16 layerIndex, s16 width,
                              s16 height, s32 toX, s32 toZ) {
  u8 *event = Ram_HeapSlots->fieldWork;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct TouchLayer *layer =
      ((struct TouchMap *)Ram_HeapSlots->mapGrid)->layers;
  struct TouchCell *cell = (struct TouchCell *)(event + 0xcc0);
  s32 lift;
  s32 scrollX = FIELD_CELL(layer->x);
  s32 scrollZ = FIELD_CELL(layer->z);
  s32 layerX, layerZ;
  s32 left, top, i;

  if (layerIndex <= 2)
    layer += layerIndex;
  layerX = FIELD_CELL(layer->x);
  layerZ = FIELD_CELL(layer->z);
  left = x - layerX;
  top = z - layerZ;
  lift = -(FIELD_CELL(owner->groundHeight));

  /* FAKEMATCH(signature): gSceneLocalState typed as 1491 TouchCells before the
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
      MapGrid_GetCellAt(cx << 20, cz << 20, 2)->collisionCode = 0;
      n = gSceneLocalState.count;
      if (n < 64) {
        gSceneLocalState.copies[n].toX = toX + dx;
        gSceneLocalState.copies[n].toZ = toZ + dz;
        gSceneLocalState.copies[n].fromX = x + dx;
        gSceneLocalState.copies[n].fromZ = z + dz;
        gSceneLocalState.copies[n].cellX = cell[i].x;
        gSceneLocalState.copies[n].cellZ = cell[i].z;
        gSceneLocalState.count++;
      }
    }
  }
}

/// A Mad Plant (sprite) springs out of the bushes cleared at the cell (x, z):
/// it grows, turns to the leader, hops twice and leaps at the leader in an arc
/// before the battle. The Gabomba Catacombs, Taopo Swamp, Gaia Rock,
/// Jupiter Lighthouse and Trial Road each hide one.
void MadPlant_LeapAtLeader(s32 sprite, s32 x, s32 z) {
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_GetById(ownerId);
  struct Actor *object = Actor_CreateFar(sprite, px(x), 0, px(z));
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
    object->facing += FACING_STEP;
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

  from[0] = object->x.fixed;
  from[1] = object->y.fixed;
  from[2] = object->z.fixed;
  to[0] = owner->x.fixed;
  to[1] = owner->y.fixed;
  to[2] = owner->z.fixed;
  steps = 20;
  for (i = 0; i < steps; i++) {
    object->x.fixed = from[0] + i * (to[0] - from[0]) / 20;
    object->y.fixed =
        from[1] + i * (to[1] - from[1]) / 20 + Trig_Sin((i << 15) / 20) * 40;
    object->z.fixed = from[2] + i * (to[2] - from[2]) / 20;
    wait(1);
  }
}

/// Plots 30 pixels of an arc at radius (at most 31) from the middle of a 64 by
/// 64 picture, from angle on, shading from colour 225 to 239; offsets maps
/// each point to its byte. The Cyclone draws its swirl with it.
void Cyclone_PlotArc(s32 radius, s32 angle, u16 *offsets, u8 *pixels) {
  s32 i;

  if (radius > RGB_LEVEL_MAX)
    radius = RGB_LEVEL_MAX;
  for (i = 0; i <= 29; i++) {
    s32 x = ((radius * Trig_Cos(angle)) >> 16) + 32;
    s32 y = ((radius * Trig_Sin(angle)) >> 16) + 32;
    s32 value = i / 2 + 225;

    pixels[offsets[y * 64 + x]] = value;
    angle += 0x170;
  }
}

/// Starts Cyclone's 96 leaves, scattered round the caster and spinning, to
/// blow toward the first cell of FieldWork.psynergyCells (ParticleBurst_Update
/// moves them).
void ParticleBurst_Start(void) {
  struct ParticleBurstWork *work =
      (struct ParticleBurstWork *)Ram_HeapSlots->battleEffect;
  struct FieldEffectWork *effect = Ram_HeapSlots->fieldEffectWork;
  u8 *event = Ram_HeapSlots->fieldWork;
  struct Actor *owner = effect->mainActor;
  struct BurstCell *cell = (struct BurstCell *)(event + 0xcc0);
  s32 dx = (cell->x << 20) - owner->x.fixed + px(8);
  s32 dz = (cell->z << 20) - owner->z.fixed + px(8);
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
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    SET_POSITION(particle, owner->x.fixed, owner->y.fixed, owner->z.fixed);
    Vector_AddPolarOffset(Random16() * 8 + px(8), Random16(), &particle->x);
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
