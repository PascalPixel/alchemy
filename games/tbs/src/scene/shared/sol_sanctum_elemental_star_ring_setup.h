#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"
#include "heap.h"

// Sol Sanctum's Elemental Star room and its eruption: the ten lights that float
// in the room, actors in a row from the first the map passes, each
// pulsing and drifting on its own path from the tables gSolSanctumRing_* and
// gSolSanctum_Ring*, by its index.

enum { SOL_SANCTUM_RING_MAX = 10 };

/// One floating light: its actor, its drift and its pulsing scale, and
/// where it is while hidden.
struct SolSanctumRingEntry {
  struct Actor *actor;
  s32 x; ///< 16.16 fixed-point pixels
  s32 y; ///< 16.16 fixed-point pixels
  s32 z; ///< 16.16 fixed-point pixels
  s32 speedX;
  s32 speedY;
  s32 speedZ;
  s32 scale;     ///< 16.16 fixed point, where Q16_ONE is full size
  s32 scaleStep; ///< 16.16 fixed point
  u8 timer;
  u8 hold;
  u8 pad26[2];
};

/// The floating lights' work in HEAP_SLOT_SOL_SANCTUM_LIGHTS, one entry per
/// light.
struct SolSanctumRingList {
  struct SolSanctumRingEntry entries[SOL_SANCTUM_RING_MAX];
  u16 count;
};

/// A task, the lights' work in HEAP_SLOT_SOL_SANCTUM_LIGHTS. Every
/// third frame each light grows to its largest scale and shrinks to a tenth; at
/// the smallest it vanishes for 24 steps while its drift carries on, and comes
/// back where the drift has taken it. Its drift is a random speed on each axis,
/// as its tables allow, turned into a sine swing.
static void SolSanctum_UpdateRing(void) {
  struct SolSanctumRingList *list = HeapSlot_AllocEwram(
      HEAP_SLOT_SOL_SANCTUM_LIGHTS, sizeof(struct SolSanctumRingList));
  struct SolSanctumRingEntry *entry = list->entries;
  u32 i;

  i = 0;
  while (i != list->count) {
    struct Actor *actor;
    s32 speedX;
    s32 speedY;
    s32 speedZ;
    s32 scale;
    s32 step;
    u8 timer;
    s32 x;
    s32 y;
    s32 z;
    u8 hold;

    actor = entry->actor;
    speedX = entry->speedX;
    speedY = entry->speedY;
    speedZ = entry->speedZ;
    scale = entry->scale;
    step = entry->scaleStep;
    hold = entry->hold;
    x = entry->x;
    y = entry->y;
    z = entry->z;
    timer = entry->timer;
    if (--timer == 0) {
      u32 rx;
      u32 ry;
      u32 rz;
      s32 dx;
      s32 dy;

      timer = 3;
      if (hold == 0) {
        scale += step;
        if (scale >= gSolSanctumRing_MaxScales[i]) {
          step = -gSolSanctumRing_ScaleSteps[i];
        } else if (scale <= Q16_ONE / 10) {
          step = gSolSanctumRing_ScaleSteps[i];
          scale = Q16_ONE / 10;
          hold = 24;
          x = actor->x.fixed;
          y = actor->y.fixed;
          z = actor->z.fixed;
          actor->x.fixed = 0;
          actor->y.fixed = 0;
          actor->z.fixed = 0;
        }
        actor->scaleX = scale;
        actor->scaleY = scale;
      }
      rx = (gSolSanctum_RingDrift[i][0] * (u32)Import_RandomNext()) >> 16;
      ry = (gSolSanctum_RingDrift[i][1] * (u32)Import_RandomNext()) >> 16;
      rz = (gSolSanctum_RingDrift[i][2] * (u32)Import_RandomNext()) >> 16;
      rx = rx != 0 ? (rx << 16) / 1000 : 0;
      ry = ry != 0 ? (ry << 16) / 1000 : 0;
      rz = rz != 0 ? (rz << 16) / 1000 : 0;
      if (gSolSanctum_RingDirection[i][0] == 1)
        speedX += rx;
      else if (gSolSanctum_RingDirection[i][0] == -1)
        speedX -= rx;
      else
        speedX = 0;
      if (gSolSanctum_RingDirection[i][1] == 1)
        speedY += ry;
      else if (gSolSanctum_RingDirection[i][1] == -1)
        speedY -= ry;
      else
        speedY = 0;
      if (gSolSanctum_RingDirection[i][2] == 1)
        speedZ += rz;
      else if (gSolSanctum_RingDirection[i][2] == -1)
        speedZ -= rz;
      else
        speedZ = 0;
      dx = Trig_Sin(speedX * gSolSanctum_RingSwing[i][0]) * 2;
      dy = Trig_Sin(speedY * gSolSanctum_RingSwing[i][1]) * 2;
      {
        s32 dz = Trig_Cos(speedZ * gSolSanctum_RingSwing[i][2]) * 2;

        if (hold != 0) {
          x += dx;
          y += dy;
          z += dz;
          if (--hold == 0) {
            actor->x.fixed = x;
            actor->targetX = x;
            if (ry != 0) {
              actor->y.fixed = y;
              actor->targetY = y;
            }
            actor->z.fixed = z;
            actor->targetZ = z;
          }
        } else {
          actor->x.fixed += dx;
          actor->targetX = actor->x.fixed;
          if (ry != 0) {
            actor->y.fixed += dy;
            actor->targetY = actor->y.fixed;
          }
          actor->z.fixed += dz;
          actor->targetZ = actor->z.fixed;
        }
      }
    }
    entry->speedX = speedX;
    entry->speedY = speedY;
    entry->speedZ = speedZ;
    entry->scale = scale;
    entry->scaleStep = step;
    entry->hold = hold;
    entry->x = x;
    entry->y = y;
    entry->z = z;
    entry->timer = timer;
    i++;
    entry++;
  }
}

/// Starts the floating lights: count actors (at most 10) from first,
/// each drawn translucent and starting at its largest scale.
void SolSanctum_StartFloatingLights(s32 first, u32 count) {
  // Each light is read here as FieldEffect, union ActorOrEffect's effect
  // view of the same object, the plain-C form this site matches with
  // (MATCHING.md): through Actor's sprite union, the sprite-flags address
  // moves after the ring-entry store. The task reads Actor.
  struct SolSanctumRingList *list;
  struct SolSanctumRingEntry *entry;
  struct FieldEffect *light;
  u32 i;
  s32 none;
  s32 cleared;

  list = HeapSlot_AllocEwram(HEAP_SLOT_SOL_SANCTUM_LIGHTS,
                             sizeof(struct SolSanctumRingList));
  entry = list->entries;
  Dma_Fill32(0, list, sizeof(struct SolSanctumRingList));
  if (count > SOL_SANCTUM_RING_MAX)
    count = SOL_SANCTUM_RING_MAX;
  none = 0;
  i = none;
  if (count != 0) {
    cleared = none;
    do {
      light = (struct FieldEffect *)Actor_GetById(first);
      {
        u8 *flags = &light->sprite->flags;

        entry->actor = (struct Actor *)light;
        *flags = cleared;
      }
      light->motionFlags = cleared;
      Actor_SetBlendMode(Actor_GetById(first), ACTOR_BLEND_TRANSLUCENT);
      entry->scale = gSolSanctumRing_MaxScales[i];
      entry->scaleStep = -gSolSanctumRing_ScaleSteps[i];
      entry->timer = 3;
      i++;
      entry++;
      first++;
    } while (i != count);
  }
  list->count = count;
  Task_AddFrame(SolSanctum_UpdateRing, TASK_PASS_FRAME);
}
