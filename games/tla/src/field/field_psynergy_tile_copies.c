/// @file
/// The Cyclone's helpers: copying the cells it touched, its arcs and its
/// burst, and a Mad Plant leaping at the leader.
// Part of field_psynergy.c.

#include "field_psynergy_internal.h"
#include "fixed_math.h"
#include "sounds.h"
#include "macros.h"

#include "system/system.h"
#include "gamepak/far_call/field.h"
#include "field/field.h"
#include "field/emote.h"
#include "field/canvas_sprite.h"

/// The frames of a Mad Plant's leap at the leader.
#define MAD_PLANT_LEAP_FRAMES 20
/// The pixels of one of the Cyclone's arcs, and the angle from one to the
/// next, about two degrees.
#define CYCLONE_ARC_PIXELS 30
#define CYCLONE_ARC_PIXEL_STEP 368

/// Clears the weeds the Cyclone blew away: for each cell it touched (the field
/// state's list from FieldPsynergy_GatherNearbyCells) inside the width by
/// height rectangle at (x, z) of the layer, copies the matching cell tiles
/// from the rectangle at (toX, toZ) over it, opens it on layer 2 and records
/// the copy in gTouchCopies, up to 64. Gaia Rock calls it after a Cyclone.
void Cyclone_CopyTouchedCells(s32 x, s32 z, s16 layerIndex, s16 width,
                              s16 height, s32 toX, s32 toZ) {
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *owner = Actor_GetById(Party_GetLeaderActor());
  struct MapLayer *layer = ((struct MapState *)Ram_HeapSlots->mapGrid)->layers;
  struct TouchCell *cell = (struct TouchCell *)field->psynergyCells;
  s32 lift;
  s32 scrollX = FIELD_CELL(layer->offsetX);
  s32 scrollZ = FIELD_CELL(layer->offsetY);
  s32 layerX, layerZ;
  s32 left, top, i;

  if (layerIndex <= 2)
    layer += layerIndex;
  layerX = FIELD_CELL(layer->offsetX);
  layerZ = FIELD_CELL(layer->offsetY);
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

      MapGrid_CopyCellTilesCellsRectFar(toX + dx, toZ + dz, 1, 1, x + dx,
                                        z + dz);
      MapGrid_GetCellAt(FIELD_CELL_ORIGIN(cx), FIELD_CELL_ORIGIN(cz), 2)
          ->collisionCode = 0;
      n = gTouchCopies.count;
      if (n < (s32)ARRAY_COUNT(gTouchCopies.copies)) {
        gTouchCopies.copies[n].toX = toX + dx;
        gTouchCopies.copies[n].toZ = toZ + dz;
        gTouchCopies.copies[n].fromX = x + dx;
        gTouchCopies.copies[n].fromZ = z + dz;
        gTouchCopies.copies[n].cellX = cell[i].x;
        gTouchCopies.copies[n].cellZ = cell[i].z;
        gTouchCopies.count++;
      }
    }
  }
}

/// A Mad Plant (sprite) springs out of the bushes cleared at the cell (x, z):
/// it grows, turns to the leader, hops twice and leaps at the leader in an arc
/// before the battle. The Gabomba Catacombs, Taopo Swamp and Gaia Rock each
/// hide one.
void MadPlant_LeapAtLeader(s32 sprite, s32 x, s32 z) {
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = Actor_GetById(ownerId);
  struct Actor *actor = Actor_CreateFar(sprite, toFIXED(x), 0, toFIXED(z));
  FIXED from[3];
  FIXED to[3];
  s32 i;
  s32 steps;

  if (actor == NULL)
    return;
  Actor_SetSpriteFlagsFar(actor, 0);
  actor->gravity = toFIXED(7) / 20;
  actor->velocityY = toFIXED(5);
  actor->scaleY = actor->scaleX = FIXED_ONE * 2 / 5;
  do {
    wait(1);
    actor->scaleY = actor->scaleX = actor->scaleX + FIXED_ONE * 3 / 200;
    actor->facing += FACING_STEP;
  } while (actor->scaleX < FIXED_ONE);
  actor->scaleY = actor->scaleX = FIXED_ONE;
  Actor_SelectAnimationFar(owner, 1);
  Actor_TurnPairToFaceEachOther(actor, owner);
  owner->timer = owner->facing;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ownerId, 3);
  FieldEffect_SetAttached(ownerId,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(32);
  actor->gravity = toFIXED(1);
  actor->velocityY = toFIXED(6);
  Sound_PlayFar(SE_JUMP);
  Event_Wait(15);
  actor->velocityY = toFIXED(6);
  Sound_PlayFar(SE_JUMP);
  Event_Wait(23);
  Sound_PlayFar(SE_ENEMY_CRY);

  from[0] = actor->x.fixed;
  from[1] = actor->y.fixed;
  from[2] = actor->z.fixed;
  to[0] = owner->x.fixed;
  to[1] = owner->y.fixed;
  to[2] = owner->z.fixed;
  // A straight line, lifted by half a sine wave 40 pixels high.
  steps = MAD_PLANT_LEAP_FRAMES;
  for (i = 0; i < steps; i++) {
    actor->x.fixed = from[0] + i * (to[0] - from[0]) / MAD_PLANT_LEAP_FRAMES;
    actor->y.fixed = from[1] + i * (to[1] - from[1]) / MAD_PLANT_LEAP_FRAMES +
                     Trig_Sin(i * (TRIG_TURN / 2) / MAD_PLANT_LEAP_FRAMES) * 40;
    actor->z.fixed = from[2] + i * (to[2] - from[2]) / MAD_PLANT_LEAP_FRAMES;
    wait(1);
  }
}

/// Plots an arc of CYCLONE_ARC_PIXELS pixels at radius (at most 31) from the
/// middle of the canvas, from angle on, shading through the canvas palette's
/// colours 1 to 15; offsets maps each point to its byte. The Cyclone draws its
/// swirl with it.
void Cyclone_PlotArc(s32 radius, s32 angle, u16 *offsets, u8 *pixels) {
  s32 i;

  if (radius > CANVAS_WIDTH / 2 - 1)
    radius = CANVAS_WIDTH / 2 - 1;
  for (i = 0; i < CYCLONE_ARC_PIXELS; i++) {
    s32 x = FIXEDtoINT(radius * Trig_Cos(angle)) + CANVAS_WIDTH / 2;
    s32 y = FIXEDtoINT(radius * Trig_Sin(angle)) + CANVAS_WIDTH / 2;
    s32 value = i / 2 + CANVAS_PALETTE * PALETTE_COLORS + 1;

    pixels[offsets[y * CANVAS_WIDTH + x]] = value;
    angle += CYCLONE_ARC_PIXEL_STEP;
  }
}

/// Starts Cyclone's 96 leaves, scattered round the caster and spinning, to
/// blow toward the first cell of FieldState.psynergyCells (ParticleBurst_Update
/// moves them).
void ParticleBurst_Start(void) {
  struct ParticleBurstState *state =
      (struct ParticleBurstState *)Ram_HeapSlots->battleEffect;
  struct FieldEffectState *effect = Ram_HeapSlots->fieldEffectState;
  struct FieldState *field = Ram_HeapSlots->fieldState;
  struct Actor *owner = effect->mainActor;
  struct BurstCell *cell = (struct BurstCell *)field->psynergyCells;
  FIXED dx = FIELD_CELL_ORIGIN(cell->x) - owner->x.fixed + toFIXED(8);
  FIXED dz = FIELD_CELL_ORIGIN(cell->z) - owner->z.fixed + toFIXED(8);
  s32 heading = (u16)Trig_Atan(dz, dx);
  struct EffectSprite *sprite;
  struct BurstParticle *particle;
  s32 resource;
  s32 vram;
  s32 i;

  state->frame = 0;
  state->active = 1;
  Decompress_TaggedLz(Resource_GetTableEntry(RESOURCE_LEAVES), state);
  resource = VramCache_FindFreeEntry();
  vram = VramCache_Load(resource, 384, state);
  state->cacheEntry = resource;
  state->tile = vram;

  for (i = 0, particle = state->particles; i < PARTICLE_BURST_COUNT;
       i++, particle++) {
    sprite = &state->sprites[i];
    EffectSprite_Init(sprite, 4, 4, 0, vram);
    sprite->depth = EFFECT_SPRITE_FRONT_DEPTH;
    sprite->oam.priority = 0;
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    SET_POSITION(particle, owner->x.fixed, owner->y.fixed, owner->z.fixed);
    Vector_AddPolarOffset(Random16() * 8 + toFIXED(8), Random16(),
                          &particle->x);
    particle->heading = heading;
    particle->speed = Random16() + toFIXED(1);
    particle->spin = (Random16() >> 6) + 384;
    if (i & 1)
      particle->spin = -particle->spin;
    particle->delay = -i / 2;
  }
  Sound_PlayFar(SE_SHIMMER);
  Task_Add(ParticleBurst_Update, TASK_PASS_FRAME);
}

#undef MAD_PLANT_LEAP_FRAMES
#undef CYCLONE_ARC_PIXELS
#undef CYCLONE_ARC_PIXEL_STEP
