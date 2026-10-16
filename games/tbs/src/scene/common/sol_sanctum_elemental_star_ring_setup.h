#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

// Legacy actor view used only while setting up the sprite flags.
struct SolSanctumRingObject {
  u8 pad00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 pad14[4];
  s32 scaleX;
  s32 scaleY;
  u8 pad20[24];
  s32 baseX;
  s32 baseY;
  s32 baseZ;
  u8 pad44[12];
  u8 *anim;
  u8 pad54;
  u8 state;
};

struct SolSanctumRingEntry {
  struct FieldActor *obj;
  s32 x;
  s32 y;
  s32 z;
  s32 speedX;
  s32 speedY;
  s32 speedZ;
  s32 scale;
  s32 scaleStep;
  u8 timer;
  u8 hold;
  u8 pad26[2];
};

struct SolSanctumRingList {
  struct SolSanctumRingEntry entries[10];
  u16 count;
};

// Pulse each Elemental Star's scale and move it through its drifting orbit.
static void SolSanctum_UpdateRing(void) {
  struct SolSanctumRingList *list = Runtime_AllocateBlock(33, 404);
  struct SolSanctumRingEntry *entry = list->entries;
  u32 i;

  i = 0;
  while (i != list->count) {
    struct FieldActor *obj;
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

    obj = entry->obj;
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
        } else if (scale <= 0x1999) {
          step = gSolSanctumRing_ScaleSteps[i];
          scale = 0x1999;
          hold = 24;
          x = obj->x.fixed;
          y = obj->y.fixed;
          z = obj->z.fixed;
          obj->x.fixed = 0;
          obj->y.fixed = 0;
          obj->z.fixed = 0;
        }
        obj->scaleX = scale;
        obj->scaleY = scale;
      }
      rx = (gSolSanctum_RingDrift[i][0] * (u32)Engine_RandomNext()) >> 16;
      ry = (gSolSanctum_RingDrift[i][1] * (u32)Engine_RandomNext()) >> 16;
      rz = (gSolSanctum_RingDrift[i][2] * (u32)Engine_RandomNext()) >> 16;
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
      dx = Engine_MathSin(speedX * gSolSanctum_RingSwing[i][0]) * 2;
      dy = Engine_MathSin(speedY * gSolSanctum_RingSwing[i][1]) * 2;
      {
        s32 dz = Engine_MathCos(speedZ * gSolSanctum_RingSwing[i][2]) * 2;

        if (hold != 0) {
          x += dx;
          y += dy;
          z += dz;
          if (--hold == 0) {
            obj->x.fixed = x;
            obj->targetX = x;
            if (ry != 0) {
              obj->y.fixed = y;
              obj->targetY = y;
            }
            obj->z.fixed = z;
            obj->targetZ = z;
          }
        } else {
          obj->x.fixed += dx;
          obj->targetX = obj->x.fixed;
          if (ry != 0) {
            obj->y.fixed += dy;
            obj->targetY = obj->y.fixed;
          }
          obj->z.fixed += dz;
          obj->targetZ = obj->z.fixed;
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

// Start the shared floating-star animation for a consecutive group of actors.
void SolSanctum_SetupElementalRings(s32 first, u32 count) {
  /* FAKEMATCH: the legacy scalar setup view keeps the sprite-flags address
   * calculation before the ring-entry store. Using FieldActor and
   * AnimationObject directly swaps those two instructions. The animation
   * callback uses FieldActor normally. */
  struct SolSanctumRingList *list;
  struct SolSanctumRingEntry *entry;
  struct SolSanctumRingObject *obj;
  u32 i;
  s32 none;
  s32 cleared;

  list = Runtime_AllocateBlock(33, 0x194);
  entry = list->entries;
  Dma_Fill32(0, list, 0x194);
  if (count > 10)
    count = 10;
  none = 0;
  i = none;
  if (count != 0) {
    cleared = none;
    do {
      obj = (struct SolSanctumRingObject *)Object_GetById(first);
      {
        u8 *frame = &obj->anim[38];

        entry->obj = (struct FieldActor *)obj;
        *frame = cleared;
      }
      obj->state = cleared;
      Engine_ObjectSetBlendMode(Object_GetById(first), 1);
      entry->scale = gSolSanctumRing_MaxScales[i];
      entry->scaleStep = -gSolSanctumRing_ScaleSteps[i];
      entry->timer = 3;
      i++;
      entry++;
      first++;
    } while (i != count);
  }
  list->count = count;
  FieldScheduler_AddCallback(SolSanctum_UpdateRing, 0xc80);
}
