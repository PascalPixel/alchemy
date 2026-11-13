/// @file
/// The scene code of overlay 0x64e that its scene tables belong to, in ROM order; the rest of the overlay is in venus_lighthouse/.

#include "types.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "scene.h"

/* VIEW(venus_lighthouse): variable gData_02001d8c as s32 gData_02001d8c[]; module/scene/venus_lighthouse.h has s16 gData_02001d8c[]. */
extern s32 gData_02001d8c__venus_lighthouse[] __asm__("gData_02001d8c");

/* VIEW(venus_lighthouse): routine Actor_FindInCell as struct StagedActor *Actor_FindInCell(struct FieldPosition *position, struct StagedActor *self); module/scene/magma_rock.h has struct Actor *Actor_FindInCell(s32 *position, struct Actor *self). */
extern struct StagedActor *Actor_FindInCell__venus_lighthouse(struct FieldPosition *position,
                                     struct StagedActor *self) __asm__("Actor_FindInCell");
void Func_02001b2c(struct StagedActor *object, s32 mode);
s32 Func_02001b64(struct StagedActor *object, struct FieldPosition *position);

void Func_0200038c(void) {
  struct FieldPosition dst;
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = (struct StagedActor *)Actor_GetById(gMainRoutineWork.currentOwner);
  dir = lead->facing >> 12;
  step = gData_02001d8c__venus_lighthouse[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell__venus_lighthouse(&dst, lead);
  if (next == 0)
    return;

  step = gData_02001d8c__venus_lighthouse[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = Actor_FindInCell__venus_lighthouse(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = Actor_FindInCell__venus_lighthouse(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gData_02001d8c__venus_lighthouse[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (Func_02001b64(next, &dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Func_02001b2c(lead, 8);
  rate = 0x3333;
  System_Wait(15);
  Event_PlaySound(185);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  FieldActor_SetMoveTarget(next, dst.x, dst.y, dst.z);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  FieldActor_SetMoveTarget(lead, dst.x, dst.y, dst.z);
  Actor_WaitForMove(next);
  Event_NoOp();
  next->x.fixed = dst.x;
  {
    /* FAKEMATCH(temporary): z read ahead of velocityX's store; as dst.z at its
     * store, 4 of 175 instructions differ in tla-en. */
    s32 z = dst.z;

    next->velocityX = 0;
    next->z.fixed = z;
    next->velocityZ = 0;
  }
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part.pixel << 16;
  lead->z.fixed = lead->z.part.pixel << 16;
  Func_02001b2c(lead, 1);
}

#include "../field/map/cell_fill_byte.c"

#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "field_event.h"
#include "scene_tables.h"
#include "frame.h"

void Func_0200056c(void) {}

#define Actor_SetSpriteColor ((void (*)(void *, s32))Actor_SetPartAttribute)

s32 OverlayObject_UpdateEveryFourFrames(void *obj) {
  if ((gFrameCount & 3) == 0)
    Actor_SetSpriteColor(obj, 7);
  else
    Actor_SetSpriteColor(obj, 0);

  if ((gFrameCount & 7) == 0)
    Event_PlaySound(138);
  return 0;
}

void Func_02000150(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase,
                   s32 life, struct SceneEffectParams *params);

/// Emits a rising particle near the actor every eight frames.
s32 VenusLighthouse_EmitRisingParticles(struct SceneEffectSource *actor) {
  struct SceneEffectParams options;
  s32 framePhase, particleX, particleY, riseSpeed;
  framePhase = gFrameCount & 7;
  if (framePhase != 0)
    goto done;
  options.priority = 3 - (s32)((u32)(Random16Far() * 2) >> 16);
  options.startScaleX = 0x6666;
  options.startScaleY = 0x6666;
  options.palette = 14;
  particleX = actor->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
  particleY = actor->y + ((32 - (s32)((u32)(Random16Far() * 32) >> 16)) << 16);
  riseSpeed =
      ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x00050000) / 10;
  Func_02000150(particleX, particleY, actor->z, 0, riseSpeed, framePhase,
                0x000b0000, &options);
done:
  return 0;
}

#define Actor_SetSingleChildFlags                                     \
  ((void (*)(void *, s32))Actor_SetSingleChildFlags)

extern const struct ActorPlacement gActorPlacementsEntrance99[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

s32 SceneActor_ApplyCounterLowBitsAsMode(struct Actor *actor) {
  Actor_SetSpriteColor(actor, actor->timer & 15);
  return 0;
}

s32 SceneActor_ClearSingleChildField(void *actor) {
  Actor_SetSingleChildFlags(actor, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.entrance == ENTRANCE_ALTERNATE_PLACEMENTS)
    return gActorPlacementsEntrance99;
  return gActorPlacements;
}
