/// @file
/// Sol Sanctum's Elemental Star room and its eruption: the ten lights that
/// float in the room, actors in a row from the first the map passes, each
/// pulsing and drifting on its own path from the tables gSolSanctum_Ring*
/// (sol_sanctum_elemental_star_ring_tables.c), by its index.

#include "system/dma.h"

/// The most lights the task keeps.
enum { SOL_SANCTUM_RING_MAX = 10 };

/// A light moves every this many frames.
#define SOL_SANCTUM_LIGHT_STEP_FRAMES 3
/// Shrunk to a tenth, a light is hidden for this many of its steps while it
/// drifts on unseen, then shows again where it has drifted to.
#define SOL_SANCTUM_LIGHT_HIDDEN_STEPS 24

/// One floating light: its actor, its drift and its pulsing scale, and
/// where it is while hidden.
struct SolSanctumLightEntry {
  struct Actor *actor;
  FIXED x; ///< 16.16 fixed-point pixels
  FIXED y; ///< 16.16 fixed-point pixels
  FIXED z; ///< 16.16 fixed-point pixels
  /// Its drift on each axis, which grows at random each step; the light
  /// swings by the sine of it.
  s32 speedX;
  s32 speedY;
  s32 speedZ;
  FIXED scale;     ///< 16.16 fixed point, where FIXED_ONE is full size
  FIXED scaleStep; ///< 16.16 fixed point
  u8 timer;        ///< frames to its next step
  u8 hold;         ///< steps it has left hidden
};

/// The floating lights' state in HEAP_SLOT_SOL_SANCTUM_LIGHTS, one entry per
/// light.
struct SolSanctumLightList {
  struct SolSanctumLightEntry entries[SOL_SANCTUM_RING_MAX];
  u16 count;
};

/// A task: every SOL_SANCTUM_LIGHT_STEP_FRAMES each light pulses between a
/// tenth of its size and its largest scale and drifts, swinging about on a
/// sine of its drift speed on each axis it drifts on.
static void SolSanctum_UpdateFloatingLights(void) {
  struct SolSanctumLightList *list = HeapSlot_AllocEwram(
      HEAP_SLOT_SOL_SANCTUM_LIGHTS, sizeof(struct SolSanctumLightList));
  struct SolSanctumLightEntry *entry = list->entries;
  u32 i;

  i = 0;
  while (i != list->count) {
    struct Actor *actor;
    s32 speedX;
    s32 speedY;
    s32 speedZ;
    FIXED scale;
    s32 step;
    u8 timer;
    FIXED x;
    FIXED y;
    FIXED z;
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
      FIXED dx;
      FIXED dy;

      timer = SOL_SANCTUM_LIGHT_STEP_FRAMES;
      if (hold == 0) {
        scale += step;
        if (scale >= gSolSanctum_LightMaxScales[i]) {
          step = -gSolSanctum_LightScaleSteps[i];
        } else if (scale <= FIXED_ONE / 10) {
          step = gSolSanctum_LightScaleSteps[i];
          scale = FIXED_ONE / 10;
          hold = SOL_SANCTUM_LIGHT_HIDDEN_STEPS;
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
      rx = Random_Below(gSolSanctum_LightDrift[i][0]);
      ry = Random_Below(gSolSanctum_LightDrift[i][1]);
      rz = Random_Below(gSolSanctum_LightDrift[i][2]);
      rx = rx != 0 ? INTtoFIXED(rx) / 1000 : 0;
      ry = ry != 0 ? INTtoFIXED(ry) / 1000 : 0;
      rz = rz != 0 ? INTtoFIXED(rz) / 1000 : 0;
      if (gSolSanctum_LightDirection[i][0] == 1)
        speedX += rx;
      else if (gSolSanctum_LightDirection[i][0] == -1)
        speedX -= rx;
      else
        speedX = 0;
      if (gSolSanctum_LightDirection[i][1] == 1)
        speedY += ry;
      else if (gSolSanctum_LightDirection[i][1] == -1)
        speedY -= ry;
      else
        speedY = 0;
      if (gSolSanctum_LightDirection[i][2] == 1)
        speedZ += rz;
      else if (gSolSanctum_LightDirection[i][2] == -1)
        speedZ -= rz;
      else
        speedZ = 0;
      dx = Trig_Sin(speedX * gSolSanctum_LightSwing[i][0]) * 2;
      dy = Trig_Sin(speedY * gSolSanctum_LightSwing[i][1]) * 2;
      {
        FIXED dz = Trig_Cos(speedZ * gSolSanctum_LightSwing[i][2]) * 2;

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

/// Starts the floating lights: count actors (at most SOL_SANCTUM_RING_MAX)
/// from first, each drawn translucent and starting at its largest scale.
void SolSanctum_StartFloatingLights(s32 first, u32 count) {
  // Each light is set up through its effect view (union ActorOrEffect); the
  // task reads it as an actor.
  struct SolSanctumLightList *list;
  struct SolSanctumLightEntry *entry;
  struct FieldEffect *light;
  u32 i;
  s32 cleared;

  list = HeapSlot_AllocEwram(HEAP_SLOT_SOL_SANCTUM_LIGHTS,
                             sizeof(struct SolSanctumLightList));
  entry = list->entries;
  Dma_Fill32(0, list, sizeof(struct SolSanctumLightList));
  if (count > SOL_SANCTUM_RING_MAX)
    count = SOL_SANCTUM_RING_MAX;
  i = 0;
  if (count != 0) {
    cleared = 0;
    do {
      light = (struct FieldEffect *)Actor_GetById(first);
      {
        u8 *flags = &light->sprite->flags;

        entry->actor = (struct Actor *)light;
        *flags = cleared;
      }
      light->motionFlags = cleared;
      Actor_SetBlendMode(Actor_GetById(first), ACTOR_BLEND_TRANSLUCENT);
      entry->scale = gSolSanctum_LightMaxScales[i];
      entry->scaleStep = -gSolSanctum_LightScaleSteps[i];
      entry->timer = SOL_SANCTUM_LIGHT_STEP_FRAMES;
      i++;
      entry++;
      first++;
    } while (i != count);
  }
  list->count = count;
  Task_AddFrame(SolSanctum_UpdateFloatingLights, TASK_PASS_FRAME);
}
